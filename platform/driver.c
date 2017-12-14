#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <asm/io.h>


static int major = 250;
static int minor = 0;
static dev_t devno;

static struct class *test_class;
static struct device *test_device;

static int test_open(struct inode *inode, struct file *filp)
{
	printk("driver: test_open\n");

	return 0;
}

static int test_release(struct inode *inode, struct file *filp)
{
	printk("driver: test_release\n");

	return 0;
}

// static long test_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
static long test_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("driver: test_ioctl\n");

	return 0;
}

static struct file_operations test_driver_ops = {
	.open = test_open,
	.release = test_release,
	.unlocked_ioctl = test_ioctl,
};

static int test_probe(struct platform_device *pdev)
{
	int ret = 0;
	printk("driver: test_probe\n");

	// ioremap();

	devno = MKDEV(major, minor);
	ret = register_chrdev(major, "test_platform_driver", &test_driver_ops);

	test_class = class_create(THIS_MODULE, "test_class");
	if (IS_ERR(test_class)) {
		unregister_chrdev(major, "test_platform_driver");
		return -EBUSY;
	}

	test_device = device_create(test_class, NULL, devno, NULL, "test_platform_driver");
	if (IS_ERR(test_device)) {
		class_destroy(test_class);
		unregister_chrdev(major, "test_platform_driver");
		return -EBUSY;
	}

	// 4412 beep init

	return 0;
}

static int test_remove(struct platform_device *pdev)
{
	// beep_unmap()
	device_destroy(test_class, devno);
	class_destroy(test_class);
	unregister_chrdev(major, "test_platform_driver");

	return 0;
}

static struct platform_driver test_driver = {
	.driver.name = "test_platform",
	.probe = test_probe,
	.remove = test_remove,
};

static int test_init(void)
{
	printk("driver: test_init\n");
	return platform_driver_register(&test_driver);
}

static void test_exit(void)
{
	printk("driver: test_exit\n");
	platform_driver_unregister(&test_driver);
	return;
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
