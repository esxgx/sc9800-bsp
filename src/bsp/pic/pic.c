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
// �жϿ���������Ҫlock��, ��Ϊ
//   1) lock���ƿ��жϻ���ʵ��
//   2) �жϿ�����Ҫ��֤ԭ�Ӳ���
//
#define __PIC_BEGIN
#define __PIC_END

#define __PIC_INP(x)		pic[x]
#define __PIC_OUTP(x, y)	pic[x] = (y)
#define __PIC_ORR(x, y)		pic[x] |= (y)

// ��Ϊ�ж�����ʱ����Ҫ֪���жϿ������� ������������ȫ�ֱ���
static volatile u32 *pic;

//
// without the interrupt state saved.
// ������ԭ�ж�״̬
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
// ����ԭ�ж�״̬
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

// ��ѯirq״̬
pic_irq_peek(irqno)
{
	return __PIC_INP(PIC_REG_ENABLE0 + (irqno >= 32)) >> (irqno & 31) & 1;
}

// �ָ�irqԭ״̬
pic_irq_recv(irq, irqno, v)
	
{
	__PIC_OUTP(PIC_REG_ENABLE0 + (irqno >= 32), 
		__PIC_INP(PIC_REG_ENABLE0 + (irqno >= 32)) & ~1<<(irqno & 31) |
		v << ~(1<<(irqno & 31)));
}

// �����ж�������(���ÿɲ���)
pic_mapvt(vtaddr)
{
	__PIC_OUTP(PIC_REG_VTBLBADDR, vtaddr);
	return 1;
}

// ��ȡ�ж�Դ(0~63)
_pic_irq_current()
{
	return __PIC_INP(PIC_REG_VCTR) >> 2;
}
