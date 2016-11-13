#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MEMC_SIZE 1024
#define MEMD_SIZE 1024


uint8_t memc[MEMC_SIZE];
uint8_t memd[MEMD_SIZE];

struct registers {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R4;
	uint32_t R5;
	uint32_t R6;
	uint32_t R7;
	uint32_t R8;
	uint32_t R9;
	uint32_t R10;
	uint32_t R11;
	uint32_t R12;
	uint32_t SP;
	uint32_t LR;
	uint32_t PC;

	
	uint32_t PSR;
	uint32_t PRIMASK;
	uint32_t CONTROL;
};

struct registers *registers;

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

int save(uint8_t *buf, size_t buf_size, char *path)
{
	int fd;
	
	fd = open(path, O_WRONLY | O_CREAT, 0666); 	
	if (fd < 0)
		return fd;

	write(fd, buf, buf_size);
	close(fd);
	
	return 0;
}

void error(char *fun_name)
{
	perror(fun_name);
	exit(1);
}

int prepare_bufs(uint8_t *memc, size_t memc_size, uint8_t *memd, 
		 size_t memd_size)
{
	int rt;

	rt = load(memc, MEMC_SIZE, "file_code.bin");
	if (rt < 0)
		return rt;

	rt = load(memd, MEMD_SIZE, "file_data_in.bin");

	return rt;
}

int main(void)
{ 
	int rt;
	
	rt = prepare_bufs(memc, MEMC_SIZE, memd, MEMD_SIZE);
	if (rt < 0)
		error("prepare_bufs");
	
	rt = save(memd, MEMD_SIZE, "file_data_out.bin");
	if (rt < 0)
		error("save");

	return 0;
} 
