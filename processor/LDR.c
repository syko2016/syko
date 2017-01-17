/* Author: Konrad Gotfryd */

#include "LDR.h"

void cm0_LDR_immediate_T1(struct cm0 *proc)
{
	uint8_t Rn, Rt;
	const uint32_t instr = cm0_get_instr(proc);
	uint32_t addr;
	uint32_t offset;
	uint32_t value;

	offset = (instr & (31 << 6)) >> 6;
	Rn = (instr & (7 << 3)) >> 3;
	Rt = instr & 7;

	offset = offset << 2;
	addr = cm0_get_reg(proc, Rn);
	addr += offset;
	
	cm0_get_word(proc, &value, addr);
	
	cm0_set_reg(proc, Rt, value);
}	

void cm0_LDR_immediate_T2(struct cm0 *proc)
{
	uint8_t Rt;
	uint32_t addr, value;
	const uint32_t instr = cm0_get_instr(proc);
	
	addr = cm0_get_reg(proc, SP);
	addr += ((instr & 0xFF) << 2);

	cm0_get_word(proc, &value, addr);
	
	Rt = (instr & (7 << 8)) >> 8;
	
	cm0_set_reg(proc, Rt, value);
}

void cm0_LDR_literal(struct cm0 *proc)
{
	uint8_t Rt;
	uint32_t addr, value;
	const uint32_t instr = cm0_get_instr(proc);

	addr = cm0_get_reg(proc, PC);
	addr = 4 * (addr / 4);
	addr += (instr & 0xFF) << 2;

	cm0_get_word(proc, &value, addr);

	Rt = (instr & (7 << 8)) >> 8;
	
	cm0_set_reg(proc, Rt, value);
}

void cm0_LDR_register(struct cm0 *proc)
{
	uint8_t Rm, Rn, Rt;
	uint32_t addr, value;
	const uint32_t instr = cm0_get_instr(proc);

	Rm = (instr & (7 << 6)) >> 6;
	Rn = (instr & (7 << 3)) >> 3;
	Rt = instr & 7;

	addr = cm0_get_reg(proc, Rn) + cm0_get_reg(proc, Rm);
	cm0_get_word(proc, &value, addr);

	cm0_set_reg(proc, Rt, value);
}
