#include "B.h"

void cm0_B_T1(struct cm0 *proc)
{
	uint8_t cond, imm8;
	uint32_t imm32;
	uint32_t PC_r;
	const uint32_t instr = cm0_get_instr(proc);
	cond = (instr & (15 << 8)) >> 8;
	imm8 = (instr & 0xFF);
	
	print_name();

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
	cm0_set_reg(proc, PC, PC_r + imm32 + 2);
	return;
}

void cm0_B_T2(struct cm0 *proc)
{
	print_name();
	uint32_t imm32;
	const PC_r = cm0_get_reg(proc, PC);
	const uint32_t instr = cm0_get_instr(proc);
	imm32 = instr & 2047;
	imm32 = (0xFFFFF000 * (imm32 > 10)) | (imm32 << 1);

	if (InITBlock() && !LastInITBlock()) {
		assert(0); 
		return;	/* UNPREDICTABLE */
	}
	cm0_set_reg(proc, PC, PC_r + imm32 + 2);
}
