#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "cortex_m0.h"

int load(uint8_t *buf, size_t buf_size, char *path)
{
	int fd;
	ssize_t cnt;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return fd;

	memset(buf, 0, buf_size);
	
	cnt = read(fd, buf, buf_size);
	close(fd);
	
	return cnt;	
}

void error(char *fun_name)
{
	perror(fun_name);
	exit(1);
}


void print_regs(uint32_t *buf_regs, uint32_t size, char *msg)
{
	printf("%s", msg);
	for (uint32_t i = 0; i < size; i++)
		printf("reg[%u] = 0x%08x\n", i, buf_regs[i]);
	printf("\n");
}

int main(void)
{ 
	int rt;
	uint8_t buf_memc[CM0_MEMC_SIZE];
	uint32_t buf_regs[19];

	struct cm0 proc;
	
	rt = load(buf_memc, CM0_MEMC_SIZE, "file_code.bin");
	if (rt < 0)
		error("load");

	rt = load((uint8_t *)buf_regs, sizeof(buf_regs), 
		  "input_regs_flags_clear.bin");
	if (rt < 0)
		error("load");

	rt = cm0_set_all_regs(&proc, buf_regs, sizeof(buf_regs));  
	if (rt < 0)
		error("cm0_set_all_regs");
	
	rt = cm0_set_memc(&proc, buf_memc, CM0_MEMC_SIZE, 0);
	if (rt < 0)
		error("cm0_set_memc");

	cm0_run(&proc);

	rt = cm0_get_all_regs(&proc, buf_regs, sizeof(buf_regs));
	if (rt < 0)
		error("cm0_get_all_regs");

	print_regs(buf_regs, 19, "Flags clear.\n");

	rt = load((uint8_t *)buf_regs, sizeof(buf_regs), 
		  "input_regs_N_flag.bin");
	if (rt < 0)
		error("load");

	rt = cm0_set_all_regs(&proc, buf_regs, sizeof(buf_regs));  
	if (rt < 0)
		error("cm0_set_all_regs");
	
	cm0_run(&proc);

	rt = cm0_get_all_regs(&proc, buf_regs, sizeof(buf_regs));
	if (rt < 0)
		error("cm0_get_all_regs");

	print_regs(buf_regs, 19, "N_flag test.\n");

	rt = load((uint8_t *)buf_regs, sizeof(buf_regs), 
		  "input_regs_Z_flag.bin");
	if (rt < 0)
		error("load");

	rt = cm0_set_all_regs(&proc, buf_regs, sizeof(buf_regs));  
	if (rt < 0)
		error("cm0_set_all_regs");
	
	cm0_run(&proc);

	rt = cm0_get_all_regs(&proc, buf_regs, sizeof(buf_regs));
	if (rt < 0)
		error("cm0_get_all_regs");

	print_regs(buf_regs, 19, "Z flag test.\n");

	return 0;
} 
