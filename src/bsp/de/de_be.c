#include "de.h"

// back-end registers offset
#define DE_BE_MODE_CTL_OFF					0x800			/*back-end mode control register offset*/

#define DE_BE_COLOR_CTL_OFF 				0x804			/*back-end color control register offset*/
#define DE_BE_LAYER_SIZE_OFF				0x810			/*back-end layer size register offset*/
#define DE_BE_LAYER_CRD_CTL_OFF				0x820			/*back-end layer coordinate control register offset*/

#define DE_BE_FRMBUF_WLINE_OFF 				0x840			/*back-end frame buffer line width register offset*/
#define DE_BE_FRMBUFA_ADDR_OFF				0X850			/*back-end frame buffera  address  register offset*/
#define DE_BE_FRMBUFB_ADDR_OFF				0X860  		/*back-end frame bufferb addreass  register offset*/
#define DE_BE_FRMBUF_CTL_OFF				0X870			/*back-end frame buffer control register offset*/

#define DE_BE_CLRKEY_MAX_OFF 				0x880			/*back-end color key max register offset*/
#define DE_BE_CLRKEY_MIN_OFF				0x884			/*back-end color key min register offset*/
#define DE_BE_CLRKEY_CFG_OFF 				0x888			/*back-end color key configuration register offset*/
#define DE_BE_LAYER_ATTRCTL_OFF0			0x890			/*back-end layer attribute control register0 offset*/
#define DE_BE_LAYER_ATTRCTL_OFF1			0x8a0			/*back-end layer attribute control register1 offset*/
#define DE_BE_DLCDP_CTL_OFF					0x8b0		   /*direct lcd pipe control register offset*/
#define DE_BE_DLCDP_FRMBUF_ADDRCTL_OFF		0x8b4		   /*direct lcd pipe frame buffer address control  register offset*/
#define DE_BE_DLCDP_CRD_CTL_OFF0			0x8b8			/*direct lcd pipe coordinate control  register0 offset*/
#define DE_BE_DLCDP_CRD_CTL_OFF1			0x8bc			/*direct lcd pipe coordinate control register1 offset*/
#define DE_BE_INTERFRMBUF_CTL_OFF			0x8c0			/*back-end internal frame buffer control register offset*/
#define DE_BE_HWC_CRD_CTL_OFF		   		0x8d8			/*hardware cursor coordinate control register offset*/
#define DE_BE_HWC_FRMBUF_OFF				0x8e0		   /*hardware cursor framebuffer control*/
#define DE_BE_INTE_EN_OFF					0x8f8			/*back-end interrupt enable register offset*/
#define DE_BE_INTE_STS_OFF					0x8fc			/*back-end interrupt status register offset*/

#define DE_BE_HWC_PALETTE_TABLE_ADDR_OFF	0x4000		  /*back-end hardware cursor palette table address*/
#define DE_BE_INTER_PALETTE_TABLE_ADDR_OFF	0x4400		  /*back-end internal framebuffer or direct lcd pipe palette table*/
#define DE_BE_HWC_PATTERN_ADDR_OFF			0x4800		  /*back-end hwc pattern memory block address*/
#define DE_BE_INTERNAL_FB_ADDR_OFF			0x5000		  /*back-end internal frame bufffer address definition*/
#define DE_BE_GAMMA_TABLE_ADDR_OFF			0x5000		  /*back-end gamma table address*/
#define DE_BE_PALETTE_TABLE_ADDR_OFF		0x5400		  /*back-end palette table address*/

// back-end register pointers
#define DE_BE_REG_MODE_CTL(x)				((volatile u32 *)((unsigned)(x) + DE_BE_MODE_CTL_OFF))				// back-end mode control register

#define DE_BE_REG_COLOR_CTL(x) 				((volatile u32 *)((unsigned)(x) + DE_BE_COLOR_CTL_OFF))				// back-end color control register
#define DE_BE_REG_LAYER_SIZE(x)				((volatile u32 *)((unsigned)(x) + DE_BE_LAYER_SIZE_OFF))			// back-end layer size register
#define DE_BE_REG_LAYER_CRD_CTL(x)			((volatile u32 *)((unsigned)(x) + DE_BE_LAYER_CRD_CTL_OFF))			// back-end layer coordinate control register

#define DE_BE_REG_FRMBUF_WLINE(x) 			((volatile u32 *)((unsigned)(x) + DE_BE_FRMBUF_WLINE_OFF))			// back-end frame buffer line width register
#define DE_BE_REG_FRMBUFA_ADDR(x)			((volatile u32 *)((unsigned)(x) + DE_BE_FRMBUFA_ADDR_OFF))			// back-end frame buffera address register
#define DE_BE_REG_FRMBUFB_ADDR(x)			((volatile u32 *)((unsigned)(x) + DE_BE_FRMBUFB_ADDR_OFF))  		// back-end frame bufferb addreass register
#define DE_BE_REG_FRMBUF_CTL(x)				((volatile u32 *)((unsigned)(x) + DE_BE_FRMBUF_CTL_OFF))			// back-end frame buffer control register

#define DE_BE_REG_CLRKEY_MAX(x) 			((volatile u32 *)((unsigned)(x) + DE_BE_CLRKEY_MAX_OFF))			// back-end color key max register
#define DE_BE_REG_CLRKEY_MIN(x)				((volatile u32 *)((unsigned)(x) + DE_BE_CLRKEY_MIN_OFF))			// back-end color key min register
#define DE_BE_REG_CLRKEY_CFG(x) 			((volatile u32 *)((unsigned)(x) + DE_BE_CLRKEY_CFG_OFF))			// back-end color key configuration register
#define DE_BE_REG_LAYER_ATTRCTL0(x)			((volatile u32 *)((unsigned)(x) + DE_BE_LAYER_ATTRCTL_OFF0))			// back-end layer attribute control register0
#define DE_BE_REG_LAYER_ATTRCTL1(x)			((volatile u32 *)((unsigned)(x) + DE_BE_LAYER_ATTRCTL_OFF1))			// back-end layer attribute control register1
#define DE_BE_REG_DLCDP_CTL(x)				((volatile u32 *)((unsigned)(x) + DE_BE_DLCDP_CTL_OFF))				// direct lcd pipe control register
#define DE_BE_REG_DLCDP_FRMBUF_ADDRCTL(x)	((volatile u32 *)((unsigned)(x) + DE_BE_DLCDP_FRMBUF_ADDRCTL_OFF))	// direct lcd pipe frame buffer address control register
#define DE_BE_REG_DLCDP_CRD_CTL0(x)			((volatile u32 *)((unsigned)(x) + DE_BE_DLCDP_CRD_CTL_OFF0)			// direct lcd pipe coordinate control register0
#define DE_BE_REG_DLCDP_CRD_CTL1(x)			((volatile u32 *)((unsigned)(x) + DE_BE_DLCDP_CRD_CTL_OFF1)			// direct lcd pipe coordinate control register1
#define DE_BE_REG_INTERFRMBUF_CTL(x)		((volatile u32 *)((unsigned)(x) + DE_BE_INTERFRMBUF_CTL_OFF))		// back-end internal frame buffer control register
#define DE_BE_REG_HWC_CRD_CTL(x)		   	((volatile u32 *)((unsigned)(x) + DE_BE_HWC_CRD_CTL_OFF))			// hardware cursor coordinate control register
#define DE_BE_REG_HWC_FRMBUF(x)				((volatile u32 *)((unsigned)(x) + DE_BE_HWC_FRMBUF_OFF))			// hardware cursor framebuffer control
#define DE_BE_REG_INTE_EN(x)				((volatile u32 *)((unsigned)(x) + DE_BE_INTE_EN_OFF))				// back-end interrupt enable register
#define DE_BE_REG_INTE_STS(x)				((volatile u32 *)((unsigned)(x) + DE_BE_INTE_STS_OFF))				// back-end interrupt status register

#define DE_BE_HWC_PALETTE_TABLE_ADDR(x)		((volatile u32 *)((unsigned)(x) + DE_BE_HWC_PALETTE_TABLE_ADDR_OFF))	// back-end hardware cursor palette table address
#define DE_BE_HWC_PALETTE_TABLE_SIZE		0x400																	// back-end hardware cursor palette table size
#define DE_BE_INTER_PALETTE_TABLE_ADDR(x)	((volatile u32 *)((unsigned)(x) + DE_BE_INTER_PALETTE_TABLE_ADDR_OFF))	// back-end internal framebuffer or direct lcd pipe palette table
#define DE_BE_INTER_PALETTE_TABLE_SIZE		0x400																	// back-end internal framebuffer or direct lcd pipe palette table size in bytes
#define DE_BE_HWC_PATTERN_ADDR(x)			((volatile u32 *)((unsigned)(x) + DE_BE_HWC_PATTERN_ADDR_OFF))			// back-end hwc pattern memory block address
#define DE_BE_HWC_PATTERN_SIZE				0x400
#define DE_BE_INTERNAL_FB_ADDR(x)			((volatile u32 *)((unsigned)(x) + DE_BE_INTERNAL_FB_ADDR_OFF))			// back-end internal frame bufffer address definition
#define DE_BE_INTERNAL_FB_SIZE				0x800																	/**back-end internal frame buffer size in byte*/
#define DE_BE_GAMMA_TABLE_ADDR(x)			((volatile u32 *)((unsigned)(x) + DE_BE_GAMMA_TABLE_ADDR_OFF))			// back-end gamma table address
#define DE_BE_GAMMA_TABLE_SIZE				0x400																	// back-end gamma table size
#define DE_BE_PALETTE_TABLE_ADDR(x)			((volatile u32 *)((unsigned)(x) + DE_BE_PALETTE_TABLE_ADDR_OFF))		// back-end palette table address
#define DE_BE_PALETTE_TABLE_SIZE			0x400																	// back-end palette table size in bytes


#define __DEBUG_DE__



de_layer_resize(base, n, sz)
	__rectsz_t sz;
{
	sz.val-= 0x00010001;
	DE_BE_REG_LAYER_SIZE(base)[n] = sz.val;
}

// 得到layer的宽度
_de_layer_width(base, n)
{
	return (DE_BE_REG_LAYER_SIZE(base)[n] & 0xffff) + 1;
}

// 得到layer的高度
_de_layer_height(base, n)
{
	return (DE_BE_REG_LAYER_SIZE(base)[n]>>16 & 0xffff) + 1;
}

// 移动layer的位置
de_layer_move(base, n, pos)
	__pos_t pos;
{
	DE_BE_REG_LAYER_CRD_CTL(base)[n] = pos.val;
}

// 得到layer的bpp
__inline__
_de_layer_bpp(base, n)
{
	const u8 fac[] = {
		1, 2, 4, 8, 16, 16, 16, 16, 16, 24, 32
	};
	return fac[DE_BE_REG_LAYER_ATTRCTL1(base)[n] >> 8 & (1<<5) - 1];
}

//
// although the m1 chip allows us using the bit-unit pitch, but
// in general framebuffer address & pitch both are in bytes. 
// it's easy to control.
//

// 得到layer的水平跨度(到下一行扫描线的字节数)
// in bytes
_de_layer_pitch(base, n)
{
	return DE_BE_REG_FRMBUF_WLINE(base)[n] >> 3;
}

de_layer_calc(base, n)
{
	DE_BE_REG_FRMBUF_WLINE(base)[n] =
		(_de_layer_bpp(base, n) * ((DE_BE_REG_LAYER_SIZE(base)[n] & 0xffff) + 1) + 7 >> 3) << 3;
#ifdef __DEBUG_DE__
	printk("de_layer_calc -> [%d]\n", DE_BE_REG_FRMBUF_WLINE(base)[n]);
#endif
}

// 切换渲染管线(管线在LCD里叫DE通道)
de_layer_pipe_cfg(base, n, v)
{
	DE_BE_REG_LAYER_ATTRCTL0(base)[n] = 
		DE_BE_REG_LAYER_ATTRCTL0(base)[n] & ~(1<<15) |
		v << 15;
}

#include "compiler.h"

// 注意, layer初始化时layer号 == prio号. 而芯片规定一个prio只能有一个layer
// 为了保证这点, prio不允许设置, 只能交换.
__always_inline
__de_layer_prio_cfg(base, n, v)
{
	DE_BE_REG_LAYER_ATTRCTL0(base)[n] = 
		DE_BE_REG_LAYER_ATTRCTL0(base)[n] & ~(1<<10) |
		v<<10;
}

__always_inline
__de_layer_prio_stat(base, n)
{
	return DE_BE_REG_LAYER_ATTRCTL0(base)[n]>>10 & 3;
}

// 交换两个layer的优先级
de_layer_prio_switch(base, a, b)
{
	int t;
	t = __de_layer_prio_stat(base, a);
	__de_layer_prio_cfg(base, a, __de_layer_prio_stat(base, b));
	__de_layer_prio_cfg(base, b, t);
}

_de_layer_framebuffer(base, n, d)
{
	if (d) return (DE_BE_REG_FRMBUFB_ADDR(base)[n]>>3) + 0x80000000;
	return (DE_BE_REG_FRMBUFA_ADDR(base)[n]>>3) + 0x80000000;
}

// 将framebuffer映射到layer上(即设置画布)
de_layer_map(base, n, fb)
	_layer_fb *fb;
{
//
// de是直接访问dram的，所以不使用cpu memory map的地址, 而且以bit为单位
// 而不是byte, 这点要十分注意. (这里 <<3(*8) 已经把0x8xxxxxxx的8挤掉)
//
	DE_BE_REG_FRMBUFA_ADDR(base)[n] = fb->buf[0] << 3;
	DE_BE_REG_FRMBUFB_ADDR(base)[n] = fb->buf[1] << 3;
//
// 置双缓冲标志
//
	if (fb->dbe) DE_BE_REG_FRMBUF_CTL(base)[n] |= 1;
	else DE_BE_REG_FRMBUF_CTL(base)[n] = ~1;
}

// layer源格式配置
de_layer_src_cfg(base, n, src)
	_layer_src *src;
{
	DE_BE_REG_LAYER_ATTRCTL0(base)[n] = DE_BE_REG_LAYER_ATTRCTL0(base)[n] & ~(3 << 22)
		| src->mode << 22;
	DE_BE_REG_LAYER_ATTRCTL1(base)[n] = DE_BE_REG_LAYER_ATTRCTL1(base)[n] & ~(15 << 8 | 7)
		| src->format << 8 | src->pixseq | src->br_swap<<2;
#ifdef __DEBUG_DE__
	printk("de_layer_src_cfg ->m=%d, f=%d, ps=%d, br=%d[%08xh, %08xh]\n", 
		src->mode, src->format, src->pixseq, src->br_swap,
		DE_BE_REG_LAYER_ATTRCTL0(base)[n],
		DE_BE_REG_LAYER_ATTRCTL1(base)[n]);
#endif

}

de_layer_enable(base, n)
{
	*DE_BE_REG_MODE_CTL(base) |= 1<<(n + 8);
}

de_layer_disable(base, n)
{
	*DE_BE_REG_MODE_CTL(base) &= ~(1<<(n + 8));
}

//
// 重置layer
// 注意, 一个layer关闭前如果与其他layer交换了prio必须交换回来.
//       在上层有一个prio管理器可以保障这点.
//
de_layer_reset(base, n)
{
	de_layer_disable(base, n);			// 确保关闭layer绘制

	DE_BE_REG_LAYER_ATTRCTL0(base)[n] = n<<10;
	DE_BE_REG_LAYER_ATTRCTL1(base)[n] = 0;
	// 其他待添加
}

de_startup_be(base)
{
	*DE_BE_REG_MODE_CTL(base) |= 3;
	
	de_layer_reset(base, 0); de_layer_reset(base, 1);
	de_layer_reset(base, 2); de_layer_reset(base, 3);
}
