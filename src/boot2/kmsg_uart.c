#include "suniip.h"
#include "uart.h"

static int base;

#define __delay(x)	do{\
	int i;			\
	for(i=0; i<x; ++i);	\
} while(0)

kmsg_uart_puts(int n, char *s)
{
	while(n) {
		if (!uart_putc(base,*s++, 0xfff))
			break;
		--n;
	}
}

kmsg_uart_setup0(v)
{
	powerdown_uart0(CCMU_REGS_BASE);
	__delay(0xff);
	powerup_uart0(CCMU_REGS_BASE);

	gpio_uart0();
//	set_gpio_();

	// set baud rate
	if (uart_set_baud_rate(v, __sclk_apbclk_freq(CCMU_REGS_BASE), 115200)) {
		// serial mode (8 data, 1 stop, no parity)
		_uart_para para;
		para.parity = NO_PARITY;
		para.char_len = _8_BITS;
		para.stop_bits = _1_STOP_BITS;

		if (uart_startup(v, &para)) {
			base = v;
			kmsg_puts_signin(kmsg_uart_puts);
			return 1;
		}
	}
	return 0;
}

kmsg_uart_pulldown0()
{
	powerdown_uart0(CCMU_REGS_BASE);
}
