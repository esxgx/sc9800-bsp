#ifndef __LCDVTC_I_H
#define __LCDVTC_I_H


#define __DEBUG_LCDC__


#define LCDC_REG_CTL   		0	// LCD Controller control register
#define LCDC_REG_STS   		1	// LCD Controller status register
#define LCDC_REG_DCLK		2	// LCD Controller dot clock register
#define LCDC_REG_BASIC0  	3	// LCD Controller base0 register
#define LCDC_REG_BASIC1  	4	// LCD Controller base1 register
#define LCDC_REG_MODE		5	// LCD Controller mode set register
#define LCDC_REG_TTL1		6	// LCD Controller TTL1 register
#define LCDC_REG_TTL2		7	// LCD Controller TTL2 register
#define LCDC_REG_TTL3		8	// LCD Controller TTL3 register
#define LCDC_REG_TTL4		9	// LCD Controller TTL4 register
#define LCDC_REG_HDTV0		12	// LCD Controller HDTV0 register
#define LCDC_REG_HDTV1		13	// LCD Controller HDTV1 register
#define LCDC_REG_HDTV2		14	// LCD Controller HDTV2 register
#define LCDC_REG_HDTV3		15	// LCD Controller HDTV3 register
#define LCDC_REG_HDTV4		16	// LCD Controller HDTV4 register
#define LCDC_REG_HDTV5		17	// LCD Controller HDTV5 register
#define LCDC_REG_HDTV6		18	// LCD Controller HDTV6 register
#define LCDC_REG_GAMMA_TBL	32	// LCD Controller gamma table register
#define LCDC_REG_CSC0		48	// LCD Controller csc0 register
#define LCDC_REG_CSC1		49	// LCD Controller csc1 register
#define LCDC_REG_CSC2		50	// LCD Controller csc2 register
#define LCDC_REG_CSC3		51	// LCD Controller csc3 register
#define LCDC_REG_SRGB		52	// LCD Controller RGB enhancement register
#define LCDC_REG_CPUWR		56	// LCD Controller cpu wr register
#define LCDC_REG_CPURD		57	// LCD Controller cpu rd register
#define LCDC_REG_CPURDNX        58	// LCD Controller cpu rdnx register
#define LCDC_REG_IOCTL1		60	// LCD Controller io control1 register
#define LCDC_REG_IOCTL2		61	// LCD Controller io control2 register
#define LCDC_REG_DUBUG          62	// LCD Controller debug register


// 这是扩展接口部分
// 将来可以通过它来修正成可重入的代码
#define __LCDVTC_0	base
#define __LCDVTC_1	int base

#define __LCDVTC_BEGIN	\
	volatile u32 *lcdc = (u32 *)base;
#define __LCDVTC_END

#define __LCDVTC_INP(y)	lcdc[y]
#define __LCDVTC_OUTP(y, z)	(lcdc[y] = (z))

// 这对应着:
// __LCDVTC_LOCK 	EnterCriticalSection
// __LCDVTC_UNLOCK	LeaveCriticalSection
// 其中X代表ID号
#define __LCDVTC_LOCK(x)
#define __LCDVTC_UNLOCK(x)

//
// LOCK号的分配原则是:
//    对于寄存器, 只分配可写入的寄存器
//    对于操作, 只分配与写寄存器有关的操作
//
#define __LCDVTC_LOCK_R_CTL		0
#define __LCDVTC_LOCK_R_STS		1
#define __LCDVTC_LOCK_R_DCLK		2
#define __LCDVTC_LOCK_R_BASIC		3
#define __LCDVTC_LOCK_R_MODE		4
#define __LCDVTC_LOCK_R_TTL		5
#define __LCDVTC_LOCK_R_HDTV		6
#define __LCDVTC_LOCK_R_GAMMA_TBL	7
#define __LCDVTC_LOCK_R_CSC		8
#define __LCDVTC_LOCK_R_SRGB		9
#define __LCDVTC_LOCK_R_IOCTL		10

// 扩展接口结束

#endif
