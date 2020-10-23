# include ../mk.conf

KENAL_DIR 	    = ~/IMX6ULL/100ask_imx6ull-sdk/Buildroot_2019.02/output/build/linux-origin_master

ccflags-y +=   ##私有内核头文件在这里添加

CUR_PATH	= $(shell pwd)

src_all		= $(wildcard *.c)
obj_all		= $(patsubst %.c, %.o, $(src_all))

# pwm_drv-y := pwm_handler_ebf6ull.o 
pwm_drv-y += pwm_drv.o
# pwm_drv-y += uldivmod.o
obj-m   += pwm_drv_ebf6ull.o

all:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules 
	@ cp *.ko ../
clean:
	@ make -C $(KENAL_DIR) M=$(CUR_PATH) modules clean 
	@ rm -rf ../*.ko

test:
	@ echo $(KENAL_DIR)
