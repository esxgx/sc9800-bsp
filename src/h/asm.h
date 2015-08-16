// GNU AS(ARM)平台头定义
#ifndef __ASM_H
#define __ASM_H

// 入口点引用
#ifndef EXT_C
//#define EXT_C(x)	_ ## x
#define EXT_C(x)	x
#endif

// ASM入口点定义
#ifndef __CDECL
#define __CDECL(x)	.globl x; x:
#endif

#endif // !asm.h
