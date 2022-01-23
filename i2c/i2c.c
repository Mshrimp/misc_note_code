#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <errno.h>

#define I2C_FILE_NAME		"/dev/i2c-0"

int i2c_fd = -1;

int i2c_open(void)
{
	i2c_fd = open(I2C_FILE_NAME, O_RDWR);
	if (i2c_fd < 0) {
		perror("i2c_fd open");
		return -1;
	}

	return 0;
}

int i2c_write(int fd, unsigned char dev_addr, unsigned short reg, unsigned char *data, int len)
{
	struct i2c_rdwr_ioctl_data wr_data;
	struct i2c_msg message;
	unsigned char *buf = NULL;
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	buf = malloc(len + 1);
	if (!buf) {
		printf("%s, buf is null!\n", __func__);
		return -2;
	}

	memset(buf, 0, len + 1);
	buf[0] = reg & 0xff;
	memcpy(&buf[1], data, len);

	message.addr = dev_addr;
	message.flags = 0;
	message.len = len + 1;
	message.buf = buf;

	wr_data.msgs = &message;
	wr_data.nmsgs = 1;

	ret = ioctl(fd, I2C_RDWR, &wr_data);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	usleep(10000);

OUT:
	if (buf) {
		free(buf);
		buf = NULL;
	}

	return ret;
}

int i2c_read(int fd, unsigned char addr, unsigned short reg, unsigned char *data, int len)
{
	struct i2c_rdwr_ioctl_data rd_data;
	struct i2c_msg message[2];
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	message[0].buf = malloc(1);
	if (!message[0].buf) {
		printf("%s, malloc failed!\n", __func__);
		return -2;
	}

	message[0].addr = addr;
	message[0].flags = 0;
	message[0].len = 1;
	message[0].buf[0] = reg & 0xff;

	message[1].addr = addr;
	message[1].flags = I2C_M_RD;
	message[1].len = len;
	message[1].buf = data;

	rd_data.msgs = message;
	rd_data.nmsgs = 2;

	ret = ioctl(fd, I2C_RDWR, &rd_data);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

OUT:
	if (message[0].buf) {
		free(message[0].buf);
		message[0].buf = NULL;
	}

	return ret;
}


int i2c_write_block(int fd, unsigned char dev_addr, unsigned short reg, unsigned char *data, int len)
{
	struct i2c_rdwr_ioctl_data wr_data;
	struct i2c_msg message;
	unsigned char *buf = NULL;
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	buf = malloc(len + 2);
	if (!buf) {
		printf("%s, malloc failed!\n", __func__);
		return -2;
	}

	memset(buf, 0, len + 2);
	buf[0] = reg & 0xff;
	buf[1] = len & 0xff;
	memcpy(&buf[2], data, len);

	message.addr = dev_addr;
	message.flags = 0;
	message.len = len + 2;
	message.buf = buf;

	wr_data.msgs = &message;
	wr_data.nmsgs = 1;

	ret = ioctl(fd, I2C_RDWR, &wr_data);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	usleep(10000);

OUT:
	if (buf) {
		free(buf);
		buf = NULL;
	}

	return ret;
}

int i2c_read_block(int fd, unsigned char addr, unsigned short reg, unsigned char *data, int len)
{
	struct i2c_rdwr_ioctl_data rd_data;
	struct i2c_msg message[2];
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	message[0].buf = malloc(1);
	if (!message[0].buf) {
		printf("%s, malloc failed!\n", __func__);
		return -2;
	}

	message[0].addr = addr;
	message[0].flags = 0;
	message[0].len = 1;
	message[0].buf[0] = reg & 0xff;

	message[1].addr = addr;
	message[1].flags = I2C_M_RD;
	message[1].len = len;
	message[1].buf = malloc(len + 1);
	if (!message[1].buf) {}

	rd_data.msgs = message;
	rd_data.nmsgs = 2;

	ret = ioctl(fd, I2C_RDWR, &rd_data);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	if (len != message[1].buf[0]) {
		printf("%s, len: %d buf[0]: %d not equal!\n", __func__, len, message[1].buf[0]);
		return -3;
	}

	memcpy(data, &message[1].buf[1], len);

OUT:
	if (message[0].buf) {
		free(message[0].buf);
		message[0].buf = NULL;
	}

	if (message[1].buf) {
		free(message[1].buf);
		message[1].buf = NULL;
	}

	return ret;
}

int i2c_read_byte(unsigned char addr, unsigned char reg, unsigned char *val)
{
	int ret = 0;

	if (!val) {
		printf("%s, val is null!\n", __func__);
		return -1;
	}

	ret = i2c_open();
	if (ret < 0) {
		printf("%s, i2c_open failed, ret: %d\n", __func__, ret);
		return ret;
	}

	ret = i2c_read(i2c_fd, addr, reg, val, 1);
	if (ret < 0) {
		printf("%s, i2c_read failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

OUT:
	close(i2c_fd);

	return ret;
}

int i2c_write_byte(unsigned char addr, unsigned char reg, unsigned char *val)
{
	int ret = 0;

	if (!val) {
		printf("%s, val is null!\n", __func__);
		return -1;
	}

	ret = i2c_open();
	if (ret < 0) {
		printf("%s, i2c_open failed, ret: %d\n", __func__, ret);
		return ret;
	}

	ret = i2c_write(i2c_fd, addr, reg, val, 1);
	if (ret < 0) {
		printf("%s, i2c_write failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

OUT:
	close(i2c_fd);

	return ret;
}



