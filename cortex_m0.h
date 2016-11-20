#ifndef _CORTEX_M0_H_
#define _CORTEX_M0_H_

#include <stdint.h> 
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#define CM0_MEMC_SIZE 1024
#define CM0_MEMD_SIZE 1024

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
	MSP = 13,
	PSP = 14,
	LR = 15,
	PC = 16,
	PSR = 17,
	PRIMASK = 18,
	CONTROL = 19
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
	uint8_t memc[CM0_MEMC_SIZE];
	uint8_t memd[CM0_MEMD_SIZE];

	struct cm0_registers regs;
};
/** @brief: Processor internal reset.
 *
 *  This function resets memory and registers as it is specified in processor
 *  documentation. As a consequence, some of registers may have unspecified 
 *  value.
 */
void cm0_reset(struct cm0 *proc);

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
int cm0_set_memc(struct cm0 *proc, uint8_t *buf, size_t buf_size, size_t off);
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
int cm0_set_memd(struct cm0 *proc, uint8_t *buf, size_t buf_size, size_t off);

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
int cm0_incr_PC(struct cm0 *proc);
uint16_t cm0_get_instr(struct cm0 *proc);

void cm0_LDR(struct cm0 *proc);
void cm0_STR(struct cm0 *proc);
void cm0_LDM(struct cm0 *proc);
void cm0_ADD(struct cm0 *proc);
void cm0_ANDS(struct cm0 *proc);
void cm0_ASRS(struct cm0 *proc);
void cm0_B(struct cm0 *proc);
void cm0_MOV(struct cm0 *proc);
void cm0_BL(struct cm0 *proc);
void cm0_REV(struct cm0 *proc);

#endif