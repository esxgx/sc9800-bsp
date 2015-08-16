#ifndef __DE_H
#define __DE_H

#include "typedef.h"

typedef union {
	struct {
		u16 width;			// [10: 0]
		u16 height;			// [26:16]
	};
	u32	val;				// [31: 0]
} __rectsz_t;	// rect size

typedef union {
	struct {
		u16 x;				// [15: 0]
		u16 y;				// [31:16]
	};
	u32 val;				// [31: 0]
} __pos_t;

// layer params
#include "port.h"

#define DE_LAYER_NORMAL_MODE		(0)
#define DE_LAYER_PALETTE_MODE		(1)
#define DE_LAYER_INTER_BUF_MODE		(2)
#define DE_LAYER_GAMMA_MODE		(3)

_DFa(layer_src)
	u8	mode;
	u8	format;
	u8	br_swap;
	u8	pixseq;
_DFz(layer_src)

#define DE_LAYER_SRC_INDEX1LSB		(&(_layer_src){0,  0, 0, 0})
#define DE_LAYER_SRC_INDEX1MSB		(&(_layer_src){0,  0, 0, 3})
#define DE_LAYER_SRC_INDEX4LSB		(&(_layer_src){0,  2, 0, 0})
#define DE_LAYER_SRC_INDEX4MSB		(&(_layer_src){0,  2, 0, 3})
#define DE_LAYER_SRC_INDEX8		(&(_layer_src){0,  3, 0, 0})
#define DE_LAYER_SRC_RGB555		(&(_layer_src){0,  4, 0, 0})
#define DE_LAYER_SRC_RGB565		(&(_layer_src){0,  5, 0, 0})
#define DE_LAYER_SRC_RGB888		(&(_layer_src){0,  9, 0, 0})
#define DE_LAYER_SRC_ARGB8888		(&(_layer_src){0, 10, 0, 0})

_DFa(layer_fb)
	u32		buf[2];
	u8		dbe;				//    [ 0] Double Buffer Mode Enable
_DFz(layer_fb)

#endif
