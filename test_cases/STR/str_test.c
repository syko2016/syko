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

void error(char *fun_name, struct cm0 *proc)
{
	perror(fun_name);
	cm0_deinit(proc);
	exit(1);
}

void print_regs(uint32_t *buf_regs, uint32_t size, char *msg)
{
	printf("%s", msg);
	for (uint32_t i = 0; i < size; i++)
		printf("reg[%u] = 0x%08x\n", i, buf_regs[i]);
	printf("\n");
}

void check_mem(uint32_t *buf)
{
	assert(buf[1] == 0x11111111);
	assert(buf[2] == 0x2F2F2F2F);
	assert(buf[3] == 0xF2F2F2F2);

	printf("str r0, [r1, #+16] ok\n");
	printf("str r2, [SP, #+32] ok\n");
	printf("str r3, [r4, r5] ok\n\n");
}

int main(void)
{ 
	int rt;
	uint8_t buf_memc[4096];
	uint32_t buf_regs[19], buf_regs_org[19];

	struct cm0 proc;

	printf("\nProgram laduje rejestry z pliku input_regs_x.bin oraz kod\n"
		"programu z pliku file_code_x.bin, wykonuje program, na koniec\n"
		"porownuje okreslone obszary pamieci.\n\n");

	printf("str r0, [r1, #+16]\n");
	printf("str r2, [SP, #+32]\n");
	printf("str r3, [r4, r5]\n\n");

	rt = cm0_init(&proc);
	if (rt < 0)
		error("cm0_init", &proc);

	rt = load(buf_memc, sizeof(buf_memc), "file_code_1.bin");
	if (rt < 0)
		error("load", &proc);


	rt = load((uint8_t *)buf_regs, sizeof(buf_regs), 
		  "input_regs_1.bin");
	if (rt < 0)
		error("load", &proc);

	rt = load((uint8_t *)buf_regs_org, sizeof(buf_regs_org), 
		  "result_regs_1.bin");
	if (rt < 0)
		error("load", &proc);

	rt = cm0_set_all_regs(&proc, buf_regs, sizeof(buf_regs));  
	if (rt < 0)
		error("cm0_set_all_regs", &proc);
	
	rt = cm0_set_memc(&proc, buf_memc, sizeof(buf_memc));
	if (rt < 0)
		error("cm0_set_memc", &proc);

	cm0_run(&proc);

	rt = cm0_get_all_regs(&proc, buf_regs, sizeof(buf_regs));
	if (rt < 0)
		error("cm0_get_all_regs", &proc);

	check_mem((uint32_t *)cm0_mem_get(&proc, cm0_get_reg(&proc, SP)));

	cm0_deinit(&proc);

	return 0;
} 
