//
// chip-mode for nand driver
// 完全兼容suni/ii rb connect mode
//

#include "nand.h"

//
// suni/ii 里的6种R/B连接模式
// rb_mode = 1	chip0
// rb_mode = 2	chip0 chip1
// rb_mode = 3	chip0 chip2
// rb_mode = 4	chip0 chip2 chip1 chip3
// rb_mode = 5	chip0 chip2 chip4 chip6
// rb_mode = 8	chip0 chip2 chip1 chip3 chip4 chip6 chip5 chip7
//

// 本系统里的三种模式
// 0  chip0 chip1 chip2 chip3 ....
// 1  chip0 chip2 chip4 chip6 ....
// 2  chip0 chip2 chip1 chip3 chip4 chip6 chip5 chip7 ...
// 对于rb_mode  1,2 即单片1,2 CE 使用 模式0
// 对于rb_mode  3,6 即2,4片1CE   使用 模式1
// 对于rb_mode  4,8 即2,4片2CE   使用 模式2
//
_cal_real_chip(bus, n)
	_nand_bus *bus;
{
	static int _rb_chipsel[][8] = {
		{0, 1, 2, 3, 4, 5, 6, 7},
		{0, 2, 4, 6},
		{0, 2, 1, 3, 4, 6, 5, 7}
	};
	return _rb_chipsel[bus->conn][n];
}

//
//                           | 0 | | 1 | | 2 | | 3 | | 4 | | 5 | | 6 | | 7 |
//                           |-0-| |-1-|
// 1片单CE:     rb_mode = 1  chip0
// 1片双CE:     rb_mode = 2  chip0 chip1
//                           |-0-| |-1-| |-0-| |-1-|
// 2片单CE:     rb_mode = 3  chip0 chip2
// 2片双CE:     rb_mode = 4  chip0 chip2 chip1 chip3
//                           |----0----| |----1----| |----0----| |----1----|
// 4片单CE:     rb_mode = 5  chip0 chip2 chip4 chip6
// 4片双CE:     rb_mode = 8  chip0 chip2 chip1 chip3 chip4 chip6 chip5 chip7
//
_cal_real_rb(bus, n)
	_nand_bus *bus;
{
	switch(bus->numchips) {
		case 1: case 2: return n & 1;
		case 4: return (n >> (bus->conn!=2)) & 1;
		case 8: return (n >> 1) & 1;
	}
	return 2;
}


// calc row
cal_real_addr_r(chip, addr, page)
	_nand_chip *chip;
	u8 *addr;
	u32 page;
{
	((u32 *)addr)[0] = ((u32 *)addr)[1] = 0;
	unsigned i;

//	i = (chip->writesize > 512) + 1;
	i = (chip->siz[1] > 512) + 1;
	while(page) {
		addr[i++] = page & 0xff;
		page>>=8;
	}
}

// calc column
cal_real_addr_c(chip, addr, i)
	_nand_chip *chip;
	u8 *addr;
	u32 i;
{
	unsigned j;
	j = 0;
	while(i) {
		addr[j++] = i & 0xff;
		i >>= 8;
	}
}

cal_real_addr(chip, cydr, lo)
	_nand_chip *chip;
	u8 *cydr;
	u64 lo;
{
	cal_real_addr_r(chip, cydr, _adr2p(chip, lo));
//	cal_real_addr_c(chip, cydr, lo & (chip->writesize - 1));
	cal_real_addr_c(chip, cydr, lo & (chip->siz[1] - 1));
}
