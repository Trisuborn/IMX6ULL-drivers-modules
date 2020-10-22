
include ../mk.conf

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld

TEMP 	 	=  	$(shell find ../ -maxdepth 4 -type d)
TEMP 	   +=  	$(shell find $(KENAL_DIR)/include -maxdepth 4 -type d)
INC_DIR	 	=  	$(addprefix -I, $(TEMP))

CFLAGS =  -pipe -O0 -std=c99 -fno-stack-protector

src_all		+= $(wildcard *.c)
src_all		+= $(wildcard ./sdk/*.c)
obj_all		= $(patsubst %.c, %.o, $(src_all))

all:led_app
	@ cp led_app ../
led_app:$(src_all)
	@ make -C ./sdk
	@ $(CC) $(CFLAGS) -o led_app led_app.c ./sdk/*.o
	
clean:
	@ rm -rf *.o led_app ../led_app
	@ make -C ./sdk clean
