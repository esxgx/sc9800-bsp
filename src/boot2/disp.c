//
// 为了兼容epos的显示控制驱动, 此文件基本采用epos的命名方式
//
#include "pio.h"
#include "suniip.h"

// LCD_Bright_Open
// 		LDR     R0, =0x1C20000
// 		... (已实现)
// 		LDR     R1, [R0,#0xE0]
// 		BIC     R1, R1, #0xF0
// 		ORR     R1, R1, #0x70
// 		STR     R1, [R0,#0xE0]
// 		BX      LR
// ; End of function LCD_Bright_Open

LCD_Bright_Open()
{
	_gpio_pin p;
	// enable PIOB8 for lcd_pwm0
	if (pioc_gpio_getpin(PIOC_REGS_BASE, &p, GPIO_GRP_B, GPIO_PORT_n(8))) {
		pioc_gpio_cfgpin(PIOC_REGS_BASE, &p, GPIO_SFN(2));
		__asm__("LDR     R1, [R0,#0xE0];BIC     R1, R1, #0xF0;ORR     R1, R1, #0x70;STR     R1, [R0,#0xE0]":::"r1");
		return 1;
	}
	return 0;
}

disp_startup()
{
	int r;
	gpio_DE_LCDC_Startup();

	powerup_lcd(CCMU_REGS_BASE);
	lcd_startup_lcdc(LCDC_REGS_BASE, __sclk_videoclk_freq(CCMU_REGS_BASE));
	powerup_de(CCMU_REGS_BASE);
	de_startup_be(DISE_REGS_BASE);
	lcd_startup_panel(LCDC_REGS_BASE);
	LCD_Bright_Open();
	return 0;
}

disp_shutdown()
{
	powerdown_de(CCMU_REGS_BASE);
	lcd_shutdown(LCDC_REGS_BASE);
	powerdown_lcd(CCMU_REGS_BASE);
}

