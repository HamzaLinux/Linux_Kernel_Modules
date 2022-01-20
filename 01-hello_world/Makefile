obj-m += hello.o

SUDO = sudo

all build:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

load:
		@make build
		@$(SUDO) insmod $(PWD)/hello.ko
		@dmesg | grep "Hello"
		
unload:
		@$(SUDO) rmmod hello

clean:
		@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean