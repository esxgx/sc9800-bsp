@echo off
arm-none-eabi-gcc -I ../h -I ../../h -Os -march=armv5te -c lradc.c

ar cqs ../lib/lradc.a *.o

::arm-none-eabi-ld -e_boot2 -Ttext 0x80010000 entry.o boot2.o nfc.o nand_scan.o nand_read.o nand_ids.o nand.o libc.a libgcc.a
::arm-none-eabi-ld -e_boot2 -Tboot2.lds entry.o boot2.o td043m_lcdc.o key.o td043m.o lradc.o ccmu_sclk.o lcdc.o uart.o pio.o de_be.o disp.o gpio.o lcdc_if.o stdio.o vsprintf.o kmsg.o kmsg_disp.o kmsg_uart.o arm.o sys.o fault0.o fault.o pic.o nfc.o nand_scan.o nand_calc.o nand_read.o nand_ids.o nand.o ccmu.o ccmu_pw.o libc.a libgcc.a
::arm-none-eabi-objcopy -O binary a.out ../boot2.bin
::arm-none-eabi-objdump -xd -S -s a.out > disasm.txt
del *.o a.out

pause