#ifndef _CHAR_H
#define	_CHAR_H

#include "linux/module.h"
#include "linux/init.h"
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/slab.h>

// Module information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ameer Hamza");
MODULE_DESCRIPTION("Simple Character Module");

// Marcos
#define MAX_CHAR_DEVS 	2
#define BUF_SIZE		1024

/* Function prototypes */
ssize_t char_read(struct file *fp, char __user *user_buf, size_t len, loff_t *loff);
ssize_t char_write (struct file *fp, const char __user *user_buf, size_t len, loff_t *loff);
loff_t char_llseek (struct file *fp, loff_t loff, int sz);
int char_close(struct inode *inode, struct file *fp);
int char_open(struct inode *inode, struct file *fp);

/* file operation structure */
struct file_operations char_fops = {
        .owner = THIS_MODULE,
        .open = char_open,
        .release = char_close,
        .write = char_write,
        .read = char_read,
        .llseek = char_llseek
};

/* My device pointer */
typedef struct my_device {
    struct cdev cdevs;
    int (*read_cb) (struct my_device *dev, char __user *user_buf, long len);
    int (*write_cb) (struct my_device *dev, const char __user *user_buf, long len);
    int dev_id;
    char *buf;
    int major_num, open_ctr, close_ctr;
} my_device;

/* My device pointer */
struct class *my_cls;
my_device *my_dev;

#endif
