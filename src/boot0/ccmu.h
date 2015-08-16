#ifndef __CCMU_H
#define __CCMU_H

#define CCMU_CLK_AHB		0
#define CCMU_CLK_APB		1
#define CCMU_CLK_DRAM		2
#define CCMU_CLK_PLL		3

typedef enum __CCMU_AHB_CLK {             // AHB clock
	CCMU_AHB_CLK_SPI1               ,
	CCMU_AHB_CLK_SPI0               ,
	CCMU_AHB_CLK_TS                 ,
	CCMU_AHB_CLK_EMAC               ,
	CCMU_AHB_CLK_BIST               ,
	CCMU_AHB_CLK_MACC               ,
	CCMU_AHB_CLK_LCD                ,
	CCMU_AHB_CLK_SDRAM              ,
	CCMU_AHB_CLK_NAND               ,
	CCMU_AHB_CLK_MS                 ,
	CCMU_AHB_CLK_SDMMC3             ,
	CCMU_AHB_CLK_SDMMC2             ,
	CCMU_AHB_CLK_SDMMC1             ,
	CCMU_AHB_CLK_SDMMC0             ,
	CCMU_AHB_CLK_DMA                ,
	CCMU_AHB_CLK_CSI                ,
	CCMU_AHB_CLK_TVE                ,
	CCMU_AHB_CLK_DE                 ,
	CCMU_AHB_CLK_SS                 ,
	CCMU_AHB_CLK_USB1               ,
	CCMU_AHB_CLK_USB0               ,
}__ccmu_ahb_clk_t;

typedef enum __CCMU_DRAM_CLK {		// dram clock
    CCMU_DRAM_CLK_MACC              ,
    CCMU_DRAM_CLK_DEFE              ,
    CCMU_DRAM_CLK_DEBE              ,
    CCMU_DRAM_CLK_CSI               ,
    CCMU_DRAM_CLK_TS                ,
}__ccmu_dram_clk_t;

typedef enum __CCMU_APB_CLK {             // APB clock
	CCMU_APB_CLK_PS2                ,
	CCMU_APB_CLK_AC97               ,
	CCMU_APB_CLK_SPDIF              ,
	CCMU_APB_CLK_IIS                ,
	CCMU_APB_CLK_IR                 ,
	CCMU_APB_CLK_CODEC              ,
	CCMU_APB_CLK_UART3              ,
	CCMU_APB_CLK_UART2              ,
	CCMU_APB_CLK_UART1              ,
	CCMU_APB_CLK_UART0              ,
	CCMU_APB_CLK_PIO                ,
	CCMU_APB_CLK_TWI1               ,
	CCMU_APB_CLK_TWI0               ,
}__ccmu_apb_clk_t;

enum  __CCMU_PLL_CLK {			 // PLLs clock
	CCMU_PLL_CLK_CORE              ,
	CCMU_PLL_CLK_MACC              ,
//	CCMU_PLL_CLK_AUDIO             ,
	CCMU_PLL_CLK_VIDEO             ,
//	CCMU_PLL_CLK_DRAM              ,
//	CCMU_PLL_CLK_USB               ,
}__ccmu_pll_clk_t;

#endif
