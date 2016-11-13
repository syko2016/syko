default:
	arm-none-eabi-as -march=armv6-m main.s -o main.elf
	arm-none-eabi-objcopy --output-target binary main.elf main.bin
	arm-none-eabi-objdump -d main.elf > main.lst
	gcc -c cortex_m0.c -o cortex_m0.o -std=gnu99 -Wall
	gcc -c processor.c -o processor.o -std=gnu99 -Wall
	gcc cortex_m0.o processor.o -o processor -std=gnu99 -Wall
