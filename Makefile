CFLAGS += -O0 -g3 -Wall -Wextra -std=gnu99 -I./processor/
.PHONY: all clean

SRC_DIR = .
include $(SRC_DIR)/processor/make.config
include $(SRC_DIR)/test_cases/ANDS/make.config
include $(SRC_DIR)/test_cases/ADD/make.config
include $(SRC_DIR)/test_cases/BL/make.config
include $(SRC_DIR)/test_cases/LDM/make.config
include $(SRC_DIR)/test_cases/MOV/make.config
include $(SRC_DIR)/test_cases/REV/make.config
include $(SRC_DIR)/test_cases/STR/make.config
include $(SRC_DIR)/test_cases/ASRS/make.config
include $(SRC_DIR)/test_cases/B/make.config
include $(SRC_DIR)/test_cases/LDR/make.config
PROC_OBJ := $(patsubst %, $(SRC_DIR)/processor/%,$(_PROC_OBJ))
ANDS_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/ANDS/%,$(_ANDS_OBJ))
ANDS_ASM := $(patsubst %, $(SRC_DIR)/test_cases/ANDS/%,$(_ANDS_ASM))
ADD_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/ADD/%,$(_ADD_OBJ))
ADD_ASM := $(patsubst %, $(SRC_DIR)/test_cases/ADD/%,$(_ADD_ASM))
BL_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/BL/%,$(_BL_OBJ))
BL_ASM := $(patsubst %, $(SRC_DIR)/test_cases/BL/%,$(_BL_ASM))
LDM_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/LDM/%,$(_LDM_OBJ))
LDM_ASM := $(patsubst %, $(SRC_DIR)/test_cases/LDM/%,$(_LDM_ASM))
MOV_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/MOV/%,$(_MOV_OBJ))
MOV_ASM := $(patsubst %, $(SRC_DIR)/test_cases/MOV/%,$(_MOV_ASM))
REV_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/REV/%,$(_REV_OBJ))
REV_ASM := $(patsubst %, $(SRC_DIR)/test_cases/REV/%,$(_REV_ASM))
ASRS_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/ASRS/%,$(_ASRS_OBJ))
ASRS_ASM := $(patsubst %, $(SRC_DIR)/test_cases/ASRS/%,$(_ASRS_ASM))
B_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/B/%,$(_B_OBJ))
B_ASM := $(patsubst %, $(SRC_DIR)/test_cases/B/%,$(_B_ASM))
LDR_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/LDR/%,$(_LDR_OBJ))
LDR_ASM := $(patsubst %, $(SRC_DIR)/test_cases/LDR/%,$(_LDR_ASM))
STR_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/STR/%,$(_STR_OBJ))
STR_ASM := $(patsubst %, $(SRC_DIR)/test_cases/STR/%,$(_STR_ASM))

all : ands_test bl_test ldm_test rev_test asrs_test b_test ldr_test add_test mov_test str_test

ands_test : $(PROC_OBJ) $(ANDS_ASM) $(ANDS_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(ANDS_OBJ) -o ./test_cases/ANDS/$@

add_test : $(PROC_OBJ) $(ADD_ASM) $(ADD_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(ADD_OBJ) -o ./test_cases/ADD/$@

bl_test : $(PROC_OBJ) $(BL_ASM) $(BL_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(BL_OBJ) -o ./test_cases/BL/$@

ldm_test : $(PROC_OBJ) $(LDM_ASM) $(LDM_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(LDM_OBJ) -o ./test_cases/LDM/$@

rev_test : $(PROC_OBJ) $(REV_ASM) $(REV_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(REV_OBJ) -o ./test_cases/REV/$@

mov_test : $(PROC_OBJ) $(MOV_ASM) $(MOV_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(MOV_OBJ) -o ./test_cases/MOV/$@

asrs_test : $(PROC_OBJ) $(ASRS_ASM) $(ASRS_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(ASRS_OBJ) -o ./test_cases/ASRS/$@

b_test : $(PROC_OBJ) $(B_ASM) $(B_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(B_OBJ) -o ./test_cases/B/$@

ldr_test : $(PROC_OBJ) $(LDR_ASM) $(LDR_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(LDR_OBJ) -o ./test_cases/LDR/$@

str_test : $(PROC_OBJ) $(STR_ASM) $(STR_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(STR_OBJ) -o ./test_cases/STR/$@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

%.bin: %.elf
	arm-none-eabi-objcopy --output-target binary $< $@

%.elf: %.s
	arm-none-eabi-as -mcpu=cortex-m0 $< -o $@

clean:
	rm -f $(PROC_OBJ) $(ANDS_ASM) $(ANDS_OBJ) $(ADD_OBJ) $(ADD_ASM) $(BL_OBJ) $(BL_ASM) $(LDM_OBJ)
	rm -f $(LDM_ASM) $(REV_OBJ) $(REV_ASM) $(ASRS_OBJ) $(ASRS_ASM) $(B_OBJ) $(B_ASM) $(LDR_OBJ) $(LDR_ASM)
	rm -f $(MOV_ASM) $(MOV_OBJ)
