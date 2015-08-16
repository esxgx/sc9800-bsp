#include "twi.h"

#define AXP182_INT_VOUT_OP         (1<<7)     // IRQ1�� �ⲿ��Դ��ѹ�����λд1��DCIN��ѹ�ָ���������Χ�������λ
#define AXP182_INT_VOUT_PLUGIN     (1<<6)     // IRQ2�� �ⲿ��Դ���룬���λд1��DCIN�ɸߵ�����ʧ�������λ
#define AXP182_INT_VOUT_PLUGOUT    (1<<5)     // IRQ3�� �ⲿ��Դ�Ƴ������λд1��DCIN�ɵ͵��߳��ֻ������λ
#define AXP182_INT_VOOUT_L_VHOLD   (1<<4)     // IRQ4�� �ⲿ��Դ��ѹ����VHOLD�����λд1��DCIN�ɸߵ�����ʧ�������λ
#define AXP182_INT_BAT_TMP_HIGH    (1<<2)     // IRQ5�� ����¶ȹ��ߣ����λд1�����¶Ȼָ���������Χ�������λ
#define AXP182_INT_BAT_TMP_LOW     (1<<1)     // IRQ6�� ����¶ȹ��ͣ����λд1�����¶Ȼָ���������Χ�������λ
#define AXP182_INT_VDCDC1_LOW      (1<<16)    // IRQ7�� DC-DC1�����ѹ���ͣ����λд1�������ѹ�ָ���������Χ�������λ
#define AXP182_INT_VDCDC2_LOW      (1<<15)    // IRQ8�� DC-DC2�����ѹ���ͣ����λд1�������ѹ�ָ���������Χ�������λ
#define AXP182_INT_VDCDC3_LOW      (1<<14)    // IRQ9,  DC-DC3�����ѹ���ͣ����λд1�������ѹ�ָ���������Χ�������λ
#define AXP182_INT_VLDO3_LOW       (1<<13)    // IRQ10��LDO3�����ѹ���ͣ����λд1�������ѹ�ָ���������Χ�������λ
#define AXP182_INT_PEK_SHORT       (1<<12)    // IRQ11��PEK�̰������λд1�������λ
#define AXP182_INT_PEK_LONG        (1<<11)    // IRQ12��PEK���������λд1�������λ
#define AXP182_INT_BAT_IN          (1<<24)    // IRQ13����ؽ��룬���λд1���ر��γ��������λ
#define AXP182_INT_BAT_OUT         (1<<23)    // IRQ14������Ƴ������λд1���ر�����������λ
#define AXP182_INT_BAT_ACTIVE_IN   (1<<22)    // IRQ15, �����ؼ���ģʽ�����λд1���˳���ؼ���ģʽ�������λ
#define AXP182_INT_BAT_ACTIVE_OUT  (1<<21)    // IRQ16���˳���ؼ���ģʽ�����λд1������ؼ���ģʽ�������λ
#define AXP182_INT_CHARGE_START    (1<<20)    // IRQ17����ʼ��磬���λд1��ֹͣ���������λ
#define AXP182_INT_CHARGE_END      (1<<19)    // IRQ18�������ɣ����λд1�������¿�ʼ�������λ
#define AXP182_INT_LOWPWR_WARN     (1<<18)    // IRQ19���͵羯�棬���λд1��AIPS��ѹ�ָ���������Χ�������λ

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
