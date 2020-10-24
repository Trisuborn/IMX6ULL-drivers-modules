# include ../mk.conf

KENAL_DIR 	    = ~/IMX6ULL/100ask_imx6ull-sdk/Buildroot_2019.02/output/build/linux-origin_master

CUR_PATH	= $(shell pwd)

src_all		= $(wildcard *.c)
obj_all		= $(patsubst %.c, %.o, $(src_all))

## hellodrv-y := hello_drv.o # leddrv.o board_100ask_imx6ull-qemu.o
obj-m   += hello_core.o
obj-m   += hello_dev.o
obj-m   += hello_drv.o


all:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules 
	cp *.ko ../
clean:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules clean 
	@ rm -rf ../*.ko

test:
	@ echo $(KENAL_DIR)
