#include "nand.h"

//
// nand_manufacturers - NAND Flash Manufacturer ID Structure
// @name:	Manufacturer name
// @id:		manufacturer ID code of device.
//
_DFa(_nand_manufacturers)
	int id;
	char *name;
_DFz(_nand_manufacturers)

// NAND Flash Manufacturer ID Codes
#define NAND_MFR_TOSHIBA	0x98
#define NAND_MFR_SAMSUNG	0xec
#define NAND_MFR_FUJITSU	0x04
#define NAND_MFR_NATIONAL	0x8f
#define NAND_MFR_RENESAS	0x07
#define NAND_MFR_INTEL		0x89
#define NAND_MFR_STMICRO	0x20
#define NAND_MFR_HYNIX		0xad
#define NAND_MFR_MICRON		0x2c
#define NAND_MFR_AMD		0x01

static __nand_manufacturers nand_manuf_ids[] = {
	{0x0, "Unknown"},
	{NAND_MFR_TOSHIBA, "Toshiba"},
	{NAND_MFR_SAMSUNG, "Samsung"},
	{NAND_MFR_FUJITSU, "Fujitsu"},
	{NAND_MFR_NATIONAL, "National"},
	{NAND_MFR_RENESAS, "Renesas"},
	{NAND_MFR_STMICRO, "ST Micro"},
	{NAND_MFR_HYNIX, "Hynix"},
	{NAND_MFR_INTEL, "Intel"},
	{NAND_MFR_MICRON, "Micron"},	// Intel
	{NAND_MFR_AMD, "AMD"}
};

//const NAND_INFO g_NandInfo[] = 
//{
//      // {NandID, NandExtID, PlaneNum Tals Talh Trp Twp Trhw Twhr dwPageSize, dwBlockSize, dwPageAddressCycle, dwMinValidBlocks, dwMaxValidBlocks}
///*SAMSUNG_K9G8G08U0A*/ {0xECD3, 0x0064a514, 2,    12,   5,   12, 12, 100, 60,   2048,   256 * 1024,   3,      3996,     4096},     
///*SAMSUNG_K9G8G08U0B*/ {0xECD3, 0x0064a514, 2,    12,   5,   12, 12, 100, 60,   2048,   256 * 1024,   3,      3996,     4096},
///*SAMSUNG_K9G8G08U0M*/ {0xECD3, 0x00642514, 2,    15,   5,   15, 15, 100, 60,   2048,   256 * 1024,   3,      3996,     4096},
///*SAMSUNG_K9GAG08U0M*/ {0xECD5, 0x0074b614, 1,    12,   5,   12, 12, 100, 60,   4096,   512 * 1024,   3,      3996,     4096},
///*SAMSUNG_K9LAG08U0M*/ {0xECD5, 0x00682555, 0x800002, 12,   5,   12, 12, 60,   60,   2048,   256 * 1024,   3,      7992,     8192},
///*SAMSUNG_***********/ {0xECD5, 0x0068a555, 0x800002, 15,   5,   15, 15, 100, 60,   2048,   256 * 1024,   3,      7992,     8192},
///*SAMSUNG_K9LBG08U0M*/ {0xECD7, 0,    1,    12,   5,   12, 12, 100, 60,   4096,   512 * 1024,   3,      7992,     8192},
///*SAMSUNG_K9F1G08U0M*/ {0xECF1, 0,    1,    15,   5,   15, 15, 100, 60,   2048,   128 * 1024,   3,      1004,     1024},
///*HYNIX_HY27UT088G2A*/ {0xADD3, 0x0034A514, 2,    12,   5,   12, 12, 100, 80,   2048,   256 * 1024,   3,      3996,     4096},
///*HYNIX_HY27UF081G2M*/ {0xADF1, 0,    1,    5,   15,   25, 40, 60,   60,   2048,   128 * 1024,   3,      1004,     1024},
///*HYNIX_HY27UU08AG5A*/ {0xADD3, 0x0134A514, 2,    12,   5,   12, 12, 100, 80,   2048,   256 * 1024,   3,      7992,     8192},
///*HYNIX_hy27UV08BG5A*/ {0xADD5, 0x0138A555, 2,    15,   15,   15, 15, 100, 100, 2048,   256 * 1024,   3,      7992*2,     8192*2},

///*Micr_MT29F16G08MAA*/ {0x2CD3, 0x00743E94, 2,    10,   5,   10, 10, 100, 60,   4096,   512 * 1024,   3,      3936,     4096},
///*Micr_MT29F16G08QAA*/ {0x2CD5, 0x00743E94, 2,    10,   5,   10, 10, 100, 60,   4096,   512 * 1024,   3,      3936*2,     4096*2},
///*Micr_MT29F16G08TAA*/ {0x2CD5, 0x00783ED5, 1,    10,   5,   10, 10, 100, 60,   4096,   512 * 1024,   3,      3936*4,     4096*4},
///*INTEL_29F16G08AAMC1*/ {0x89D3, 0x00783E94, 2,    10,   5,   10, 10, 100, 60,   4096,   512 * 1024,   3,      3936,     4096},
///*INTEL_29F32G08CAMC1*/ {0x89D5, 0x00783E94, 2,    10,   5,   10, 10, 100, 60,   4096,   512 * 1024,   3,      3936*2,     4096*2},
///*INTEL_29F64G08FAMC1*/ {0x89D7, 0x00783ED5, 1,    10,   5,   10, 10, 100, 60,   4096,   512 * 1024,   3,      3936*4,     4096*4},
//};
////Note: NandID = Manufacture code + device code; and if your chip is not mutil cs,you must set NandExtId = 0,whether it has or no.

_DFa(_nand_info)
	char	Maf[8], Dev[19];
	char	Id[5];			// #5 chip numbers

	u16	wChipNum;
	u16	wPlaneNum;		// Plane numbers for use

	// Essential Timing
	u8	Tals;			// ALE Setup Time(Unit: ns)
	u8 	Talh;			// ALE Hold Time(Unit: ns)
	u8	Trp;			// /RE Pulse Width(Unit: ns)
	u8	Twp;			// /WE Pulse Width(Unit: ns)
	u8	Trhw;			// /RE High to /WE Low(Unit: ns)
	u8	Twhr;			// /WE High to /RE Low(Unit: ns)

	// Essential Nand Size
	u8	bPageAddressCycle;
	u16	wPageSize;		// 512 or 2048
	u16	wOOBSize;		// 64  or 128 or 218
	u32	dwBlockSize;		// 16K or 128K

	u32	dwMinValidBlocks;
	u32	dwMaxValidBlocks;
_DFz(_nand_info)

//__nand_info _nand_info[] = {
//#include "nand_cfg.h"
//};

char *_nand_manuf(char id)
{
	
	int i;
	for(i=1; i<sizeof(nand_manuf_ids) / sizeof(__nand_manufacturers); ++i)
		if (nand_manuf_ids[i].id == id)
			return nand_manuf_ids[i].name;
	return 0;
}

_DFa(nand_flash_dev)
	char	*name;
	u16	id;
	u8	volt, busw;
	u32	siz[3];		// 0 - blksiz, 1 - pgsiz, 2 - chipsiz
_DFz(nand_flash_dev)

_nand_flash_dev nand_flash_ids[] = {
	{"Unknown", 0, 0, 0, {0, 0, 0}},
	{"8Gb, x8, 3V", 0xd7, 30, 8, {0, 0, 8192}},	// JS29F64G08FAMC1
	{"2Gb, x8, 3V", 0xda, 30, 8, {0, 0, 2048}}	// JS29F02G08AANB3
};

_nand_genarchi_0(_NandFlashArchi *ar, u32 v)
{
	int i;
	char *s;
	s = _nand_manuf(v);
	if (s) {
		for(i=0; i<8; ++i) {
			if (*s) ar->maf[i] = *s++;
			else break;
		}
		while(i<8) ar->maf[i++] = 0;
		return 1;
	}
	return 0;
}

_nand_genarchi_1(_NandFlashArchi *ar, u32 v)
{
	int i;

	for(i=1; i<sizeof(nand_flash_ids)/sizeof(_nand_flash_dev); ++i) {
		ar->BytesOfPg = nand_flash_ids[i].siz[1];
		ar->BytesOfBlk = nand_flash_ids[i].siz[0];
		ar->SizeOfChip = nand_flash_ids[i].siz[2];
		return 1;
	}
	return 0;
}

_nand_genarchi_2(_NandFlashArchi *ar, u32 v)
{
	ar->NumOfDie = 1<<(v&3); v>>=2;
	ar->CellInfo = v;
	return 1;
}

_nand_genarchi_3(_NandFlashArchi *ar, u32 v)
{
	ar->BytesOfPg = 1024 <<(v&3); v>>=2;
	ar->BytesOfSpare = (8 << (v & 1)) * (ar->BytesOfPg >> 9); v>>=1;
	ar->SATime = v&1; v>>=1;
	ar->BytesOfBlk = (64*1024)<<(v&3); v>>=2;
	ar->BWidth = v&1; v>>=1;
	if (!ar->SATime)
		ar->SATime = v&1 ? 25 : 50;
	else ar->SATime = 0;
	return 1;
}

_nand_genarchi_4(_NandFlashArchi *ar, u32 v)
{
	ar->NumOfPlanes = 1 << ((v>>2) & 3); v>>=4;
	ar->SizOfPlane = v&7;
	return 1;
}
