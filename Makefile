MOD_NAME := veikk
BUILD_DIR := /lib/modules/$(shell uname -r)/build

obj-m := $(MOD_NAME).o
$(MOD_NAME)-objs := veikkdrv.o veikkparams.o llist.o

all:
	make -C $(BUILD_DIR) M=$(CURDIR) modules
	chmod +x ./config.sh

clean:
	make -C $(BUILD_DIR) M=$(CURDIR) clean

install:
	make -C $(BUILD_DIR) M=$(CURDIR) modules_install
	depmod
	modprobe $(MOD_NAME)

uninstall:
	modprobe -r $(MOD_NAME)
	rm $(shell modinfo -n $(MOD_NAME))
	depmod
