#include "typedef.h"

#include "compiler.h"
#include "suniip.h"
#include "trap.h"

#include <stdio.h>

ccmu_info()
{
	printk("ac320clk_freq = %dHz\nahbclk_freq = %dHz\napbclk_freq = %dHz\n",
	__sclk_ac320clk_freq(CCMU_REGS_BASE), __sclk_ahbclk_freq(CCMU_REGS_BASE), __sclk_apbclk_freq(CCMU_REGS_BASE));
	return 1;
}

ccmu_dump()
{

	volatile u32 *ccmu = (u32 *)0x1c20000;
	int i;
	for(i=0; i<0xd8/4; ++i)
		((u32 *)0x80029000)[i] = ccmu[i];
}

gpio_set()
{
	volatile u32 *ccmu = (u32 *)0x1c20800;
	ccmu[8] = 0x20442424; ccmu[9] = 0x00; ccmu[10] = 0;
	ccmu[11] = 0x030640; ccmu[12] = 0xa0a8; ccmu[14] = 0xaa96a956;
	ccmu[16] = 0x22222222;ccmu[17] = 0x22222222; ccmu[18] = 0x22222222;
	ccmu[19] = 4;ccmu[22] = 0x051540;
	ccmu[23] = 0x00;  ccmu[24] = 0; ccmu[25] = 0; ccmu[26] = 0;ccmu[27] = 0;ccmu[32] = 0xaaaaa8;
}

ccmu_set()
{
	volatile u32 *ccmu = (u32 *)0x1c20000;
	ccmu[3] = 0x20C1;
	ccmu[4] = 0x30;
	ccmu[5] = 0xc10006; ccmu[6] = 0xc000; ccmu[7] = 0;
}

gpio_dump()
{

	volatile u32 *ccmu = (u32 *)0x1c20800;
	int i;
	for(i=0; i<0xa4/4; ++i)
		((u32 *)0x80028000)[i] = ccmu[i];
}

uart_dump()
{

	volatile u32 *ccmu = (u32 *)0x1c21000;
	int i;
	for(i=0; i<0xa8/4; ++i)
		((u32 *)0x8002A000)[i] = ccmu[i];
}

boot2()
{
	set_pll();
//	pio_op_for_lcd();
	arm_setup();
	powerdown_usb0(CCMU_REGS_BASE);
	cli();
	pic_startup0(INTC_REGS_BASE);
//	sti();
	int i;

	printk("boot2 starting.... & hello world :-)\r\n");

//	__sclk_coreclk_freq_cfg(CCMU_REGS_BASE, 192000000);

	printk("initialize uart module: %d\n",kmsg_uart_setup0(UART0_REGS_BASE));
//	kmsg_80020000h_setup();
	ccmu_info();

	gpio_twi0();
	powerup_twi0(CCMU_REGS_BASE);
	printk("twic_startup: %d\n", twic_startup(TWIC0_REGS_BASE));
	twic_clk_cfg(TWIC0_REGS_BASE, 0, __sclk_apbclk_freq(CCMU_REGS_BASE));		// standard clk mode

	printk("axp182: supply_status = %02xh\n", axp182_query_status());
	axp182_lcd_open();

//	i = nand_startup0();
//	if (i) printk("nands are connected, the number of chips(CEs#) = %d.\n", i);
//	else printk("cannot detect nand flash chip, error?\n");


	DE_LCDC_Init();

	printk("disp module startup : %d \n",disp_startup());

	kmsg_disp_setup0(0x80030000,0, 1);

	key_startup();

	i=0;
//	while(i<1000) printk("%d\n", i++);
//	boo21();

//	while(1);
	int ret;
	__asm__ volatile("swi #7; mov %0, r0":"=r"(ret)::"r0","lr");
	printk("syscall: %08x\n",ret);
	
	// 不恢复原状态关闭模块
	sti();

	extern volatile unsigned keycount;
	while(keycount<1000);
	
//	powerdown_lcd(CCMU_REGS_BASE);
	kmsg_uart_pulldown0();

	powerdown_usb0(CCMU_REGS_BASE);
	nand_shutdown0();
	pic_shutdown0();
	arm_pulldown0();
	// 因为usb驱动(和其他东西)已被我们关闭，所以只能这样回fel
	gotofel();
	return 0;
}

boo21()
{
	int i,d;

//	if (((unsigned long *)0x80020000)[0] < 1) return 0;

//	nand_erase(0, 0);

//	nand_read_page_fifo(0x80030000, 0, 0, 0);

//	for(i=0; i<0x4000; ++i)
//		((unsigned long *)0x80030000)[i] = 0x00;

//	for(i=1792,d=0; i<1892; i++, d+=0x2000) {		// 读1M的数据
//		nand_read_page_fifo(0x80030000 + d, 1, i, 1);

	//	nand_read_page_fifo(0x80030000, 1, i, 1);

		//if (((unsigned long *)0x80030000)[0] != 0xFFFFFFFF) {
		//	((unsigned long * volatile)0x80020000)[6] = i;
		//	return 0;
		//}

//	}


	for(i=0,d=0; i<128*10; i++, d+=0x1000){		// 读1M的数据
		nand_read_page_fifo(0x80110000 + d, 0, i, 0);
//		printk( "reading nand: page %d\n", i);
	}

	for(i=0,d=0; i<128*10; i++, d+=0x80) d+=nand_read_page_oob(0x80100000 + d, 0, i);
}