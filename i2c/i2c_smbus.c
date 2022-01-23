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

int i2c_smbus_write(int fd, unsigned char dev_addr, unsigned short reg, unsigned char *data, int len)
{
	struct i2c_smbus_ioctl_data wr_data;
	union i2c_smbus_data u_data;;
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	ret = ioctl(fd, I2C_SLAVE, dev_addr);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	ret = ioctl(fd, I2C_PEC, 1);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	memcpy(u_data.block, data, len);

	wr_data.read_write = I2C_SMBUS_WRITE;
	wr_data.command = reg;
	if (1 == len) {
		wr_data.size = I2C_SMBUS_BYTE_DATA;
	} else if (2 == len) {
		wr_data.size = I2C_SMBUS_WORD_DATA;
	} else {
		printf("%s, wr_data.size: %d error!\n", __func__, wr_data.size);
		goto OUT;
	}

	wr_data.data = &u_data;

	ret = ioctl(fd, I2C_SMBUS, &wr_data);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	usleep(10000);

OUT:

	return ret;
}

int i2c_smbus_read(int fd, unsigned char addr, unsigned short reg, unsigned char *data, int len)
{
	struct i2c_smbus_ioctl_data rd_data;
	union i2c_smbus_data u_data;
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	ret = ioctl(fd, I2C_SLAVE, addr);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	ret = ioctl(fd, I2C_PEC, 1);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	rd_data.read_write = I2C_SMBUS_READ;
	rd_data.command = reg;
	if (1 == len) {
		rd_data.size = I2C_SMBUS_BYTE_DATA;
	} else if (2 == len) {
		rd_data.size = I2C_SMBUS_WORD_DATA;
	} else {
		printf("%s, rd_data.size: %d error!\n", __func__, rd_data.size);
		goto OUT;
	}

	rd_data.data = &u_data;

	ret = ioctl(fd, I2C_SMBUS, &rd_data);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	memcpy(data, rd_data.data->block, len);

	usleep(10000);

OUT:

	return ret;
}


int i2c_smbus_write_block(int fd, unsigned char dev_addr, unsigned short reg, unsigned char *data, int len)
{
	struct i2c_smbus_ioctl_data wr_data;
	union i2c_smbus_data blk_data;
	int i = 0;
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	ret = ioctl(fd, I2C_SLAVE, dev_addr);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	ret = ioctl(fd, I2C_PEC, 0);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	blk_data.block[0] = len;
	for (i = 0; i < len; i++) {
		blk_data.block[i+1] = *(data + i);
	}

	wr_data.read_write = I2C_SMBUS_WRITE;
	wr_data.command = reg;
	if ((len > 0) && (len <= 5)) {
		wr_data.size = I2C_SMBUS_BLOCK_DATA;
	} else {
		printf("%s, wr_data.size: %d error!\n", __func__, wr_data.size);
		goto OUT;
	}

	wr_data.data = &blk_data;

	ret = ioctl(fd, I2C_SMBUS, &wr_data);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	usleep(10000);

OUT:

	return ret;
}

int i2c_smbus_read_block(int fd, unsigned char addr, unsigned short reg, unsigned char *data, int len)
{
	struct i2c_smbus_ioctl_data rd_data;
	union i2c_smbus_data blk_data;
	int i = 0;
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	ret = ioctl(fd, I2C_SLAVE, addr);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	ret = ioctl(fd, I2C_PEC, 1);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	rd_data.read_write = I2C_SMBUS_READ;
	rd_data.command = reg;
	if ((len > 0) && (len <= 5)) {
		rd_data.size = I2C_SMBUS_BLOCK_DATA;
	} else {
		printf("%s, rd_data.size: %d error!\n", __func__, rd_data.size);
		goto OUT;
	}

	rd_data.data = (union i2c_smbus_data *)&blk_data;

	ret = ioctl(fd, I2C_SMBUS, &rd_data);
	if (ret < 0) {
		printf("%s, ioctl failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	if (blk_data.block[0] != len) {
		printf("%s, len: %d, block[0]: %d not equal!\n", __func__, len, blk_data.block[0]);
		goto OUT;
	}

	for (i = 0; i < len; i++) {
		*(data + i) = blk_data.block[i+1];
	}

	usleep(10000);

OUT:

	return ret;
}

int i2c_smbus_read_byte(unsigned char addr, unsigned char reg, unsigned char *val)
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

	ret = i2c_smbus_read(i2c_fd, addr, reg, val, 1);
	if (ret < 0) {
		printf("%s, i2c_smbus_read failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

OUT:
	close(i2c_fd);

	return ret;
}

int i2c_smbus_write_byte(unsigned char addr, unsigned char reg, unsigned char *val)
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

	ret = i2c_smbus_write(i2c_fd, addr, reg, val, 1);
	if (ret < 0) {
		printf("%s, i2c_smbus_write failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

OUT:
	close(i2c_fd);

	return ret;
}

int i2c_smbus_read_word(unsigned char addr, unsigned char reg, unsigned short *val)
{
	unsigned char data[2] = {0};
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

	ret = i2c_smbus_read(i2c_fd, addr, reg, data, 2);
	if (ret < 0) {
		printf("%s, i2c_smbus_read failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

	*val = (unsigned short)(data[0] & 0xff);
	*val |= (unsigned short)((data[1] & 0xff) << 8);

OUT:
	close(i2c_fd);

	return ret;
}

int i2c_smbus_write_word(unsigned char addr, unsigned char reg, unsigned short val)
{
	unsigned char data[2] = {0};
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

	data[0] = (unsigned char)(val & 0xff);
	data[1] = (unsigned char)((val & 0xff00) >> 8);

	ret = i2c_smbus_write(i2c_fd, addr, reg, data, 2);
	if (ret < 0) {
		printf("%s, i2c_smbus_write failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

OUT:
	close(i2c_fd);

	return ret;
}

int i2c_smbus_read_by_block(unsigned char addr, unsigned char reg, unsigned char *data, int len)
{
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	if ((len < 0) || (len > I2C_SMBUS_BLOCK_DATA)) {
		printf("%s, len: %d error!\n", __func__, len);
		return -2;
	}

	ret = i2c_open();
	if (ret < 0) {
		printf("%s, i2c_open failed, ret: %d\n", __func__, ret);
		return ret;
	}

	ret = i2c_smbus_read_block(i2c_fd, addr, reg, data, len);
	if (ret < 0) {
		printf("%s, i2c_smbus_read_block failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

OUT:
	close(i2c_fd);

	return ret;
}

int i2c_smbus_write_by_block(unsigned char addr, unsigned char reg, unsigned char *data, int len)
{
	int ret = 0;

	if (!data) {
		printf("%s, data is null!\n", __func__);
		return -1;
	}

	if ((len < 0) || (len > I2C_SMBUS_BLOCK_DATA)) {
		printf("%s, len: %d error!\n", __func__, len);
		return -2;
	}

	ret = i2c_open();
	if (ret < 0) {
		printf("%s, i2c_open failed, ret: %d\n", __func__, ret);
		return ret;
	}

	ret = i2c_smbus_write_block(i2c_fd, addr, reg, data, len);
	if (ret < 0) {
		printf("%s, i2c_smbus_write_block failed, ret: %d\n", __func__, ret);
		goto OUT;
	}

OUT:
	close(i2c_fd);

	return ret;
}


