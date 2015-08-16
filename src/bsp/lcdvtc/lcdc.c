#include "lcdc_tve.h"
#include "lcdvtc_i.h"

#define LCDC_CTL_EN		31	// [31]

lcdc_startup(__LCDVTC_0)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_CTL)

	__LCDVTC_OUTP(LCDC_REG_CTL, __LCDVTC_INP(LCDC_REG_CTL) | 1<<LCDC_CTL_EN);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_CTL)
	__LCDVTC_END
}

lcdc_shutdown(__LCDVTC_0)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_CTL)

	__LCDVTC_OUTP(LCDC_REG_CTL, __LCDVTC_INP(LCDC_REG_CTL) & ~(1<<LCDC_CTL_EN));

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_CTL)
	__LCDVTC_END

}

#define LCDC_DCLK_EN	31		//    [31]
#define LCDC_DCLK_DIV	0		// [ 7: 0]

lcdc_cfg_dclk_div(__LCDVTC_0, div)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_DCLK)

	__LCDVTC_OUTP(LCDC_REG_DCLK, __LCDVTC_INP(LCDC_REG_DCLK) | div<<LCDC_DCLK_DIV);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_DCLK)
	__LCDVTC_END
}

lcdc_dclk_on(__LCDVTC_0)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_DCLK)

	__LCDVTC_OUTP(LCDC_REG_DCLK, __LCDVTC_INP(LCDC_REG_DCLK) | 1<<LCDC_DCLK_EN);
	
	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_DCLK)
	__LCDVTC_END
}

lcdc_dclk_off(__LCDVTC_0)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_DCLK)

	__LCDVTC_OUTP(LCDC_REG_DCLK, __LCDVTC_INP(LCDC_REG_DCLK) | ~(1<<LCDC_DCLK_EN));
	
	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_DCLK)
	__LCDVTC_END
}

lcdc_cfg_dclk_stat(__LCDVTC_0)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN

	return __LCDVTC_INP(LCDC_REG_DCLK) >> LCDC_DCLK_EN & 1;

	__LCDVTC_END
}


#define LCDC_CTL_LCD_EN		30	// [30]


lcdc_tcon0_on(__LCDVTC_0)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_CTL)

	__LCDVTC_OUTP(LCDC_REG_CTL, __LCDVTC_INP(LCDC_REG_CTL) | 1<<LCDC_CTL_LCD_EN);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_CTL)

#ifdef __DEBUG_LCDC__
	printk("lcdc_reg_ctl -> [%08xh]\n", __LCDVTC_INP(LCDC_REG_CTL));
#endif
	__LCDVTC_END
}


lcdc_tcon0_off(__LCDVTC_0)
	__LCDVTC_1;
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_CTL)

	__LCDVTC_OUTP(LCDC_REG_CTL, __LCDVTC_INP(LCDC_REG_CTL) & ~(1<<LCDC_CTL_LCD_EN));

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_CTL)
	__LCDVTC_END
}

lcdc_cfg_ioctl(__LCDVTC_0, ioctl)
	__LCDVTC_1;
	int ioctl[];
{
	__LCDVTC_BEGIN
	__LCDVTC_LOCK(__LCDVTC_LOCK_R_IOCTL)

	__LCDVTC_OUTP(LCDC_REG_IOCTL1, ioctl[0]);
	__LCDVTC_OUTP(LCDC_REG_IOCTL2, ioctl[1]);

	__LCDVTC_UNLOCK(__LCDVTC_LOCK_R_IOCTL)
	__LCDVTC_END
}