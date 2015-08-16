// kevin.z      2009-9-7 10:53    1.0.0        build the file
#ifndef  __SUNII_H
#define  __SUNII_H

//*******************************************************************************
//*   hardware registers base define
//*******************************************************************************
#define REGS_pBASE		0x01C00000		// 寄存器物理地址
#define REGS_pSIZE		0x00300000  		// 寄存器物理空间大小

#define DRAM_pBASE		0x80000000

#define SRAM_pBASE		0x00000000
#define SRAM_SIZE		(32 * 1024)

// 物理地址
#define ___SRAM          0x00000	//SRAM controller
#define ___DRAM          0x01000	//SDRAM/DDR controller
#define ___DMAC          0x02000	//DMA controller
#define ___NAFC          0x03000	//nand flash controller
#define ___TSC           0x04000	//transport stream interface
#define ___SPIC0         0x05000	//spi
#define ___SPIC1         0x06000	//spi
#define ___MSHC          0x07000	//ms
#define ___CSIC          0x09000	//csi controller
#define ___TVEC          0x0a000	//tv
#define ___LCDC          0x0c000	//lcd
#define ___MACC          0x0e000	//media accelerate
#define ___SDMC0         0x0f000	//sdmmc0 controller
#define ___SDMC1         0x10000	//sdmmc1 controller
#define ___SDMC2         0x11000	//sdmmc2 controller
#define ___SDMC3         0x12000	//sdmmc3 controller
#define ___USBC0         0x13000	//usb/otg 0 controller
#define ___USBC1         0x14000	//usb/otg 1 controller
#define ___CCMU          0x20000	//clock manager unit
#define ___INTC          0x20400	//arm interrupt controller
#define ___PIOC          0x20800	//general perpose I/O
#define ___TMRC          0x20c00	//timer
#define ___UART0         0x21000	//uart0
#define ___UART1         0x21400	//uart1
#define ___UART2         0x21800	//uart2
#define ___UART3         0x21c00	//uart3
#define ___SPDIF         0x22000	//SPDIF interface
#define ___PS2           0x22400	//media accelerate
#define ___AC97          0x22800	//AC97 interface
#define ___IRCC          0x22c00	//fir
#define ___I2SC          0x23000	//i2s
#define ___LRAC          0x23400	//lradc
#define ___ADDA          0x23c00	//AD/DA
#define ___TWIC0         0x24000	//twi0
#define ___TWIC1         0x24400	//twi1
#define ___TPC           0x24800	//touch panel controller
#define ___DISE          0x200000		//display engine

#define __SPx(x)	(REGS_pBASE + (x))

#endif // end of #ifndef __SUNII_H_
