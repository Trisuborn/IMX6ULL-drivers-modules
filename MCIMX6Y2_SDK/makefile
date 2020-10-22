include mk.conf

OUT_DIR	 =  ./out/
LIB_DIR  =  ./lib/

TEMP 	 	=  	$(shell find . -maxdepth 4 -type d)
TEMP 	   +=  	$(shell find ../ -maxdepth 4 -type d)
TEMP 	   +=  	$(shell find $(KENAL_DIR)/include -maxdepth 4 -type d)
INC_DIR	 	=  	$(addprefix -I, $(TEMP))

C_FILE 		=   $(foreach dir,$(TEMP),$(wildcard $(dir)/*.c))
S_FILE 		+= 	$(foreach dir,$(TEMP),$(wildcard $(dir)/*.S))
ALL_FILE  	= 	$(C_FILE) $(S_FILE)

ALL_C_SRC  	= 	$(notdir $(C_FILE))
ALL_S_SRC  	= 	$(notdir $(S_FILE))
ALL_SRC  	= 	$(ALL_S_SRC) $(ALL_C_SRC)

ALL_OBJ  	:= 	$(patsubst %.S,%.o, $(ALL_S_SRC))
ALL_OBJ  	+= 	$(patsubst %.c,%.o, $(ALL_C_SRC))

RUN_ADDR = 0x80010000

all:bootload.imx

bootload.imx:bootload.bin

bootload.bin:bootload.elf
	$(OBJDUMP) -D bootload.elf > bootload.dis
	$(OBJCOPY) -O binary bootload.elf bootload.bin

bootload.elf: $(ALL_FILE)
	$(CC) $(CFLAGS) $(INC_DIR) -c -o $(OUT_DIR)main.o ./main/main.c
	$(CC) $(CFLAGS) $(INC_DIR) -c -o $(OUT_DIR)startup_MCIMX6Y2.o ./gcc/startup_MCIMX6Y2.S
	$(CC) $(CFLAGS) $(INC_DIR) -c -o $(OUT_DIR)system_MCIMX6Y2.o ./system_MCIMX6Y2.c

	$(MAKE) -C ./drivers

	$(LD) -T ./gcc/MCIMX6Y2xxx08_ram.ld  -o bootload.elf  $(OUT_DIR)*o

	# -L/usr/lib/gcc-cross/arm-linux-gnueabi/9/ -lgcc
	# $(LIB_DIR)*.a
	# ./*.a


clean:
	rm -rf *.o *.bin *.elf *.dis ./out/*

test:
	@echo $(ALL_C_SRC)
	@echo $(ALL_S_SRC)
	@echo $(ALL_OBJ)
	@echo $(ALL_SRC)
