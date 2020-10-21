
include ../mk.conf

CC = $(CROSS_COMPILE)gcc

TEMP 	 	=  	$(shell find ../ -maxdepth 4 -type d)
TEMP 	   +=  	$(shell find $(KENAL_DIR)/include -maxdepth 4 -type d)
INC_DIR	 	=  	$(addprefix -I, $(TEMP))

CFLAGS =  -pipe -O0 -std=c99

src_all		= $(wildcard *.c)
obj_all		= $(patsubst %.c, %.o, $(src_all))

all:led_app
	@ cp led_app ../
led_app:$(src_all)
	@ $(CC) $(CFLAGS) -o led_app $(src_all)
	
clean:
	@ rm -rf *.o led_app ../led_app
