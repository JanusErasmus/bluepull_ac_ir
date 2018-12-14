#!/usr/bin/bash
#echo 'Building'
#make
#
#retVal=$?
#if [ $retVal -ne 0 ]; then
#	echo -e '\x1b[31mBuild failed\x1b[0m'
#	exit $retVal
#else
#	echo -e '\x1b[32mBuild OK \x1b[0m'
#fi

echo 'Programming'
/c/Program\ Files\ \(x86\)/STMicroelectronics/Software/Flash\ Loader\ Demo/STMFlashLoader.exe -c --pn 3 --br 115200 --db 8 --pr EVEN --sb 1 --ec OFF --to 10000 --co OFF -i STM32F1_Med-density_64K -d --fn bluepill.bin --a 0x8000000 -r --a 0x08000000
#stm32flash -w $1 -b 115200 -g 0x8000000  /dev/ttyUSB0

retVal=$?
if [ $retVal -ne 0 ]; then
	echo -e '\x1b[31mProgramming failed\x1b[0m'
	exit $retVal
else
	echo -e '\x1b[32mProgram OK\x1b[0m'
fi

minicom -D /dev/ttyUSB0 -c on
