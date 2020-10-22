include ../mk.conf

SRC		= $(wildcard *.c)
OBJ		= $(patsubst %.c, %.o, $(SRC))

TEMP 	 =  $(shell find ../ -maxdepth 4 -type d)
INC_DIR	 =  $(addprefix -I,$(TEMP))

OUT_DIR = ../out/

all:$(OUT_DIR)$(OBJ)
%.o:%.c
	$(CC) $(CFLAGS) $(INC_DIR) -c -o $(OUT_DIR)$@ $^

clean:
	rm -rf *.o *.bin *.elf
