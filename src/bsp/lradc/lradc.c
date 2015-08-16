#include "typedef.h"
#include "lradc.h"

#define __LRADC_DEBUG__


#define __LRADC_0	base
#define __LRADC_1	int base

#define __LRADC_BEGIN	\
	volatile u32 *lradc = (u32 *)base;
#define __LRADC_END

#define __LRADC_INP(x)		lradc[x]
#define __LRADC_OUTP(x,y) 	lradc[x] = y
#define __LRADC_ORR(x,y)	lradc[x] |= y

#define __LRADC_LOCK_R_CTL	0
#define __LRADC_LOCK_R_INT	0

#define __LRADC_LOCK(x)
#define __LRADC_UNLOCK(x)


#define LRADC_REG_CTRL               0
#define LRADC_REG_INT                1
#define LRADC_REG_DATA0              2
#define LRADC_REG_DATA1              3


#define LRADC_CTL_EN			0
#define LRADC_CTL_CHANNEL_NO		1	// LRADC data channel select
#define LCADC_CTL_BAUD_RATE		2
#define LRADC_CTL_LEVELB_VOLTAGE_SET	4	// LRADC LevelB voltage setting
#define LRADE_CTL_SAMPLE_HOLD_ON	6

#define LRADC_INT_SEC_PENDING1		14
#define LRADC_INT_FST_PENDING1		13
#define LRADC_INT_DATA_PENDING1		12
#define LRADC_INT_SEC_PENDING0		10
#define LRADC_INT_FST_PENDING0		 9
#define LRADC_INT_DATA_PENDING0		 8
#define LRADC_INT_SEC_IRQ1		 6		// LRADC1 Second IRQ Enable
#define LRADC_INT_FST_IRQ1		 5		// LRADC1 First IRQ Enable
#define LRADC_INT_DATA_IRQ1		 4		// LRADC1 Channel2 Data IRQ Enable
#define LRADC_INT_SEC_IRQ0		 2		// LRADC0 Second IRQ Enable
#define LRADC_INT_FST_IRQ0		 1		// LRADC0 First IRQ Enable
#define LRADC_INT_DATA_IRQ0		 0		// LRADC0 Channel1 Data IRQ Enable

lradc_startup(__LRADC_0, n)
	__LRADC_1;
{
	__LRADC_BEGIN
	__LRADC_LOCK(__LRADC_LOCK_R_CTL);

	__LRADC_OUTP(LRADC_REG_CTRL, __LRADC_INP(LRADC_REG_CTRL) & ~3 | 1<<LRADC_CTL_EN | 1<<LRADE_CTL_SAMPLE_HOLD_ON | n<<LRADC_CTL_CHANNEL_NO);

	__LRADC_UNLOCK(__LRADC_LOCK_R_CTL);

	__LRADC_END
#ifdef __LRADC_DEBUG__
	printk("lradc_reg_ctrl: %08x\n", __LRADC_INP(LRADC_REG_CTRL));
#endif
}

lradc_set_baud_rate(__LRADC_0, baud)
	__LRADC_1;
{
	__LRADC_BEGIN
	__LRADC_LOCK(__LRADC_LOCK_R_CTL);

	__LRADC_OUTP(LRADC_REG_CTRL, __LRADC_INP(LRADC_REG_CTRL) & ~(3<<LCADC_CTL_BAUD_RATE) | 
		baud << LCADC_CTL_BAUD_RATE);
	
	__LRADC_UNLOCK(__LRADC_LOCK_R_CTL);
	__LRADC_END	
}

lradc_irq_enable(__LRADC_0, n, t)
	__LRADC_1;
{
	__LRADC_BEGIN
	__LRADC_LOCK(__LRADC_LOCK_R_INT);

	__LRADC_ORR(LRADC_REG_INT, 1 << ((n != 0) * LRADC_INT_DATA_IRQ1) + t);
//	__LRADC_ORR(LRADC_REG_INT, 31);

	__LRADC_UNLOCK(__LRADC_LOCK_R_INT);
	__LRADC_END
#ifdef __LRADC_DEBUG__
	printk("lradc_reg_int: %08x\n", __LRADC_INP(LRADC_REG_INT));
#endif
}

lradc_irq_disable(__LRADC_0, n, t)
	__LRADC_1;
{
	__LRADC_BEGIN
	__LRADC_LOCK(__LRADC_LOCK_R_INT);

	__LRADC_OUTP(LRADC_REG_INT, __LRADC_INP(LRADC_REG_INT) & ~(1<<((n != 0) * LRADC_INT_DATA_IRQ1) + t));

	__LRADC_UNLOCK(__LRADC_LOCK_R_INT);
	__LRADC_END
}

lradc_irq_peek(__LRADC_0, n, t)
	__LRADC_1;	
{
	__LRADC_BEGIN

	return __LRADC_INP(LRADC_REG_INT) >> (1<<((n != 0) * LRADC_INT_DATA_IRQ1) + t) & 1;

	__LRADC_END
}

// 0     : no irq pending
// 1 - 3 : lradc0
// 4 - 6 : lradc1
lradc_irq_ack(__LRADC_0)
	__LRADC_1;
{
	__LRADC_BEGIN

// 注意, FST的顺序一定在DATA的前面, 否则就被ignore了.
	if (__LRADC_INP(LRADC_REG_INT) & 1<<LRADC_INT_FST_PENDING0) {
		__LRADC_LOCK(__LRADC_LOCK_R_INT);
		__LRADC_ORR(LRADC_REG_INT, 1<<LRADC_INT_FST_PENDING0);
		__LRADC_UNLOCK(__LRADC_LOCK_R_INT);
		return 1;
	}

	if (__LRADC_INP(LRADC_REG_INT) & 1<<LRADC_INT_DATA_PENDING0) {
		__LRADC_LOCK(__LRADC_LOCK_R_INT);
		__LRADC_ORR(LRADC_REG_INT, 1<<LRADC_INT_DATA_PENDING0);
		__LRADC_UNLOCK(__LRADC_LOCK_R_INT);
		return 2;
	}

	if (__LRADC_INP(LRADC_REG_INT) & 1<<LRADC_INT_FST_PENDING1) {
		__LRADC_LOCK(__LRADC_LOCK_R_INT);
		__LRADC_ORR(LRADC_REG_INT, 1<<LRADC_INT_FST_PENDING1);
		__LRADC_UNLOCK(__LRADC_LOCK_R_INT);
		return 4;
	}

	if (__LRADC_INP(LRADC_REG_INT) & 1<<LRADC_INT_DATA_PENDING1) {
		__LRADC_LOCK(__LRADC_LOCK_R_INT);
		__LRADC_ORR(LRADC_REG_INT, 1<<LRADC_INT_DATA_PENDING1);
		__LRADC_UNLOCK(__LRADC_LOCK_R_INT);
		return 5;
	}
	__LRADC_END
	return 0;
}

_lradc_data0(__LRADC_0)
	__LRADC_1;
{
	__LRADC_BEGIN

	return __LRADC_INP(LRADC_REG_DATA0);

	__LRADC_END
}

_lradc_data1(__LRADC_0)
	__LRADC_1;
{
	__LRADC_BEGIN

	return __LRADC_INP(LRADC_REG_DATA1);

	__LRADC_END
}

