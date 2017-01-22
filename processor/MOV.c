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
	print_name();
}
