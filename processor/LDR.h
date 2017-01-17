/* Author: Konrad Gotfryd */

#ifndef _LDR_H_
#define _LDR_H_

#include "cortex_m0.h"
void cm0_LDR_immediate_T1(struct cm0 *proc);
void cm0_LDR_immediate_T2(struct cm0 *proc);
void cm0_LDR_literal(struct cm0 *proc);
void cm0_LDR_register(struct cm0 *proc);

#endif
