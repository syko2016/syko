/* Author: Konrad Gotfryd */

#include "ASR.h"

void cm0_ASR_immediate(struct cm0 *proc)
{
	uint8_t Rm, Rd, setflags, carry, imm5;
	const uint16_t instr = cm0_get_instr(proc);
	uint32_t val, result;
	
	setflags = !InITBlock();
	Rm = (instr & (7 << 3)) >> 3;
	Rd = (instr & 7);
	imm5 = (instr & (31 << 6)) >> 6;
	
	val = cm0_get_reg(proc, Rm);

	if (imm5) {
		result = ((((1 << imm5) - 1) * (val >> 31)) << (32 - imm5)) 
			 | (val >> imm5);
		carry = (val >> (imm5 - 1)) & 1;

	} else {
		result = 0xFFFFFFFF * (val >> 31);
		carry = val >> 31;
	}
	
	cm0_set_reg(proc, Rd, result);
	if (setflags) {	
		cm0_set_flag(proc, N, result >> 31);
		cm0_set_flag(proc, C, carry);
		cm0_set_flag(proc, Z, !result);
	}
}

void cm0_ASR_register(struct cm0 *proc)
{
	uint8_t Rm, Rd, carry, shift, setflags;
	const uint16_t instr = cm0_get_instr(proc);
	uint32_t val, result;

	Rm = (instr & (7 << 3)) >> 3;
	Rd = (instr & 7);
	
	shift = cm0_get_reg(proc, Rm) & 0xFF;
	val = cm0_get_reg(proc, Rd);

	setflags = !InITBlock();
	
	if (shift) {
		result = ((((1 << shift) - 1) * (val >> 31)) << (32 - shift)) 
			 | (val >> shift);
		carry = (val >> (shift - 1)) & 1;
	} else {
		result = val;
		carry = 0;
	}
	
	cm0_set_reg(proc, Rd, result);
	if (setflags) {
		cm0_set_flag(proc, N, result >> 31);
		cm0_set_flag(proc, C, carry);
		cm0_set_flag(proc, Z, !result);
	}
}
