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
#include <linux/platform_device.h>

/**
 * @brief user header file
 */
#include "imx6ull_common_inc.h"
#include "hello_dev.h"

/************************************************
 * @brief functions declare
************************************************/
static ssize_t hello_drv_core_read (struct file *file, char __user * ubuf, size_t size, loff_t *off);
static ssize_t hello_drv_core_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off);
static int hello_drv_core_open (struct inode *inode, struct file *file);
static int hello_drv_core_release (struct inode *inode, struct file *file);
static int   hello_drv_core_init(void);
static void  hello_drv_core_exit(void);
/************************************************
 * @brief EXPORT_SYMBOL function
 ************************************************/
static void hello_drv_core_device_create( u8 minor );
static void hello_drv_core_device_destroy( u8 minor );

/************************************************
 * @brief module and EXPORT_SYMBOL conf
************************************************/
EXPORT_SYMBOL( hello_drv_core_device_create );
EXPORT_SYMBOL( hello_drv_core_device_destroy );

module_init(hello_drv_core_init);
module_exit(hello_drv_core_exit);
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>  Github: https://github.com/Trisuborn" );
MODULE_DESCRIPTION( "Hello world driver core." );
MODULE_LICENSE("GPL");
MODULE_VERSION( "2.0" );

/************************************************
 * @brief static var
************************************************/
static u8 major = 0;
static struct class *hello_drv_core_class;
static char mod_buf[512] = {0};

struct file_operations hello_fopt = {
    .owner      = THIS_MODULE,
    .read       = hello_drv_core_read,
    .write      = hello_drv_core_writ,
    .open       = hello_drv_core_open,
    .release    = hello_drv_core_release,
};


/************************************************
 * @brief function realized
 ************************************************/
static ssize_t hello_drv_core_read (struct file *file, char __user * ubuf, size_t size, loff_t *off) 
{
    int status = 0;
    status = copy_to_user(ubuf, mod_buf, size);
    return size;
}

static ssize_t hello_drv_core_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off) 
{
    int status = 0;
    memset( mod_buf, 0, 512 );
    status = copy_from_user( mod_buf, ubuf, size );
    pr_info( "hello write \"%s\" \n", mod_buf );
    return size;
}

static int hello_drv_core_open (struct inode *inode, struct file *file) 
{   
    pr_info("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int hello_drv_core_release (struct inode *inode, struct file *file) 
{
    pr_info("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
    return 0;
}

static int __init hello_drv_core_init(void) 
{
    int res = 0;

    pr_info( "%s line %d\n", __FUNCTION__, __LINE__ );

    major = register_chrdev( 0, HELLO_MOD_NAME, &hello_fopt );

    hello_drv_core_class = class_create(THIS_MODULE, "hello_class");
	if (IS_ERR(hello_drv_core_class)) {
        pr_info( "%s line %d class create error\n", __FUNCTION__, __LINE__ );
		res = PTR_ERR(hello_drv_core_class);
        unregister_chrdev( major, HELLO_MOD_NAME );
		return -1;
	}

    device_create(hello_drv_core_class, NULL, MKDEV(major, 0), NULL, "hello"); 

    return 0;
}

static void __exit hello_drv_core_exit(void) 
{
    
    pr_info( "%s line %d\n", __FUNCTION__, __LINE__ );
    device_destroy(hello_drv_core_class, MKDEV(major, 0));
    class_destroy(hello_drv_core_class);
    unregister_chrdev( major, HELLO_MOD_NAME );
}

static void hello_drv_core_device_create( u8 minor )
{
    device_create(hello_drv_core_class, NULL, MKDEV(major, minor), NULL, "hello%d", minor+1 ); 
}

static void hello_drv_core_device_destroy( u8 minor )
{
    device_destroy(hello_drv_core_class, MKDEV(major, minor));
}

