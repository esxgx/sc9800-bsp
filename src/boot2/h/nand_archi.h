#ifndef __NAND_ARCHI_H
#define __NAND_ARCHI_H


#include "frame.h"

#define	NAND_NO_PADDING	(1<<4)
#define	NAND_CACHEPRG	(1<<5)

_DFa(NandFlashArchi)
	char	maf[8], dev[20];
	char	id[5];

	u16	SizeOfChip;	// Number of size of chip

	u16	NumOfDie;	// Number of Die
	u16	BytesOfPg;	// Number of Bytes of page
	u16	BytesOfSpare;
	u32	BytesOfBlk;

	u16	SizOfPlane;
	u16	NumOfPlanes;	// Number of Planes
	u16	BlksInRsv;	// The Number of Blocks in Reservior for Bad Blocks
	// 0-1bit	Cell type (0-SLC,1-MLC)
	// 2-3		Number of simultaneously programmed pages
	// 4		Interleaved operations between multiple die
	// 5		Cache programming
	u8	CellInfo;
	u8	BadPos;	// BadBlock Information Position
	u8	LsnPos;	// LSN Position
	u8	ECCPos;	// ECC Policy : HW_ECC, SW_ECC
	u16	BWidth;	// Nand Organization X8 or X16

	u16	SATime;	// Serial Access Time (ns)

//	u16	nTrTime;	// Typical Read Op Time
//	u16	nTwTime;	// Typical Write Op Time
//	u16	nTeTime;	// Typical Erase Op Time
//	u16	nTfTime;	// Typical Transfer Op Time

_DFz(NandFlashArchi)

#endif
