#include <linux/module.h>
#include <linux/i2c.h>

static struct i2c_client *client;

static struct i2c_board_info eeprom_info = {
	I2C_BOARD_INFO("eeprom_device", 0x51),
};

static int eeprom_device_init(void)
{
	struct i2c_adapter *adap = NULL;

	printk("%s: %d\n", __func__, __LINE__);

	adap = i2c_get_adapter(2);
	if (!adap) {
		printk("%s, i2c_get_adapter failed!\n", __func__);
		return -1;
	}

	client = i2c_new_device(adap, &eeprom_info);
	if (!client) {
		printk("%s, i2c_new_device failed!\n", __func__);
		return -2;
	}

	i2c_put_adapter(adap);

	return 0;
}

static void eeprom_device_exit(void)
{
	printk("%s: %d\n", __func__, __LINE__);

	i2c_unregister_device(client);
}

module_init(eeprom_device_init);
module_exit(eeprom_device_exit);
MODULE_DESCRIPTION("mcy eeprom i2c device");
MODULE_AUTHOR("Mshrimp");
MODULE_LICENSE("GPL");

