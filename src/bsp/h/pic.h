#ifndef __PIC_H
#define __PIC_H

#include "frame.h"

// irq no for sunii
#define PIC_IRQNO_FIQ           0
#define PIC_IRQNO_UART0         1
#define PIC_IRQNO_UART1         2
#define PIC_IRQNO_UART2         3
#define PIC_IRQNO_UART3         4
#define PIC_IRQNO_IR            6
#define PIC_IRQNO_TWI0          7
#define PIC_IRQNO_TWI1          8

#define PIC_IRQNO_SPI0          10
#define PIC_IRQNO_SPI1          11
#define PIC_IRQNO_SPDIF         13
#define PIC_IRQNO_AC97          14
#define PIC_IRQNO_TS            15
#define PIC_IRQNO_IIS           16

#define PIC_IRQNO_PS2           21
#define PIC_IRQNO_TIMER0        22
#define PIC_IRQNO_TIMER1        23
#define PIC_IRQNO_TIMER24       24
#define PIC_IRQNO_TIMER5        25
#define PIC_IRQNO_DMA           27
#define PIC_IRQNO_PIO           28
#define PIC_IRQNO_TP            29

#define PIC_IRQNO_ADDA          30
#define PIC_IRQNO_LRADC         31
#define PIC_IRQNO_SDMMC0        32
#define PIC_IRQNO_SDMMC1        33
#define PIC_IRQNO_SDMMC2        34
#define PIC_IRQNO_SDMMC3        35
#define PIC_IRQNO_MS            36
#define PIC_IRQNO_NAND          37
#define PIC_IRQNO_USB0          38
#define PIC_IRQNO_USB1          39

#define PIC_IRQNO_IMGSENSOR     42
#define PIC_IRQNO_TVENC         43
#define PIC_IRQNO_LCDCTRL       44
#define PIC_IRQNO_DE            45
#define PIC_IRQNO_SCLACC        46
#define PIC_IRQNO_MACC          48

#define PIC_IRQNO_SS            50
#define PIC_IRQNO_EMAC          51
#define PIC_IRQNO_SOFTIRQ0      52
#define PIC_IRQNO_SOFTIRQ1      53


// 当前活跃的中断请求
#define pic_irq_current	_pic_irq_current()

#endif