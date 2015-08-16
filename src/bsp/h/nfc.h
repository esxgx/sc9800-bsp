#ifndef __NFC_H
#define __NFC_H

#include "frame.h"

// �˸�ʽ����ȫ����epos
_DFa(nfc_cmd_list)
	// ԭ�ṹ��4���ֽ� next
	// 7.22 ��epDKԭ�ṹ�нϴ�Ķ�
	u8	*addr/*NOTE: []*/;	// ָ��Ҫ���ʵ� nand flash �ڲ��������ַ��ָ��
	union {
		struct {
			u8	addr_cycle;		// 0-8 �ֱ��ʾ�������������Ҫ�� 0-8 �� cycle �ĵ�ַ
			u8	datr;			// 1 - ��ʾ�����ݴ���(bytecnt��Ч) , 0 - ��ʾ�����ݴ���
			u8	wait_rb_flag;		// 1 ��ʾ��������ǰ��Ҫ�� RB ready , 0 ��ʾ������ RB ��״̬��

////			u8	data_fetch_flag;	// 1 ��ʾ�����������Ҫ��ȡ��д������, 0 ����Ҫ
//			u8	dma;			// 1 - ��ʾʹ��DMA(DMA Bus)  0 - ��ʾʹ��Internal RAM(AHB Bus)
////			u8	main_data_fetch;	// 1 ���ȡ�����������ݣ� 0 ��ʾ��ȡ spare �����ݡ�

		};
		u32 w2;
	};
	union {
		struct {
			u8	mode;			// 0 - ��ͨģʽ 2 - ��ʾҳģʽ
			u8	dir;			// 0 - ��flash  1 - дflash
			u8	value1;		// ��Ӧ��NFC_READ_CMD �� NFC_PROGRAM_CMD
			u8	blks;			// ��Ӧ��NFC_REG_SECTOR_NUM(0 - 8 0��8��blks)
		};
		u32 w3;
	};
	u32 bytecnt;
	u16	value;				// ����ֵ
	u8	value2[2];			// ��Ӧ��NFC_RANROM_RW_CMDx	
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
