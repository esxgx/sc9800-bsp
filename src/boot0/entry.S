
#include "asm.h"

#define _A(x)	_ ## EXT_C(x)

.extern EXT_C(_boot0)
.arm
.text
__CDECL(_boot0)
1:	STMFD   SP!, {LR}

	.extern _bss, _bss_end
	@ ���δ��ʼ����BSS����
	mov	r2, #0
	ldr	r0, =_bss
	ldr	r1, =_bss_end		@ ע��: _bss_end�Ѳ�����bss��
2:	cmp	r0, r1
	strcc	r2, [r0], #4
	bcc	2b

	bl	EXT_C(boot0)

	@ �����Sig(BT0\0), ��ʾ�ɹ�ִ��
	ldr	r0, =0x7210
	mov	r4, #'B'
	str	r4, [r0]
	mov	r4, #'T'
	str	r4, [r0, #1]
	mov	r4, #'0'
	str	r4, [r0, #2]
	mov	r4, #'\0'
	str	r4, [r0, #3]
	LDMFD   SP!, {PC}

__CDECL(gpio_sstl_2)
	LDR	R1, =0x1C20800
	LDR	R0, [R1,#0x9C]
	ORR	R0, R0, #0x10000
	STR	R0, [R1,#0x9C]
	BX	LR