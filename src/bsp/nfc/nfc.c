//
// Allwinner's Nand Flash Controllor
// Author: esxgx
//
#include "nfc.h"

//******************************************************************
//   Nand Flash Controller defintion
//******************************************************************
#define NFC_REG_CTL				0
#define NFC_REG_ST				1
#define NFC_REG_INT				2
#define NFC_REG_TIMING_CTL		3
#define NFC_REG_TIMING_CFG		4
#define NFC_REG_ADDR_LOW		5
#define NFC_REG_ADDR_HIGH		6
#define NFC_REG_SECTOR_NUM		7
#define NFC_REG_CNT				8
#define NFC_REG_CMD				9
#define NFC_REG_RCMD_SET		10
#define NFC_REG_WCMD_SET		11
#define NFC_REG_IO_DATA			12
#define NFC_REG_ECC_CTL			13
#define NFC_REG_ECC_ST			14
#define NFC_REG_DEBUG			15
#define NFC_REG_ECC_CNT0		16
#define NFC_REG_ECC_CNT1		17
#define NFC_REG_USER_DATA_BASE	20
#define NFC_RAM0_BASE			256
#define NFC_RAM1_BASE			512

// define bit use in NFC_CTL
#define NFC_EN					(1 << 0)
#define NFC_RESET				(1 << 1)
#define NFC_BUS_WIDYH			(1 << 2)
#define NFC_RB_SEL				(1 << 3)
#define NFC_CE_SEL				(1 << 24)
#define NFC_CE_CTL				(1 << 6)
#define NFC_CE_CTL1				(1 << 7)
#define NFC_PAGE_SIZE			(1 << 8)
#define NFC_SAM					(1 << 12)
#define NFC_RAM_METHOD			(1 << 14)
#define NFC_DEBUG_CTL			(1 << 31)

// define bit use in NFC_ST
#define NFC_RB_B2R				(1 << 0)
#define NFC_CMD_INT_FLAG		(1 << 1)
#define NFC_DMA_INT_FLAG		(1 << 2)
#define NFC_CMD_FIFO_STATUS		(1 << 3)
#define NFC_STA					(1 << 4)
#define NFC_NATCH_INT_FLAG		(1 << 5)
#define NFC_RB_STATEx(x)		(1 << (8 + (x)))
#define NFC_RB_STATE0			(1 << 8)
#define NFC_RB_STATE1			(1 << 9)
#define NFC_RB_STATE2			(1 << 10)
#define NFC_RB_STATE3			(1 << 11)

// define bit use in NFC_INT
#define NFC_B2R_INT_ENABLE		(1 << 0)
#define NFC_CMD_INT_ENABLE		(1 << 1)
#define NFC_DMA_INT_ENABLE		(1 << 2)
#define NFC_BATCH_INT_ENABLE	(1 << 5)

// define bit use in NFC_CMD
#define NFC_CMD_LOW_BYTE		(0xff << 0)
#define NFC_CMD_HIGH_BYTE		(0xff << 8)
#define NFC_ADR_NUM				(1 << 16)
#define NFC_SEND_ADR			(1 << 19)
#define NFC_ACCESS_DIR			(1 << 20)
#define NFC_DATA_TRANS			(1 << 21)
#define NFC_SEND_CMD			(1 << 22)
#define NFC_WAIT_FLAG			(1 << 23)
#define NFC_RD_CYCLE			(1 << 24)
#define NFC_SEQ					(1 << 25)
#define NFC_DATA_SWAP_METHOD	(1 << 26)
#define NFC_ROW_AUTO_INC		(1 << 27)
#define NFC_CMD_TYPE			(1 << 30)

// define bit use in NFC_RCMD_SET
#define NFC_READ_CMD			(0xff<< 0)
#define NFC_RANDOM_READ_CMD0 	(0xff << 8)
#define NFC_RANDOM_READ_CMD1 	(0xff << 16)

// define bit use in NFC_WCMD_SET
#define NFC_PROGRAM_CMD			(0xff << 0)
#define NFC_RANDOM_WRITE_CMD	(0xff << 8)

// define bit use in NFC_ECC_CTL
#define NFC_ECC_EN				(1 << 0)
#define NFC_ECC_MODE			(3 << 1)
#define NFC_ECC_PIPELINE		(1 << 3)

#define NFC_IRQ_MAJOR			13

// cmd flag bit
#define NFC_PAGE_MODE  			0x1
#define NFC_NORMAL_MODE  		0x0

#define NFC_DATA_FETCH 			0x1
#define NFC_NO_DATA_FETCH 		0x0
#define NFC_MAIN_DATA_FETCH 	0x1
#define NFC_SPARE_DATA_FETCH	0X0
#define NFC_WAIT_RB				0x1
#define NFC_NO_WAIT_RB			0x0
#define NFC_IGNORE				0x0

#define NFC_INT_RB				0
#define NFC_INT_CMD				1
#define NFC_INT_DMA				2
#define NFC_INT_BATCH			5

// 今后有任务调度会改变这个
#define nfc_wait4u(x)	while(x)

#define nfc_wait4r(x)	({	\
	unsigned __dly = 0xffff;	\
	while(__dly > 0 && (x)) --__dly;	\
__dly;})

#define _wait_cmdfifo_free(x)	nfc_wait4r(x[NFC_REG_ST] & NFC_CMD_FIFO_STATUS)

#include <string.h>

// 为了代码简洁， 采用macro
#define __NFC_R(y)	\
nfc_read_ ## y (base) {	\
	return *((u ##y *)(base + NFC_RAM0_BASE << 2));	\
}

__NFC_R(8) __NFC_R(16) __NFC_R(32)



nfc_fetchdata(int base, u8 *buf, unsigned b)
{
	volatile u8 *src = (u8 *)(base + (NFC_RAM0_BASE<<2));
//	注意不能用memcpy, 否则会出问题= =
//	memcpy(buf, (u8 *)(base + NFC_RAM0_BASE<<2), b);
	while(b) {
		*buf++ = *src++;
		--b;
	}

}


nfc_postdata(int base, u8 *buf, unsigned b)
{
	volatile u8 *dst = (u8 *)(base + (NFC_RAM1_BASE<<2));
//	注意不能用memcpy, 否则会出问题= =
//	memcpy((u8 *)(base + NFC_RAM1_BASE<<2), buf, b);
	while(b) {
		*dst++ = *buf++;
		--b;
	}
}

nfc_cmdfunc(int base, _nfc_cmd_list *c)
{
	volatile u32 *nand = (u32 *)base;


	//nfc_wait4u(nand[NFC_REG_ST] & NFC_CMD_FIFO_STATUS);	// _wait_cmdfifo_free
	_wait_cmdfifo_free(nand);

	//
	// 设定nth Cycles地址
	//

	if (c->addr_cycle) {
		u32 cmd_addr[2];

#define __cycle(a, x, r)				\
	case (x):							\
		cmd_addr[a] |= c->addr[x] << (r << 3)

		cmd_addr[0] = cmd_addr[1] = 0;
		// 虽然用循环更直观, 但这样更快。
		switch(c->addr_cycle - 1) {
			__cycle(1, 7, 3); __cycle(1, 6, 2); __cycle(1, 5, 1); __cycle(1, 4, 0);
			__cycle(0, 3, 3); __cycle(0, 2, 2); __cycle(0, 1, 1); __cycle(0, 0, 0);
		}
		nand[NFC_REG_ADDR_LOW] = cmd_addr[0]; nand[NFC_REG_ADDR_HIGH] = cmd_addr[1];
	}

	// SEND COMMAND
	u32 cmd_o;	// NFC_SEQ |
	cmd_o = NFC_SEND_CMD | c->value | (c->mode * NFC_CMD_TYPE) | (c->dir * NFC_ACCESS_DIR);	// ORDER 1 | SEND CMD | MODE | ACCESS_DIR
	if (c->addr_cycle)
		cmd_o |= NFC_SEND_ADR | ((c->addr_cycle - 1) * NFC_ADR_NUM);

//	u32 oldrm = nand[NFC_REG_CTL] & NFC_RAM_METHOD;

	if (c->mode == 2) {
		if (c->dir) nand[NFC_REG_WCMD_SET] = (c->value2[0] << 8) | c->value1;
		else nand[NFC_REG_RCMD_SET] = (c->value2[1] << 16) | (c->value2[0] << 8) | c->value1;
		nand[NFC_REG_SECTOR_NUM] = c->blks;
		cmd_o |= NFC_DATA_SWAP_METHOD;
	}
	
//	nand[NFC_REG_CTL] = (nand[NFC_REG_CTL] & ~NFC_RAM_METHOD) | (c->dma * NFC_RAM_METHOD);

	if (c->wait_rb_flag) cmd_o |= NFC_WAIT_FLAG;

	//
	// check if data_trans needed
	//
	if (c->datr) {
		cmd_o |= NFC_DATA_TRANS;
		nand[NFC_REG_CNT] = c->bytecnt;			// 这里没有问题, 因为10:31是Reserved
	}
	nand[NFC_REG_CTL] &= ~NFC_RAM_METHOD;

//	((unsigned long *)0x80020000)[5] = nand[NFC_REG_CNT];
//	((unsigned long *)0x80020000)[6] = nand[NFC_REG_CTL];
//	((unsigned long *)0x80020000)[7] = cmd_o;
//	((unsigned long *)0x80020000)[9] = nand[NFC_REG_SECTOR_NUM];
//	((unsigned long *)0x80020000)[10] = nand[NFC_REG_RCMD_SET];

//	((unsigned long *)0x80020000)[2] = cmd_o;// DEBUG
	nand[NFC_REG_CMD] = cmd_o;
	return 1;
}

#define nfc_cmd_o nfc_cmdfunc

nfc_chipsel(int base,int n)
{
	volatile u32 *nand = (u32 *)base;
	nand[NFC_REG_CTL] = (nand[NFC_REG_CTL] & (~(0x7 * NFC_CE_SEL))) | (n * NFC_CE_SEL);
	return 1;
}

nfc_rbsel(int base,int n)
{
	volatile u32 *nand = (u32 *)base;
	nand[NFC_REG_CTL] = (nand[NFC_REG_CTL] & (~(0x3 * NFC_RB_SEL))) | (n * NFC_RB_SEL);
	return 1;
}

//
// _wait_cmd_finish
//
nfc_wait_cmd_finish(int base)
{

	volatile u32 *nand = (u32 *)base;
	int ret = nfc_wait4r(!(nand[NFC_REG_ST] & NFC_CMD_INT_FLAG));
	nand[NFC_REG_ST] |= NFC_CMD_INT_FLAG;
	return ret;
}

static __inline__ __nfc_cmdfunc_common(int base, _nfc_cmd_list *c)
{
	volatile u32 *nand = (u32 *)base;

	int ret;

	ret = nfc_cmdfunc(base, c);
	if (ret) {
		ret = _wait_cmdfifo_free(nand);
		if (ret) ret = nfc_wait_cmd_finish(base);
	}
	return ret;
}

#define NFC_COMMON_F(x)	\
x (int base, _nfc_cmd_list *c)	{\
	return __nfc_cmdfunc_common(base, c);	\
}

// 对于不是单一命令的命令加"__"
NFC_COMMON_F(__nfc_read_fifo)
NFC_COMMON_F(nfc_reset_chip)
NFC_COMMON_F(__nfc_erase)
NFC_COMMON_F(nfc_status)


/*
nfc_reset_chip(int base, _nfc_cmd_list *c)
{
	volatile u32 *nand = (u32 *)base;
	nfc_cmd_o(base, c);

	//nfc_chipsel(base, n);
	//nfc_wait4u(nand[NFC_REG_ST] & NFC_CMD_FIFO_STATUS);	// _wait_cmdfifo_free

	int ret;
	ret = _wait_cmdfifo_free(nand);
	if (ret) ret = nfc_wait_cmd_finish(base);
	return ret;
}
*/


nfc_clr_rb_b2r(base)
{
	((u32 *)base)[NFC_REG_ST] |= NFC_RB_B2R;
}

nfc_chk_rb_b2r(base)
{
	return ((u32 *)base)[NFC_REG_ST] & NFC_RB_B2R;
}

// 0 - BUSY 1 - READY
nfc_check_rbready(base, n)
{
	return ((u32 *)base)[NFC_REG_ST] & NFC_RB_STATEx(n);
}

nfc_st(base)
{
	return ((u32 *)base)[NFC_REG_ST];
}

#include "compiler.h"

static __always_inline _nfc_disable(int base)
{
	volatile u32 *nand = (u32 *)base;
	nand[NFC_REG_CTL] &= ~NFC_EN;
}

// #include "suniip.h"

nfc_reset(int base)
{
	volatile u32 *nand = (u32 *)base;
	nand[NFC_REG_CTL] |= NFC_RESET;
//	memory(0x80020100, NAFC_REGS_BASE, 0x100);
//	memory(0x80020100, NAFC_REGS_BASE, 0x100);

	return nfc_wait4r(nand[NFC_REG_CTL] & NFC_RESET);
}

nfc_chmode(int base, _nfc_init_info *i)
{
	volatile u32 *nand = (u32 *)base;

	// reset controller
	if (nfc_reset(base)) {
		nand[NFC_REG_CTL] = ((i->pagesize>>2) * NFC_PAGE_SIZE) | (i->bus_width * NFC_BUS_WIDYH) | (i->ce_ctl * NFC_CE_CTL) |\
			(i->ce_ctl1 * NFC_CE_CTL1) | (i->serial_access_mode*NFC_SAM) | (i->debug * NFC_DEBUG_CTL) | (i->rb_sel * NFC_RB_SEL) |\
			NFC_EN;
		//nand[NFC_REG_TIMING_CFG] = 0xFF;
		return 1;
	}
	return 0;
}

/*
nfc_seteccmode(base, b)
{
	return ((u32 *)base)[NFC_REG_ECC_CTL] = b | (((u32 *)base)[NFC_REG_ECC_CTL] & ~6);
}*/

nfc_ecc_enable(base, pipline)
	u32 base;
{
	volatile u32 *nand = (u32 *)base;
	u32 cfg = nand[NFC_REG_ECC_CTL];
	if (pipline) cfg |= NFC_ECC_PIPELINE;
	else cfg &= ~NFC_ECC_PIPELINE;

	/*After erased, all data are 0xff, but ecc data are not 0xff, so ecc asume that is right*/
	cfg |= (1 << 4);

	//cfg |= (1 << 1); 16 bit ecc

	cfg |= NFC_ECC_EN;
	nand[NFC_REG_ECC_CTL] = cfg;

	return 1;
}

nfc_ecc_disable(base)
	u32 base;
{
	volatile u32 *nand = (u32 *)base;
	nand[NFC_REG_ECC_CTL] = nand[NFC_REG_ECC_CTL] & (~NFC_ECC_EN);
	return 1;
}


nfc_ecc_isvaild(base)
	u32 base;
{
	volatile u32 *nand = (u32 *)base;
	return nand[NFC_REG_ECC_CTL] & NFC_ECC_EN;
}

NFC_ResetINT(base)
{
	volatile u32 *nand = (u32 *)base;
	u32 o = nand[NFC_REG_INT];
	nand[NFC_REG_INT] = 0;
	return o;
}

NFC_RestoreINT(base, v)
{
	volatile u32 *nand = (u32 *)base;
	nand[NFC_REG_INT] = v;
}

nfc_init(int base, _nfc_init_info *i)
{
	// before nfc_init, u should close all interrupt
	NFC_ResetINT(base);
	nfc_ecc_disable();			// disable ECC
	return nfc_chmode(base, i);
}

nfc_exit(base)
{
	_nfc_disable(base);
}

/*
nand_reset_chip(int base, int n)
{
	nfc_chipsel(base, n);
	_nfc_cmd_list c;
	c.addr = NULL;
	c.w2 = 0; c.bytecnt = 0;
	c.value = 0xFF;
	nfc_reset_chip(base, &c);
	do nfc_rbsel(base, 0);
	nfc_wait4u(!nfc_check_rbready(base, 0));
	do nfc_rbsel(base, 1);
	nfc_wait4u(!nfc_check_rbready(base, 1));
	return 1;
}
*/

nfc_fncmd(_nfc_cmd_list *c, int command, int naddr, u8 *page_addr)
{
	static u8 def_addr[8];

	c->w2 = 0;
	c->w3 = 0;
	c->value = command;

	if (naddr) {
		if (page_addr == NULL) c->addr = def_addr;
		else c->addr = page_addr;
	}
	c->addr_cycle = naddr;

	return 1;
}

nfc_rwcmd(_nfc_cmd_list *c, int rw, int cmd, int rcmd0, int rcmd1, int blks)
{
	c->mode = 2;
	c->dir = rw;
	c->value1 = cmd;
	c->value2[0] = rcmd0; c->value2[1] = rcmd1;
	c->blks = blks;
	return 1;
}


nfc_get_flash_id(int base, _nfc_cmd_list *c, u8 *idbuf, int sz)
{
	u8 tmp5[5];
	volatile u32 *nand = (u32 *)base;

	c->bytecnt = 5;
	c->datr = 1; c->wait_rb_flag = 0;

	int ret;
	ret = __nfc_cmdfunc_common(base, c);
	nfc_fetchdata(base, idbuf, sz);

/*
	if (ret) {
		nfc_fetchdata(base, tmp5, 5);
		switch(sz) {
			case 5: idbuf[4] = tmp5[4]; 
			case 4: idbuf[3] = tmp5[3];
			case 3: idbuf[2] = tmp5[2];
			case 2: idbuf[1] = tmp5[1];
			case 1: idbuf[0] = tmp5[0];
				break;
		}
	}*/
	return ret;
}

//0xFFFFFFFF 0x00000000 0x00000000 0x00000000 0x00000000 0x8042F194
