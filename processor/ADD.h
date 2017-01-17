/* Author: Sebastian Sowiński */

#ifndef _ADD_H_
#define _ADD_H_

#include "cortex_m0.h"

void cm0_ADD_immediate_T1(struct cm0 *proc);
void cm0_ADD_immediate_T2(struct cm0 *proc);
void cm0_ADD_register_T1(struct cm0 *proc);
void cm0_ADD_register_T2(struct cm0 *proc);
void cm0_ADD_SP_plus_immediate_T1(struct cm0 *proc);
void cm0_ADD_SP_plus_immediate_T2(struct cm0 *proc);
void cm0_ADD_SP_plus_register_T1(struct cm0 *proc);
void cm0_ADD_SP_plus_register_T2(struct cm0 *proc);

#endif
