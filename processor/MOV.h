/* Author: Konrad Gotfryd */

#ifndef _MOV_H_
#define _MOV_H_

#include "cortex_m0.h"

void cm0_MOV_immediate(struct cm0 *proc);
void cm0_MOV_register_T1(struct cm0 *proc);
void cm0_MOV_register_T2(struct cm0 *proc);

#endif
