
#include "ccmu.h"

#define _PWR_(x,y,z)	\
powerup_##x(base) {	\
	ccmu_clkon(base, y, z);	\
}	\
powerdown_##x(base) {	\
	ccmu_clkoff(base, y, z);\
}

_PWR_(sdram, CCMU_CLK_AHB, CCMU_AHB_CLK_SDRAM)
