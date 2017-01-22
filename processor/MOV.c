#include "MOV.h"

void cm0_MOV_immediate(struct cm0 *proc)
{
	/* page A6-156 of ARM DDI 0419C ID092410 */
	uint16_t instr;
	uint8_t Rd;
	int8_t imm8;
	
	/* Get actual instruction from program memory */
	instr = cm0_get_instr(proc);

	/* Get destination register from instruction */
	Rd = (instr & 0b0000011100000000) >> 8;

	/* Get immediate value from instruction */
	imm8 = instr & 0b0000000011111111;

	/* Set this value in register */
	cm0_set_reg(proc, Rd, imm8);
}

void cm0_MOV_register_T1(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rd, Rm;
	int32_t val;
	uint8_t D;
	instr = cm0_get_instr(proc);
	Rd = instr & 0b0000000000000111;
	Rm = (instr & 0b0000000001111000) >> 3;

	D = instr & 0b0000000010000000;
	D = !!(D);
	if (D) 
		Rd |= (1 << 3);

	val = cm0_get_reg(proc, Rm);

	cm0_set_reg(proc, Rd, val);
}
