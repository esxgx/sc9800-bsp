@echo off
arm-none-eabi-gcc -I ../h/ -O1 -c entry.S
arm-none-eabi-gcc -I ../h/ -O1 -c boot0.c
arm-none-eabi-gcc -I ../h/ -O1 -c dram.c

arm-none-eabi-gcc -I ../h/ -O1 -c ccmu.c
arm-none-eabi-gcc -I ../h/ -O1 -c ccmu_pw.c

::arm-none-eabi-ld -e_boot0 -Ttext 0x7220 entry.o boot0.o dram.o
arm-none-eabi-ld -e_boot0 -Tboot0.lds entry.o ccmu.o ccmu_pw.o boot0.o dram.o libgcc.a
arm-none-eabi-objcopy -O binary a.out ../boot0.bin
:: -D
arm-none-eabi-objdump -S a.out > disasm.txt
del *.o a.out
pause