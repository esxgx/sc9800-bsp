
#include "asm.h"

#define _A(x)	_ ## EXT_C(x)

.extern EXT_C(boot2)
.arm
.text
__CDECL(_boot2)
1:	STMFD   SP!, {LR}

	str	sp, 3f
	ldr	sp, =0x80090000

.extern _bss, _bss_end
	@ 清除未初始化的BSS区域
	mov	r2, #0
	ldr	r0, =_bss
	ldr	r1, =_bss_end		@ 注意: _bss_end已不属于bss段
2:	cmp	r0, r1
	strcc	r2, [r0], #4
	bcc	2b

	@ 执行主函数
	bl	EXT_C(boot2)
	ldr	sp, 3f

	@ 这里的r0是返回值, 若r0 = 0, 要回到fel
	@ 若r0 != 0, 则直接POWER OFF

	LDMFD   SP!, {PC}

3:	.long	0

__CDECL(gotofel)
	@ 设置高向量表0xFFFF0000, 因为fel使用这个表
	mov	r0, #1
	bl	EXT_C(cp15c1_vect_lo_hi)

	@ jump to fel
	ldr	pc, =0xFFFF0020

__CDECL(disable_watch_dog)
	MOV     R0, #0
	LDR     R1, =0x1C20000
	STR     R0, [R1,#0xC1C]
	BX      LR

__CDECL(restart_watch_dog)
	LDR     R0, =0x14AF
	LDR     R1, =0x1C20000
	STR     R0, [R1,#0xC18]
	BX      LR

__CDECL(reset_sys_watch_dog)
	MOV     R0, #0
	LDR     R1, =0x1C20000
	STR     R0, [R1,#0xC1C]
	MOV     R0, #3
	STR     R0, [R1,#0xC1C]
	BX      LR
.globl pio_op_for_lcd
pio_op_for_lcd:	
	LDR     R1, =0x1C20000
	LDR     R1, [R1,#0x880]
	BIC     R0, R1, #3
	LDR     R2, =0x1C20000
	STR     R0, [R2,#0x880]
	MOV     R1, R2
	LDR     R1, [R1,#0x858]
	BIC     R0, R1, #0xF00000
	STR     R0, [R2,#0x858]
	BX      LR


.globl set_pll
set_pll:
	LDR     R1, =0x1C20000
	LDR     R2, [R1,#8]
	MOV     R3, #0x600
	BIC     R2, R2, R3
	MOV     R3, #0x200
	ORR     R2, R2, R3
	STR     R2, [R1,#8]
	LDR     R2, [R1,#0xC]
	MOV     R3, #0x40
	ORR     R2, R2, R3
	STR     R2, [R1,#0xC]
	LDR     R2, [R1,#0x10]
	MOV     R3, #0x10
	ORR     R2, R2, R3
	STR     R2, [R1,#0x10]
	LDR     R2, [R1]
	LDR     R3, =0x247F0000
	BIC     R2, R2, R3
	LDR     R3, =0x20230000
	ORR     R2, R2, R3
	STR     R2, [R1]
	MOV     R0, #0x32
loc_804001DC:
	SUBS    R0, R0, #1
	BNE     loc_804001DC
	LDR     R2, [R1,#8]
	MOV     R3, #0xD9
	BIC     R2, R2, R3
	MOV     R3, #0x48
	ORR     R2, R2, R3
	STR     R2, [R1,#8]
	BIC     R2, R2, #0x600
	ORR     R2, R2, #0x400
	STR     R2, [R1,#8]
	bx lr

__CDECL(set_gpio_)
	MOV     R0, R1
	LDR     R0, [R0,#0x838]
BIC     R0, R0, #0x330
STR     R0, [R1,#0x838]
MOV     R0, R1
LDR     R0, [R0,#0x838]
ORR     R0, R0, #0x110
STR     R0, [R1,#0x838]
MOV     R0, R1
LDR     R0, [R0,#0x820]
BIC     R0, R0, #0x70000
BIC     R0, R0, #0x700
STR     R0, [R1,#0x820]
MOV     R0, R1
LDR     R0, [R0,#0x820]
ORR     R0, R0, #0x40000
ORR     R0, R0, #0x400
STR     R0, [R1,#0x820]
	BX	LR


__CDECL(DE_LCDC_Init)

LDR     R0, =0x1C20000

LDR     R1, =0x22222222
STR     R1, [R0,#0x860]
STR     R1, [R0,#0x864]
STR     R1, [R0,#0x868]
STR     R1, [R0,#0x86C]

LDR     R1, [R0,#0x30]
ORR     R1, R1, #0x80
STR     R1, [R0,#0x30]

LDR     R1, [R0,#0xC]
ORR     R1, R1, #0x4000
ORR     R1, R1, #8
STR     R1, [R0,#0xC]
LDR     R1, [R0,#0x20]
ORR     R1, R1, #0x60000000
STR     R1, [R0,#0x20]

LDR     R1, [R0,#0x24]
LDR     R1, =0xA000A000
STR     R1, [R0,#0x24]
LDR     R1, [R0,#0x28]
ORR     R1, R1, #0x10000
STR     R1, [R0,#0x28]

LDR     R1, [R0,#0x30]
ORR     R1, R1, #0x80
STR     R1, [R0,#0x30]
BX      LR

__CDECL(PWM_Open)
LDR     R0, =4
LDRB    R0, [R0]
ORR     R1, R0, #0x60
LDR     R0, =0x1C20000
STRB    R1, [R0,#0xE0]
LDR     R1, =0xF000C
STR     R1, [R0,#0xE4]
BX	LR
