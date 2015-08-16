#include "nfc.h"
#include "nand.h"

__nand_wait_rbready(base, n)
{
	int c;
	c = 0xfffff;
	nfc_rbsel(base, n);
	while(c) {
		if(nfc_check_rbready(base,n)) break;
		--c;
	}
	nfc_rbrel(base, n);
	return c;
}


#define _WRT	1
#define _OOB	2

__nand_read_page_oob(base, bus, buf, n, np)
	_nand_bus *bus;
	char *buf;
	u32 np;
{
	u8 cydr[8], rb,cid;
	nfc_chipsel(base, cid = _cal_real_chip(bus, n));
	nfc_rbsel(base, rb = _cal_real_rb(bus, n));

	cal_real_addr_r(&bus->nand[n], cydr, np);			// cal row(page) address
	cal_real_addr_c(&bus->nand[n], cydr, bus->nand[n].siz[_WRT]);	// cal column address

	_nfc_cmd_list c;

	nfc_fncmd(&c, NAND_CMD_READ0, 5, cydr);
	__nfc_read_fifo(base, &c);

	nfc_fncmd(&c, NAND_CMD_READSTART, 0, NULL);
	c.bytecnt = bus->nand[n].siz[_OOB]; //c.mode = 1;
	c.datr = 1; c.wait_rb_flag = 1;
	__nfc_read_fifo(base, &c);

//((unsigned long * volatile)0x80020000)[8] = ((unsigned long *)addr)[0];

	int r;

	if (__nand_wait_rbready(base, rb)) {
		nfc_fetchdata(base, buf, c.bytecnt);
		r = c.bytecnt;
	} else r = 0;

//	((unsigned long *)buf)[0] = ((unsigned long *)cydr)[0];

	nfc_rbrel(base, cid);
	nfc_chiprel(base, rb);

	return r;
}

__nand_read_page_fifo(base, bus, buf, n, np, oob)
	_nand_bus *bus;
	char *buf;
	u32 np;
{
	u32 ret;
	u8 cydr[8], nch, nrb;
	nfc_chipsel(base, nch = _cal_real_chip(bus, n));
	nfc_rbsel(base, nrb = _cal_real_rb(bus, n));

	cal_real_addr_r(&bus->nand[n], cydr, np);		// cal row(page) address


	if (bus->nand[n].siz[1] > 512) {
		//
		// large page read operation
		//
		_nfc_cmd_list c;

		nfc_fncmd(&c, NAND_CMD_READ0, 5, cydr);
		__nfc_read_fifo(base, &c);

		nfc_fncmd(&c, NAND_CMD_READSTART, 0, NULL);

		c.bytecnt = bus->nand[n].siz[_WRT] > 1024 ? 1024 : bus->nand[n].siz[_WRT];

		c.datr = 1; c.wait_rb_flag = 1;
		__nfc_read_fifo(base, &c);

		// check r/b ready
		if (__nand_wait_rbready(base, nrb)) {
			nfc_fetchdata(base, buf, c.bytecnt);

			int j;
			for(j = 1024; j<bus->nand[n].siz[_WRT]; j += 1024) {
				cal_real_addr_c(&bus->nand[n], cydr, j);
				nfc_fncmd(&c, NAND_CMD_RNDOUT, 2, cydr);	// 只发送地址后两位
				__nfc_read_fifo(base, &c);

				nfc_fncmd(&c, NAND_CMD_RNDOUTSTART, 0, NULL);
				c.bytecnt = 1024;
				c.datr = 1; c.wait_rb_flag = 0;
				__nfc_read_fifo(base, &c);

				// 注意，这里不需再检查R/B线, 详见芯片手册
				nfc_fetchdata(base, buf + j, c.bytecnt);
			}

			ret = bus->nand[n].siz[_WRT];

			if (oob) {
				cal_real_addr_c(&bus->nand[n], cydr, bus->nand[n].siz[_WRT]);
				nfc_fncmd(&c, NAND_CMD_RNDOUT, 2, cydr);

				__nfc_read_fifo(base, &c);

				nfc_fncmd(&c, NAND_CMD_RNDOUTSTART, 0, NULL);
				c.bytecnt = bus->nand[n].siz[_OOB];
				c.datr = 1; c.wait_rb_flag = 0;
				__nfc_read_fifo(base, &c);

				// 注意，这里不需再检查R/B线, 详见芯片手册
				nfc_fetchdata(base, buf + ret, c.bytecnt);

				ret += c.bytecnt;
			}
		} else ret = 0;
	} else ret = 0;		// not support for small page

	nfc_rbrel(base, nrb);
	nfc_chiprel(base, nch);

	return ret;
}


_read_status(base)
{
	_nfc_cmd_list c;
	nfc_fncmd(&c, NAND_CMD_STATUS, 0, NULL);
	c.datr = 1; c.bytecnt = 1; c.wait_rb_flag = 0;
	if (nfc_status(base, &c)) return nfc_read_8(base);
	return ~0;
}

__nand_erase(base, bus, n, np)
	_nand_bus *bus;
	u32 np;
{
	u32 nch, nrb;
	nfc_chipsel(base, nch = _cal_real_chip(bus, n));
	nfc_rbsel(base, nrb = _cal_real_rb(bus, n));

	u8 cydr[8];
	cal_real_addr_r(&bus->nand[n], cydr, np);				// cal row(page) address

	_nfc_cmd_list c;

	nfc_fncmd(&c, NAND_CMD_ERASE1, 3, cydr);
	__nfc_erase(base, &c);

	nfc_fncmd(&c, NAND_CMD_ERASE2, 0, NULL);
	c.wait_rb_flag = 1;
	__nfc_erase(base, &c);

	int status;

	// check r/b line
	if (__nand_wait_rbready(base, nrb)) {
		// read status (....)
		status = _read_status(base, &bus->nand[n]);
	} else status = 0;

	nfc_rbrel(base, nrb);
	nfc_chiprel(base, nch);

	// 0 - successful
	// 1 - failed

	return 1 - (status & 1);
}

/*
int nand_do_read_page(int base, _nand_chip *chip, char *buf, int n, u32 sadr)
{
	u8 addr[8];
	int i;

	nfc_chipsel(base, n);				// Select the device
//	nfc_rbsel(base, chip->rb[n]);
	nfc_rbsel(base, 0);

	_nfc_cmd_list c;
//	if (!cal_real_addr(chip, addr, sadr, &i)) {
	cal_real_addr(chip, addr, sadr, &i); */
/*
		nfc_fncmd(&c, NAND_CMD_READ0, i, addr);
		c.data_fetch_flag = 1; c.wait_rb_flag = 0; c.main_data_fetch =  1;
		c.bytecnt = 256;

		nfc_cmdfunc(base, &c);
		while(nfc_check_rbready(n));		// 等待数据
		if (chip->writesize < 256)
			nfc_fetchdata(base, buf, chip->writesize);
		else {
			nfc_fetchdata(base, buf, 0xff);
			nfc_fncmd(&c, NAND_CMD_READ1, i, addr);

			c.data_fetch_flag = 1; c.wait_rb_flag = 0; c.main_data_fetch =  1;
			c.bytecnt = 256;
			nfc_cmdfunc(base, &c);
			while(nfc_check_rbready(n));		// 等待数据
			nfc_fetchdata(base, buf, chip->writesize - 256);
		}
	} else {
*//*		nfc_fncmd(&c, NAND_CMD_READ0, 5, addr);

//		c.datr = 1; c.wait_rb_flag = 1;
//		c.bytecnt = 5;

//		nfc_rwcmd(&c, 0, NAND_CMD_READSTART, NAND_CMD_RNDOUT, NAND_CMD_RNDOUTSTART, chip->writesize >> 10);
//		c.bytecnt = 1024;
//		c.datr = 1; c.wait_rb_flag = 1;
		__nfc_read_fifo(base, &c);

		nfc_fncmd(&c, NAND_CMD_READSTART, 0, NULL);
		c.bytecnt = 1024;
		c.datr = 1; c.wait_rb_flag = 1;
		__nfc_read_fifo(base, &c);

		while(!nfc_check_rbready(base, 0));		// r/b low

		nfc_fetchdata(base, buf, 1024);

		int j;
		for(j=1; j<(chip->writesize >> 10); ++j) {
			sadr+=1024;
			cal_real_addr(chip, addr, sadr, &i);
			nfc_fncmd(&c, NAND_CMD_RNDOUT, 2, addr);	// 只发送地址后两位
			__nfc_read_fifo(base, &c);

			nfc_fncmd(&c, NAND_CMD_RNDOUTSTART, 0, NULL);
			c.bytecnt = 1024;
			c.datr = 1; c.wait_rb_flag = 0;
			__nfc_read_fifo(base, &c);

			// 注意，这里不需再检查R/B线, 详见芯片手册

			nfc_fetchdata(base, buf + (j<<10), 1024);
		}


//	}
	return chip->writesize;
}*/

