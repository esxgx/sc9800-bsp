//
// 2010.7.24 支持多种芯片链接方式(根据SoftWinner的研究结果添加)
// 2010.7.23 修复BUG, 支持读状态操作
//

#include "nfc.h"
#include "nand.h"

//extern struct nand_flash_dev nand_flash_ids[];
//extern struct nand_manufacturers nand_manuf_ids[];

#define delay(n) do {	\
	int f = n;	\
	while(f--);	\
}while(0)

// ffs相关
#include "bitops.h"


// 因为在枚举chip的时候不知道r/b的连接方式，所以都枚举一遍
__nand_wait_rbready1(base)
{
	int c;
	c = 0xffff;
	nfc_rbsel(base, 0);
	while(c) {
		if(nfc_check_rbready(base,0)) break;
		--c;
	}
	nfc_rbrel(base, 0);
	if (!c) {
		nfc_rbsel(base, 1);
		while(c) {
			if(nfc_check_rbready(base,1)) break;
			--c;	
		}
		nfc_rbrel(base, 1);
	} 
	return c;
}

memory(void *dst, void *src, int count)
{
	while(count) {
		*((char *)dst++) = *((char *)src++);
		--count;
	}
}

// Get the flash and manufacturer id and lookup if the type is supported
//int nand_get_flash_type(int base, _nand_chip *chip)

nand_flash_initialize(base, bus, n)
	_nand_bus *bus;
{
	// Select the nth dev
	nfc_chipsel(base, n);

	_nfc_cmd_list c;

	//
	// Reset the chip, required by some chips (e.g. Micron MT29FxGxxxxx)
	// after power-up
	//
	nfc_fncmd(&c, NAND_CMD_RESET, 0, NULL);
	nfc_reset_chip(base, &c);

	if (!__nand_wait_rbready1(base)) return -1;

	delay(1000);

	// Send the command for reading device ID
	nfc_fncmd(&c, NAND_CMD_READID, 1, NULL);
	char idbuf[5];
	nfc_get_flash_id(base, &c, idbuf, 4);

	// Ensure the device is working
	do {
		unsigned i;
		i = idbuf[0]<<8 | idbuf[1];

		// Try again to make sure, as some systems the bus-hold or other
		// interface concerns can cause random data which looks like a
		// possibly credible NAND flash to appear. If the two results do
		// not match, ignore the device completely.
		//
		nfc_get_flash_id(base, &c, idbuf, 4);

		if ((idbuf[0]<<8 | idbuf[1]) != i) return -2;
	} while(0);


//	((unsigned long * volatile)0x80020000)[7] = idbuf[1];

	// Generate the Archi
	if (!_nand_genarchi_0(&bus->nand[n].archi,idbuf[0])) return -3;	// Lookup the manufacturer id
	if (!_nand_genarchi_1(&bus->nand[n].archi,idbuf[1])) return -4;	// Lookup the device id
	if (!_nand_genarchi_2(&bus->nand[n].archi,idbuf[2])) return -5;
	if (!_nand_genarchi_3(&bus->nand[n].archi,idbuf[3])) return -6;
	if (!_nand_genarchi_4(&bus->nand[n].archi,idbuf[4])) return -7;


//	memory(&((unsigned long *)0x80020000)[10],&bus->nand[n].archi,sizeof(bus->nand[n].archi));
	bus->nand[n].siz[1] = bus->nand[n].archi.BytesOfPg;
	bus->nand[n].siz[0] = bus->nand[n].archi.BytesOfBlk;
	bus->nand[n].siz[2] = bus->nand[n].archi.BytesOfSpare;
	bus->nand[n].siz[3] = bus->nand[n].archi.SizeOfChip;

	// maybe bugfix!
	// calc __ffz[0] and __ffz[1] for omit the idiv operation
	bus->nand[n].__ffz[0] = ffs(bus->nand[n].siz[0]) - 1;
	bus->nand[n].__ffz[1] = ffs(bus->nand[n].siz[1]) - 1;

	nfc_chiprel(base, n);
	return 0;
}

nand_scan(base, bus,maxchips)
	_nand_bus *bus;
{
	bus->numchips = 0;
	if (maxchips <= 8) {

		unsigned conn_map;
		conn_map = 0;
		int i,j;
		for(i=0; i<maxchips; ++i) {
			j = !nand_flash_initialize(base, bus, i);
			conn_map |= j << i;
			bus->numchips += j;
		}
		bus->conn = 3;
		// 分析芯片, 确定rb_mode
		switch(bus->numchips) {
			case 8:
				bus->conn = 2;
				break;
			case 4:
				if (conn_map & 1) {
					if (conn_map & (1<<1)) bus->conn = 2;
					else if (!conn_map & (1<<3)) bus->conn = 1;
				}
				break;
			case 2:
				if (conn_map & 1) {
//					if (conn_map & (1<<1)) bus->conn = 0;	// 单片双CE
//					else bus->conn = 1;				// 双片单CE
					bus->conn = ((conn_map & (1<<1)) == 0);
				}
				break;
			case 1:
				if (conn_map == 1) bus->conn = 0;
				break;
		}
		if (bus->conn == 3) bus->numchips = 0;
	}
	return bus->numchips;
}

#define nand_scan_ident nand_scan
