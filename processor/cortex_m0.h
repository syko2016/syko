#ifndef _CORTEX_M0_H_
#define _CORTEX_M0_H_

#include <stdlib.h>
#include <stdint.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define print_name()	printf("%s\n", __func__);

enum cm0_reg_name {
	R0 = 0,
	R1 = 1,
	R2 = 2,
	R3 = 3,
	R4 = 4,
	R5 = 5,
	R6 = 6,
	R7 = 7,
	R8 = 8,
	R9 = 9,
	R10 = 10,
	R11 = 11,
	R12 = 12,
	SP = 13,
	LR = 14,
	PC = 15,
	PSR = 16,
	PRIMASK = 17,
	CONTROL = 18
};

/* 
 * Enums for PSR register.
 * It starts with value 31 and ends with 28 to represent bit offset in APSR.
 * Full description of APSR reg. and flags is on page A2-38.
 */
enum cm0_flags {
	V = 28, /* Overflow condition code flag */
	C = 29, /* Carry condition code flag    */
	Z = 30, /* Zero condition code flag     */
	N = 31  /* Negative condition code flag */
};

struct cm0_registers {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R4;
	uint32_t R5;
	uint32_t R6;
	uint32_t R7;
	uint32_t R8;
	uint32_t R9;
	uint32_t R10;
	uint32_t R11;
	uint32_t R12;
	uint32_t MSP;
	uint32_t PSP;
	uint32_t LR;
	uint32_t PC;
	
	uint32_t PSR;
	uint32_t PRIMASK;
	uint32_t CONTROL;
};

struct cm0 {
	uint8_t *memc;
	uint8_t *memd;

	struct cm0_registers regs;
};
/** @brief: Processor internal reset.
 *
 *  This function resets memory and registers as it is specified in processor
 *  documentation. As a consequence, some of registers may have unspecified 
 *  value.
 */
void cm0_reset(struct cm0 *proc);


/** @brief: initalize proc struct.
 *
 * This function allocates memory for internal buffers.
 * Returns -1 and sets errno if there is some problem with memory alloc.
 */
int cm0_init(struct cm0 *proc);

/** @brief: deinitalize proc struct.
 */
void cm0_deinit(struct cm0 *proc);

/** @brief: Processor register value setter. */
void cm0_set_reg(struct cm0 *proc, enum cm0_reg_name reg, uint32_t val);
/** @brief: Processor register value getter. */
uint32_t cm0_get_reg(struct cm0 *proc, enum cm0_reg_name reg);

/** @brief: Memory code setter.
 *
 * This function copies value from buf into the code memory. In future
 * alignment check may be perfomed before copy! 
 * RETURN VALUES:
 * On success: 0.
 * On failure: negative error code:
 * -EINVAL when alignment is wrong (NOT IMPLEMENTED YET),
 * -ENOMEM when there is no enough space.
 * On failure errno is set accordingly to the return value.
 */
int cm0_set_memc(struct cm0 *proc, uint8_t *buf, size_t buf_size);
/** @brief: Memory data setter.
 *
 * This function copies value from buf into the data memory.
 * RETURN VALUES:
 * On success: 0.
 * On failure: negative error code:
 * -EINVAL when alignment is wrong (NOT IMPLEMENTED YET),
 * -ENOMEM when there is no enough space.
 * On failure errno is set accordingly to the return value.
 */
int cm0_set_memd(struct cm0 *proc, uint8_t *buf, size_t buf_size);

/** @brief: Memory data getter.
 *
 * This function returns value to the data memory. 
 */
const uint8_t *cm0_get_memd(struct cm0 *proc);

/** @brief: Byte setter function.
 *
 * THIS FUNCTION MAY BE REMOVED!
 *
 * This function sets byte at specific address. in future, alignment check may
 * be performed before write! 
 * RETURN VALUES:
 * on success: 0.
 * on faliure: negative error code:
 * -ENOMEM if memory corruption may happen.
 * -EINVAL if alignment is wrong (NOT IMPLEMENTED YET).
 * on failure errno is set accordingly to the return value.
 */
int cm0_set_byte(struct cm0 *proc, uint8_t value, size_t address);
/** @brief: Halfword setter function.
 *
 * This function sets halfword at specific address.
 * RETURN VALUES:
 * on success: 0.
 * on faliure: negative error code:
 * -ENOMEM if memory corruption may happen.
 * -EINVAL if alignment is wrong (NOT IMPLEMENTED YET).
 * on failure errno is set accordingly to the return value.
 */
int cm0_set_halfword(struct cm0 *proc, uint16_t value, size_t address);

/** @brief: Word setter function.
 *
 * This function sets word at specific address.
 * RETURN VALUES:
 * on success: 0.
 * on faliure: negative error code:
 * -ENOMEM if memory corruption may happen.
 * -EINVAL if alignment is wrong (NOT IMPLEMENTED YET).
 * on failure errno is set accordingly to the return value.
 */
int cm0_set_word(struct cm0 *proc, uint32_t value, size_t address);

/** @brief Processor run function.
 *
 *  Function fetches all instructions from program memory, executes them,
 * and returns 0 on exit.
 */
int cm0_run(struct cm0 *proc);

/*
 * TODO comments
 */
int cm0_incr_PC(struct cm0 *proc);
uint16_t cm0_get_instr(struct cm0 *proc);

/** @brief Register setter function.
 * This function sets all registers before any work is done.
 * It may be used for testing purposes.
 */
int cm0_set_all_regs(struct cm0 *proc, uint32_t *buf, size_t buf_size);

/** @brief Register getter function.
 * This function gets all registers after work is done.
 * It may be used for testing purposes.
 */
int cm0_get_all_regs(struct cm0 *proc, uint32_t *buf, size_t buf_size);

/** @brief Flag setter function. 
 * value - must be 0 or 1. If it is not - negative value (-EINVAL) is returned.
 * On success 0 is returned.
 */
int cm0_set_flag(struct cm0 *proc, enum cm0_flags flag, uint8_t value);  

#endif
