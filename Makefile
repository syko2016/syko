CFLAGS += -O0 -g3 -Wall -Wextra -std=gnu99 -I./processor/
.PHONY: all clean

SRC_DIR = .
include $(SRC_DIR)/processor/make.config
include $(SRC_DIR)/test_cases/ANDS/make.config
PROC_OBJ := $(patsubst %, $(SRC_DIR)/processor/%,$(_PROC_OBJ))
ANDS_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/ANDS/%,$(_ANDS_OBJ))
ANDS_ASM := $(patsubst %, $(SRC_DIR)/test_cases/ANDS/%,$(_ANDS_ASM))

OBJS := $(PROC_OBJ) $(ANDS_OBJ)
all : ands_test

ands_test : $(OBJS) $(ANDS_ASM)
	gcc $(CFLAGS) $(OBJS) -o ./test_cases/ANDS/$@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

%.bin: %.elf
	arm-none-eabi-objcopy --output-target binary $< $@

%.elf: %.s
	arm-none-eabi-as -march=armv6-m $< -o $@

clean:
	rm -f $(OBJS) $(ANDS_ASM)
