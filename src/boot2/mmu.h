#ifndef __MMU_H
#define __MMU_H

// Ë¢ÐÂicache ¡¢ dcache
#define mmu_flush_id_cache() \
	__asm__("mov r0, #0; mcr p15, 0, r0,c7,c5; mcr p15, 0, r0,c7,c6;")

#endif
