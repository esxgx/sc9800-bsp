@echo off
del /Q /S esta\*
copy efex.dll esta
copy efel2p.exe esta
copy "doc\efel2p 使用说明.pdf" esta
copy *.txt esta
copy *.bat esta
cd esta
md UsbDriver
md src
md bat
md output
md linux
del /Q esta.bat esta_cr.bat
cd ..
cd linux
copy *.* ..\esta\linux
cd ..
cd output
copy *.txt ..\esta\output
cd ..
cd UsbDriver
copy *.* ..\esta\UsbDriver
cd ..
cd src
copy boot0_up.bat ..\esta\src
copy boot1_up.bat ..\esta\src
copy boot2_up.bat ..\esta\src
copy *.bin ..\esta\src
cd ..
cd bat
copy *.bat ..\esta\bat
cd ..
cd esta
cd output
del /Q code.txt sig.txt nfc.txt nand_oob.txt sdram.txt lcd.txt de.txt
cd ..
cd ..
del /Q fes2.7z
cd esta
..\7z\7z a ..\fes2.7z *
del /Q /S *
rd src
rd bat
pause