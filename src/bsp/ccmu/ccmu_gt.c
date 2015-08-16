#include "ccmu.h"
#include "ccmu_i.h"

static u32 _gtahb[][2] = {
	#define __AHB__
	#include "ccmu_ahb.h"
};

static u32 _gtapb[][2] = {
	#define __APB__
	#include "ccmu_apb.h"
};

static u32 _gtdram[][2] = {
	#define __DRAM__ (u32 [])
	#include "ccmu_dram.h"
};

static u32 _gtplls[][2] = {
	{CCMU_REG_AC320PLL, 29},	// CCMU_SCLK_COREPLL
	{CCMU_REG_AC320PLL, 15},	// CCMU_SCLK_MACCPLL
	{CCMU_REG_VIDEOPLL,  7},	// CCMU_SCLK_VIDEOPLL
	{ CCMU_REG_DRAMPLL, 11}		// CCMU_SCLK_DRAMPLL
};


static u32 (*_gta[])[2] = {
//	[        0][         1][       2][       3]
	    _gtahb,     _gtapb,  _gtdram, _gtplls
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

// 查看时钟状态
ccmu_clkstat(base, t, n)
{
	volatile u32 *ccmu = (u32 *)base;
	return (ccmu[_gta[t][n][0]] >> _gta[t][n][1]) & 1;
}
