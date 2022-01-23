#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct eeprom_s {
	int reg;
	int len;
	unsigned char data[16];
} eeprom_data_t;

int eeprom_read(int reg, int len)
{
	eeprom_data_t eeprom_data;
	int fd = -1;
	int i = 0;
	int ret = -1;

	if (len > 16) {
		printf("%s, len: %d over range!\n", __func__, len);
		return -1;
	}

	memset(&eeprom_data, 0, sizeof(eeprom_data_t));
	eeprom_data.reg = reg;
	eeprom_data.len = len;
	
	ret = read(fd, &eeprom_data, sizeof(eeprom_data_t));
	if (ret < 0) {
		printf("%s, read failed, ret: %d\n", __func__, ret);
		return -2;
	}

	for (i = 0; i < len; i++) {
		printf("%s, reg[%d]: %d, data: 0x%2x\n", __func__, i, reg + i, eeprom_data.data[i]);
	}

	return 0;
}

int eeprom_write(int reg, int len, unsigned char *data)
{
	eeprom_data_t eeprom_data;
	int fd = -1;
	int i = 0;
	int ret = -1;

	if (len > 16) {
		printf("%s, len: %d over range!\n", __func__, len);
		return -1;
	}

	memset(&eeprom_data, 0, sizeof(eeprom_data_t));
	eeprom_data.reg = reg;
	eeprom_data.len = len;
	
	ret = write(fd, &eeprom_data, sizeof(eeprom_data_t));
	if (ret < 0) {
		printf("%s, read failed, ret: %d\n", __func__, ret);
		return -2;
	}

	for (i = 0; i < len; i++) {
		printf("%s, reg[%d]: %d, data: 0x%2x\n", __func__, i, reg + i, eeprom_data.data[i]);
	}

	return 0;
}


int main(int argc, char *argv[])
{
	char buf[8] = {0};
	int count = 0;
	int reg = 0;
	int len = 0;
	int fd = -1;
	int ret = -1;
	
	fd = open("/dev/eeprom_driver", O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		printf("open failed, fd: %d\n", fd);
		return -1;
	}

	ret = eeprom_write(reg, len, buf);
	if (ret < 0) {
		printf("eeprom_write failed, ret: %d\n", ret);
		close(fd);
		return -1;
	}

	ret = eeprom_read(reg, len);
	if (ret < 0) {
		printf("eeprom_read failed, ret: %d\n", ret);
		close(fd);
		return -1;
	}

	close(fd);

	return 0;
}
