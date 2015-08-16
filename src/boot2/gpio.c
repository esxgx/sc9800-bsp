#include "pio.h"
#include "suniip.h"

#define __delay(x)	({	\
	int ee = (x);			\
		while(ee) --ee;		\
	ee;})

///// control white LED( 0 - RST, 1 - EN)
/*gpio_LCDIO(n)
{
	_gpio_pin p;
	// enable PIOC10 for lcd_rst
	if (pioc_gpio_cfg(PIOC_REGS_BASE, GPIO_GRP_C, GPIO_PORT_n(10 + n), GPIO_OUTPUT)) {
		pioc_gpio_outp(PIOC_REGS_BASE, GPIO_GRP_C, GPIO_PORT_n(10 + n), 1);
		return 1;
	}
	return 0;
}

gpio_LCDIO_Startup()
{
	gpio_LCDIO(0); __delay(400);
	gpio_LCDIO(1); __delay(400);
	return 1;
}*/

#define gpio_LCDx(n)	\
	pioc_gpio_cfg(PIOC_REGS_BASE, GPIO_GRP_D, GPIO_PORT_n(n), GPIO_SFN(2))

gpio_DE_LCDC_Startup()
{

	gpio_LCDx( 0); gpio_LCDx( 1); gpio_LCDx( 2); gpio_LCDx( 3); gpio_LCDx( 4); gpio_LCDx( 5); 
	gpio_LCDx( 6); gpio_LCDx( 7); gpio_LCDx( 8); gpio_LCDx( 9); gpio_LCDx(10); gpio_LCDx(11); 
	gpio_LCDx(12); gpio_LCDx(13); gpio_LCDx(14); gpio_LCDx(15); gpio_LCDx(16); gpio_LCDx(17); 
	gpio_LCDx(18); gpio_LCDx(19); gpio_LCDx(20); gpio_LCDx(21); gpio_LCDx(22); gpio_LCDx(23);
	gpio_LCDx(24); gpio_LCDx(25); gpio_LCDx(26); gpio_LCDx(27);
}

gpio_twi0()
{
	pioc_gpio_cfg(PIOC_REGS_BASE, GPIO_GRP_B, GPIO_PORT_n(6), GPIO_SFN(2));
	pioc_gpio_cfg(PIOC_REGS_BASE, GPIO_GRP_B, GPIO_PORT_n(7), GPIO_SFN(2));
}

gpio_uart0()
{
	pioc_gpio_muldrv(PIOC_REGS_BASE, GPIO_GRP_B, GPIO_PORT_n(2), 2);
	pioc_gpio_muldrv(PIOC_REGS_BASE, GPIO_GRP_B, GPIO_PORT_n(4), 2);

	pioc_gpio_cfg(PIOC_REGS_BASE, GPIO_GRP_B, GPIO_PORT_n(2), GPIO_SFN(4));
	pioc_gpio_cfg(PIOC_REGS_BASE, GPIO_GRP_B, GPIO_PORT_n(4), GPIO_SFN(4));
}

#define gpio_NFx(n)	\
	pioc_gpio_cfg(PIOC_REGS_BASE, GPIO_GRP_C, GPIO_PORT_n(n), GPIO_SFN(2))

gpio_nand()
{
	gpio_NFx( 0); /* WE# */ gpio_NFx( 1); /* ALE */ gpio_NFx( 2); /* CLE */
	gpio_NFx( 3); /* CE3 */ gpio_NFx( 4); /* CE2 */ gpio_NFx( 5); /* CE1 */ gpio_NFx( 6); /* CE0 */
	gpio_NFx( 7); /* RD# */
	gpio_NFx( 8); /* RB0 */ gpio_NFx( 9); /* RB1 */
	gpio_NFx(12); /*  D0 */ gpio_NFx(13); /*  D1 */ gpio_NFx(14); /*  D2 */ gpio_NFx(15); /*  D3 */
	gpio_NFx(16); /*  D4 */ gpio_NFx(17); /*  D5 */ gpio_NFx(18); /*  D6 */ gpio_NFx(19); /*  D7 */
}
