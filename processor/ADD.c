#include "ADD.h"

void cm0_ADD_immediate_T1(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rd, Rn;
	uint32_t op1, result;
	int8_t imm3;

	instr = cm0_get_instr(proc);
	Rd = instr & 0b0000000000000111;
	Rn = (instr & 0b0000000000111000) >> 3;
	imm3 = (instr & 0b0000000111000000) >> 6;

	op1 = cm0_get_reg(proc, Rn);
	result = op1 + imm3;
	cm0_set_reg(proc, Rd, result);
}

void cm0_ADD_immediate_T2(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rdn;
	int32_t op1, result;
	int8_t imm8;

	instr = cm0_get_instr(proc);
	imm8 = instr & 0b0000000011111111;
	Rdn = (instr & 0b0000011100000000) >> 8;
	op1 = cm0_get_reg(proc, Rdn);
	result = op1 + imm8;
	cm0_set_reg(proc, Rdn, result);
}

void cm0_ADD_register_T1(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rd, Rn, Rm;
	int32_t op1, op2, result;

	instr = cm0_get_instr(proc);
	Rd = instr & 0b0000000000000111;
	Rn = (instr & 0b0000000000111000) >> 3;
	Rm = (instr & 0b0000000111000000) >> 6;

	op1 = cm0_get_reg(proc, Rn);
	op2 = cm0_get_reg(proc, Rm);
	result = op1 + op2;
	cm0_set_reg(proc, Rd, result);
}

void cm0_ADD_register_T2(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rdn, Rm;
	int32_t op1, op2, result;

	instr = cm0_get_instr(proc);
	Rdn = instr & 0b0000000000000111;
	Rm = (instr & 0b0000000001111000) >> 3;

	op1 = cm0_get_reg(proc, Rdn);
	op2 = cm0_get_reg(proc, Rm);
	result = op1 + op2;
	cm0_set_reg(proc, Rdn, result);

}

void cm0_ADD_SP_plus_immediate_T1(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rd;
	int8_t imm8;
	int32_t sp, result;

	instr = cm0_get_instr(proc);
	imm8 = instr & 0b0000000011111111;
	Rd = (instr & 0b0000011100000000) >> 8;
	sp = cm0_get_reg(proc, SP);
	result = sp + imm8;
	cm0_set_reg(proc, Rd, result);
}

void cm0_ADD_SP_plus_immediate_T2(struct cm0 *proc)
{
	uint16_t instr;
	int8_t imm7;
	int32_t sp, result;

	instr = cm0_get_instr(proc);
	imm7 = instr & 0b0000000001111111;
	sp = cm0_get_reg(proc, SP);
	result = sp + imm7;
	cm0_set_reg(proc, SP, result);
}

void cm0_ADD_SP_plus_register_T1(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rdm;
	int32_t op, sp, result;

	instr = cm0_get_instr(proc);
	Rdm = instr & 0b0000000000000111;
	sp = cm0_get_reg(proc, SP);
	op = cm0_get_reg(proc, Rdm);
	result = sp + op;
	cm0_set_reg(proc, Rdm, result);
}

void cm0_ADD_SP_plus_register_T2(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rm;
	int32_t op, sp, result;

	instr = cm0_get_instr(proc);
	Rm = instr & 0b0000000000111000;
	op = cm0_get_reg(proc, Rm);
	sp = cm0_get_reg(proc, SP);
	result = sp + op;
	cm0_set_reg(proc, SP, result);
}
