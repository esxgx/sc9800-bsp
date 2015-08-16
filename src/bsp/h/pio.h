#ifndef __PIO_H
#define __PIO_H

//
//   GPIO Controller define          < maintained by Jerry >
//

// Defines for generic pin configurations
#define GPIO_INPUT					(0)
#define GPIO_OUTPUT					(1)
#define GPIO_SFN(x)					(x)

#include "frame.h"

//
// the structure represent a pin in gpio. it means you
// can use the structure to operate the pin it represents.
//
_DFa(gpio_pin)
	u32 piogrp;
	u32 n;
_DFz(gpio_pin)

#define	GPIO_GRP_A		0
#define GPIO_GRP_B		1
#define GPIO_GRP_C		2
#define GPIO_GRP_D		3
#define GPIO_GRP_E		4

#define GPIO_PORT_n(x)	(x)

//
// get the function of the gpio pin
// 0 - input	1 - output
// 2 - 7   functional pin
//
#define pioc_gpio_pin_getcfg(base, p)			\
	pioc_gpio_getcfg(base, (p)->piogrp, (p)->n)


#define pioc_gpio_pin_outp(base, p, v)			\
	pioc_gpio_outp(base, (p)->piogrp, (p)->n, v)

//
// config the pin
// 0 - input	1 - output
// 2 - 7   functional pin
//
#define pioc_gpio_cfgpin(base, p, sel)			\
	pioc_gpio_cfg(base, (p)->piogrp, (p)->n, sel)

//
// 0 - no pull	1 - pull up
// 2 - pull down(another pull up, see the datasheet in detail).
//
#define pioc_gpio_pin_pullup(base, p, m)			\
	pioc_gpio_pullup(base, (p)->piogrp, (p)->n, m)

/*
extern __s32             PIOC_Init(void);
extern __s32             PIOC_Exit(void);
extern __pioc_pinmap_t * PIOC_GetDevPinMap(__pioc_devno_t devno);
extern __s32             PIOC_DevPinsEnable(__pioc_devno_t devno, __u32 * arg);
extern __s32             PIOC_DevPinsDisable(__pioc_devno_t devno, __pCBK_t cb_check);

extern __s32             PIOC_PIOReq(__u8 piogrp, __u32 bitmap, __u8 inputoutput);
extern __s32             PIOC_PIORel(__u8 piogrp, __u32 bitmap);
extern __s32             PIOC_PIOPull(__u8 piogrp, __u32 bitmap, __u8 en_or_dis);
extern __s32             PIOC_PIOInOutSwitch(__u8 piogrp, __u32 bitmap, __u8 inputoutput, __u8 en_or_dis);
extern __u32             PIOC_PIODataIn(__u8 piogrp, __u32 bitmap);
extern __s32             PIOC_PIODataOut(__u8 piogrp, __u32 bitmap, __u32 data);

extern __s32             PIOC_EnableInt(__u32 int_no);
extern __s32             PIOC_DisableInt(__u32 int_no);
extern __s32             PIOC_SetIntMode(__u32 int_no, __u32 int_mode);
extern __u32             PIOC_QueryInt(void);
*/

#endif
