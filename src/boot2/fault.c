#include "compiler.h"
#include "trap.h"

trap_frame_debug(char *ds, _trap_frame *tf)
{
	printk("== System Trap==\ndescription: %s\n",ds);
	printk("r0  %08xh r1  %08xh r2  %08xh r3  %08xh\n", tf->r[0], tf->r[1], tf->r[2], tf->r[3]);
	printk("r4  %08xh r5  %08xh r6  %08xh r7  %08xh\n", tf->r[4], tf->r[5], tf->r[6], tf->r[7]);
	printk("r8  %08xh r9  %08xh r10 %08xh r11 %08xh\n", tf->r[8], tf->r[9], tf->r[10], tf->r[11]);
	printk("r12 %08xh usp %08xh ulr %08xh pc  %08xh\n", tf->r[12], tf->usp, tf->ulr, tf->pc);
	printk("spsr %08xh\n\n", tf->spsr);
	return 1;
}

__noreturn
arm_undefined_handler(_trap_frame *frame)
{
	trap_frame_debug("undefined",frame);
	gotofel();
	while(1) {}
}
__noreturn
arm_data_abort_handler(_trap_frame *frame)
{
	trap_frame_debug("data abort",frame);
	gotofel();
	while(1) {}
}
__noreturn
arm_prefetch_abort_handler(_trap_frame *frame)
{
	trap_frame_debug("prefetch",frame);
	gotofel();
	while(1) {}
}

//////////////////////////                   ///////////////////////////////////
////////////////////////// irq服务程序管理器 ///////////////////////////////////
//////////////////////////                   ///////////////////////////////////

_DFa(irq_handler)
	struct irq_handler *next;
	unsigned irq_no;		// irq标识号
	int (*irqsr)(int, _trap_frame *);
_DFz(irq_handler)

static _irq_handler _ih[2048];
static _irq_handler *ir_[64];
static unsigned _irp, count;

#include "pic.h"

irq_handler(_trap_frame *tf)
{
	_irq_handler *c;
	int irqno;
	tf->pc -= 4;
	irqno = pic_irq_current;			// 获取中断源
	for(c = ir_[irqno]; c != NULL; c = c->next)
		if (c->irqsr)
			if (c->irqsr(irqno, tf)) break;
	return 1;		// OK;
}

// 由于是单向链表，所以不能反注册
irq_signup(irqno, sr)
	unsigned irqno;
	int (*sr)(int, _trap_frame *);
{
	_ih[_irp].next = ir_[irqno];
	_ih[_irp].irqsr = sr;
	ir_[irqno] = &_ih[_irp];
	return ++_irp;
}
