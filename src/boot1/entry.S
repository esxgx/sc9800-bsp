
.globl _boot1

.arm
.text
_boot1:
	STMFD   SP!, {R0-R7,LR}

	ldr	r0, =0x80010000
	blx	r0

	@ 打结束Sig(BT1\0), 表示成功执行
	ldr	r0, =0x7210
	mov	r4, #'B'
	str	r4, [r0]
	mov	r4, #'T'
	str	r4, [r0, #1]
	mov	r4, #'0'
	str	r4, [r0, #2]
	mov	r4, #'\0'
	str	r4, [r0, #3]
	LDMFD   SP!, {R0-R7,PC}
