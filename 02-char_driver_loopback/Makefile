obj-m += char.o

all build:
		make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
		gcc -o user user.c

load:
		@sudo insmod $(shell pwd)/char.ko
		@sudo chown $(USER):$(USER) /dev/chardev-*

unload:
		@sudo rmmod char

reload:
		make build
		-make unload
		make load

clean:
		@make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
		-rm -rf user