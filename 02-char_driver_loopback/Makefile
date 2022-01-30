obj-m += char.o

all build:
		make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
		gcc -o user user.c

load:
		@sudo insmod $(PWD)/char.ko
		@sudo chown $(USER):$(USER) /dev/chardev-*

unload:
		@sudo rmmod char

reload:
		make build
		-make unload
		make load

clean:
		@make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
		-rm -rf user