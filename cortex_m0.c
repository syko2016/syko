#include "cortex_m0.h"

#define print_name()	printf("%s\n", __func__);

static inline uint32_t *cm0_get_reg_addr(struct cm0 *proc, 
					 enum cm0_reg_name reg)
{
	switch (reg){
	case R0:
		return &proc->regs.R0;
	case R1:
		return &proc->regs.R1;
	case R2:
		return &proc->regs.R2;
	case R3:
		return &proc->regs.R3;
	case R4:
		return &proc->regs.R4;
	case R5:
		return &proc->regs.R5;
	case R6:
		return &proc->regs.R6;
	case R7:
		return &proc->regs.R7;
	case R8:
		return &proc->regs.R8;
	case R9:
		return &proc->regs.R9;
	case R10:
		return &proc->regs.R10;
	case R11:
		return &proc->regs.R11;
	case R12:
		return &proc->regs.R12;
	/* TODO: SP, and MSP/PSP problem */
	case MSP:
		return &proc->regs.MSP;	
	case PSP:
		return &proc->regs.PSP;
	case LR:
		return &proc->regs.LR;
	case PC:
		return &proc->regs.PC;
	case PSR:
		return &proc->regs.PSR;
	case PRIMASK:
		return &proc->regs.PRIMASK;
	case CONTROL:
		return &proc->regs.CONTROL;
	default:
		break;
	} 
	return 0;
}

void cm0_reset(struct cm0 *proc)
{
	/* TODO */
}

void cm0_set_reg(struct cm0 *proc, enum cm0_reg_name reg, uint32_t val)
{
	uint32_t *reg_addr;

	reg_addr = cm0_get_reg_addr(proc, reg);
	*reg_addr = val;
}

uint32_t cm0_get_reg(struct cm0 *proc, enum cm0_reg_name reg)
{
	uint32_t *reg_addr;

	reg_addr = cm0_get_reg_addr(proc, reg);
	return *reg_addr;
}

int cm0_set_memc(struct cm0 *proc, uint8_t *buf, size_t buf_size, size_t off)
{
	if (buf_size + off > CM0_MEMC_SIZE) {
		errno = ENOMEM;
		return -ENOMEM;
	}
	memcpy(proc->memc + off, buf, buf_size);
	return 0;
}

/* not tested. */
int cm0_set_memd(struct cm0 *proc, uint8_t *buf, size_t buf_size, size_t off)
{
	if (buf_size + off > CM0_MEMD_SIZE) {
		errno = ENOMEM;
		return -ENOMEM;
	}
	memcpy(proc->memd + off, buf, buf_size);
	return 0;
}

const uint8_t *cm0_get_memd(struct cm0 *proc)
{	
	return proc->memd;
}

/* not tested. */
int cm0_set_byte(struct cm0 *proc, uint8_t value, size_t address)
{
	if (address > CM0_MEMD_SIZE) {
		errno = ENOMEM;
		return -ENOMEM;
	}
	proc->memd[address] = value;
	return 0;
}

/* not tested. */
int cm0_set_halfword(struct cm0 *proc, uint16_t value, size_t address)
{
	if (address + sizeof(uint16_t) > CM0_MEMD_SIZE) {
		errno = ENOMEM;
		return -ENOMEM;
	}

	for (int i = 0; i < 2; i++)
		proc->memd[address + i] = value >> (8 * i);	

	return 0;
}

/* not tested. */
int cm0_set_word(struct cm0 *proc, uint32_t value, size_t address)
{
	if (address + sizeof(uint32_t) > CM0_MEMD_SIZE) {
		errno = ENOMEM;
		return -ENOMEM;
	}

	for (int i = 0; i < 4; i++) 
		proc->memd[address + i] = value >> (8 * i);

	return 0;
}

int cm0_incr_PC(struct cm0 *proc)
{
	uint32_t pc = cm0_get_reg(proc, PC);
	pc += 2;
	
	/* End of code detection */
	if ((proc->memc[pc] | proc->memc[pc + 1]) == 0)
		return -1;

	cm0_set_reg(proc, PC, pc);	
	return 0;
}

uint16_t cm0_get_instr(struct cm0 *proc)
{
	uint16_t instr;
	uint32_t pc;

	pc = cm0_get_reg(proc, PC);
	
	/* Little endian to big endian conversion */
	instr = proc->memc[pc] << 8;
	instr |= proc->memc[pc + 1];

	return instr;
}

void cm0_ADD_immediate_T1(struct cm0 *proc)
{
	print_name();
}

void cm0_ADD_immediate_T2(struct cm0 *proc)
{
	print_name();
}

void cm0_ADD_register_T1(struct cm0 *proc)
{
	print_name();
}

void cm0_ADD_register_T2(struct cm0 *proc)
{
	print_name();
}

void cm0_ADD_SP_plus_immediate_T1(struct cm0 *proc)
{
	print_name();
}

void cm0_ADD_SP_plus_immediate_T2(struct cm0 *proc)
{
	print_name();
}

void cm0_ADD_SP_plus_register_T1(struct cm0 *proc)
{
	print_name();
}

void cm0_ADD_SP_plus_register_T2(struct cm0 *proc)
{
	print_name();
}

int cm0_decode_instruction(struct cm0 *proc, uint16_t instr)
{
	if ((instr & 0b1000000011111111) == 0b0000000010110000)
		cm0_ADD_SP_plus_immediate_T2(proc);

	else if ((instr & 0b0111100011111111) == 0b0110100001000100)
		cm0_ADD_SP_plus_register_T1(proc);

	else if ((instr & 0b1000011111111111) == 0b1000010101000100)
		cm0_ADD_SP_plus_register_T2(proc);

	else if ((instr & 0b11111111) == 0b00011100)
		cm0_ADD_immediate_T1(proc);

	else if ((instr & 0b11111000) == 0b00110000)
		cm0_ADD_immediate_T2(proc);
	
	else if ((instr & 0b11111110) == 0b00011000)
		cm0_ADD_register_T1(proc);

	else if ((instr & 0b11111111) == 0b01000100)
		cm0_ADD_register_T2(proc);

	else if ((instr & 0b11111000) == 0b10101000)
		cm0_ADD_SP_plus_immediate_T1(proc);

	else 
		printf("%s got instruction %04x, which is not understandable."
		"\n", __func__, instr);

	return 0;
}

int cm0_run(struct cm0 *proc)
{
	uint16_t instr;
	int ret;
	do {
		instr = cm0_get_instr(proc);
		ret = cm0_incr_PC(proc);
		cm0_decode_instruction(proc, instr);
	} while (!ret);
	return 0;
}
