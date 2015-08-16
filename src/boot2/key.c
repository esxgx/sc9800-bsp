#include "pic.h"
#include "suniip.h"
#include "trap.h"

int keymap[64];
unsigned keycount;


key_isr(irqno, tf)
	_trap_frame *tf;
{
	int c;
	while(c = lradc_irq_ack(LRAC_REGS_BASE)) {
		printk("lradc : %d", c);
		if (c == 2)
			printk("[%0xh, %0xh]",_lradc_data0(LRAC_REGS_BASE),_lradc_data1(LRAC_REGS_BASE));
		printk(" keycount =%d, pc=%08xh\n", keycount,tf->pc);
		++keycount;
	}
	return 1;
}

#include "lradc.h"

key_startup()
{
	keycount = 0;
	irq_signup(PIC_IRQNO_LRADC, key_isr);

	lradc_set_baud_rate(LRAC_REGS_BASE, LRADC_SAMPLE_125H);
	lradc_startup(LRAC_REGS_BASE, LRADC_CHANNEL_TWO);
	lradc_irq_enable(LRAC_REGS_BASE, 0, 0);	lradc_irq_enable(LRAC_REGS_BASE, 0, 1); lradc_irq_enable(LRAC_REGS_BASE, 0, 2);
	pic_irq_enable(PIC_IRQNO_LRADC);
}