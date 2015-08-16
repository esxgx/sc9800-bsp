// 注意这是阉割后的ccmu模块，不能替代正式模块使用
#include "ccmu.h"
#include "ccmu_i.h"

#include "typedef.h"

static u32 _gtahb[][2] = {
	#define __AHB__
	#include "ccmu_ahb.h"
};

static u32 _gtplls[][2] = {
	{                0,  0},	// CCMU_SCLK_COREPLL
	{                0,  0},	// CCMU_SCLK_MACCPLL
	{                0,  0},	// CCMU_SCLK_VIDEOPLL
	{ CCMU_REG_DRAMPLL, 11}		// CCMU_SCLK_DRAMPLL
};

static u32 (*_gta[])[2] = {
//	[        0][         1][       2][       3]
	    _gtahb,       NULL,     NULL, _gtplls
};

// 开启一个时钟
ccmu_clkon(base, t, n)
{
	volatile u32 *ccmu = (u32 *)base;
	ccmu[_gta[t][n][0]] |= 1<< _gta[t][n][1];
}

// 关闭一个时钟
ccmu_clkoff(base, t, n)
{
	volatile u32 *ccmu = (u32 *)base;
	ccmu[_gta[t][n][0]] &= ~(1<< _gta[t][n][1]);
}

/* CCMU  DRAM PLL */
#define CCMU_BP_DRAM_CKB_EN                 16
#define CCMU_BP_DRAM_CLK_EN                 15
#define CCMU_BP_DRAM_PLL_BYPASS_EN          10
#define CCMU_BP_DRAM_PLL_FACTOR             0

static
__ccmu_drampll_freq_cfg(__CCMU_0, freq)
	__CCMU_1;
{
	__CCMU_BEGIN
	if (freq > __sclk_HOSC_freq() + 36000000) {
		int factor = (freq - __sclk_HOSC_freq()) / 12000000 - 3;
		__CCMU_OUTP(CCMU_REG_DRAMPLL, 
			__CCMU_INP(CCMU_REG_DRAMPLL) & ~(63 << CCMU_BP_DRAM_PLL_FACTOR | 1<<CCMU_BP_DRAM_PLL_BYPASS_EN) |
			factor << CCMU_BP_DRAM_PLL_FACTOR);
	} else __CCMU_OUTP_OR(CCMU_REG_DRAMPLL, 1<<CCMU_BP_DRAM_PLL_BYPASS_EN);
	__CCMU_END
}

int
(*___pll_freq_cfg[])() = {
	NULL,
	NULL,
	NULL,
	__ccmu_drampll_freq_cfg
};

// 调整pll的频率
ccmu_pll_freq_cfg(base, n, freq)
{
	___pll_freq_cfg[n](base, freq);
}

__sclk_dramclk_output_on(base, v)
{
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_DRAMPLL] = ccmu[CCMU_REG_DRAMPLL] & ~(1 << CCMU_BP_DRAM_CLK_EN) | v << CCMU_BP_DRAM_CLK_EN;
	return v;
}

__sclk_dram_ckb_on(base, v)
{
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_DRAMPLL] = ccmu[CCMU_REG_DRAMPLL] & ~(1 << CCMU_BP_DRAM_CKB_EN) | v << CCMU_BP_DRAM_CKB_EN;
	return v;
}


// CCMU  AC320 / AHB / APB
#define CCMU_BP_AC320_CLK_SRC		9

__sclk_coreclk_src_cfg(base, src)
{
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_AC320AHB] = ccmu[CCMU_REG_AC320AHB] & ~(3<<CCMU_BP_AC320_CLK_SRC) | src << CCMU_BP_AC320_CLK_SRC;
	return src;
}
