
#include "ccmu.h"

#define _PWR_(x,y,z)	\
powerup_##x(base) {	\
	ccmu_clkon(base, y, z);	\
}	\
powerdown_##x(base) {	\
	ccmu_clkoff(base, y, z);\
}

_PWR_(nfc, CCMU_CLK_AHB, CCMU_AHB_CLK_NAND)
_PWR_(ahb_de, CCMU_CLK_AHB, CCMU_AHB_CLK_DE)
_PWR_(usb0, CCMU_CLK_AHB, CCMU_AHB_CLK_USB0)
_PWR_(ahb_lcd, CCMU_CLK_AHB, CCMU_AHB_CLK_LCD)
_PWR_(dram_debe, CCMU_CLK_DRAM, CCMU_DRAM_CLK_DEBE)
_PWR_(dram_defe, CCMU_CLK_DRAM, CCMU_DRAM_CLK_DEFE)

// _PWR_(ir, CCMU_CLK_APB, CCMU_APB_CLK_IR)

// uart
_PWR_(uart0, CCMU_CLK_APB, CCMU_APB_CLK_UART0)
_PWR_(uart1, CCMU_CLK_APB, CCMU_APB_CLK_UART1)
_PWR_(uart2, CCMU_CLK_APB, CCMU_APB_CLK_UART2)
_PWR_(uart3, CCMU_CLK_APB, CCMU_APB_CLK_UART3)


//_PWR_(pio, CCMU_CLK_APB, CCMU_APB_CLK_PIO)
_PWR_(twi0, CCMU_CLK_APB, CCMU_APB_CLK_TWI0)

powerup_lcd(base)
{
	ccmu_clkon(base, CCMU_CLK_PLL, CCMU_PLL_CLK_VIDEO);
	printk("videoclk_freq = %dHz\n", __sclk_videoclk_freq(base));
	powerup_ahb_lcd(base);	
}

powerdown_lcd(base)
{
}

powerup_de(base)
{
	powerup_ahb_de(base);
	powerup_dram_debe(base);
//	powerup_dram_defe(base);

}


powerdown_de(base)
{
}