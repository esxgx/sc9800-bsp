#ifndef __NFC_H
#define __NFC_H

#include "frame.h"

// 此格式不完全兼容epos
_DFa(nfc_cmd_list)
	// 原结构多4个字节 next
	// 7.22 较epDK原结构有较大改动
	u8	*addr/*NOTE: []*/;	// 指向要访问的 nand flash 内部的物理地址的指针
	union {
		struct {
			u8	addr_cycle;		// 0-8 分别表示紧跟命令后面需要发 0-8 个 cycle 的地址
			u8	datr;			// 1 - 表示有数据传输(bytecnt有效) , 0 - 表示无数据传输
			u8	wait_rb_flag;		// 1 表示传输数据前需要等 RB ready , 0 表示不关心 RB 的状态。

////			u8	data_fetch_flag;	// 1 表示紧跟命令后需要读取或写入数据, 0 则不需要
//			u8	dma;			// 1 - 表示使用DMA(DMA Bus)  0 - 表示使用Internal RAM(AHB Bus)
////			u8	main_data_fetch;	// 1 表读取主数据区数据， 0 表示读取 spare 区数据。

		};
		u32 w2;
	};
	union {
		struct {
			u8	mode;			// 0 - 普通模式 2 - 表示页模式
			u8	dir;			// 0 - 读flash  1 - 写flash
			u8	value1;		// 对应着NFC_READ_CMD 和 NFC_PROGRAM_CMD
			u8	blks;			// 对应着NFC_REG_SECTOR_NUM(0 - 8 0到8个blks)
		};
		u32 w3;
	};
	u32 bytecnt;
	u16	value;				// 命令值
	u8	value2[2];			// 对应着NFC_RANROM_RW_CMDx	
_DFz(nfc_cmd_list)

_DFa(nfc_init_info)
	u8	bus_width;
	u8	rb_sel;
	u8	ce_ctl;
	u8	ce_ctl1;
	u8	pagesize;
	u8	serial_access_mode;
	u8	debug;
_DFz(nfc_init_info)


int nfc_chipsel(int,int);
int nfc_rbsel(int,int);

#define nfc_chiprel(x, n) (1)
#define nfc_rbrel(x,n) (1)

#endif
