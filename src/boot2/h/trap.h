#ifndef __TRAP_H
#define __TRAP_H

#include "frame.h"

#define _arm_fault_frame	_arm_trap_frame
#define _trap_frame		_arm_trap_frame

_DFa(arm_trap_frame)
	u32 spsr;
	u32 usp;
	u32 ulr;
	u32 r[13];
	u32 pc;
_DFz(arm_trap_frame)


#define cli() __asm__ volatile("mrs r0, cpsr; orr r0, #3<<6; msr cpsr_c, r0":::"r0")
#define sti() __asm__ volatile("mrs r0, cpsr; bic r0, #3<<6; msr cpsr_c, r0":::"r0")

#endif