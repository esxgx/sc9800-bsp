#include "de.h"

#define FONT_WIDTH	8
#define FONT_HEIGHT	12

#define __DIX(x)	((x)*FONT_WIDTH)
#define __DIY(x)	((x)*FONT_HEIGHT)
#define __FNX(x)	((x)/FONT_WIDTH)
#define __FNY(x)	((x)/FONT_HEIGHT)


static unsigned disp_x, disp_y, fnt_x, fnt_y, disp_n, disp_bpp, disp_pitch;
static char *disp_buf[2], *disp_cur;

static int disp_v;

kmsg_disp_callback(int n, char *s)
{
	while(n) {
		kmsg_disp_putch(*s++);
		--n;
	}
}

#include "suniip.h"


void *memset(void *, int, unsigned);

kmsg_disp_setup0(buf, n, v)
	char *buf;
{	
	disp_x = lcdc_getwidth(LCDC_REGS_BASE);
	disp_y = lcdc_getheight(LCDC_REGS_BASE);

	fnt_x = __FNX(disp_x);
	fnt_y = __FNY(disp_y);

	printk("disp width = %u height = %u\n", disp_x, disp_y);

	de_layer_resize(DISE_REGS_BASE, n, (__rectsz_t){disp_x, disp_y});
	de_layer_src_cfg(DISE_REGS_BASE, n, DE_LAYER_SRC_INDEX1LSB);

	de_layer_calc(DISE_REGS_BASE, n);
	disp_pitch = _de_layer_pitch(DISE_REGS_BASE, n);

	disp_cur = disp_buf[0] = buf;
	disp_buf[1] = disp_buf[0] + disp_y * disp_pitch;
	disp_bpp = _de_layer_bpp(DISE_REGS_BASE, n);

	_layer_fb fb;
	fb.buf[0] = (int)disp_cur;
	fb.dbe = 0;
	de_layer_map(DISE_REGS_BASE, n,&fb);

	memset((void *)buf, 0, disp_pitch * disp_y);
	de_layer_enable(DISE_REGS_BASE, n);

	disp_n = n;
	disp_v = v;
	kmsg_puts_signin(kmsg_disp_callback);
}

//kmsg_disp_drawpixel(n, x, y, v)
//{
//	unsigned bits = y * disp_pitch + x * disp_bpp;
//	u32 *p = (u32 *)disp_buf[n] + (bits >> 5);
//	*p = *p & ~((1<<disp_bpp) - 1 << (bits & 31)) | v << (bits & 31);
//	return 1;
//}

u8 __asc12[] = {
#include "asc12.h"
};

kmsg_disp_drawch(n, x, y, ch)
{
	char *k = (char *)&__asc12[(ch - ' ') * FONT_HEIGHT * FONT_WIDTH >> 3];		// look up the start ptr of ch 
	int i, j;
	char *p;
	p = disp_buf[n] + __DIY(y) * disp_pitch;
	for(i=0; i<12; ++i) {
		for(j=0; j<8; ++j)
			p[__DIX(x)>>3] |= disp_v * (k[i]>>j & 1) << 7 - j;
		p += disp_pitch;
	}
//	kmsg_disp_drawpixel(n, x+16-j, y+i, v & ((k[i]>>j) & 1));
}

static int cur_x, cur_y, cur_bak;

kmsg_disp_putch(ch, v)
{
	switch(ch) {
		case '\r':
			cur_x = 0;
			break;
		default:
			kmsg_disp_drawch(1, cur_x, cur_y, ch);
			kmsg_disp_drawch(0, cur_x, cur_y, ch);
			cur_x ++;
			if (cur_x < fnt_x) break;
			cur_x = 0;
		case '\n':
			cur_y ++;
			if (cur_y >= fnt_y) {
				disp_cur = disp_buf[0];
				cur_bak = 1; cur_y = 0;
			}
			memset(disp_buf[1] + disp_pitch * __DIY(cur_y), 0, FONT_HEIGHT * disp_pitch);
			if (cur_bak) {
				memset(disp_cur, 0, FONT_HEIGHT * disp_pitch);
				disp_cur += disp_pitch * FONT_HEIGHT;

				_layer_fb fb;
				fb.buf[0] = (int)disp_cur;
				fb.dbe = 0;
				de_layer_map(DISE_REGS_BASE, disp_n,&fb);				
			}
			break;
	}
}

