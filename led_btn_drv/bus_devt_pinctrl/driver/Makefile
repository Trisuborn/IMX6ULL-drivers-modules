# include ../mk.conf

KENAL_DIR 	    = ~/IMX6ULL/100ask_imx6ull-sdk/Buildroot_2019.02/output/build/linux-origin_master

ccflags-y +=   ##私有内核头文件在这里添加

CUR_PATH	= $(shell pwd)

src_all		= $(wildcard *.c)
obj_all		= $(patsubst %.c, %.o, $(src_all))

# led_core-y := led_handler_ebf6ull.o led_drv.o led_drv_ctl.o
led_btn_drv_ctl-y := led_btn_handler_ebf6ull.o led_btn_drv.o
obj-m   += led_btn_drv_ctl.o
obj-m   += led_btn_core.o
# obj-m   += led_dev.o

all:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules 
	@ cp *.ko ../
clean:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules clean 
	@ rm -rf ../*.ko

test:
	@ echo $(KENAL_DIR)
