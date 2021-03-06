#include "cortex_m0.h"

#include "ADD.h"
#include "ASRS.h"
#include "B.h"
#include "LDR.h"
#include "MOV.h"
#include "OTHER_INSTR.h"
#include "STR.h"

#define CM0_MEMC_OFFSET 0x100UL
#define CM0_MEMD_OFFSET 0x200000UL /* Here should be 0x20000000! */

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
	/* There are 2 stacks on M0. */
	case SP:
		if ((proc->regs.CONTROL & (1 << 1)) == 0) 
			return &proc->regs.MSP;	
		else
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
	cm0_set_reg(proc, PRIMASK, 0);
	cm0_set_reg(proc, CONTROL, 0);
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

int cm0_set_memc(struct cm0 *proc, uint8_t *buf, size_t buf_size)
{
	if (buf_size + CM0_MEMC_OFFSET > CM0_MEMC_SIZE) {
		errno = ENOMEM;
		return -ENOMEM;
	}
	memcpy(proc->mem.memc + CM0_MEMC_OFFSET, buf, buf_size);
	return 0;
}

int cm0_set_memd(struct cm0 *proc, uint8_t *buf, size_t buf_size)
{
	if (buf_size > CM0_MEMD_SIZE) {
		errno = ENOMEM;
		return -ENOMEM;
	}
	memcpy(proc->mem.memd, buf, buf_size);
	return 0;
}

const uint8_t *cm0_get_memd(struct cm0 *proc)
{
	return proc->mem.memd;
}

int cm0_set_all_regs(struct cm0 *proc, uint32_t *buf, size_t buf_size)
{
	if (buf_size < 19 * sizeof(uint32_t)) {
		errno = ENOMEM;
		return -ENOMEM;
	}		

	for (int i = 0; i < 19; i++)
		cm0_set_reg(proc, i, buf[i]);

	if ((proc->regs.CONTROL & (1 << 1)) == 1) {
		proc->regs.PSP = buf[13];
		proc->regs.MSP = 0;
	}

	return 0;
}

int cm0_get_all_regs(struct cm0 *proc, uint32_t *buf, size_t buf_size)
{
	if (buf_size < 19 * sizeof(uint32_t)) {
		errno = ENOMEM;
		return -ENOMEM;
	}
	
	for (int i = 0; i < 19; i++)
		buf[i] = cm0_get_reg(proc, i);

	return 0;
}

/* not tested. */
int cm0_set_byte(struct cm0 *proc, uint8_t value, size_t address)
{
	uint8_t *addr;

	addr = cm0_mem_get(proc, address);
	*addr = value;

	return 0;
}

/* not tested. */
int cm0_set_halfword(struct cm0 *proc, uint16_t value, size_t address)
{
	uint16_t *addr;
		
	addr = (uint16_t *)cm0_mem_get(proc, address);

	*addr = value;

	return 0;
}

/* not tested. */
int cm0_set_word(struct cm0 *proc, uint32_t value, size_t address)
{
	uint32_t *addr;
	
	addr = (uint32_t *)cm0_mem_get(proc, address);

	*addr = value;

	return 0;
}

int cm0_get_word(struct cm0 *proc, uint32_t *value, size_t address)
{
	const uint32_t *addr;

	addr = (uint32_t *)cm0_mem_get(proc, address);

	*value = *addr;

	return 0;
}

int cm0_incr_PC(struct cm0 *proc)
{
	uint8_t *addr;
	uint32_t pc = cm0_get_reg(proc, PC);
	pc += 2;
	
	addr = cm0_mem_get(proc, pc - 2);

	/* End of code detection */
	if ((*addr | *(addr + 1)) == 0)
		return -1;

	else if (pc >= CM0_MEMC_SIZE)
		return -1;

	cm0_set_reg(proc, PC, pc);	
	return 0;
}

uint16_t cm0_get_instr(struct cm0 *proc)
{
	uint16_t instr;
	uint32_t pc;
	const uint8_t *addr;

	pc = cm0_get_reg(proc, PC);
	pc -= 2;

	addr = cm0_mem_get(proc, pc);

	instr = (uint16_t)(*addr);
	instr |= (*(addr + 1)) << 8;

	return instr;
}

void cm0_check_32bit_instr(struct cm0 *proc, uint16_t prev_instr)
{
	uint16_t instr;

	/* this is 32 bit instruction, so PC will be incremented to get
	 * latter 16 bits.
	 */
	cm0_incr_PC(proc);
	instr = cm0_get_instr(proc);

	if ((instr & 0b1100000000000000) == 0b1100000000000000)
		cm0_BL(proc, prev_instr);
}

int cm0_decode_instruction(struct cm0 *proc)
{
	uint16_t instr = cm0_get_instr(proc);

	if ((instr & 0b1111100000000000) == 0b1010100000000000)
		cm0_ADD_SP_plus_immediate_T1(proc);

	else if ((instr & 0b1111111110000000) == 0b1011000000000000)
		cm0_ADD_SP_plus_immediate_T2(proc);

	else if ((instr & 0b1111111101111000) == 0b0100010001101000)
		cm0_ADD_SP_plus_register_T1(proc); 	

	else if ((instr & 0b1111111110000111) == 0b0100010010000101)
		cm0_ADD_SP_plus_register_T2(proc);

	else if ((instr & 0b1111111000000000) == 0b0001110000000000)
		cm0_ADD_immediate_T1(proc);

	else if ((instr & 0b1111100000000000) == 0b0011000000000000)
		cm0_ADD_immediate_T2(proc);
	
	else if ((instr & 0b1111111000000000) == 0b0001100000000000)
		cm0_ADD_register_T1(proc);

	else if ((instr & 0b1111111100000000) == 0b0100010000000000)
		cm0_ADD_register_T2(proc);
	
	else if ((instr & 0b1111111111000000) == 0b0100000000000000)
		cm0_ANDS(proc);
		
	else if ((instr & 0b1111100000000000) == 0b0001000000000000)
		cm0_ASRS_immediate(proc);
	
	else if ((instr & 0b1111111111000000) == 0b0100000100000000)
		cm0_ASRS_register(proc);

	else if ((instr & 0b1111000000000000) == 0b1101000000000000)
		cm0_B_T1(proc);

	else if ((instr & 0b1111100000000000) == 0b1110000000000000)
		cm0_B_T2(proc);

	else if ((instr & 0b1111100000000000) == 0b1111000000000000)
		 cm0_check_32bit_instr(proc, instr);

	else if	((instr & 0b1111100000000000) == 0b1100100000000000)
		cm0_LDM(proc);
	
	else if ((instr & 0b1111100000000000) == 0b0110100000000000)
		cm0_LDR_immediate_T1(proc);

	else if ((instr & 0b1111100000000000) == 0b1001100000000000)
		cm0_LDR_immediate_T2(proc);

	else if ((instr & 0b1111100000000000) == 0b0100100000000000)
		cm0_LDR_literal(proc);		
	
	else if ((instr & 0b1111111000000000) == 0b0101100000000000)
		cm0_LDR_register(proc);

	else if ((instr & 0b1111100000000000) == 0b0010000000000000)
		cm0_MOV_immediate(proc);

	else if ((instr & 0b1111111100000000) == 0b0100011000000000)
		cm0_MOV_register_T1(proc);

	else if ((instr & 0b1111111111000000) == 0b1011101000000000)
		cm0_REV(proc);
	
	else if ((instr & 0b1111100000000000) == 0b0110000000000000)
		cm0_STR_immediate_T1(proc);
	
	else if ((instr & 0b1111100000000000) == 0b1001000000000000)
		cm0_STR_immediate_T2(proc);
	
	else if ((instr & 0b1111111000000000) == 0b0101000000000000)
		cm0_STR_register(proc);
	
	else 
		printf("%s got instruction %04x, which is not understandable."
		"\n", __func__, instr);

	return 0;
}

int cm0_run(struct cm0 *proc)
{
	int ret;
	do {
		ret = cm0_incr_PC(proc);
		if (ret)
			break;

		cm0_decode_instruction(proc);
	} while (!ret);
	return 0;
}

int cm0_set_flag(struct cm0 *proc, enum cm0_flags flag, uint8_t value)
{
	uint32_t PSR_r = cm0_get_reg(proc, PSR);

	/* Wrong flag value test */
	if (value != 1 && value != 0) {
		errno = EINVAL;
		return -EINVAL;
	}	

	/* Clearing flag */
	PSR_r &= ~(1 << flag);	
	/* Setting flag */
	PSR_r |= (value << flag);

	cm0_set_reg(proc, PSR, PSR_r);

	return 0;
}

uint8_t cm0_get_flag(struct cm0 *proc, enum cm0_flags flag)
{
	uint32_t PSR_r = cm0_get_reg(proc, PSR);
	return ((PSR_r & (1 << flag)) >> flag);
}

int cm0_init(struct cm0 *proc)
{
	proc->mem.memc = calloc(1, CM0_MEMC_SIZE);
	if (!proc->mem.memc) {
		errno = ENOMEM;
		return -ENOMEM;
	}
	proc->mem.memd = calloc(1, CM0_MEMD_SIZE);
	if (!proc->mem.memd) {
		errno = ENOMEM;
		free(proc->mem.memc);
		proc->mem.memc = NULL;
		return -ENOMEM;
	}

	return 0;
}

void cm0_deinit(struct cm0 *proc)
{
	free(proc->mem.memd);
	free(proc->mem.memc);
}

uint8_t *cm0_mem_get(struct cm0 *proc, size_t offset)
{
	if (offset < CM0_MEMD_OFFSET) {
		if (offset > CM0_MEMC_SIZE)
			return NULL;

		return proc->mem.memc + offset;
	} else {
		if (offset - CM0_MEMD_OFFSET > CM0_MEMD_SIZE)
			return NULL;

		return proc->mem.memd + (offset - CM0_MEMD_OFFSET);
	}
}
