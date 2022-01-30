#include "char.h"


/*
 * @brief: per char dev callback. Just call on each write and read
 */
static int read_cb(my_device *dev, char __user *user_buf, long len) {
    long read=0;
    printk("READ CALLBACK!! Device-id:%d, Read:%ld, Requested:%ld\n", dev->dev_id, (read = (strlen(dev->buf) - copy_to_user(user_buf, dev->buf, strlen(dev->buf)))), len);
    return (read);
}

/*
 * @brief: per char dev callback. Just call on each write and read
 */
int write_cb(my_device *dev, const char __user *user_buf, long len) {
    long written=0;
    dev->buf[len] = '\0';
    printk("WRITE CALLBACK!! Device-id:%d, Written:%ld, Requested:%ld\n", dev->dev_id, (written = len - copy_from_user(dev->buf, user_buf, len)), len);
    return (written);
}

/*
 * @brief: char dev open routine. Just increment a counter
 */
int char_open(struct inode *inode, struct file *fp) {
    my_device *dev = container_of(inode->i_cdev, my_device, cdevs);
    fp->private_data = dev;
    printk("opened %d times\n", ++dev->open_ctr);
    return 0;
}

/*
 * @brief: char dev close routine. Just increment a counter
 */
int char_close(struct inode *inode, struct file *fp){
    my_device *dev = container_of(inode->i_cdev, my_device, cdevs);
    printk( "closed %d times\n\n", ++dev->close_ctr);
    return 0;
}

/*
 * @brief: char dev llseek routine. Not Implemented
 */
loff_t char_llseek (struct file *fp, loff_t loff, int sz){
    printk("llseek %lld loff\n", loff);
    return 0;
}

/*
 * @brief: character device read routine, e.g. loopback previous write
 */
ssize_t char_read(struct file *fp, char __user *user_buf, size_t len, loff_t *loff){
    my_device *dev = (my_device *) fp->private_data;
    if (dev) {
        return (dev->read_cb(dev, user_buf, len));
    } else {
        printk(KERN_ALERT "No device found");
    }
    return (0);
}

/*
 * @brief: character device write routine
 */
ssize_t char_write (struct file *fp, const char __user *user_buf, size_t len, loff_t *loff){
    my_device *dev = (my_device *) fp->private_data;
    if (dev) {
        return (dev->write_cb(dev, user_buf, len));
    } else {
        printk(KERN_ALERT "No device found");
    }
    return (0);
}

 /*
 * @brief: Module init routine
 */
static int __init char_init(void) {
    int i, j;
    dev_t dev;
    int ret = 0;
    struct device *internal_dev;
    int major;

    // Allocate available major number for character devices
    if ((ret = alloc_chrdev_region(&dev, 0, MAX_CHAR_DEVS, "charclass")) < 0) {
        return (ret);
    }

    // Create class within sysfs for char devices to attach to
    my_cls = class_create(THIS_MODULE, "charclass");

    major = MAJOR(dev);

    if (IS_ERR(my_cls)) {
        ret = PTR_ERR(my_cls);
        goto dealloc_chrdev;
    }

    my_dev = kmalloc(MAX_CHAR_DEVS * sizeof(my_device), GFP_KERNEL);

    if (my_dev) {
        my_dev->buf = kmalloc(BUF_SIZE, GFP_KERNEL);
    } else {
        goto destroy_class;
    }

    my_dev->major_num = major;

    if (! my_dev->buf) {
        goto dealloc_dev;
    }

    for (i=0; i<MAX_CHAR_DEVS; i++) {
        (my_dev + i)->dev_id = i;
        (my_dev + i)->write_cb = write_cb;
        (my_dev + i)->read_cb = read_cb;
        (my_dev + i)->major_num = major;
        (my_dev + i)->buf = kmalloc(BUF_SIZE, GFP_KERNEL);

        if (!(my_dev + i)->buf) {
            goto free_dev;
        }

        strcpy((my_dev + i)->buf, "No data written yet!");

        // Add character device in the system & register filops
        cdev_init(&((my_dev + i)->cdevs), &char_fops);

        if ((ret = cdev_add(&((my_dev + i)->cdevs), MKDEV(major, i), 1)) < 0) {
            goto free_dev;
        }

        // Create /sys and /dev device entries
        if (IS_ERR(internal_dev = device_create(my_cls, NULL, MKDEV(my_dev->major_num, i), NULL, "chardev-%d", i))) {
            ret = PTR_ERR(internal_dev);
            goto free_dev;
        }
    }

    printk("Simple loopback char dev loaded successfully\n\n");
    return 0;

free_dev:
    for (j = 0; j < i + 1; j++) {
        // Remove cdev from the system
        cdev_del(&my_dev[i].cdevs);

        // remove devices in /sys and /dev
        device_destroy(my_cls, MKDEV(my_dev->major_num, i));

        // free allocated buffer
        kfree((my_dev + i)->buf);
    }

dealloc_dev:
    kfree(my_dev);

destroy_class:
    class_destroy(my_cls);

dealloc_chrdev:
    unregister_chrdev_region(MKDEV(my_dev->major_num, i), MAX_CHAR_DEVS);
    printk(KERN_ALERT "Simple loopback char dev load failed. ret:%d\n", ret);
    return (ret);
}

/*
 * @brief: Module exitroutine
 */
static void __exit char_exit(void) {
    int i;

    for (i=0; i<MAX_CHAR_DEVS; i++) {
        // remove devices in /sys and /dev
        device_destroy(my_cls, MKDEV(my_dev->major_num, i));

        // Remove cdev from the system
        cdev_del(&my_dev[i].cdevs);
    }

    // Remove class
    class_destroy(my_cls);

    if (my_dev) {
        kfree(my_dev->buf);
        kfree(my_dev);
    }

    // Release major and minor numbers
    unregister_chrdev_region(MKDEV(my_dev->major_num, i), MAX_CHAR_DEVS);
    printk("Simple loopback char dev unloaded successfully\n\n");
}

module_init(char_init);
module_exit(char_exit);
