// GNU AS(ARM)ƽ̨ͷ����
#ifndef __ASM_H
#define __ASM_H

// ��ڵ�����
#ifndef EXT_C
//#define EXT_C(x)	_ ## x
#define EXT_C(x)	x
#endif

// ASM��ڵ㶨��
#ifndef __CDECL
#define __CDECL(x)	.globl x; x:
#endif

#endif // !asm.h
