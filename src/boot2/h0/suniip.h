#ifndef _SUNIIP_H
#define _SUNIIP_H

#include "sunii.h"

#undef DISE_REGS_BASE
#undef NAFC_REGS_BASE
#undef PIOC_REGS_BASE
#undef CCMU_REGS_BASE
#undef INTC_REGS_BASE
#undef LCDC_REGS_BASE
#undef LRAC_REGS_BASE
#undef TWIC0_REGS_BASE

#define DISE_REGS_BASE		__SPx(___DISE)
#define NAFC_REGS_BASE		__SPx(___NAFC)
#define PIOC_REGS_BASE		__SPx(___PIOC)
#define CCMU_REGS_BASE		__SPx(___CCMU)
#define INTC_REGS_BASE		__SPx(___INTC)
#define LCDC_REGS_BASE		__SPx(___LCDC)
#define LRAC_REGS_BASE		__SPx(___LRAC)
#define TWIC0_REGS_BASE		__SPx(___TWIC0)

// uart
#define UART0_REGS_BASE		__SPx(___UART0)
#define UART1_REGS_BASE		__SPx(___UART1)
#define UART2_REGS_BASE		__SPx(___UART2)
#define UART3_REGS_BASE		__SPx(___UART3)

#endif
