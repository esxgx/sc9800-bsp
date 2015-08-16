#include "uart.h"

//
// UART defination   (Thanks Gary)
// rewritten by esxgx
//

// offset
#define UART_REG_RBR              0x00
#define UART_REG_THR              0x00
#define UART_REG_DLL              0x00
#define UART_REG_DLH              0x04
#define UART_REG_IER              0x04
#define UART_REG_IIR              0x08
#define UART_REG_FCR              0x08
#define UART_REG_LCR              0x0C
#define UART_REG_MCR              0x10
#define UART_REG_LSR              0x14
#define UART_REG_MSR              0x18
#define UART_REG_SCH              0x1C
#define UART_REG_USR              0x7C
#define UART_REG_TFL              0x80
#define UART_REG_RFL              0x84
#define UART_REG_HALT             0xA4

// 中断状态定义
#define UART_STATUS_RXBUF_RDY              (1<<0)
#define UART_STATUS_TXBUF_EMPTY            (1<<1)
#define UART_STATUS_TX_EMPTY               (1<<2)

#define UART_STATUS_OVERRUNERR             (1<<0)
#define UART_STATUS_PARITYERR              (1<<1)
#define UART_STATUS_FRAMEERR               (1<<2)
#define UART_STATUS_BREAKDETECT            (1<<3)


// UART port type
#define UART_TYPE_INTACT                0
#define UART_TYPE_SIMPLE                1

// UART port operation mode
#define SIMPLE_POLLING                  0
#define SIMPLE_INT                      1
#define AUTO_HFC_POLLING                2
#define AUTO_HFC_INT                    3




// 中断触发方式定义
#define UART_INTTYPE_TX_MSK                (1<<9)
#define UART_INTTYPE_TX_PULSE              (0<<9)
#define UART_INTTYPE_TX_LEVEL              (0<<9)
#define UART_INTTYPE_RX_MSK                (1<<8)
#define UART_INTTYPE_RX_PULSE              (1<<8)
#define UART_INTTYPE_RX_LEVEL              (1<<8)


// 中断源定义， 用于UART_EnableINT,UART_DisableINT,UART_QueryINT接口
#define UART_INTSRC_RXBUF_READY            (1<<0)
#define UART_INTSRC_TXBUF_EMPTY            (1<<1)
#define UART_INTSRC_TX_EMPTY               (1<<2)
#define UART_INTSRC_ERR                    (1<<4)


// Line Control Register
#define UART_LCR_DLS	(1<<0)			// Data Length Select
#define UART_LCR_STB	(1<<2)			// Number of stop Bits, off = 1, on = 1.5 or 2) */
#define UART_LCR_PEN	(1<<3)			// Parity eneble
#define UART_LCR_EPS	(1<<4)			// Even Parity Select
#define UART_LCR_STKP	(1<<5)			// reserved(Stick Parity)
#define UART_LCR_SBRK	(1<<6)			// Set Break
#define UART_LCR_BKSE	(1<<7)			// Bank select enable

uart_set_baud_rate(base, apb_clk, baud)
{
	unsigned baud_divisor = (apb_clk>>4) / baud;
	volatile u8 *uart = (u8 *)base;

	// select baud registers
	uart[UART_REG_LCR] = (uart[UART_REG_LCR] & (~UART_LCR_BKSE)) | UART_LCR_BKSE;
	uart[UART_REG_DLL] = baud_divisor & 0xff;
	uart[UART_REG_DLH] = baud_divisor>>8 & 0xff;

	int i;
	for(i=0; i<0xffffff;++i);
	// restore to operation mode
	uart[UART_REG_LCR] &= ~UART_LCR_BKSE;
	return 1;
}

// Modem Control Register
#define UART_MCR_DTR		(1<<0)		// Reserved
#define UART_MCR_RTS		(1<<1)		// Reserved
#define UART_MCR_DMA_EN		(1<<2)		// Reserved
#define UART_MCR_TX_DFR		(1<<3)		// Reserved
#define UART_MCR_LOOP		(1<<4)		// Look Back Mode
#define UART_MCR_AFCE		(1<<5)		// Auto Flow Control Enable
#define UART_MCR_SIRE		(1<<6)		// SIR Mode Enable

// FIFO Control Register
#define UART_FCR_FIFO_EN	(1<<0)		// Fifo enable
#define UART_FCR_RXSR		(1<<1)		// Receiver soft reset
#define UART_FCR_TXSR		(1<<2)		// Transmitter soft reset
#define UART_FCR_DMA_MODE	(1<<3)		// DMA Mode	(0 - Mode0, 1 - Mode1)
#define UART_FCR_TFT		(1<<4)		// TX Empty Trigger
#define UART_FCR_RT		(1<<6)		// RCVR Trigger


uart_startup(base, para)
	_uart_para *para;
{
	volatile u8 *uart = (u8 *)base;
	uart[UART_REG_LCR] = para->stop_bits * UART_LCR_STB | para->char_len * UART_LCR_DLS | 
		(para->parity != NO_PARITY) * UART_LCR_PEN | (para->parity == EVEN_PARITY) * UART_LCR_EPS;
	/* ignore MCR */
	uart[UART_REG_FCR] = UART_FCR_FIFO_EN | UART_FCR_RXSR | UART_FCR_TXSR;
	return 1;
}

#define uart_wait_4_dr(base,timeout) ({		\
	int __timeout = timeout;		\
	volatile u8 *uart = (u8 *)base;		\
	while(__timeout) {			\
		if (uart[UART_REG_LSR] & 1)	\
			break;			\
		--timeout;			\
	}					\
__timeout;})

#define uart_wait_4_thre(base,timeout) ({	\
	int __timeout = timeout;		\
	volatile u8 *uart = (u8 *)base;		\
	while(__timeout) {			\
		if (uart[UART_REG_LSR] & 1<<6)	\
			break;			\
		--timeout;			\
	}					\
__timeout;})


uart_putc(base, c, timeout)
	char c;
{
	if (timeout) {
		timeout = uart_wait_4_thre(base, timeout);
		if (!timeout) return 0;
	}
	volatile u8 *uart = (u8 *)base;
	uart[UART_REG_THR] = c;
	return 1;
}

uart_getc(base, timeout)
{
	timeout = uart_wait_4_dr(base, timeout);
	volatile u8 *uart = (u8 *)base;
	if (timeout) return uart[UART_REG_RBR];
	return -1;
}

uart_check_busy(base)
{
	volatile u8 *uart = (u8 *)base;
	return (uart[UART_REG_USR] & 1);
}

#define UART_LSR_DR	0x01		// Data ready
#define UART_LSR_OE	0x02		// Overrun
#define UART_LSR_PE	0x04		// Parity error
#define UART_LSR_FE	0x08		// Framing error
#define UART_LSR_BI	0x10		// Break
#define UART_LSR_THRE	0x20		// Xmit holding register empty
#define UART_LSR_TEMT	0x40		// Xmitter empty
#define UART_LSR_ERR	0x80		// Error


// port
#define UART0_IRQ_NO                1
#define UART1_IRQ_NO                2
#define UART2_IRQ_NO                3
#define UART3_IRQ_NO                4
