//
// (c) Copyright 2011, esxgx
// (c) Copyright 2008, SoftWinners Co,Ld.
//                                         All Right Reserved
// file : uart.h
// description : this file provides some physic functions for upper nand driver layer.
// history:
//	v0.11 esxgx
//	v0.1  Gary.Wang
//
#ifndef __UART_H
#define __UART_H

#define UART_PORT_COUNT     4

// UART port work mode
#define UART_WORK_MODE	0
#define ISR_WORK_MODE	1

// stop bits
#define _1_STOP_BITS                    0
#define _1_5_STOP_BITS                  1
#define _2_STOP_BITS                    2

// parity check
#define NO_PARITY                       0
#define ODD_PARITY                      1
#define EVEN_PARITY                     2

// data length
#define _5_BITS                         0
#define _6_BITS                         1
#define _7_BITS                         2
#define _8_BITS                         3

#include "frame.h"

// 完全兼容epos
_DFa(uart_para)
	u32 stop_bits;
	u32 parity;		// 校验位
	u32 char_len;
_DFz(uart_para)

#endif
