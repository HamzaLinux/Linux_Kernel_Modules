obj-m += hello.o

SUDO = sudo

all build:
		make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

load:
		@make build
		@$(SUDO) insmod $(shell pwd)/hello.ko
		@dmesg | grep "Hello"
		
unload:
		@$(SUDO) rmmod hello

clean:
		@make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean