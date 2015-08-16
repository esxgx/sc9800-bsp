#include "nfc.h"
#include "nand.h"
#include "sunii.h"

#define NAFC_REGS_BASE		__SPx(___NAFC)
#define PIOC_REGS_BASE		__SPx(___PIOC)
#define CCMU_REGS_BASE		__SPx(___CCMU)

_SetCE4567()
{
	u32 cfg;

//	if(version == EPDK_PID_PVP103)
//	{

		cfg = *(volatile u32 *)(PIOC_REGS_BASE + 0x24);

		// set PIOC for nand, select NCE7 as the second CE
		cfg &= (~(0xf<<4));
		cfg |= (0x4<<4);

		*(volatile u32 *)(PIOC_REGS_BASE + 0x24) = cfg;

//	}
	return 0;

}

static _nand_bus __nand_bus_4_boot;

nand_startup0()
{
	gpio_nand();
	powerup_nfc(CCMU_REGS_BASE);	
//	_SetCE4567();

	// 全胜的ebios就是这样初始化的
	_nfc_init_info i;

	i.bus_width = 0; 	// 8-bit width
	i.ce_ctl = 0;		// CE# in active
	i.ce_ctl1 = 0;		// CE# alway active for cmd
	i.debug = 0;		// dont care

	i.pagesize = 4; // 12;// 4;	// in Kbytes

	i.serial_access_mode = 1;	// EDO mode
	i.rb_sel = 1;

	// nfc 初始化
	if (nfc_init(NAFC_REGS_BASE, &i)) 
		return nand_scan(NAFC_REGS_BASE, &__nand_bus_4_boot, 2);	// 扫描每一粒芯片
	else return 0;
}

nand_shutdown0(x)
{
	return nfc_exit(NAFC_REGS_BASE);
}

nand_startup(o)
	int *o;
{
	*o = NFC_ResetINT();
	return nand_startup0();
}

nand_stutdown(o)
{
	int r = nand_shutdown0();
	NFC_RestoreINT(o);
	return r;
}

int nand_read_page_fifo(char *buf, int n, u32 np, int oob)
{
	return __nand_read_page_fifo(NAFC_REGS_BASE, &__nand_bus_4_boot, buf, n, np, oob);
}

int nand_read_page_oob(char *buf, int n, u32 np)
{
	return __nand_read_page_oob(NAFC_REGS_BASE, &__nand_bus_4_boot, buf, n, np);
}

int nand_erase(int n, u32 np)
{
	return __nand_erase(NAFC_REGS_BASE, &__nand_bus_4_boot, n, np);
}

// 清除所有块(毁灭性)
//int nand_eraseall(int n)
//{
//	int i;
//	for(i=0; i<_adr2b(&nand_flash_info, nand_flash_info.chipsize); ++i)
//		__nand_erase(NAFC_REGS_BASE, &nand_flash_info, n, i);
//	return 1;
//}


