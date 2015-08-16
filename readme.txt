fes demo 0.35
短期目标：
	实现低功耗全格式电子书固件 + epos os转接

已实现的驱动:
	ARMv5 驱动( 支持系统呼叫, 支持陷阱frame )
	LCDC 驱动 ( 初步实现, 未调试 )
	DE 驱动 ( DE_BE实现 )
	td043m 驱动 ( 已实现 )
	GPIO 驱动 ( 已实现 )
	UART 驱动 ( 支持RS-232 )
	CCMU 驱动 ( 支持24Mhz, RTC Clock, cpu32 PLL, AHB, APB , On/off控制 )
	INTC 驱动 ( 已实现 )
	LRADC 驱动 ( 已实现 )
	SDRAM 驱动( 部分实现 )
	NAND 驱动( 读操作、擦除操作OK, 支持多种NAND FLASH型号、支持多片 )
	TWI 驱动 (已实现, 除repeat start操作)
	AXP182 驱动 (SW1控制，读状态控制OK)

demo使用方法:
1、请确保能正常使用efel2p, 即小机状态返回正常。
2、运行c&d.bat即可.
3、输出信息在output文件夹内, 因为每次demo演示的功能都不同, 所以请自行查看.

注意，串口输出调试信息，有兴趣可以查看.

esxgx
2011.07.11
