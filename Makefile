default:
	arm-none-eabi-as -march=armv6-m main.s -o main.elf
	arm-none-eabi-objcopy --output-target binary main.elf main.bin
	arm-none-eabi-objdump -d main.elf > main.lst
	gcc processor.c -o processor -std=gnu99 -Wall
