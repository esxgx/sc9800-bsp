#include "pio.h"
#include "suniip.h"

#define __delay(x)	({	\
	int ee = (x);			\
		while(ee) --ee;		\
	ee;})

#define lcd_delay	__delay

//#define _DEBUG_TPO_

//                   lcd_cs     lcd_scl
// enable PIOC10 for lcd_rst or lcd_en
#define gpio_td043m_MPIOx_En(n, en)	do{\
	pioc_gpio_cfg(PIOC_REGS_BASE, GPIO_GRP_C, GPIO_PORT_n(10 + (n)), (en)? GPIO_OUTPUT : GPIO_INPUT);	\
	lcd_delay(400);												\
}while(0)

#define gpio_td043m_MPIOx_Outp(n, val)	do{\
	pioc_gpio_outp(PIOC_REGS_BASE, GPIO_GRP_C, GPIO_PORT_n(10 + (n)), val);		\
	lcd_delay(500);									\
}while(0)	

#define gpio_td043m_MPIOx_Inp(n)	\
	pioc_gpio_inp(PIOC_REGS_BASE, GPIO_GRP_C, GPIO_PORT_n(10 + (n)))

//io1
#define MPIO1_OUTEN(en)		gpio_td043m_MPIOx_En(0, en)
#define MPIO1_OUTVALUE(val)	gpio_td043m_MPIOx_Outp(0, val)
#define MPIO1_INVALUE()		gpio_td043m_MPIOx_Inp(0)
//#define MPIO1_INEN(en)	gpio_td043m_MPIOx_En(0, !(en))

//io2
#define MPIO2_OUTEN(en)		gpio_td043m_MPIOx_En(1, en)
#define MPIO2_OUTVALUE(val)	gpio_td043m_MPIOx_Outp(1, val)
#define MPIO2_INVALUE()		gpio_td043m_MPIOx_Inp(1)
//#define MPIO2_INEN(en)	gpio_td043m_MPIOx_En(1, !(en))

//io3
#define MPIO3_OUTEN(en)		(en?lcdc_hvif_mpio_cfg(LCDC_REGS_BASE, 1) : lcdc_lde_mpio_cfg(LCDC_REGS_BASE, 0))
#define MPIO3_OUTVALUE(val)	lcdc_hvif_mpio_outp(LCDC_REGS_BASE, val)
#define MPIO3_INVALUE()		(0)


/////////////////HV PARALLEL:TD043MGEB1 800_480 /////////////////////////////
#define SCL_x(x)	MPIO2_OUTVALUE(!((x) == 0))
#define SDA_x(x)	MPIO1_OUTVALUE(!((x) == 0))

#define CS_1    MPIO3_OUTVALUE(1)
#define SCL_1   SCL_x(1)
#define SDA_1   SDA_x(1)

#define CS_0    MPIO3_OUTVALUE(0)
#define SCL_0   SCL_x(0)
#define SDA_0   SDA_x(0)

#define SDA_tri MPIO1_OUTEN(0)
#define SDA_va  MPIO1_INVALUE()
#define SDA_out MPIO1_OUTEN(1)


//
// SCEN  I    Serial interface chip enable line 
// SC    I    Serial interface clock line 
// SDA   I/O  Serial interface data line 
//

//
// IIC_TPO_RREG (i2c tpo read reg)
//
static IIC_TPO_RREG(addr)
{
#ifdef _DEBUG_TPO_
	printk("tpo_td043m_read: (addr: %xh)", addr);
#endif
	// send cmd for starting
	CS_1;
	SDA_0; SCL_0;
	CS_0;

	unsigned i;
	// send addr(6bit)
	for(i=0; i<6; ++i) {
		SCL_0;
		SDA_x(addr & (1<<5));
		SCL_1;
		addr<<=1;
	}
    
	//read bit
	SCL_0;
	SDA_1;		// read command
	SCL_1;

	//turn around bit
	SCL_0;
	SDA_tri;		// get MPIO1 ready for read operation
	SCL_1;

	unsigned data = 0;
	//read Data
	for(i=0; i<8; ++i) {
		SCL_0;
		data = (data << 1) | SDA_va;
		SCL_1;
	}

	SCL_0;
	CS_1;
	SDA_out;

#ifdef _DEBUG_TPO_
	printk(" %08xh\n", data);
#endif
	return data;
}

//
//IIC_TPO_WREG (i2c tpo write reg)
//
static IIC_TPO_WREG(addr, data)
{
#ifdef _DEBUG_TPO_
	printk("tpo_td043m_write: (addr: %xh) %08xh\n", addr, data);
#endif

	// send cmd for starting
	CS_1;
	SDA_0; SCL_0;

	CS_0;
	
	unsigned i;
	// send addr(6bit)
	for(i=0; i<6; ++i) {
		SCL_0;
		SDA_x(addr & (1<<5));
		SCL_1;
		addr<<=1;
	}

	// write bit
	SCL_0;
	SDA_0;		// write command
	SCL_1;

	//turn around bit
	SCL_0;
	SDA_tri;		// get MPIO1 ready for read operation
	SCL_1;

	SDA_out;
	//write Data
	for(i=0; i<8; ++i) {
		SCL_0;
		SDA_x(data & (1<<7));
		SCL_1;
		data<<=1;
	}
	SCL_0;
	CS_1;
}

#define tpo_td043m_write		IIC_TPO_WREG
#define tpo_td043m_read			IIC_TPO_RREG

static tpo_td043m_write_gamma(gamma)
	u16 gamma[12];
{
	u8 i, val;

	/** gamma bits [9:8] */
	val = 0;
	for (i=0; i<4; ++i)
		val |= (gamma[i] & 0x300) >> ((i + 1) * 2);
	tpo_td043m_write(0x11, val);

	val = 0;
	for (i=0; i<4; ++i)
		val |= (gamma[i+4] & 0x300) >> ((i + 1) * 2);
	tpo_td043m_write(0x12, val);

	val = 0;
	for (i=0; i<4; ++i)
		val |= (gamma[i+8] & 0x300) >> ((i + 1) * 2);
	tpo_td043m_write(0x13, val);

	/** gamma bits [7:0] */
	val = 0;
	for (i=0; i<12; ++i) tpo_td043m_write(0x14 + i, gamma[i] & 0xff);
}


// SOCHIP gamma correction
// gamma[0] = 00 10000110 gamma[1] = 01 10001110 gamma[2] = 01 11010110
// gamma[3] = 01 11111110 gamma[4] = 10 00101000 gamma[5] = 10 01010010
// gamma[6] = 10 01111100 gamma[7] = 10 11101001 gamma[8] = 11 01000010
// gamma[9] = 11 10001000 gamma[A] = 11 10111000 gamma[B] = 11 11111111


static const u16 tpo_td043m_def_gamma[12] = {
//	106, 200, 289, 375, 460, 543, 625, 705, 785, 864, 942, 1020
	134, 398, 470, 510, 552, 594, 636, 745, 834, 904, 952, 1023
};


#define TPO_R02_MODE(x)		((x) & 7)
#define TPO_R02_MODE_800x480	7
#define TPO_R02_NCLK_RISING	(1<<3)
#define TPO_R02_HSYNC_HIGH	(1<<4)
#define TPO_R02_VSYNC_HIGH	(1<<5)

#define TPO_R03_NSTANDBY	(1<<0)
#define TPO_R03_EN_CP_CLK	(1<<1)
#define TPO_R03_EN_VGL_PUMP	(1<<2)
#define TPO_R03_EN_PWM		(1<<3)
#define TPO_R03_DRIVING_CAP_100	(1<<4)
#define TPO_R03_EN_PRE_CHARGE	(1<<6)
#define TPO_R03_SOFTWARE_CTL	(1<<7)

#define TPO_R04_NFLIP_H		(1<<0)
#define TPO_R04_NFLIP_V		(1<<1)
#define TPO_R04_CP_CLK_FREQ_1H	(1<<2)
#define TPO_R04_VGL_FREQ_1H	(1<<4)

#define TPO_R03_VAL_NORMAL (TPO_R03_NSTANDBY | TPO_R03_EN_CP_CLK | \
			TPO_R03_EN_VGL_PUMP |  TPO_R03_EN_PWM | \
			TPO_R03_DRIVING_CAP_100 | TPO_R03_EN_PRE_CHARGE)

#define TPO_R03_VAL_STANDBY (TPO_R03_DRIVING_CAP_100 | TPO_R03_EN_PRE_CHARGE | TPO_R03_SOFTWARE_CTL)

static tpo_td043m_write_mirror(h, v)
{
	u8 reg4 = TPO_R04_NFLIP_H | TPO_R04_NFLIP_V | \
		TPO_R04_CP_CLK_FREQ_1H | TPO_R04_VGL_FREQ_1H;
	if (h) reg4 &= ~TPO_R04_NFLIP_H;
	if (v) reg4 &= ~TPO_R04_NFLIP_V;

	tpo_td043m_write(4, reg4);
}

tpo_td043m_poweron()
{
	MPIO1_OUTEN(1);
	MPIO2_OUTEN(1);

	MPIO1_OUTVALUE(1);
	MPIO2_OUTVALUE(1);
	MPIO3_OUTVALUE(1);

	MPIO3_OUTEN(1);

	lcd_delay(700);
	
	MPIO1_OUTVALUE(1);
	MPIO2_OUTVALUE(1);
	MPIO3_OUTVALUE(1);


	lcd_delay(700);
}

tpo_td043m_brightness_cfg(r,g,b)
{
	tpo_td043m_write(0xC, r);
	tpo_td043m_write(0xD, g);
	tpo_td043m_write(0xE, b);
}

tpo_td043m_cfg()
{
	lcd_delay(60000);


	printk("tpo_td043m: chipid=%04xh\n",tpo_td043m_read(1));
	tpo_td043m_write(2, TPO_R02_MODE_800x480);
	tpo_td043m_write(3, TPO_R03_VAL_NORMAL);

	tpo_td043m_write(0x20, 0xf0);
	tpo_td043m_write(0x21, 0xf0);

	tpo_td043m_write_mirror(0, 0);
	tpo_td043m_brightness_cfg(16, 16 , 10);
	tpo_td043m_write_gamma(&tpo_td043m_def_gamma);

	tpo_td043m_write(0x22, 0x08);

	tpo_td043m_write(0x02,0x07);
}
//
//	IIC_TPO_WREG(0x05,0x20);
//	IIC_TPO_WREG(0x06,0x08);
//	IIC_TPO_WREG(0x07,0x20);
//	IIC_TPO_WREG(0x08,0x20);
//	IIC_TPO_WREG(0x09,0x20);
//	IIC_TPO_WREG(0x0A,0x20);
//	IIC_TPO_WREG(0x0B,0x20);
//	IIC_TPO_WREG(0x0C,0x20);
//	IIC_TPO_WREG(0x0D,0x20);
//	IIC_TPO_WREG(0x0E,0x10);
//
tpo_td043m_standby()
{
	tpo_td043m_write(3, TPO_R03_VAL_STANDBY | TPO_R03_EN_PWM);

	gpio_td043m_MPIOx_En(0, 1);
	gpio_td043m_MPIOx_Outp(0, 0);

	// wait for at least 2 vsyncs before cutting off power
	lcd_delay(400);

	tpo_td043m_write(3, TPO_R03_VAL_STANDBY);

	lcd_delay(400);
}

tpo_td043m_poweroff()
{
	MPIO1_OUTEN(1);
	MPIO1_OUTVALUE(0);
	MPIO2_OUTEN(1);
	MPIO2_OUTVALUE(0);
}