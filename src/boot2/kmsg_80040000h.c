//
// 如果uart不能用就开启这个调试
//
#include "typedef.h"

static int ptr;

kmsg_80040000h_cb(n, s)
	char *s;
{
	while(n) {
		((u8 *)0x80030000)[ptr++] += *s++;
		--n;
	}
}

kmsg_80040000h_setup()
{
	kmsg_puts_signin(kmsg_80030000h_cb);
}
