//
// Parameter                 Symbol             Panel Resolution               Unit
//                	                800xRGBx480  640xRGBx480  480xRGBx272
// NCLK Frequency            FNCLK        33.2         25.2         9          MHz 
// Horizontal valid data     thd          800          640          480        NCLK 
// 1 Horizontal Line         th           1056         800          525        NCLK          (lcd_ht + 1)
//                      Min.                           1 
// HSYNC Pulse Width    Typ. thpw                      -                       NCLK
//                      Max.                           -
// Hsync blanking            thbp         216          136          43         NCLK         (lcd_hbp + 1)
// Hsync front porch         thfp         40           24           2          NCLK
// DENB Enable Time          tep          800          640          480        NCLK
//
//
// Vertical display area     tvd          480          480          272        H 
// Vertical period time      tv           525          525          286        H            (lcd_vt / 2)
//                      Min.                           1 
// VSYNC Pulse Width    Typ. tvpw                      -                       H
//                      Max.                           - 
// Vertical blanking         tvbo         35           27           12         H            (lcd_vbp + 1)
// Vertical Front Porch      tvfpo        10           18           2          H
// Vertical blanking of DENB tvb          45           45           14         H 
//
#include "lcdc_tve.h"

_lcdc_basic td043m_bx = {
	.lcd_vt = 525 * 2,
	.lcd_ht = 1056 - 1,
	.lcd_vbp = 35 - 1,
	.lcd_hbp = 216 - 1,
	.lcd_uf = 0
};

_lcdc_cfg_hvif td043m_hvif = {
	.lcd_hv_if = 0,
	.lcd_hv_smode = 0,
	.lcd_hv_s888_if = 0,
	.lcd_hv_syuv_if = 0,
	.lcd_hv_vspw = 0,
	.lcd_hv_hspw = 0
};

_lcdc_cfg_panel td043m_panel = {
	.lcd_if = 0,
	.lcd_swap = 0,
	.lcd_x = 800,
	.lcd_y = 480
};

// 33Mhz
#define TD043M_DCLK_FREQ	33000000

lcd_startup_lcdc(base, vclk)
{
	lcdc_startup(base);
	lcdc_cfg_panel(base, &td043m_panel);
	lcdc_cfg_hv(base, &td043m_hvif);
	lcdc_cfg_basic(base, &td043m_bx);
	lcdc_cfg_ioctl(base, &(int []){0,0});

	lcdc_cfg_dclk_div(base, vclk / TD043M_DCLK_FREQ);
}

lcd_startup_panel(base)
{
	tpo_td043m_poweron();
	PWM_Open();
	lcdc_tcon0_on(base);
	lcdc_dclk_on(base);
	tpo_td043m_cfg();
}

lcd_shutdown(base)
{
	tpo_td043m_standby();
	lcdc_tcon0_off(base);
	tpo_td043m_poweroff();
	lcdc_shutdown(base);
}
