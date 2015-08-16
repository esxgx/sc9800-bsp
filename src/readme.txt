fes2 source code

The fes2 is suitable for M1 processor(common known as sunii),
provides a base system to load operating system kernel and
USB->nand burning system.

The fes2 source code is written in C and GNU ARM assembly
language, and include:
	1. boot0 ！！ used for booting boot1/boot2 from USB,
setting up the sdram/ddr.
	2. boot2 ！！ the main boot loader, used for loading
kernel and burning nand.

any problems, contact me at esxgx@163.com.

esxgx
2011.7.22