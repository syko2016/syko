default:
	arm-none-eabi-as -march=armv6-m file_code.s -o file_code.elf -g0
	arm-none-eabi-objcopy --output-target binary file_code.elf file_code.bin
	arm-none-eabi-objdump -d file_code.elf > file_code.lst
	gcc -c ADD.c -o ADD.o -std=gnu99 -Wall -g3
	gcc -c ASR.c -o ASR.o -std=gnu99 -Wall -g3
	gcc -c B.c -o B.o -std=gnu99 -Wall -g3
	gcc -c LDR.c -o LDR.o -std=gnu99 -Wall -g3
	gcc -c MOV.c -o MOV.o -std=gnu99 -Wall -g3
	gcc -c OTHER_INSTR.c -o OTHER_INSTR.o -std=gnu99 -Wall -g3
	gcc -c STR.c -o STR.o -std=gnu99 -Wall -g3
	gcc -c cortex_m0.c -o cortex_m0.o -std=gnu99 -Wall -g3
	gcc -c processor.c -o processor.o -std=gnu99 -Wall -g3
	gcc ADD.o ASR.o B.o LDR.o MOV.o OTHER_INSTR.o STR.o cortex_m0.o processor.o -o processor -std=gnu99 -Wall -g3
