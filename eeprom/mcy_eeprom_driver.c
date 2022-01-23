#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <asm-generic/uaccess.h>


typedef struct eeprom_data_s {
	int reg;
	int len;
	unsigned char data[16];
} eeprom_data_t;

typedef struct eeprom_driver_s {
	dev_t devnum;
	struct cdev *cdev;
	struct class *class;
	struct device *dev;
	struct i2c_client *client;
} eeprom_driver_t;

eeprom_data_t eeprom_data;
eeprom_driver_t eeprom_driver;

int i2c_send_bytes(const struct i2c_client *client, unsigned short reg, const char *buf, int count)
{
	unsigned char *data = NULL;
	struct i2c_msg msg;
	int len = 0;
	int ret = -1;

	data = kmalloc(count + 2, GFP_KERNEL);
	if (!data) {
		printk("%s, kmalloc failed!\n", __func__);
		return -1;
	}
	data[0] = (char)((reg & 0x0000ff00) >> 8);
	data[1] = (char)(reg & 0x000000ff);
	memcpy(&data[2], buf, count);
	len = count + 2;

	memset(&msg, 0, sizeof(msg));
	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = count;
	msg.buf = data;

	ret = i2c_transfer(client->adapter, &msg, 1);

	if (data) {
		kfree(data);
		data = NULL;
	}

	return ret;
}

int i2c_recv_bytes(const struct i2c_client *client, unsigned short reg, unsigned char *buf, int count)
{
	struct i2c_msg msg[2] = { 0 };
	unsigned char txbuf[2] = { 0 };
	int ret = -1;

	txbuf[0] = (char)((reg & 0x0000ff00) >> 8);
	txbuf[1] = (char)(reg & 0x000000ff);

	memset(&msg, 0, sizeof(msg));
	msg[0].addr = client->addr;
	msg[0].flags = 0;
	msg[0].len = 2;
	msg[0].buf = txbuf;

	msg[1].addr = client->addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = count;
	msg[1].buf = buf;

	ret = i2c_transfer(client->adapter, msg, 2);

	return ret;
}

static int eeprom_driver_open(struct inode *inode, struct file *filp)
{
	printk("%s\n", __func__);

	return 0;
}

static int eeprom_driver_release(struct inode *inode, struct file *filp)
{
	printk("%s\n", __func__);

	return 0;
}

#if 1

static ssize_t eeprom_driver_read(struct file *filp, char __user *buf, size_t count, loff_t *loff)
{
	eeprom_data_t eeprom_data;
	int i = 0;
	int ret = -1;

	printk("%s\n", __func__);

	memset(&eeprom_data, 0, sizeof(eeprom_data_t));
	ret = copy_from_user(&eeprom_data, buf, count);
	if (ret < 0) {
		printk("%s, copy_from_user failed, ret: %d\n", __func__, ret);
		return -1;
	}

	ret = i2c_recv_bytes(eeprom_driver.client, eeprom_data.reg, eeprom_data.data, eeprom_data.len);

	for (i = 0; i < eeprom_data.len; i++) {
		printk("%s, i2c_master_recv, reg[%d]: 0x%x, data: 0x%2x\n", __func__, i, eeprom_data.reg + i, eeprom_data.data[i]);
	}

	ret = copy_to_user(buf, &eeprom_data, count);
	if (ret < 0) {
		printk("%s, copy_to_user failed, ret: %d\n", __func__, ret);
		return -1;
	}

	return 0;
}
	
static ssize_t eeprom_driver_write(struct file *filp, const char __user *buf, size_t count, loff_t *loff)
{
	eeprom_data_t eeprom_data;
	int i = 0;
	int ret = -1;

	printk("%s\n", __func__);

	memset(&eeprom_data, 0, sizeof(eeprom_data_t));
	ret = copy_from_user(&eeprom_data, buf, count);
	if (ret < 0) {
		printk("%s, copy_from_user failed, ret: %d\n", __func__, ret);
		return -1;
	}

	for (i = 0; i < eeprom_data.len; i++) {
		printk("%s, data[%d]: 0x%2x\n", __func__, i, eeprom_data.data[i]);
	}

	ret = i2c_send_bytes(eeprom_driver.client, eeprom_data.reg, eeprom_data.data, eeprom_data.len);

	return 0;
}

#else
static ssize_t eeprom_driver_read(struct file *filp, char __user *buf, size_t count, loff_t *loff)
{
	eeprom_data_t eeprom_data;
	unsigned char reg[2] = {0};
	int i = 0;
	int ret = -1;

	printk("%s\n", __func__);

	memset(&eeprom_data, 0, sizeof(eeprom_data_t));
	ret = copy_from_user(&eeprom_data, buf, count);
	if (ret < 0) {
		printk("%s, copy_from_user failed, ret: %d\n", __func__, ret);
		return -1;
	}

	reg[0] = (char)((eeprom_data.reg & 0x0000ff00) >> 8);
	reg[1] = (char)(eeprom_data.reg & 0x000000ff);

	ret = i2c_master_send(eeprom_driver.client, reg, 2);
	ret = i2c_master_recv(eeprom_driver.client, eeprom_data.data, eeprom_data.len);

	for (i = 0; i < eeprom_data.len; i++) {
		printk("%s, i2c_master_recv, reg[%d]: 0x%x, data: 0x%2x\n", __func__, i, eeprom_data.reg + i, eeprom_data.data[i]);
	}

	ret = copy_to_user(buf, &eeprom_data, count);
	if (ret < 0) {
		printk("%s, copy_to_user failed, ret: %d\n", __func__, ret);
		return -1;
	}

	return 0;
}
	
static ssize_t eeprom_driver_write(struct file *filp, const char __user *buf, size_t count, loff_t *loff)
{
	eeprom_data_t eeprom_data;
	unsigned char data[18] = {0};
	int len = 0;
	int i = 0;
	int ret = -1;

	printk("%s\n", __func__);

	memset(&eeprom_data, 0, sizeof(eeprom_data_t));
	ret = copy_from_user(&eeprom_data, buf, count);
	if (ret < 0) {
		printk("%s, copy_from_user failed, ret: %d\n", __func__, ret);
		return -1;
	}

	memcpy(&data[2], eeprom_data.data, eeprom_data.len);
	data[0] = (char)((eeprom_data.reg & 0x0000ff00) >> 8);
	data[1] = (char)(eeprom_data.reg & 0x000000ff);
	len = eeprom_data.len + 2;

	for (i = 0; i < len; i++) {
		printk("%s, data[%d]: 0x%2x\n", __func__, i, data[i]);
	}

	ret = i2c_master_send(eeprom_driver.client, data, len);

	return 0;
}
#endif

struct file_operations eeprom_driver_fops = {
	.open = eeprom_driver_open,
	.release = eeprom_driver_release,
	.read = eeprom_driver_read,
	.write = eeprom_driver_write,
};

int eeprom_driver_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int major = 0;
	int ret = -1;

	printk("%s\n", __func__);

	eeprom_driver.client = client;

	ret = alloc_chrdev_region(&eeprom_driver.devnum, 0, 1, "eeprom_driver_chrdev");
	if (ret < 0) {
		printk("%s, alloc_chrdev_region failed!\n", __func__);
		goto chrdev_err;
	}
	major = MAJOR(eeprom_driver.devnum);
	printk("%s, alloc_chrdev_region, devnum: 0x%x, major: %d\n", __func__, eeprom_driver.devnum, major);

	eeprom_driver.cdev = cdev_alloc();
	if (!eeprom_driver.cdev) {
		printk("%s, cdev_alloc failed!\n", __func__);
		goto cdev_alloc_err;
	}
	cdev_init(eeprom_driver.cdev, &eeprom_driver_fops);
	ret = cdev_add(eeprom_driver.cdev, eeprom_driver.devnum, 1);
	if (ret < 0) {
		printk("%s, cdev_add failed!\n", __func__);
		goto cdev_add_err;
	}
	
	// /sys/class
	eeprom_driver.class = class_create(THIS_MODULE, "eeprom_driver_class");
	if (IS_ERR(eeprom_driver.class)) {
		printk("%s, class_create failed!\n", __func__);
		ret = PTR_ERR(eeprom_driver.class);
		goto class_create_err;
	}
	// /dev
	eeprom_driver.dev = device_create(eeprom_driver.class, NULL, eeprom_driver.devnum, NULL, "eeprom_driver");
	if (IS_ERR(eeprom_driver.dev)) {
		printk("%s, device_create failed!\n", __func__);
		ret = PTR_ERR(eeprom_driver.dev);
		goto device_create_err;
	}

	printk("%s, OK!\n", __func__);

	return 0;

device_create_err:
	class_destroy(eeprom_driver.class);
class_create_err:
	cdev_del(eeprom_driver.cdev);
cdev_add_err:
	kfree(eeprom_driver.cdev);
cdev_alloc_err:
	unregister_chrdev_region(eeprom_driver.devnum, 1);
chrdev_err:
	return -EINVAL;
}

int eeprom_driver_remove(struct i2c_client *client)
{
	printk("%s\n", __func__);

	device_destroy(eeprom_driver.class, eeprom_driver.devnum);
	class_destroy(eeprom_driver.class);

	cdev_del(eeprom_driver.cdev);

	kfree(eeprom_driver.cdev);
	unregister_chrdev_region(eeprom_driver.devnum, 1);

	return 0;
}

struct i2c_device_id eeprom_driver_id[] = {
	{"mcy_eeprom", 0},
	{},
};

struct of_device_id eeprom_driver_table[] = {
	{
		//.name = "mcy_eeprom",
		.compatible = "mcy,mcy_eeprom",
	},
	{},
};

struct i2c_driver eeprom_i2c_driver= {
	.driver = {
		.owner = THIS_MODULE,
		.name = "mcy_eeprom",
		//.of_match_table = eeprom_driver_table,
	},
	.probe = eeprom_driver_probe,
	.remove = eeprom_driver_remove,
	.id_table = eeprom_driver_id,
};

static int eeprom_driver_init(void)
{
	printk("%s\n", __func__);

	i2c_add_driver(&eeprom_i2c_driver);

	return 0;
}

static void eeprom_driver_exit(void)
{
	printk("%s\n", __func__);

	i2c_del_driver(&eeprom_i2c_driver);
}

module_init(eeprom_driver_init);
module_exit(eeprom_driver_exit);
MODULE_DESCRIPTION("mcy eeprom i2c driver");
MODULE_AUTHOR("Mshrimp");
MODULE_LICENSE("GPL");

