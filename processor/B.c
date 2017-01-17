#include "B.h"

void cm0_B_T1(struct cm0 *proc)
{
	int8_t cond, imm8;
	int32_t imm32;
	int32_t PC_r;
	const uint32_t instr = cm0_get_instr(proc);
	cond = (instr & (15 << 8)) >> 8;
	imm8 = (instr & 0xFF);

	if (InITBlock()) {
		assert(0);
		return; /* UNPREDICTABLE */
	}
	
	if (cond == 14) {
		assert(0);
		return; /* UNDEFINED */
	} else if (cond == 15) {
		/*
		 * SVCall exception (exception #11)
		 * PC goes to 44, as it is written in section B1.5.3
		 * of ARM DDI 0419C IDO92410.
		 * 
		 * here cm0_set_reg gets 42, because it will be incremented 
		 * later.
		 */
		cm0_set_reg(proc, PC, 42); 
		return;
	}
	
	imm32 = (0xFFFFE00 * (imm8 > 7))  | (imm8 << 1);
	PC_r = cm0_get_reg(proc, PC);
	
	/* 
	 * There should be no "+ 2", but without this addition wrong
	 * instruction is executed.
	 */
	cm0_set_reg(proc, PC, PC_r + imm32 + 2);
	return;
}

void cm0_B_T2(struct cm0 *proc)
{
	int32_t imm32;
	const int32_t PC_r = cm0_get_reg(proc, PC);
	const int32_t instr = cm0_get_instr(proc);
	imm32 = instr & 2047;
	imm32 = (0xFFFFF000 * (imm32 > 10)) | (imm32 << 1);

	if (InITBlock() && !LastInITBlock()) {
		assert(0); 
		return;	/* UNPREDICTABLE */
	}

	/* 
	 * there should be no "+ 2", but without this addition wrong
	 * instruction is executed.
	 */
	cm0_set_reg(proc, PC, PC_r + imm32 + 2);
}
