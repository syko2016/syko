#ifndef _INITBLOCK_H_
#define _INITBLOCK_H_

/* 
 * This functions return 0, because ARMv6-M (which Cortex-M0 uses) does not 
 * support IT instruction. It was written, because instruction encondings have
 * some condition checks. 
 */ 
static uint8_t InITBlock(void) {
	return 0;
}

static uint8_t LastInITBlock(void) {
	return 0;
}

#endif
