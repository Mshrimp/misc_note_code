#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define MEM_ADDR		(0x40000000)
#define MEM_SIZE		(0x1000)

unsigned char *mem_base;

#define MEM_READ16(offset)	\
	do {	\
		*((volatile unsigned short *)((mem_base) + ((unsigned long)offset)));	\
		msync((mem_base) + ((unsigned long)(offset)), 2, MS_SYNC);	\
	} while(0)

#define MEM_WRITE16(offset, val)	\
	do {	\
		(*((volatile unsigned short *)((mem_base) + ((unsigned long)(offset)))) = (val))	\
	} while(0)

int main(void)
{
	int mem_fd = -1;
	void *map = NULL;
	int ret = -1;

	mem_fd = open("/dev/mem", O_RDWR);
	if (mem_fd < 0) {
		printf("%s, open /dev/mem failed!\n", __func__);
		return -1;
	}

	map = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, MEM_ADDR);
	if (!map) {
		printf("%s, mmap failed!\n", __func__);
		goto mmap_err;
	}

	mem_base = (unsigned char *)map;
	printf("%s, mem base addr: %p\n", __func__, mem_base);

	ret = munmap(map, MEM_SIZE);
	if (ret < 0) {
		printf("%s, munmap failed, ret: %d\n", __func__, ret);
	}

mmap_err:
	close(mem_fd);

	return ret;
}
