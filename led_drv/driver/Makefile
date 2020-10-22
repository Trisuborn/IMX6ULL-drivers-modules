# include ../mk.conf

KENAL_DIR 	    = ~/IMX6ULL/100ask_imx6ull-sdk/Buildroot_2019.02/output/build/linux-origin_master

ccflags-y += -I/usr/lib/gcc-cross/arm-linux-gnueabihf/9/ -lgcc  ##私有内核头文件在这里添加

CUR_PATH	= $(shell pwd)

src_all		= $(wildcard *.c)
obj_all		= $(patsubst %.c, %.o, $(src_all))

ebf6ull_led_drv-y := ebf6ull_led_handler.o 
ebf6ull_led_drv-y += led_drv.o
ebf6ull_led_drv-y += fsl_clock.o
ebf6ull_led_drv-y += fsl_gpio.o
ebf6ull_led_drv-y += uldivmod.o
# ebf6ull_led_drv-y += fsl_common.o
obj-m   += ebf6ull_led_drv.o

all:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules 
	@ cp *.ko ../
clean:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules clean 
	@ rm -rf ../*.ko

test:
	@ echo $(KENAL_DIR)
