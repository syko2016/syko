default:
	arm-none-eabi-as -march=armv6-m file_code.s -o file_code.elf -g0
	arm-none-eabi-objcopy --output-target binary file_code.elf file_code.bin
	arm-none-eabi-objdump -d file_code.elf > file_code.lst
	gcc -c cortex_m0.c -o cortex_m0.o -std=gnu99 -Wall -g3
	gcc -c processor.c -o processor.o -std=gnu99 -Wall -g3
	gcc cortex_m0.o processor.o -o processor -std=gnu99 -Wall -g3
