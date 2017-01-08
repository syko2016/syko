CFLAGS += -O0 -g3 -Wall -Wextra -std=gnu99 -I./processor/
.PHONY: all clean

SRC_DIR = .
include $(SRC_DIR)/processor/make.config
include $(SRC_DIR)/test_cases/ANDS/make.config
include $(SRC_DIR)/test_cases/BL/make.config
PROC_OBJ := $(patsubst %, $(SRC_DIR)/processor/%,$(_PROC_OBJ))
ANDS_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/ANDS/%,$(_ANDS_OBJ))
ANDS_ASM := $(patsubst %, $(SRC_DIR)/test_cases/ANDS/%,$(_ANDS_ASM))
BL_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/BL/%,$(_BL_OBJ))
BL_ASM := $(patsubst %, $(SRC_DIR)/test_cases/BL/%,$(_BL_ASM))

all : ands_test bl_test

ands_test : $(PROC_OBJ) $(ANDS_ASM) $(ANDS_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(ANDS_OBJ) -o ./test_cases/ANDS/$@

bl_test : $(PROC_OBJ) $(BL_ASM) $(BL_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(BL_OBJ) -o ./test_cases/BL/$@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

%.bin: %.elf
	arm-none-eabi-objcopy --output-target binary $< $@

%.elf: %.s
	arm-none-eabi-as -march=armv6-m $< -o $@

clean:
	rm -f $(PROC_OBJ) $(ANDS_ASM) $(ANDS_OBJ) $(BL_OBJ) 
