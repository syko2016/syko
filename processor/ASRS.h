#ifndef _ASR_H_
#define _ASR_H_

#include "cortex_m0.h"
#include "initblock.h"

void cm0_ASRS_immediate(struct cm0 *proc);
void cm0_ASRS_register(struct cm0 *proc);

#endif
