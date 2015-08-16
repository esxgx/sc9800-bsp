#include "pic.h"

// Offset
#define PIC_REG_VCTR          0
#define PIC_REG_VTBLBADDR     1
#define PIC_REG_PENDCLR0      2
#define PIC_REG_PENDCLR1      3
#define PIC_REG_CTRL          4
#define PIC_REG_ENABLE0       5
#define PIC_REG_ENABLE1       6
#define PIC_REG_MASK0         7
#define PIC_REG_MASK1         8
#define PIC_REG_RESPONSE0     9
#define PIC_REG_RESPONSE1     10
#define PIC_REG_FF0           11
#define PIC_REG_FF1           12
#define PIC_REG_PRIO0         13
#define PIC_REG_PRIO1         14
#define PIC_REG_PRIO2         15
#define PIC_REG_PRIO3         16

//
// 中断控制器不需要lock锁, 因为
//   1) lock机制靠中断机制实现
//   2) 中断控制器要保证原子操作
//
#define __PIC_BEGIN
#define __PIC_END

#define __PIC_INP(x)		pic[x]
#define __PIC_OUTP(x, y)	pic[x] = (y)
#define __PIC_ORR(x, y)		pic[x] |= (y)

// 因为中断来的时候需要知道中断控制器， 所以我们设置全局变量
static volatile u32 *pic;

//
// without the interrupt state saved.
// 不保护原中断状态
//

// void
pic_startup0(base)
{
	__PIC_BEGIN

	pic = (u32 *)base;

//
//	Disable all interrupts
//

	__PIC_OUTP(PIC_REG_ENABLE0, 0);
	__PIC_OUTP(PIC_REG_ENABLE1, 0);

	__PIC_OUTP(PIC_REG_MASK0, 0);
	__PIC_OUTP(PIC_REG_MASK1, 0);

	__PIC_END
}

// void
pic_shutdown0()
{
	__PIC_BEGIN

//
//	Disable all interrupts
//

	__PIC_OUTP(PIC_REG_ENABLE0, 0);
	__PIC_OUTP(PIC_REG_ENABLE1, 0);

	__PIC_OUTP(PIC_REG_MASK0, 0);
	__PIC_OUTP(PIC_REG_MASK1, 0);

	__PIC_END
}

//
// with the interrupt state saved.
// 保护原中断状态
//
pic_startup(base, o)
	u32 o[];
{
	pic = (u32 *)base;

	o[0] = __PIC_INP(PIC_REG_ENABLE0); o[1] = __PIC_INP(PIC_REG_ENABLE1);
	o[2] = __PIC_INP(PIC_REG_MASK0); o[3] = __PIC_INP(PIC_REG_MASK1);
	return pic_startup0(base);	
}

// void
pic_shutdown(i)
	u32 i[];
{
	__PIC_OUTP(PIC_REG_ENABLE0, i[0]);
	__PIC_OUTP(PIC_REG_ENABLE1, i[1]);

	__PIC_OUTP(PIC_REG_MASK0, i[2]);
	__PIC_OUTP(PIC_REG_MASK1, i[3]);
}

pic_irq_enable(irqno)
{
	__PIC_ORR(PIC_REG_ENABLE0 + (irqno >= 32) , 1<<(irqno & 31));
	printk("PIC_REG_ENABLE%d : %08xh\n", irqno >= 32, __PIC_INP(PIC_REG_ENABLE0 + (irqno >= 32)));
}

pic_irq_disable(irqno)
{
	__PIC_OUTP(PIC_REG_ENABLE0 + (irqno >= 32), 
		__PIC_INP(PIC_REG_ENABLE0 + (irqno >= 32)) & ~1<<(irqno & 31));
}

// 查询irq状态
pic_irq_peek(irqno)
{
	return __PIC_INP(PIC_REG_ENABLE0 + (irqno >= 32)) >> (irqno & 31) & 1;
}

// 恢复irq原状态
pic_irq_recv(irq, irqno, v)
	
{
	__PIC_OUTP(PIC_REG_ENABLE0 + (irqno >= 32), 
		__PIC_INP(PIC_REG_ENABLE0 + (irqno >= 32)) & ~1<<(irqno & 31) |
		v << ~(1<<(irqno & 31)));
}

// 设置中断向量表(可用可不用)
pic_mapvt(vtaddr)
{
	__PIC_OUTP(PIC_REG_VTBLBADDR, vtaddr);
	return 1;
}

// 获取中断源(0~63)
_pic_irq_current()
{
	return __PIC_INP(PIC_REG_VCTR) >> 2;
}
