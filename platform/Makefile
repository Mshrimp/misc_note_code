
#KERNEL_SRC := /opt/hi3518/Hi3518E_SDK_V1.0.3.0/osdrv/opensource/kernel/linux-3.4.y
KERNEL_SRC := /opt/hi3518/Hi3518_SDK_V1.0.a.0/osdrv/kernel/linux-3.0.y
PWD := $(shell pwd)
CROSS_COMPILE := arm-hisiv100nptl-linux-

#DRIVER_NAME := test_plat_driver

#obj-m := ${DRIVER_NAME}.o
#${DRIVER_NAME}-objs := device.o driver.o

obj-m := device.o driver.o


all:
	make -C ${KERNEL_SRC} M=${PWD} modules

clean:
	make -C ${KERNEL_SRC} M=${PWD} clean


