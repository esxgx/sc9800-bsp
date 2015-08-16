#include "dram.h"

//
// 默认dram配置参数
// [dram_para]
// baseaddr        = 0x80000000
// clk             = 120000000
// access_mode     = 0
// cs_num          = 1
// ddr8_remap      = 0
//
// ;*****************************************************************************
// ;ddr sdram (x16)
// ;ddr 32mx16 (64mb)(k4h511638c-ucb3, k4h511638c-uccc, hy5du121622dtp-d43)
// ;*****************************************************************************
// sdr_ddr   = 1
// bwidth    = 16
// col_width = 10
// row_width = 13
// bank_size = 4
// cas       = 3
// size      = 64
//

// interleave		交错存取模式(因为会出错，暂时关闭)

_dram_para _ddram = {	// 来自sys_config_pvp_v03.ini
	0x80000000,		// baseaddr
	64,			// size
	120000000,		// clk
	0,			// access_mode
	1,			// cs_num
	0,			// ddr8_remap
	1,			// DDR
	16,			// bwidth
	10,			// col_width
	13,			// row_width
	4,			// bank_size
	3			// cas
};

#define DRAM_REG_SCONR          0	// sdram configration register
#define DRAM_REG_STMG0R         1	// sdram timer0 register
#define DRAM_REG_STMG1R         2	// sdram timer1 register
#define DRAM_REG_SCTLR          3	// sdram control register
#define DRAM_REG_SREFR          4	// sdram refresh interval register
#define DRAM_REG_SEXTMR         5	// sdram extended mode register
#define DRAM_REG_SPRYSR         6	// sdram priority set register
#define DRAM_REG_SANR           7	// sdram access number register
#define DRAM_REG_SDLTR          8	// sdram reserved register
#define DRAM_REG_DDLYR          9	// sdram ddr delay register
#define DRAM_REG_DADRR          10	// sdram ddr delay detecting address register
#define DRAM_REG_DVALR          11	// sdram ddr delay detecting value register
#define DRAM_REG_DRPTR0         12	// sdram ddr delay report register 0
#define DRAM_REG_DRPTR1         13	// sdram ddr delay report register 1
#define DRAM_REG_DRPTR2         14	// sdram ddr delay report register 2
#define DRAM_REG_DRPTR3         15	// sdram ddr delay report register 3
#define DRAM_REG_SEFR           16	// sdram extension feature register
#define DRAM_REG_SPCR           17
#define DRAM_REG_ASPR           18

dram_init(void)
{
	return dram_setup(0x1C01000, &_ddram);
}

#include "compiler.h"

__always_inline
dram_check_delay(base)
{
	int i, j, k;

	volatile u32 *dram = (u32 *)base;
	k = 0;
	for(i=0; i<4; ++i) {
		j = dram[DRAM_REG_DRPTR0 + i];
		while(j) {
			if (j & 1) k++;
			j>>=1;
		}
	}
	return k;
}

dram_scan_readpipe(base)
{
	int npip = 0;

	volatile u32 *dram = (u32 *)base;
	do {											// 依次检查read_pipe, 直到无错
		dram[DRAM_REG_SCTLR] = dram[DRAM_REG_SCTLR] & ~(7<<6) | npip+1 << 6;		// read pipe
		dram[DRAM_REG_DDLYR] |= 1;
		while(dram[DRAM_REG_DDLYR] & 1);		// 等待read pipe结束

		// 注意, bit 4/5是是否成功的标志，然后读取report register可以获得
		// 详细内容，这里我们不管它.
		++npip;
	} while (npip<4);
}

dram_setup(base, p)
	_dram_para *p;
{
//
// - Dynamic Memory Controller Registers (SDR/Mobile/DDR-SDRAM)
// if(SDRAM Controller is enabled) {
//   - Program SDRAM Registers (SCONR,STMG0R,STMG1R,SREFR,SCTLR).
//   - SDRAM bank address , row/column address and data width (SCONR)
//     Program SDRAM Configuration Register (Address: REGISTER_BASE_ADDRESS + 0)
//   - SDRAM timing parameters (STMG0R)
//     Program SDRAM Timing Register0 (Address: REGISTER_BASE_ADDRESS+4)
//   - The remaining SDRAM timing parameters (STMG1R)
//     Program SDRAM Timing Register1 (Address: REGISTER_BASE_ADDRESS+8)
//   - Refresh period (SREFR)
//     Program SDRAM Refresh Interval register (Address: REGISTER_BASE_ADDRESS + h'10)
//   - Mobile/DDR-SDRAM Extend Mode Register (EXN_MODE_REG)
//     if(Mobile/DDR-SDRAM is used) {
//       Program Extended Mode Register (Address: REGISTER_BASE_ADDRESS + h'AC)
//     }
//   - SDRAM control Register (SCTLR). Set the "initialize" bit for the controller to do auto
//   - SDRAM initialization sequence. Since setting the "initialize" bit starts the SDRAM
//   - initialization sequence, this register should be programmed last 
//     Program SDRAM Control Register (Address REGISTER_BASE_ADDRESS+h'C)
//  }
//
	volatile u32 *dram = (u32 *)base;

	//
	// if the dram controller is initializing, we will wait for
	// it until it's done.
	//
	while(dram[DRAM_REG_SCTLR] & 1);

	unsigned s_data_width;

	// For SDR SDRAM 
	// 00 – 16 bits. Others - Reserved.
	// For DDR SDRAM
	// 00 –  8 bits. 01 – 16 bits.
	s_data_width = p->bwidth >> 4 + (!p->type);

	// configure the control register
	dram[DRAM_REG_SCONR] = // (1<<17) |
		(p->type<<16) | (p->access_mode<<15) |	\
		(s_data_width<<13) | ((p->col_width-1)<<9) | ((p->row_width-1)<<5) | ((p->cs_num-1)<<4) | \
		((p->bank_size>>2) << 3) | (0x01<<1) | (p->ddr8_remap);

	dram[DRAM_REG_STMG0R] = (dram[DRAM_REG_STMG0R] & ~7) | (p->cas - 1);

	// lookup the A3S56D40ETP datasheet
	// we find the refresh period = 7.8125 us
	// typical, this can be 15.625 us or 7.8125 us
	// then,
	//	t_ref = 7.8125us * 10^-6 * HZ
	//            = HZ / 64000
	dram[DRAM_REG_SREFR] = p->clk / 128000;

	//if (p->type) dram[DRAM_REG_SREFR] = 0x0 | 7<<16 | 0x410;
	
	dram[DRAM_REG_SPRYSR] = 1 << 20 |	// Priority Mode
		 3 <<  4 |			// AHB Continous Access Number
		15 <<  0;			// D-DMA Continous Access Number

	// set the extended mode register
	dram[DRAM_REG_SEXTMR] = dram[DRAM_REG_SEXTMR] & ~0x6000 |	// keep the reserved
		p->type << 13;

	dram[DRAM_REG_SCTLR] |= 1<<26 |		// AUTO_POWER_DOWN
		1<<19 |				// AUTO_REFRESH_MODE
		1;				// INITIALIZE
	while(dram[DRAM_REG_SCTLR] & 1);	// waiting for the sdram initiazation process finished

	dram_scan_readpipe(base);
//	dram[DRAM_REG_SCTLR] |= 1<<3;		// POWER_DOWN_MODE

	return 1;
}
