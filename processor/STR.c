#include "STR.h"

void cm0_STR_immediate_T1(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rt, Rn;
	int8_t imm5;
	int32_t value;
	uint32_t address;

	instr = cm0_get_instr(proc);
	Rt = instr & 0b0000000000000111;
	Rn = (instr & 0b0000000000111000) >> 3;
	imm5 = (instr & 0b0000011111000000) >> 6;
	value = cm0_get_reg(proc, Rn);
	address = imm5 + value;
	cm0_set_word(proc, Rt, address);

}

void cm0_STR_immediate_T2(struct cm0 *proc)
{
	uint16_t instr;
	int8_t imm8;
	int32_t sp;
	uint8_t Rt;
	uint32_t address;

	instr = cm0_get_instr(proc);
	imm8 = instr & 0b0000000011111111;
	Rt = (instr & 0b0000011100000000) >> 8;
	sp = cm0_get_reg(proc, SP);
	address = imm8 + sp;
	cm0_set_word(proc, Rt, address);

}

void cm0_STR_register(struct cm0 *proc)
{
	uint16_t instr;
	uint8_t Rt, Rm, Rn;
	int32_t value1, value2;
	uint32_t address;

	instr = cm0_get_instr(proc);
	Rt = instr & 0b0000000000000111;
	Rn = (instr & 0b0000000000111000) >> 3;
	Rm = (instr & 0b0000000111000000) >> 6;
	value1 = cm0_get_reg(proc, Rn);
	value2 = cm0_get_reg(proc, Rm);
	address = value1 + value2;
	cm0_set_word(proc, Rt, address);
}
