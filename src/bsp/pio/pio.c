#include "pio.h"

//#define __DEBUG_PIOC__

// offset
#define PIOC_REG_A_CFG0		0
#define PIOC_REG_A_CFG1		1
#define PIOC_REG_A_CFG2		2
#define PIOC_REG_A_DATA		3
#define PIOC_REG_A_MULDRV0	4
#define PIOC_REG_A_MULDRV1	5
#define PIOC_REG_A_PULL0	6
#define PIOC_REG_A_PULL1	7

#define PIOC_REG_B_CFG0		8
#define PIOC_REG_B_CFG1		9
#define PIOC_REG_B_CFG2		10
#define PIOC_REG_B_DATA		11
#define PIOC_REG_B_MULDRV0	12
#define PIOC_REG_B_MULDRV1	13
#define PIOC_REG_B_PULL0	14
#define PIOC_REG_B_PULL1	15

#define PIOC_REG_C_CFG0		16
#define PIOC_REG_C_CFG1		17
#define PIOC_REG_C_CFG2		18
#define PIOC_REG_C_DATA		19
#define PIOC_REG_C_MULDRV0	20
#define PIOC_REG_C_MULDRV1	21
#define PIOC_REG_C_PULL0  	22
#define PIOC_REG_C_PULL1  	23

#define PIOC_REG_D_CFG0		24
#define PIOC_REG_D_CFG1		25
#define PIOC_REG_D_CFG2		26
#define PIOC_REG_D_CFG3		27
#define PIOC_REG_D_DATA		28
#define PIOC_REG_D_MULDRV0	29
#define PIOC_REG_D_MULDRV1	30
#define PIOC_REG_D_PULL0  	31
#define PIOC_REG_D_PULL1  	32

#define PIOC_REG_E_CFG0		33
#define PIOC_REG_E_CFG1		34
#define PIOC_REG_E_DATA		35
#define PIOC_REG_E_MULDRV0	36
#define PIOC_REG_E_PULL0  	37

#define PIOC_REG_F_MULDRV0	38
#define PIOC_REG_F_PULL0  	39

#define PIOC_REG_INT		40

//
// piogrp - (GPIO_GRP_A ~ GPIO_GRP_E)
// prt - (GPIO_PORT_n(x))
//
pioc_gpio_getpin(base, pin, piogrp, prt)
	_gpio_pin *pin;
{
	pin->piogrp = piogrp;
	pin->n = prt;
	return 1;
}

const int pgt[] = {//px_grp_tbl[] = {
	PIOC_REG_A_CFG0,
	PIOC_REG_B_CFG0,
	PIOC_REG_C_CFG0,
	PIOC_REG_D_CFG0,
	PIOC_REG_E_CFG0
};

pioc_gpio_getcfg(base, grp, n)
{
	volatile u32 *pioc = (u32 *)base;
	return (pioc[pgt[grp] + (n >> 3)] >> ((n & 0x7) << 2)) & 7;
}

pioc_gpio_cfg(base, grp, n, sel)
{
	volatile u32 *pioc = (u32 *)base;
	pioc[pgt[grp] + (n>>3)] = pioc[pgt[grp] + (n >> 3)] & ~(7 << ((n & 7) << 2)) |
		sel << ((n & 7) << 2);
#ifdef __DEBUG_PIOC__
	printk("pincfg: (%u, %u)%xh\n", grp, n, pioc[pgt[grp] + (n>>3)]);
#endif
	return 1;
}

pioc_gpio_pullup(base, grp, n, m)
{
	const int ppt[] = {
		PIOC_REG_A_PULL0,	PIOC_REG_B_PULL0,	PIOC_REG_C_PULL0,
		PIOC_REG_D_PULL0,	PIOC_REG_E_PULL0,	PIOC_REG_F_PULL0
	};
	volatile u32 *pioc = (u32 *)base;
	pioc[ppt[grp] + (n >> 4)] = pioc[ppt[grp] + (n >> 4)] & ~(0x3 << ((n & 0xF) << 1)) |
		m << ((n & 0xF) << 1);
	// VOID
}

pioc_gpio_muldrv(base, grp, n, m)
{
	const int ppt[] = {
		PIOC_REG_A_MULDRV0,	PIOC_REG_B_MULDRV0,	PIOC_REG_C_MULDRV0,
		PIOC_REG_D_MULDRV0,	PIOC_REG_E_MULDRV0,	PIOC_REG_F_MULDRV0
	};
	volatile u32 *pioc = (u32 *)base;
	pioc[ppt[grp] + (n >> 4)] = pioc[ppt[grp] + (n >> 4)] & ~(0x3 << ((n & 0xF) << 1)) |
		m << ((n & 0xF) << 1);
	// VOID
}

const int pst[] = {
	PIOC_REG_A_DATA,	PIOC_REG_B_DATA,	PIOC_REG_C_DATA,
	PIOC_REG_D_DATA,	PIOC_REG_E_DATA
};

pioc_gpio_outp(base, grp, n, v)
{
	volatile u32 *pioc = (u32 *)base;
	pioc[pst[grp]] = (pioc[pst[grp]] & (~(1<<n))) | 
		(v << n);
#ifdef __DEBUG_PIOC__
	printk("pinout: (%u, %u, %u)%xh\n", grp, n, v, pioc[pst[grp]]);
#endif
	// VOID
}

pioc_gpio_inp(base, grp, n)
{
	volatile u32 *pioc = (u32 *)base;
	return (pioc[pst[grp]] >> n) & 1;
}

pioc_gpio_setup0(base)
{
	volatile u32 *pioc = (u32 *)base;
	pioc[PIOC_REG_INT] = 0;
}
