#include "typedef.h"
#include "ccmu_i.h"

// CCMU  AC320 / MACC PLL
#define CCMU_BP_CORE_PLL_EN                 29
#define CCMU_BP_CORE_PLL_BYPASS_EN          26
#define CCMU_BP_CORE_PLL_FACTOR             16
#define CCMU_BP_MACC_PLL_EN                 15
#define CCMU_BP_MACC_PLL_BYPASS_EN          12
#define CCMU_BP_MACC_PLL_FACTOR             0

static
__ccmu_corepll_freq(__CCMU_0)
	__CCMU_1;
{
	__CCMU_BEGIN

	int freq, factor;
	factor = __CCMU_INP(CCMU_REG_AC320PLL) >> CCMU_BP_CORE_PLL_FACTOR & 0x7f;

	// 是否开启旁路, 对于程序员来讲以下的不同旁路有不同的计算方法
	if (__CCMU_INP(CCMU_REG_AC320PLL) & 1<<CCMU_BP_CORE_PLL_BYPASS_EN)
		freq = __sclk_HOSC_freq() / factor;
	else	freq = 6000000 * (factor + 1) + __sclk_HOSC_freq();	// 手册中的计算方式
	
	return freq;

	__CCMU_END
}

static
__ccmu_corepll_freq_cfg(__CCMU_0, freq)
	__CCMU_1;
{
	__CCMU_BEGIN

	if (freq <= __sclk_HOSC_freq()) {
		int factor = __sclk_HOSC_freq() / freq;
		__CCMU_OUTP(CCMU_REG_AC320PLL, 
			__CCMU_INP(CCMU_REG_AC320PLL) & ~(0x7f << CCMU_BP_CORE_PLL_FACTOR) |
			factor << CCMU_BP_CORE_PLL_FACTOR | 1<<CCMU_BP_CORE_PLL_BYPASS_EN
		);
	} else {
		int factor = (freq - __sclk_HOSC_freq()) / 6000000 - 1;
		__CCMU_OUTP(CCMU_REG_AC320PLL, 
			__CCMU_INP(CCMU_REG_AC320PLL) & ~(0x7f << CCMU_BP_CORE_PLL_FACTOR) |
			factor << CCMU_BP_CORE_PLL_FACTOR
		);
	}	
	__CCMU_END
}

/* CCMU  DRAM PLL */
#define CCMU_BP_DRAM_PLL_EN                 11
#define CCMU_BP_DRAM_PLL_BYPASS_EN          10
#define CCMU_BP_DRAM_PLL_BIAS               8
#define CCMU_BP_DRAM_PLL_FACTOR             0

static
__ccmu_drampll_freq(__CCMU_0)
	__CCMU_1;
{
	__CCMU_BEGIN

	int freq;

	if (__CCMU_INP(CCMU_REG_DRAMPLL) & 1<<CCMU_BP_DRAM_PLL_BYPASS_EN)
		freq = __sclk_HOSC_freq();
	else {
		int factor = __CCMU_INP(CCMU_REG_DRAMPLL) >> CCMU_BP_DRAM_PLL_FACTOR & 63;
		freq = 12000000 * (factor + 3) + __sclk_HOSC_freq();
	}	
	return freq;

	__CCMU_END
}

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

/* CCMU  VIDEO PLL */
#define CCMU_BP_VIDEO_PLL_FACTOR            8
#define CCMU_BP_VIDEO_PLL_EN                7

static
__ccmu_videopll_freq(__CCMU_0)
	__CCMU_1;
{
	__CCMU_BEGIN

	int factor;
	factor = __CCMU_INP(CCMU_REG_VIDEOPLL) >> CCMU_BP_VIDEO_PLL_FACTOR & 0x7f;	
	return factor * 3000000;

	__CCMU_END
}

static
__ccmu_videopll_freq_cfg(__CCMU_0, freq)
	__CCMU_1;
{
	__CCMU_BEGIN

	__CCMU_OUTP(CCMU_REG_VIDEOPLL, __CCMU_INP(CCMU_REG_VIDEOPLL) & ~(0x7f << CCMU_BP_VIDEO_PLL_FACTOR) |
		freq/300000 << CCMU_BP_VIDEO_PLL_FACTOR);

	__CCMU_END
}


// 接口部分
// 注意，除了ccmu模块其他的调用只能使用下面的部分.

int
(*___pll_freq[])() = {
	__ccmu_corepll_freq,
	NULL,
	__ccmu_videopll_freq,
	__ccmu_drampll_freq
};

// 查看pll的频率
ccmu_pll_freq(base, n)
{
	return ___pll_freq[n](base);
}


int
(*___pll_freq_cfg[])() = {
	__ccmu_corepll_freq_cfg,
	NULL,
	__ccmu_videopll_freq_cfg,
	__ccmu_drampll_freq_cfg
};

// 调整pll的频率
ccmu_pll_freq_cfg(base, n, freq)
{
	___pll_freq_cfg[n](base, freq);
}

