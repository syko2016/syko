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

void check_regs(uint32_t *buf_regs, uint32_t *buf_regs_org, uint32_t size)
{
	for (uint32_t i = 0; i < size; i++) {
		assert(buf_regs[i] == buf_regs_org[i]);
		printf("reg %u ok.\n", i);
	}
}

int main(void)
{ 
	int rt;
	uint8_t buf_memc[4096];
	uint32_t buf_regs[19], buf_regs_org[19];

	struct cm0 proc;

	printf("\nProgram laduje rejestry z pliku input_regs.bin oraz kod\n"
		"programu z pliku file_code.bin, wykonuje program, na koniec\n"
		"porownuje rejestry procesora z docelowymi rejestrami z pliku\n"
		"result_regs.bin.\n\n");

	rt = cm0_init(&proc);
	if (rt < 0)
		error("cm0_init", &proc);

	printf("Sprawdzanie instrukcji bmi (wywola kodowanie T1).\n\n");

	rt = load(buf_memc, sizeof(buf_memc), "file_code_1.bin");
	if (rt < 0)
		error("load", &proc);

	rt = load((uint8_t *)buf_regs, sizeof(buf_regs), 
		  "input_regs.bin");
	if (rt < 0)
		error("load", &proc);

	rt = load((uint8_t *)buf_regs_org, sizeof(buf_regs_org), 
		  "result_regs.bin");
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

	check_regs(buf_regs, buf_regs_org, sizeof(buf_regs)/sizeof(uint32_t));

	printf("Sprawdzanie instrukcji b (wywola kodowanie T2).\n\n");

	rt = load(buf_memc, sizeof(buf_memc), "file_code_2.bin");
	if (rt < 0)
		error("load", &proc);

	rt = load((uint8_t *)buf_regs, sizeof(buf_regs), 
		  "input_regs.bin");
	if (rt < 0)
		error("load", &proc);

	rt = load((uint8_t *)buf_regs_org, sizeof(buf_regs_org), 
		  "result_regs.bin");
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

	check_regs(buf_regs, buf_regs_org, sizeof(buf_regs)/sizeof(uint32_t));


	cm0_deinit(&proc);

	return 0;
} 
