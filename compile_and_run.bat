@echo off
cd src
call boot0_up

call bat/upload

::cd src
::call boot1_up
::call bat/upload1

cd src

call boot2_up

call bat/upload2

del a.bin