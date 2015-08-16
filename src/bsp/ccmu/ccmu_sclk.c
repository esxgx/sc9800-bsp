//
//
// system clk & special clk

//
#include "ccmu.h"
#include "ccmu_i.h"

/*
static __inline__ __sclk_corepll_freq(base)
{
	volatile u32 *ccmu = (u32 *)base;

	if (ccmu[CCMU_REG_AC320PLL] & (1<<CCMU_BP_CORE_PLL_EN)) {
		int factor = (ccmu[CCMU_REG_AC320PLL] >> CCMU_BP_CORE_PLL_FACTOR) & (1<<8 - 1);
		return 6000000 * (factor + 1) + __sclk_HOSC_freq();
	}
	return __sclk_HOSC_freq();	// pll disabled
}
*/

#include "typedef.h"

static __inline__ __sclk_corepll_freq(base)
{
	if (ccmu_clkstat(base, 3, CCMU_PLL_CLK_CORE)) return ccmu_pll_freq(base, CCMU_PLL_CLK_CORE);
	return __sclk_HOSC_freq();	// pll disabled
}

__sclk_corepll_freq_cfg(base, freq)
{
//	ccmu[CCMU_REG_AC320PLL] = (ccmu[CCMU_REG_AC320PLL] & 0xC780FFFF) | (2<<27); 	// 27 - 28 should be 0x02
	ccmu_pll_freq_cfg(base, CCMU_PLL_CLK_CORE, freq);
	ccmu_clkon(base, 3, CCMU_PLL_CLK_CORE);
}

// CCMU  AC320 / AHB / APB
#define CCMU_BP_AC320_CLK_SRC		9
#define CCMU_BP_APB_CLK_DIV		6
#define CCMU_BP_AHB_CLK_DIV		3
#define CCMU_BP_AC320_CLK_DIV		0

static __sclk_coreclk_src(base)
{
	volatile u32 *ccmu = (u32 *)base;
	return (ccmu[CCMU_REG_AC320AHB] >> CCMU_BP_AC320_CLK_SRC) & 3;
}

static __sclk_coreclk_src_cfg(base, src)
{
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_AC320AHB] = ccmu[CCMU_REG_AC320AHB] & ~(3<<CCMU_BP_AC320_CLK_SRC) | src << CCMU_BP_AC320_CLK_SRC;
	return src;
}

static __sclk_coreclk_freq(base)
{
	volatile u32 *ccmu = (u32 *)base;

	switch(__sclk_coreclk_src(base)) {
		case 0:	return __sclk_LOSC_freq(); break;
		case 1: return __sclk_HOSC_freq(); break;
		case 2: return __sclk_corepll_freq(base); break;
		case 3:	return 0;		// DC		
	}
	return 0;
}

__sclk_coreclk_freq_cfg(base, freq)
{
	if (freq > __sclk_HOSC_freq()) __sclk_corepll_freq_cfg(base, freq);
	__sclk_coreclk_src_cfg(base, (freq > __sclk_LOSC_freq()) + (freq > __sclk_HOSC_freq()));
}

//
// CPU32H Clock Operations
//
static __sclk_ac320_clk(base)
{
	volatile u32 *ccmu = (u32 *)base;
	return 1<<((ccmu[CCMU_REG_AC320AHB] >> CCMU_BP_AC320_CLK_DIV) & 1);
}

__sclk_ac320clk_freq(base)
{
	int freq = __sclk_coreclk_freq(base);
	return freq / __sclk_ac320_clk(base);
}

// 
static __sclk_ac320clk_cfg(base, div)
{
	if (div > 2) div = 2;
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_AC320AHB] = ccmu[CCMU_REG_AC320AHB] & ~(1<<CCMU_BP_AC320_CLK_DIV) | ((div - 1) << CCMU_BP_AC320_CLK_DIV);
	return div;
}

//
// AHB Clock Operations
//
static __sclk_ahb_clk(base)
{
	volatile u32 *ccmu = (u32 *)base;
	return 1<<((ccmu[CCMU_REG_AC320AHB] >> CCMU_BP_AHB_CLK_DIV) & 3);
}

#include "bitops.h"

static __sclk_ahbclk_cfg(base, div)
{
	int __ffs = ffs(div) - 1;
	if (__ffs > 3) __ffs = 3;
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_AC320AHB] = ccmu[CCMU_REG_AC320AHB] & ~(3<<CCMU_BP_AHB_CLK_DIV) | __ffs << CCMU_BP_AHB_CLK_DIV;
	return 1<<__ffs;
}

__sclk_ahbclk_freq(base)
{
	int freq = __sclk_ac320clk_freq(base);
	return freq / __sclk_ahb_clk(base);
}

//
// APB Clock Operations
//
static __sclk_apb_clk(base)
{
	volatile u32 *ccmu = (u32 *)base;
	int clk = (ccmu[CCMU_REG_AC320AHB] >> CCMU_BP_APB_CLK_DIV & 3);
	if (!clk) clk = 1;
	return 1<<clk;
//	return 1<<(ccmu[CCMU_REG_AC320AHB] >> CCMU_BP_APB_CLK_DIV & 3);
}

#include "bitops.h"

static __sclk_apbclk_cfg(base, div)
{
	int __ffs = ffs(div) - 1;
	if (__ffs > 3) __ffs = 3;
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_AC320AHB] = ccmu[CCMU_REG_AC320AHB] & ~(3<<CCMU_BP_APB_CLK_DIV) | __ffs << CCMU_BP_APB_CLK_DIV;
	return 1<<__ffs;
}

__sclk_apbclk_freq(base)
{
	int freq = __sclk_ahbclk_freq(base);
	return freq / __sclk_apb_clk(base);
}

__sclk_videoclk_freq(base)
{
	if (ccmu_clkstat(base, 3, CCMU_PLL_CLK_VIDEO)) return ccmu_pll_freq(base, CCMU_PLL_CLK_VIDEO);
	return 0;		/* disabled */
}

__sclk_videoclk_freq_cfg(base, freq)
{
	ccmu_pll_freq_cfg(base, CCMU_PLL_CLK_VIDEO, freq);
	ccmu_clkon(base, 3, CCMU_PLL_CLK_VIDEO);
}


/* CCMU  DRAM PLL */
#define CCMU_BP_MACC_DRAM_GATE              31
#define CCMU_BP_DEFE_DRAM_GATE              30
#define CCMU_BP_DEBE_DRAM_GATE              29
#define CCMU_BP_CSI_DRAM_GATE               28
#define CCMU_BP_TS_DRAM_GATE                27
#define CCMU_BP_DRAM_CKB_EN                 16
#define CCMU_BP_DRAM_CLK_EN                 15
#define CCMU_BP_DRAM_CLK_DIV                12
#define CCMU_BP_DRAM_PLL_EN                 11
#define CCMU_BP_DRAM_PLL_BYPASS_EN          10
#define CCMU_BP_DRAM_PLL_BIAS               8
#define CCMU_BP_DRAM_PLL_FACTOR             0

__sclk_dramclk_output_on(base, v)
{
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_DRAMPLL] = ccmu[CCMU_REG_DRAMPLL] & ~(1 << CCMU_BP_DRAM_CLK_EN) | v << CCMU_BP_DRAM_CLK_EN;
//	printk("%08x, %08x,%08x\n",ccmu[CCMU_REG_DRAMPLL], ccmu[CCMU_REG_DE], ccmu[CCMU_REG_LCDMACC]);
	return v;
}

__sclk_dram_ckb_on(base, v)
{
	volatile u32 *ccmu = (u32 *)base;
	ccmu[CCMU_REG_DRAMPLL] = ccmu[CCMU_REG_DRAMPLL] & ~(1 << CCMU_BP_DRAM_CKB_EN) | v << CCMU_BP_DRAM_CKB_EN;
//	printk("%08x, %08x,%08x\n",ccmu[CCMU_REG_DRAMPLL], ccmu[CCMU_REG_DE], ccmu[CCMU_REG_LCDMACC]);
	return v;
}
