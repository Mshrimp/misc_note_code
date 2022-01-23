ifneq ($(KERNELRELEASE),)
	obj-m := mcy_eeprom_driver.o mcy_eeprom_device.o
else

#generate the path
CURRENT_PATH:=$(shell pwd)

#the absolute path
LINUX_KERNEL_PATH:=/home/xiami/tool/linux-4.9.115

CROSS_COMPILE:=arm-linux-gnueabi-
CC:=$(CROSS_COMPILE)gcc

#complie object
all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
	-rm main -rf
app:
	$(CC) main.c -o main
install:
	chmod a+x *.ko
	cp *.ko /home/xiami/qemu/nfs_root/mcy -rfp
	cp main /home/xiami/qemu/nfs_root/mcy -rfp
	chmod a+x /home/xiami/qemu/nfs_root/mcy/*
endif
