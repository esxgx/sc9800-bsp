#include "asm.h"

@
@ it is a dummy IVT, since the low memory(0x00xx) is limited.
@ we should reduce the data to keep the system OK. and the system
@ would be slower if the data.
@
system_vect:
	b	.			@ Reset
	ldr	pc, 1f			@ Undefined instruction
	ldr	pc, 2f			@ SWI
	ldr	pc, 3f			@ Prefetch Abort
	ldr	pc, 4f			@ Data Abort
	b	.			@ Reserved
	ldr	pc, 5f			@ IRQ
	ldr	pc, 6f			@ FIQ

@ 这里存放真实中断表绝对地址
1:	.long	11f
2:	.long	12f
3:	.long	13f
4:	.long	14f
5:	.long	15f
6:	.long	16f
system_vect_end:

11:	stmfd	sp!, {r0-r12, r14}
	ldr	r14, _jumptable + 0
	b	19f
12:	stmfd	sp!, {r0-r12, r14}
	ldr	r14, _jumptable + 4
	b	19f
13:	stmfd	sp!, {r0-r12, r14}
	ldr	r14, _jumptable + 8
	b	19f
14:	stmfd	sp!, {r0-r12, r14}
	ldr	r14, _jumptable + 12
	b	19f
15:	stmfd	sp!, {r0-r12, r14}
	ldr	r14, _jumptable + 16
@ 建立trap_frame
19:	sub	sp, sp, #12
	mov	r0, sp
	mrs	r3, spsr
	stmia	r0, {r3,r13-r14}^
	bx	lr

@ fiq先不实现
16:	b	16b

_jumptable:
	.long	arm_undefined_handler		@ Undefined instruction
	.long	arm_syscall_handler		@ SWI
	.long	arm_prefetch_abort_handler	@ Prefetch Abort
	.long	arm_data_abort_handler		@ Data Abort
	.long	arm_irq_handler			@ Reserved
	.long	arm_irq_handler			@ 在m1处理器中, 只有irqno=0是fiq, 
						@ 而处理方式如同irq


__CDECL(reposition_int_vect)
	STMFD   SP!, {R4,R5,LR}
	MOV     LR, #system_vect_end - system_vect
	ldr     R0, =system_vect
	MOV	R1, #0
	MOVS	R2, LR,LSR#2
	BEQ	2f
1:	LDR	R4, [R0],#4
	STR	R4, [R1],#4
	SUBS	R2, R2, #1
	BNE	1b
2:	ANDS    R2, LR, #0x3
	BEQ	4f
3:	LDRB    R4, [R0],#1
	STRB    R4, [R1],#1
	SUBS    R2, R2, #1
	BNE	3b
4:	LDMFD   SP!, {R4,R5,PC}


__CDECL(arm_irq_handler)
	bl	EXT_C(irq_handler)
	ldr	r1, [sp], #12
	msr	spsr_cxsf, r1
	ldmfd	sp!, {r0-r12, pc}^

__CDECL(arm_syscall_handler)
	bl	EXT_C(syscall_handler)
	ldr	r1, [sp], #16		@ 丢弃原r0, 将返回值保留
	msr	spsr_cxsf, r1
	ldmfd	sp!, {r1-r12, pc}^
