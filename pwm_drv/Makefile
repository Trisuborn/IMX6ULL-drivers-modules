

APP_PATH	= ./app

all: 
	@ +make -C ./driver
	@ +make -C $(APP_PATH)
app:
	@ +make -C $(APP_PATH)

clean:
	@ +make -C ./driver clean
	@ +make -C $(APP_PATH) clean