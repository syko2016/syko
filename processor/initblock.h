#ifndef _INITBLOCK_H_
#define _INITBLOCK_H_
#include <stdint.h>
/* 
 * This functions return 0, because ARMv6-M (which Cortex-M0 uses) does not 
 * support IT instruction. It was written, because instruction encondings have
 * some condition checks. 
 */ 
uint8_t InITBlock(void);
uint8_t LastInITBlock(void);
#endif
