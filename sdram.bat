@echo off
cd src
call boot0_up

call bat/upload

del a.bin

pause