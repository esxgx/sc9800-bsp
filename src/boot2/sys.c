#include "trap.h"


syscall_handler(_trap_frame *tf)
{
	trap_frame_debug("syscall", tf);
	return 1;
}