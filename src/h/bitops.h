#ifndef __BITOPS_H
#define __BITOPS_H

//
// ffs: find first bit set. This is defined the same way as
// the libc and compiler builtin ffs routines, therefore
// differs in spirit from the above ffz (man ffs).
//
static __inline__ int generic_ffs(int x)
{
	if (!x) return 0;

//	摘自linux源代码
//	类似二分法的思想, 速度比较快log2(32)

	int r = 1;

	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

#define ffs generic_ffs
#define lowbit(x)((x) & (-(x)))

#endif
