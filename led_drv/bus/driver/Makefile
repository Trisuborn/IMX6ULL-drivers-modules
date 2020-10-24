# include ../mk.conf

KENAL_DIR 	    = ~/IMX6ULL/100ask_imx6ull-sdk/Buildroot_2019.02/output/build/linux-origin_master

ccflags-y +=   ##私有内核头文件在这里添加

CUR_PATH	= $(shell pwd)

src_all		= $(wildcard *.c)
obj_all		= $(patsubst %.c, %.o, $(src_all))

# led_drv_ebf6ull-y := led_handler_ebf6ull.o led_drv.o
# led_handler_ebf6ull-y += 
# led_handler_ebf6ull-y += uldivmod.o
# led_handler_ebf6ull-y += fsl_common.o
obj-m   += led_core.o
obj-m   += led_drv.o
obj-m   += led_dev.o

all:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules 
	@ cp *.ko ../
clean:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules clean 
	@ rm -rf ../*.ko

test:
	@ echo $(KENAL_DIR)
