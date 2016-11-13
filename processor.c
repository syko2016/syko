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

int main(void)
{ 
	int rt;
	
	rt = load(memc, MEMC_SIZE, "file_code.bin");
	if (rt < 0)
		error("load");

	rt = load(memd, MEMD_SIZE, "file_data_in.bin");
	if (rt < 0)
		error("load");
	
	rt = save(memd, MEMD_SIZE, "file_data_out.bin");
	if (rt < 0)
		error("save");

	return 0;
} 
