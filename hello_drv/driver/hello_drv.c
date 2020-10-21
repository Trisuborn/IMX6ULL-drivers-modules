#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

/**
 * @brief functions declare
 */
static ssize_t hello_drv_read (struct file *file, char __user * ubuf, size_t size, loff_t *off);
static ssize_t hello_drv_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off);
static int hello_drv_open (struct inode *inode, struct file *file);
static int hello_drv_release (struct inode *inode, struct file *file);

static int  __init hello_drv_init(void);
static void __exit hello_drv_exit(void);

/**
 * @brief module's info
 */
module_init(hello_drv_init);
module_exit(hello_drv_exit);
MODULE_AUTHOR( "Trisuborn" );
MODULE_DESCRIPTION( "Hello world driver." );
MODULE_LICENSE("GPL");

/**
 * @brief static var
 */
static u8 major = 0;
static u8 mod_name[] = "hello_mod";
static struct class *hello_drv_class;
static char mod_buf[512] = {0};

struct file_operations hello_fopt = {
    .owner      = THIS_MODULE,
    .read       = hello_drv_read,
    .write      = hello_drv_writ,
    .open       = hello_drv_open,
    .release    = hello_drv_release,
};


static ssize_t hello_drv_read (struct file *file, char __user * ubuf, size_t size, loff_t *off) 
{
    int status = 0;
    status = copy_to_user(ubuf, mod_buf, size);
    return size;
}

static ssize_t hello_drv_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off) 
{
    int status = 0;
    memset( mod_buf, 0, 512 );
    status = copy_from_user( mod_buf, ubuf, size );
    printk( "hello write \"%s\" \n", mod_buf );
    return size;
}

static int hello_drv_open (struct inode *inode, struct file *file) 
{   
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int hello_drv_release (struct inode *inode, struct file *file) 
{
    printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int __init hello_drv_init(void) 
{
    int res = 0;

    printk( "%s line %d\n", __FUNCTION__, __LINE__ );

    major = register_chrdev( 0, mod_name, &hello_fopt );

    hello_drv_class = class_create(THIS_MODULE, "hello_class");
	if (IS_ERR(hello_drv_class)) {
        printk( "%s line %d class create error\n", __FUNCTION__, __LINE__ );
		res = PTR_ERR(hello_drv_class);
        unregister_chrdev( major, mod_name );
		return -1;
	}

    device_create(hello_drv_class, NULL, MKDEV(major, 0), NULL, "hello"); 

    return 0;
}

static void __exit hello_drv_exit(void) 
{
    printk( "%s line %d\n", __FUNCTION__, __LINE__ );
    device_destroy(hello_drv_class, MKDEV(major, 0));
    class_destroy(hello_drv_class);
    unregister_chrdev( major, mod_name );
}

