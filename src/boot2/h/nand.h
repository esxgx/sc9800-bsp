#ifndef __NAND_H
#define __NAND_H

//
// Standard NAND flash commands
//
#define NAND_CMD_READ0		0
#define NAND_CMD_READ1		1
#define NAND_CMD_RNDOUT		5
#define NAND_CMD_PAGEPROG		0x10
#define NAND_CMD_READOOB		0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_RNDIN		0x85
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_RESET		0xff

// Extended commands for large page devices
#define NAND_CMD_READSTART	0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG	0x15

#include "nand_archi.h"

#include "nand_archi.h"

//
// NAND FLASH OPERATIONS
//
_DFa(nand_chip)
	// oobsize  -  size of out-of-band (OOB)
	u32	siz[4];			// 0 - blk(Bytes), 1 - page(Bytes), 2 - oob(Bytes), 3 - chip(MB)
					//  erasesize     writesize      oobsize     chipsize
	u32	__ffz[2];		// 0 - erasesize_ffs, 1 - writesize_ffs
	_NandFlashArchi archi;
_DFz(nand_chip)

_DFa(nand_bus)
	unsigned conn, numchips;
	_nand_chip nand[8];
_DFz(nand_bus)

#define _p2adr(c, x) (((u64)x) << (c)->__ffz[1])
#define _adr2p(c, x) ((x) >> (c)->__ffz[1])

#define _b2adr(c, x) (((u64)x) << (c)->__ffz[0])
#define _adr2b(c, x) ((x) >> (c)->__ffz[0])

// 转为函数在nand_rbmode.c内
//#define _cal_real_chip(c,x)	((c)->c2h[x])

//
//nand_cal_real_rb(n)
//{
//	if (c->numchips > 2) return (n >> 1) & 1;
//	return n;
//}

//#define _cal_real_rb(c, x) ((x) >> ((c)->numchips > 2) & 1)
// 转为函数在nand_rbmode.c内
//#define _cal_real_rb(c, x) ((x) >> ((c)->cmRb > 0) & 1)

#endif
