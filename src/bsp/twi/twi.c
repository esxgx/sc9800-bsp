#include "twi.h"
#include "typedef.h"


#define __TWIC_0	base
#define __TWIC_1	int base

#define __TWIC_BEGIN	\
	volatile u32 *twic = (u32 *)base;

#define __TWIC_END

#define __TWIC_INP(x)		twic[x]
#define __TWIC_OUTP(x,y)	(twic[x] = (y))
#define __TWIC_OUTP_OR(x,y)	(twic[x] |= (y))

#define __TWIC_WAIT4U(x)  while(x)

#define __TWIC_WAIT4R(x, y) ({	\
	unsigned __ret = y;	\
	while(__ret) {		\
		if (!(x)) break;\
		__ret--;	\
	}			\
__ret;})


//#define __TWIC_DEBUG__

// Registers
#define TWIC_REG_ADDR			0
#define TWIC_REG_XADDR			1
#define TWIC_REG_DATA			2
#define TWIC_REG_CTL			3
#define TWIC_REG_STATUS			4
#define TWIC_REG_CLKR			5
#define TWIC_REG_SRST			6
#define TWIC_REG_EFR			7

// 0 - failed, 1 or more - success
twic_startup(__TWIC_0)
	__TWIC_1;
{
	__TWIC_BEGIN

	__TWIC_OUTP(TWIC_REG_SRST, 1);
	return __TWIC_WAIT4R(__TWIC_INP(TWIC_REG_SRST) & 1, 0xffff);

	__TWIC_END
}

// the TWI Sample Rate is 12Mhz
#define __TWI_F0	12000000

static
twic_clk_cfg_cse(__TWIC_0, apb_clk, clk)
	__TWIC_1;
{
	__TWIC_BEGIN

	int clk_n, clk_m;
	clk_n = ffs(apb_clk/ __TWI_F0) - 1;
	clk_m = __TWI_F0 / clk /10 - 1;
	__TWIC_OUTP(TWIC_REG_CLKR, clk_n << 0 | clk_m << 3);

#ifdef __TWIC_DEBUG_
	printk("twi_clk: %d, %d\n", clk_n, clk_m);
#endif

	__TWIC_END
}


static
twic_clk_cfg_standard(__TWIC_0, apb_clk)
	__TWIC_1;
{
	twic_clk_cfg_cse(__TWIC_0, apb_clk, 100000);
}

static
twic_clk_cfg_fast(__TWIC_0, apb_clk)
	__TWIC_1;
{
	twic_clk_cfg_cse(__TWIC_0, apb_clk, 400000);
}

int
(*__twic_clk_cfg[])() = {
	twic_clk_cfg_standard,
	twic_clk_cfg_fast,
	twic_clk_cfg_cse	// cse stands for custom

};

#include <stdarg.h>

twic_clk_cfg(__TWIC_0, mode, apb_clk, clk)
	__TWIC_1;
{
	__twic_clk_cfg[mode](__TWIC_0, apb_clk, clk);
}

#include "compiler.h"

static
__always_inline
_twic_status(__TWIC_0)
	__TWIC_1;
{
	__TWIC_BEGIN
	return __TWIC_INP(TWIC_REG_STATUS);
	__TWIC_END
}

//#define __TWIC_REL
//#define __TWIC_REL	__TWIC_OUTP(TWIC_REG_CTL, __TWIC_INP(TWIC_REG_CTL) & ~(1<<3));


/* status or interrupt source */

//
// Code   Status
// 00h    Bus error
// 08h    START condition transmitted
// 10h    Repeated START condition transmitted
// 18h    Address + Write bit transmitted, ACK received
// 20h    Address + Write bit transmitted, ACK not received
// 28h    Data byte transmitted in master mode, ACK received
// 30h    Data byte transmitted in master mode, ACK not received
// 38h    Arbitration lost in address or data byte
// 40h    Address + Read bit transmitted, ACK received
// 48h    Address + Read bit transmitted, ACK not received
// 50h    Data byte received in master mode, ACK transmitted
// 58h    Data byte received in master mode, not ACK transmitted
// 60h    Slave address + Write bit received, ACK transmitted
// 68h    Arbitration lost in address as master, slave address + Write bit received, ACK transmitted
// 70h    General Call address received, ACK transmitted
// 78h    Arbitration lost in address as master, General Call address received, ACK transmitted
// 80h    Data byte received after slave address received, ACK transmitted
// 88h    Data byte received after slave address received, not ACK transmitted
// 90h    Data byte received after General Call received, ACK transmitted
// 98h    Data byte received after General Call received, not ACK transmitted
// A0h    STOP or repeated START condition received in slave mode
// A8h    Slave address + Read bit received, ACK transmitted
// B0h    Arbitration lost in address as master, slave address + Read bit received, ACK transmitted
// B8h    Data byte transmitted in slave mode, ACK received
// C0h    Data byte transmitted in slave mode, ACK not received
// C8h    Last byte transmitted in slave mode, ACK received
// D0h    Second Address byte + Write bit transmitted, ACK received
// D8h    Second Address byte + Write bit transmitted, ACK not received
// F8h    No relevant status information or no interrupt
//

char __type[] = {
	0,		/* 00h: error */
	0<<4 | 1,	/* 08h: start */
	1<<4 | 1,	/* 10h: start */
	0<<4 | 2,	/* 18h, addr send */
	1<<4 | 2,	/* 20h, addr send */
	0<<4 | 4,	/* 28h, send */
	1<<4 | 4,	/* 30h, send */
	0<<4 | 7,	/* 38h, lost */
	2<<4 | 2,	/* 40h, addr send */
	3<<4 | 2,	/* 48h, addr send*/
	0<<4 | 5,	/* 50h, recv */
	1<<4 | 5,	/* 58h, recv */
	0<<4 | 3,	/* 60h, addr recv, slv */
	0<<4 | 8,	/* 68h, addr recv, lost, slv */
	1<<4 | 3,	/* 70h, addr recv, slv */
	1<<4 | 8,	/* 78h, addr recv, lost, slv */
	2<<4 | 5,	/* 80h, recv, slv */
	3<<4 | 5,	/* 88h, recv, slv */
	4<<4 | 5,	/* 90h, recv, slv */
	5<<4 | 5,	/* 98h, recv, slv */
	6,		/* A0h, stop */
	2<<4 | 3,	/* A8h, addr recv, slv */
	2<<4 | 8,	/* B0h, addr recv, lost, slv */
	2<<4 | 4,	/* B8h, send, slv */
	3<<4 | 4,	/* C0h, send, slv */
	4<<4 | 4,	/* C8h, send, slv */
	4<<4 | 2,	/* D0h, addr send */
	5<<4 | 2,	/* D8h, addr send */
	0,		/* E0h, reserved */
	0,		/* E8h, reserved */
	0,		/* F0h, reserved */
	15,		/* F8h, no status(idle, pending) */
};

#define __TWIC_TIMEOUT		14

#define __TWIC_RECV		5
#define __TWIC_IDLE		15
#define __TWIC_WAIT		15


twic_pipe(__TWIC_0)
	__TWIC_1;
{
	__TWIC_BEGIN

	int status;
	__TWIC_OUTP_OR(TWIC_REG_CTL, 1<<5 | 1<<6);	// M_STA | BUS_EN
	if (!__TWIC_WAIT4R(!(__TWIC_INP(TWIC_REG_CTL) & 1<<3), 0xffff))
		status = __TWIC_TIMEOUT;
	else {
		status = __type[_twic_status(__TWIC_0)>>3];
#ifdef __TWIC_DEBUG__
		printk("twic_pipe: %x, %x\n", _twic_status(__TWIC_0), status);
#endif
	}
	return status;
	__TWIC_END
}

twic_connect(__TWIC_0, addr, rw)
	__TWIC_1;
{
	__TWIC_BEGIN

	int status;
	__TWIC_OUTP(TWIC_REG_DATA, addr<<1 | rw);
	__TWIC_OUTP(TWIC_REG_CTL, __TWIC_INP(TWIC_REG_CTL) & 0xf7);	// release, connect
	if (!__TWIC_WAIT4R(!(__TWIC_INP(TWIC_REG_CTL) & 1<<3), 0xffff))
		status = __TWIC_TIMEOUT;
	else {
		status = __type[_twic_status(__TWIC_0)>>3];
#ifdef __TWIC_DEBUG__
		printk("twic_connect: (%x, %x, %x)%x, %x\n", addr, rw, addr<<1 | rw, _twic_status(__TWIC_0), status);
#endif
	}
	return status;
	__TWIC_END
}

twic_close(__TWIC_0, addr)
	__TWIC_1;
{
	__TWIC_BEGIN
	int status;

	__TWIC_OUTP(TWIC_REG_CTL, __TWIC_INP(TWIC_REG_CTL) & ~(1<<6 | 1<<7 | 1<<3) | 1<<4);	// M_STP
	if (!__TWIC_WAIT4R(__TWIC_INP(TWIC_REG_CTL) & 1<<3, 0xffff))
		status = __TWIC_TIMEOUT;
	else {
		status = __type[_twic_status(__TWIC_0)>>3];
#ifdef __TWIC_DEBUG__
		printk("twic_close: %x, %x\n", _twic_status(__TWIC_0), status);
#endif
	}
	return status;
	__TWIC_END
}

twic_recv(__TWIC_0, data)
	__TWIC_1;
	u8 *data;
{
	__TWIC_BEGIN

	int status;
	__TWIC_OUTP(TWIC_REG_CTL, __TWIC_INP(TWIC_REG_CTL) & 0xf7);	// release, recv
	// 确定是一个读消息
	if (!__TWIC_WAIT4R(((status = __type[_twic_status(__TWIC_0)>>3]) & 15) != __TWIC_RECV, 0xffff))
		status = __TWIC_TIMEOUT;
	else {
		*data = __TWIC_INP(TWIC_REG_DATA);
#ifdef __TWIC_DEBUG__
		printk("twic_recv: %x, %x\n", _twic_status(__TWIC_0), status);
#endif
	}
	return status;
	__TWIC_END
}

twic_send(__TWIC_0, v)
	__TWIC_1;
{
	__TWIC_BEGIN

	int status;
	__TWIC_OUTP(TWIC_REG_DATA, v);
	__TWIC_OUTP(TWIC_REG_CTL, __TWIC_INP(TWIC_REG_CTL) & 0xf7);	// release, send
	if (!__TWIC_WAIT4R(!(__TWIC_INP(TWIC_REG_CTL) & 1<<3), 0xffff))
		status = __TWIC_TIMEOUT;
	else {
		status = __type[_twic_status(__TWIC_0)>>3];
#ifdef __TWIC_DEBUG__
		printk("twic_send: %x, %x\n", _twic_status(__TWIC_0), status);
#endif
	}

	return status;
	__TWIC_END
}

twic_host_outp(__TWIC_0, addr, data, cnt)
	char data[];
{
	__TWIC_BEGIN
	if (__TWIC_WAIT4R(__TWIC_INP(TWIC_REG_CTL) & 1<<3, 0xffff)) {
		if ((twic_pipe(__TWIC_0) & 15) == 1) {
			int c;
			if (twic_connect(__TWIC_0, addr, 0) == (0<<4 | 2)) {
				for(c=0; c<cnt; ++c)
					if (twic_send(__TWIC_0, data[c]) != (0<<4 | 4))
						break;
			} else c = 0;
			twic_close(__TWIC_0);
			return c;
		}
	}
	return 0;
	__TWIC_END
}

twic_host_inp(__TWIC_0, addr, data, cnt)
	char data[];
{
	__TWIC_BEGIN
	if (__TWIC_WAIT4R(__TWIC_INP(TWIC_REG_CTL) & 1<<3, 0xffff)) {
		if ((twic_pipe(__TWIC_0) & 15) == 1) {
			int c;
			if (twic_connect(__TWIC_0, addr, 1) == (2<<4 | 2)) {
				for(c=0; c<cnt; ++c)
					if (twic_recv(__TWIC_0, &data[c]) != (1<<4 | 5))	// not Ack transmitted
						break;
			} else c = 0;
			twic_close(__TWIC_0);
			return c;
		}
	}
	__TWIC_END
	return 0;
}
