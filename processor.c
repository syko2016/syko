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

int save(const uint8_t *buf, size_t buf_size, char *path)
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

int main(void)
{ 
	int rt;
	uint8_t buf_memd[CM0_MEMD_SIZE]; /* temp buf */
	uint8_t buf_memc[CM0_MEMC_SIZE]; /* temp buf */
	struct cm0 proc;

	rt = load(buf_memd, CM0_MEMD_SIZE, "file_data_in.bin");
	if (rt < 0)
		error("load");
	
	rt = load(buf_memc, CM0_MEMC_SIZE, "file_code.bin");
	if (rt < 0)
		error("load");
	
	rt = cm0_set_memc(&proc, buf_memc, CM0_MEMC_SIZE, 0);
	if (rt < 0)
		error("cm0_set_memc");

	rt = cm0_set_memd(&proc, buf_memd, CM0_MEMD_SIZE, 0);
	if (rt < 0)
		error("cm0_set_memd");

	cm0_run(&proc);

	rt = save(cm0_get_memd(&proc), CM0_MEMD_SIZE, "file_data_out.bin");
	if (rt < 0)
		error("save");
	
	return 0;
} 
