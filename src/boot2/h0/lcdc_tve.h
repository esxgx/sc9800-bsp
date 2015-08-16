#ifndef _LCDC_TVE_H
#define _LCDC_TVE_H

#include "frame.h"

//
// m1处理器是tve/lcdc集成在一起的, 在命名时我们有必要区分(因为要获得
// 有效操作的最大化).
//     对于lcdc的操作，结构上用lcdc_命名, 操作用lcdc_命名.
//     对于tv timing controller的操作, 结构上用vtc_命名, 操作上用vtc_命名
//     对于两者的共同操作, 结构上用lcdtv_命名, 操作上用lcdvtc_命名(注意不同点)
//

_DFa(lcdc_basic)
	u16	lcd_vt;
	u16	lcd_ht;
	u16	lcd_vbp;
	u16	lcd_hbp;
	u8	lcd_uf;		// for vtc, this value is unused.
				// to be compatible with vtc, this value is moved to the end.
_DFz(lcdc_basic)

_DFa(lcdc_cfg_hvif)
	u8	lcd_hv_if;
	u8	lcd_hv_smode;
	u8	lcd_hv_s888_if;
	u8	lcd_hv_syuv_if;
	u16	lcd_hv_vspw;
	u16	lcd_hv_hspw;
_DFz(lcdc_cfg_hvif)

_DFa(lcdc_cfg_panel)
	u8	lcd_if;
	u8	lcd_swap;
	u16	lcd_x, lcd_y;
_DFz(lcdc_cfg_panel)



/*
_DFa(lcd_cfg)
	u8	lcd_if;
	u8	lcd_swap;
	u16	lcd_x;
	u16 lcd_y;

// reserved
//	u8	lcd_ccamp;		// unknown(1<<12)
//	u8	lcd_ccamn;		// unknown(1<<8)
	u8	lcd_dclk_freq;

	u8	sta_dly;

	_basic_lcdtv lcd_basic;

//	lcd_hv
	u8	lcd_hv_if;
	u8	lcd_hv_smode;
	u8	lcd_hv_s888_if;
	u8	lcd_hv_syuv_if;
	u8	lcd_hv_vspw;
	u16	lcd_hv_hspw;

	u8	lcd_hv_lde_used;
	u8	lcd_hv_lde_iovalue;

	u32     lcd_csc[4];
	
	u8	lcd_cpu_if;
	u8	lcd_cpu_da;
	u8	lcd_cpu_vysnc_cs;

	u32	lcd_io_cfg0;
	u32	lcd_io_cfg1;
_DFz(lcd_cfg)
*/
#endif
