#include "twi.h"

#define AXP182_INT_VOUT_OP         (1<<7)     // IRQ1， 外部电源超压，向此位写1或DCIN电压恢复到正常范围会清除此位
#define AXP182_INT_VOUT_PLUGIN     (1<<6)     // IRQ2， 外部电源插入，向此位写1或DCIN由高到低消失会清除此位
#define AXP182_INT_VOUT_PLUGOUT    (1<<5)     // IRQ3， 外部电源移除，向此位写1或DCIN由低到高出现会清除此位
#define AXP182_INT_VOOUT_L_VHOLD   (1<<4)     // IRQ4， 外部电源电压低于VHOLD，向此位写1或DCIN由高到低消失会清除此位
#define AXP182_INT_BAT_TMP_HIGH    (1<<2)     // IRQ5， 电池温度过高，向此位写1或电池温度恢复到正常范围会清除此位
#define AXP182_INT_BAT_TMP_LOW     (1<<1)     // IRQ6， 电池温度过低，向此位写1或电池温度恢复到正常范围会清除此位
#define AXP182_INT_VDCDC1_LOW      (1<<16)    // IRQ7， DC-DC1输出电压过低，向此位写1或输出电压恢复到正常范围会清除此位
#define AXP182_INT_VDCDC2_LOW      (1<<15)    // IRQ8， DC-DC2输出电压过低，向此位写1或输出电压恢复到正常范围会清除此位
#define AXP182_INT_VDCDC3_LOW      (1<<14)    // IRQ9,  DC-DC3输出电压过低，向此位写1或输出电压恢复到正常范围会清除此位
#define AXP182_INT_VLDO3_LOW       (1<<13)    // IRQ10，LDO3输出电压过低，向此位写1或输出电压恢复到正常范围会清除此位
#define AXP182_INT_PEK_SHORT       (1<<12)    // IRQ11，PEK短按，向此位写1可清除此位
#define AXP182_INT_PEK_LONG        (1<<11)    // IRQ12，PEK长按，向此位写1可清除此位
#define AXP182_INT_BAT_IN          (1<<24)    // IRQ13，电池接入，向此位写1或电池被拔出会清除此位
#define AXP182_INT_BAT_OUT         (1<<23)    // IRQ14，电池移除，向此位写1或电池被接入会清除此位
#define AXP182_INT_BAT_ACTIVE_IN   (1<<22)    // IRQ15, 进入电池激活模式，向此位写1或退出电池激活模式会清除此位
#define AXP182_INT_BAT_ACTIVE_OUT  (1<<21)    // IRQ16，退出电池激活模式，向此位写1或进入电池激活模式会清除此位
#define AXP182_INT_CHARGE_START    (1<<20)    // IRQ17，开始充电，向此位写1或停止充电会清除此位
#define AXP182_INT_CHARGE_END      (1<<19)    // IRQ18，充电完成，向此位写1或充电重新开始会清除此位
#define AXP182_INT_LOWPWR_WARN     (1<<18)    // IRQ19，低电警告，向此位写1或AIPS电压恢复到正常范围会清除此位

#define	__AXP182_TWI_ADDR	0x16

#include "suniip.h"
#include "typedef.h"

axp182_query_status()
{
	u8 status;
	twic_host_outp(TWIC0_REGS_BASE, __AXP182_TWI_ADDR, (u8 []){0}, 1);
	twic_host_inp(TWIC0_REGS_BASE, __AXP182_TWI_ADDR, &status, 1);
	return status;
}

axp182_lcd_open()
{
	u8 status;
	twic_host_outp(TWIC0_REGS_BASE, __AXP182_TWI_ADDR, (u8 []){0x10}, 1);
	twic_host_inp(TWIC0_REGS_BASE, __AXP182_TWI_ADDR, &status, 1);

	status |= 1<<7;

	twic_host_outp(TWIC0_REGS_BASE, __AXP182_TWI_ADDR, (u8 []){0x10, status}, 2);
	
	return status;

}
