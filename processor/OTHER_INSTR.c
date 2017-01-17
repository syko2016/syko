#include "OTHER_INSTR.h"

void cm0_ANDS(struct cm0 *proc)
{
	/* page A6-116 of ARM DDI 0419C ID092410 */
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

	result = op1 & op2;

	/* Write result into the destination register */
 	cm0_set_reg(proc, Rdn, result);	

	/* Update flags */

	/* 
	 * Update negative value flag. If MSB is set to 1, value is negative. 
	 * Operator !! used to get 1 or 0 from value.
	 */
	cm0_set_flag(proc, N, !!(result & (1 << 31)));
	/*
	 * Update zero flag.
	 * Set flag to 1 if result equals to zero.
	 */
	cm0_set_flag(proc, Z, !result);
	/* Flags V and C not changed. */	
}

/*
 * this function has prev_instr argument which is first 16 bits of this
 * instruction, because this instruction is 32 bits wide!
 */
void cm0_BL(struct cm0 *proc, const uint16_t prev_instr)
{
/* page A6-123 of ARM DDI 0419C ID092410 */
	uint32_t next_instr_addr;
	uint8_t S, I1, I2, J1, J2;
	int16_t imm10, imm11;
	int32_t temp;
	const uint16_t instr = cm0_get_instr(proc); 
	const int32_t PC_r = cm0_get_reg(proc, PC);

	/*Get operands.*/
	S = !!(prev_instr & (1 << 10));
	J1 = !!(instr & (1 << 13));
	J2 = !!(instr & (1 << 11));
	imm10 = prev_instr & 0x01FF;
	imm11 = instr & 0x03FF;

	next_instr_addr = PC_r | 1;

	/*Set variables.*/
	I1 = !(J1 ^ S);
	I2 = !(J2 ^ S);
	temp = imm11 << 1;
	temp |= imm10 << (1 + 11);
	temp |= I2 << (1 + 11 + 10);
	temp |= I1 << (1 + 11 + 10 + 1);
	temp |= S << (1 + 11 + 10 + 1 + 1);
	temp = ((S * 0b01111111) << 25) | temp;

	/*Check condition*/
	if (InITBlock() && (!LastInITBlock())) {
		assert(0);
		return; /* UNPREDICTABLE */	
	}	
	
	cm0_set_reg(proc, LR, next_instr_addr);
	
	cm0_set_reg(proc, PC, PC_r + temp);	
}

void cm0_LDM(struct cm0 *proc)
{
	uint8_t Rn, wback;
	uint16_t registers;
	uint32_t address;
	uint32_t value;
	const uint8_t *base_addr = cm0_get_memd(proc);
	const uint32_t instr = cm0_get_instr(proc);
	
	Rn = (instr & 0b0000011100000000) >> 8;
	registers = instr & 0xFF;
	wback = !(registers & (1 << Rn));
	address = cm0_get_reg(proc, Rn);

	if (!registers) { 
		assert(0);
		return; /* UNPREDICTABLE */
	}

	for (int i = 0; i < 8; i++) {
		if (registers & (1 << i)) {
			value = *(uint32_t *)(base_addr + address);
			cm0_set_reg(proc, i, value);
			address += 4;
		}
	}
	if (wback) 
		cm0_set_reg(proc, Rn, address);
}

void cm0_REV(struct cm0 *proc)
{
	uint8_t Rm, Rd;
	uint32_t val, tmp, rot = 0;
 	const uint16_t instr = cm0_get_instr(proc);
	
	Rd = instr & 0b00000111;
	Rm = (instr & 0b00111000) >> 3;
	val = cm0_get_reg(proc, Rm);
	for (int i = 0; i < 4; i++) {
		tmp = (val & (0xFF << 8 * i)) >> (8 * i);
		rot |= tmp << (8 * (3 - i));
	}
	cm0_set_reg(proc, Rd, rot);
}
