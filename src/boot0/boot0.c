#include "typedef.h"

#define CCMU_REGS_BASE		0x01C20000

/*
ccmu_dump()
{

	volatile u32 *ccmu = (u32 *)0x1c20000;
	int i;
	for(i=0; i<0xd8/4; ++i)
		((u32 *)0x7000)[i] = ccmu[i];

}*/

boot0()
{
	powerup_sdram(CCMU_REGS_BASE);

	gpio_sstl_2();
	__sclk_coreclk_src_cfg(CCMU_REGS_BASE, 2);		// 设置时钟源为24Mhz
	ccmu_pll_freq_cfg(CCMU_REGS_BASE, 3, 120000000);
	ccmu_clkon(CCMU_REGS_BASE, 3, 3);

	__sclk_dramclk_output_on(CCMU_REGS_BASE, 1);
	__sclk_dram_ckb_on(CCMU_REGS_BASE, 1);
	dram_init();			// 初始化DRAM

	return 0;

}
