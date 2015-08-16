#ifndef __DRAM_H
#define __DRAM_H

#include "frame.h"

// �˸�ʽ��ȫ����epos
_DFa(dram_para)
	u32	base;			// dram base address
	u32	size;		     	// MΪ��λ
	u32	clk;
	u32	access_mode;
	u32	cs_num;
	u32	ddr8_remap;
	u32	type;
	u32	bwidth;
	u32	col_width;
	u32	row_width;
	u32	bank_size;
	u32	cas;
_DFz(dram_para)

#endif
