#ifndef __CCMU_I_H
#define __CCMU_I_H

#define __CCMU_DEBUG__

// Offset
#define CCMU_REG_AC320AHB	2
#define CCMU_REG_AHBMOD		3
#define CCMU_REG_DRAMPLL	8

// 24Mhz Clk
#define __sclk_HOSC_freq()	24000000
// RTC Clk
#define __sclk_LOSC_freq()	32768

#include "typedef.h"

#define __CCMU_0	base
#define __CCMU_1	int base

#define __CCMU_BEGIN	volatile u32 *ccmu = (u32 *)base;
#define __CCMU_END

#define __CCMU_INP(x)		ccmu[x]
#define __CCMU_OUTP(x,y)	(ccmu[x] = (y))
#define __CCMU_OUTP_OR(x,y)	(ccmu[x] |= (y))

#endif