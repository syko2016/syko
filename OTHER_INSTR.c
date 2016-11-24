#include "OTHER_INSTR.h"

void cm0_ANDS(struct cm0 *proc)
{
	/* page A6-116 of ARM DDI 0419C ID092410 */

	/* TODO: FLAGS! */
	uint16_t instr;
	uint8_t Rm;
	uint8_t Rdn;
	int32_t op1, op2, result;	

	/* Get actual instruction from program memory */
	instr = cm0_get_instr(proc);
	
	/* Get register which contains the first operand. */
	Rdn = instr & 0b0000000000000111;
	
	/*Get register which contains the second operand. */
	Rm = (instr & 0b0000000000111000) >> 3;

	/* Get operands values */
	op1 = cm0_get_reg(proc, Rdn);
	op2 = cm0_get_reg(proc, Rm);

	result = op1 * op2;

	/* Write result into the destination register */
 	cm0_set_reg(proc, Rdn, result);	
}

/*
 * this function has prev_instr argument which is first 16 bits of this
 * instruction, because this instruction is 32 bits wide!
 */
void cm0_BL(struct cm0 *proc, uint16_t prev_instr)
{
	print_name();
}

void cm0_LDM(struct cm0 *proc)
{
	print_name();
}

void cm0_REV(struct cm0 *proc)
{
	print_name();
}
