#include "compiler.h"

static
__always_inline cp15_c1_set(m, v)
{
	register unsigned r;
	__asm__("mrc p15, 0, %0, c1, c0": "=r"(r));
	__asm__("mcr p15, 0, %0, c1, c0":: "r"((r & (~m)) | (m & ((!v) - 1))));
	return r & m;
}

// vect_lo_hi function
// 1 = vector address range is 0xFFFF0000 to 0xFFFF001C 
// 0 = vector address range is 0x00000000 to 0x0000001C. 
// At Reset, the VINITHI pin determines the value of the V bit. You can 
// write to V after Reset.
cp15c1_vect_lo_hi(v)
{
	return cp15_c1_set(1<<13, v);
}

arm_setup()
{
	disable_watch_dog();
	reposition_int_vect();
	return cp15c1_vect_lo_hi(0);
}

arm_pulldown0()
{
}

arm_pulldown(_vect)
{
	cp15c1_vect_lo_hi(_vect);
}
