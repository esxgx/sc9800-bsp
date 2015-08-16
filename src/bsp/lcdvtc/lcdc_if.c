#include "lcdc_tve.h"
#include "lcdvtc_i.h"

// lcd hv
#define LCDC_HV_HSPW			0
#define LCDC_HV_VSPW			10
#define LCDC_HV_LDE_OUTPUT_VALUE	16
#define LCDC_HV_LDE_O_SEL		17
#define LCDC_HV_YUV_SM			22
#define LCDC_HV_RGB888_SM1		24
#define LCDC_HV_RGB888_SM0		26
#define LCDC_HV_SERIAL_MOD		30
#define LCDC_HV_IF_MOD			31


#define LCDC_REG_BASIC0  	3	// LCD Controller base0 register
#define LCDC_REG_BASIC1  	4	// LCD Controller base1 register
#define LCDC_REG_MODE		5	// LCD Controller mode set register

lcdc_cfg_hv(__LCDVTC_0, hvif)
	__LCDVTC_1;
	_lcdc_cfg_hvif *hvif;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_MODE);

	__LCDVTC_OUTP(LCDC_REG_MODE, hvif->lcd_hv_if << LCDC_HV_IF_MOD |
			hvif->lcd_hv_smode << LCDC_HV_SERIAL_MOD |
			hvif->lcd_hv_s888_if << LCDC_HV_RGB888_SM0 |
			hvif->lcd_hv_s888_if << LCDC_HV_RGB888_SM1 |
			hvif->lcd_hv_syuv_if << LCDC_HV_YUV_SM |
//			hvif->lcd_hv_lde_used << LCDC_HV_LDE_O_SEL |
//			hvif->lcd_hv_lde_iovalue << LCDC_HV_LDE_OUTPUT_VALUE |
			hvif->lcd_hv_hspw << LCDC_HV_HSPW |
			hvif->lcd_hv_vspw << LCDC_HV_VSPW);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_MODE);
#ifdef __DEBUG_LCDC__
	printk("lcdc_reg_mode -> [%08xh]\n", __LCDVTC_INP(LCDC_REG_MODE));
#endif
	__LCDVTC_END
}

// LCDC_CTL
#define LCDC_CTL_LCD_Y		0	// [0:9]
#define LCDC_CTL_LCD_X		10	// [10:19]
#define LCDC_CTL_SWAP		20	// [20]
#define LCDC_CTL_WHITE_DATA	21	// [21]
#define LCDC_CTL_DF_SEL		22	// [22:23]
#define LCDC_CTL_FIFO1_RST	24	// [24]
#define LCDC_CTL_SRC_SEL	25	// [26:25]
#define LCDC_CTL_IF		28	// [28:29]
#define LCDC_CTL_LCD_EN		30	// [30]
#define LCDC_CTL_EN		31	// [31]


lcdc_cfg_panel(__LCDVTC_0, pc)
	__LCDVTC_1;
	_lcdc_cfg_panel *pc;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_CTL);

	__LCDVTC_OUTP(LCDC_REG_CTL, 
		__LCDVTC_INP(LCDC_REG_CTL) & ~(0x3fffff << LCDC_CTL_LCD_Y | 3<<LCDC_CTL_IF) |
		pc->lcd_x - 1 << LCDC_CTL_LCD_X |
		pc->lcd_y - 1 << LCDC_CTL_LCD_Y |
		pc->lcd_swap << LCDC_CTL_SWAP |
		pc->lcd_if << LCDC_CTL_IF |
		1 << LCDC_CTL_EN);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_CTL);

#ifdef __DEBUG_LCDC__
	printk("lcdc_reg_ctl -> [%08xh]\n", __LCDVTC_INP(LCDC_REG_CTL));
#endif
	__LCDVTC_END
}

#define LCDC_BASIC0_UF		31
#define LCDC_BASIC0_HT		10
#define LCDC_BASIC0_HBP		0
//#define LCDC_BASIC0_MSK		~0x7fe00000

#define LCDC_BASIC1_VT		10
#define LCDC_BASIC1_VBP		0
//#define LCDC_BASIC1_MSK		~0xffe00000

lcdc_cfg_basic(__LCDVTC_0, bx)
	__LCDVTC_1;
	_lcdc_basic *bx;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_BASIC);

	// we ignore the reserved values.
	// I think it's useless to be mask, if you have an idea, plz...
	__LCDVTC_OUTP(LCDC_REG_BASIC0,
		bx->lcd_uf << LCDC_BASIC0_UF | 
		bx->lcd_ht << LCDC_BASIC0_HT |
		bx->lcd_hbp << LCDC_BASIC0_HBP);

	__LCDVTC_OUTP(LCDC_REG_BASIC1,
		bx->lcd_vt << LCDC_BASIC1_VT |
		bx->lcd_vbp << LCDC_BASIC1_VBP);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_BASIC);
#ifdef __DEBUG_LCDC__
	printk("lcdc_reg_basic -> [%08xh, %08xh]\n", __LCDVTC_INP(LCDC_REG_BASIC0), __LCDVTC_INP(LCDC_REG_BASIC1));
#endif
	__LCDVTC_END
}

lcdc_cfg_srgb(__LCDVTC_0, srgb)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_SRGB);

	__LCDVTC_OUTP(LCDC_REG_SRGB, srgb);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_SRGB);
	__LCDVTC_END
}

//
// hv interface have a multi-purpose i/o line.
// it means, 
// 0 - LCD Data Enable(the LCD_DE line)
// 1 - Output Line (connected with LCD_SCDE(CS,td043m), low-active)
//
lcdc_hvif_mpio_cfg(__LCDVTC_0, sel)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_MODE);

	__LCDVTC_OUTP(LCDC_REG_MODE, __LCDVTC_INP(LCDC_REG_MODE) & ~(1<<LCDC_HV_LDE_O_SEL) |
		sel << LCDC_HV_LDE_O_SEL);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_MODE);
	__LCDVTC_END
}

lcdc_hvif_mpio_outp(__LCDVTC_0, v)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_MODE);

	__LCDVTC_OUTP(LCDC_REG_MODE, __LCDVTC_INP(LCDC_REG_MODE) & ~(1<<LCDC_HV_LDE_OUTPUT_VALUE) |
		v << LCDC_HV_LDE_OUTPUT_VALUE);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_MODE);
	__LCDVTC_END
}

lcdc_getwidth(__LCDVTC_0)
	__LCDVTC_1;
{
// for read operation, we dont use lock to sync.
// cuz, if you read ahead, you will read a precious value, so on.
	__LCDVTC_BEGIN

	return (__LCDVTC_INP(LCDC_REG_CTL) >> LCDC_CTL_LCD_X & 1023) + 1;

	__LCDVTC_END
}

lcdc_getheight(__LCDVTC_0)
	__LCDVTC_1;
{
// for read operation, we dont use lock to sync.
// cuz, if you read ahead, you will read a precious value, so on.
	__LCDVTC_BEGIN

	return (__LCDVTC_INP(LCDC_REG_CTL) >> LCDC_CTL_LCD_Y & 1023) + 1;

	__LCDVTC_END
}
