#include "linux/module.h"
#include "linux/init.h"

// Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ameer Hamza");
MODULE_DESCRIPTION("Hello World Linux Kernel Module");

/*
 * @brief: Module init routine
 */
static int __init hello_init(void) {
	printk(KERN_ALERT "Hello, World!");
	return 0;
}

/*
 * @brief: Module exitroutine
 */
static void __exit hello_exit(void) {
	printk(KERN_ALERT "Bye, World!");
}

module_init(hello_init);
module_exit(hello_exit);
