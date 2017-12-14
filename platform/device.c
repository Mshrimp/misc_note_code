#include <linux/module.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>

static struct resource test_release[] = {
	[0] = {
		
	},
};

static void test_release(struct device *dev)
{
	printk("device: test_release\n");
	return;
}

static struct platform_device test_device = {
	.name = "test_platform",
	.id = -1,
	.dev.release = test_release,
	.num_resource = ARRAY_SIZE(test_resource),
	.resource = test_resource,
};

static int test_init(void)
{
	printk("device: test_init\n");
	return platform_device_register(&test_device);
}

static void test_exit(void)
{
	printk("device: test_exit\n");
	platform_device_unregister(&test_device);
	return;
}


module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
