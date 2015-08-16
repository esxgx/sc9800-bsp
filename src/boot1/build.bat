@echo off
arm-elf-gcc -I ../h/ -c entry.S
arm-elf-ld -e_boot1 -Ttext 0x7220 entry.o
arm-elf-objcopy -O binary a.out ../boot1.bin
arm-elf-objdump -S a.out > disasm.txt
del *.o a.out
pause