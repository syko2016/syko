CFLAGS += -O0 -g3 -Wall -Wextra -std=gnu99 -I./processor/
.PHONY: all clean

SRC_DIR = .
include $(SRC_DIR)/processor/make.config
include $(SRC_DIR)/test_cases/ANDS/make.config
include $(SRC_DIR)/test_cases/BL/make.config
include $(SRC_DIR)/test_cases/LDM/make.config
include $(SRC_DIR)/test_cases/REV/make.config
include $(SRC_DIR)/test_cases/ASR/make.config
include $(SRC_DIR)/test_cases/B/make.config
include $(SRC_DIR)/test_cases/LDR/make.config
PROC_OBJ := $(patsubst %, $(SRC_DIR)/processor/%,$(_PROC_OBJ))
ANDS_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/ANDS/%,$(_ANDS_OBJ))
ANDS_ASM := $(patsubst %, $(SRC_DIR)/test_cases/ANDS/%,$(_ANDS_ASM))
BL_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/BL/%,$(_BL_OBJ))
BL_ASM := $(patsubst %, $(SRC_DIR)/test_cases/BL/%,$(_BL_ASM))
LDM_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/LDM/%,$(_LDM_OBJ))
LDM_ASM := $(patsubst %, $(SRC_DIR)/test_cases/LDM/%,$(_LDM_ASM))
REV_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/REV/%,$(_REV_OBJ))
REV_ASM := $(patsubst %, $(SRC_DIR)/test_cases/REV/%,$(_REV_ASM))
ASR_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/ASR/%,$(_ASR_OBJ))
ASR_ASM := $(patsubst %, $(SRC_DIR)/test_cases/ASR/%,$(_ASR_ASM))
B_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/B/%,$(_B_OBJ))
B_ASM := $(patsubst %, $(SRC_DIR)/test_cases/B/%,$(_B_ASM))
LDR_OBJ := $(patsubst %, $(SRC_DIR)/test_cases/LDR/%,$(_LDR_OBJ))
LDR_ASM := $(patsubst %, $(SRC_DIR)/test_cases/LDR/%,$(_LDR_ASM))

all : ands_test bl_test ldm_test rev_test asr_test b_test ldr_test

ands_test : $(PROC_OBJ) $(ANDS_ASM) $(ANDS_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(ANDS_OBJ) -o ./test_cases/ANDS/$@

bl_test : $(PROC_OBJ) $(BL_ASM) $(BL_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(BL_OBJ) -o ./test_cases/BL/$@

ldm_test : $(PROC_OBJ) $(LDM_ASM) $(LDM_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(LDM_OBJ) -o ./test_cases/LDM/$@

rev_test : $(PROC_OBJ) $(REV_ASM) $(REV_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(REV_OBJ) -o ./test_cases/REV/$@

asr_test : $(PROC_OBJ) $(ASR_ASM) $(ASR_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(ASR_OBJ) -o ./test_cases/ASR/$@

b_test : $(PROC_OBJ) $(B_ASM) $(B_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(B_OBJ) -o ./test_cases/B/$@

ldr_test : $(PROC_OBJ) $(LDR_ASM) $(LDR_OBJ)
	gcc $(CFLAGS) $(PROC_OBJ) $(LDR_OBJ) -o ./test_cases/LDR/$@

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

%.bin: %.elf
	arm-none-eabi-objcopy --output-target binary $< $@

%.elf: %.s
	arm-none-eabi-as -march=armv6-m $< -o $@

clean:
	rm -f $(PROC_OBJ) $(ANDS_ASM) $(ANDS_OBJ) $(BL_OBJ) 
