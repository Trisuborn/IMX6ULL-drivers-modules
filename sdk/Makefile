
SRC		= $(wildcard *.c)
OBJ		= $(patsubst %.c, %.o, $(SRC))

# TEMP 	 =  $(shell find ./ -maxdepth 4 -type d)
# TEMP 	 +=  	$(shell find $(KENAL_DIR)/include -maxdepth 4 -type d)
INC_DIR	 =  $(addprefix -I,$(TEMP))

CFLAGS =  -pipe -O0
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc

all:$(OBJ)
	@ echo "done."
%.o:%.c
	$(CC) $(CFLAGS) $(INC_DIR) -I./ -c -o $@ $^
clean:
	rm -rf *.o *.bin *.elf
