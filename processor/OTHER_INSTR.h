#ifndef _OTHER_INSTR_H_
#define _OTHER_INSTR_H_

#include <assert.h>
#include <endian.h>
#include "cortex_m0.h"

void cm0_ANDS(struct cm0 *proc);
void cm0_BL(struct cm0 *proc, const uint16_t prev_instr);
void cm0_LDM(struct cm0 *proc);
void cm0_REV(struct cm0 *proc);

#endif
