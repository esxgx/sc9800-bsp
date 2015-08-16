#ifndef __CCMU_I_H
#define __CCMU_I_H

#define __CCMU_DEBUG__

// Offset
#define CCMU_REG_AC320PLL	0
#define CCMU_REG_AUDIOPLL	1
#define CCMU_REG_AC320AHB	2
#define CCMU_REG_AHBMOD		3
#define CCMU_REG_APBMOD		4
#define CCMU_REG_NANDMS		5
#define CCMU_REG_SDMMC01	6
#define CCMU_REG_SDMMC23	7
#define CCMU_REG_DRAMPLL	8
#define CCMU_REG_DE		9
#define CCMU_REG_LCDMACC	10
#define CCMU_REG_TVCSI		11
#define CCMU_REG_VIDEOPLL	12
#define CCMU_REG_IR		13
#define CCMU_REG_SPAC97		14
#define CCMU_REG_TS		15
#define CCMU_REG_AVSUSB		16
#define CCMU_REG_CGTM		17
#define CCMU_REG_SWPR		18

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