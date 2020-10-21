
/************************
 * @file led_drv.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief led's driver code
 * @version 0.1
 * @date 2020-10-21
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************/

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

/************************
 * @brief user header files
 ************************/
#include "led_typedef.h"
#include "ebf6ull_led_handler.h"

/**
 * @brief functions declare
 */
static ssize_t led_drv_read (struct file *file, char __user * ubuf, size_t size, loff_t *off);
static ssize_t led_drv_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off);
static int led_drv_open (struct inode *inode, struct file *file);
static int led_drv_release (struct inode *inode, struct file *file);

static int  __init led_drv_init(void);
static void __exit led_drv_exit(void);

/**
 * @brief module's info
 */
module_init(led_drv_init);
module_exit(led_drv_exit);
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>" );
MODULE_DESCRIPTION( "led driver." );
MODULE_LICENSE("GPL");

/************************
 * @brief define
 ************************/
#define LED_NUM     3

/************************
 * @brief static var
 ************************/
static u8 major = 0;
static u8 mod_name[] = "my_led_mod";
static struct class *led_drv_class;
static led_ctl_typedef *led_opr_p;
struct file_operations led_fopt = {
    .owner      = THIS_MODULE,
    .read       = led_drv_read,
    .write      = led_drv_writ,
    .open       = led_drv_open,
    .release    = led_drv_release,
};

static ssize_t led_drv_read (struct file *file, char __user * ubuf, size_t size, loff_t *off) 
{
    printk( "read  size: %d\n", size );
    return size;
}

static ssize_t led_drv_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off) 
{
    printk( "write size: %d\n", size );

    
    
    return size;
}

static int led_drv_open (struct inode *inode, struct file *file) 
{
    u8 ledx = iminor( inode );
    led_opr_p->init( ledx );
    return 0;
}

static int led_drv_release (struct inode *inode, struct file *file) 
{
    u8 ledx = iminor( inode );
    return 0;
}

static int __init led_drv_init(void) 
{
    int res = 0;
    u8 i;

    printk( "%s line %d\n", __FUNCTION__, __LINE__ );

    major = register_chrdev( 0, mod_name, &led_fopt );

    led_drv_class = class_create(THIS_MODULE, "led_class");
	if (IS_ERR(led_drv_class)) {
        printk( "%s line %d class create error\n", __FUNCTION__, __LINE__ );
		res = PTR_ERR(led_drv_class);
        unregister_chrdev( major, mod_name );
		return -1;
	}

    led_opr_p = ebf6ull_led_opr_get();

    for ( i = 0; i < LED_NUM; i++ )
        device_create(led_drv_class, NULL, MKDEV(major, i), NULL, "ebf6ull_led_D%d", i+4); 

    return 0;
}

static void __exit led_drv_exit(void) 
{
    u8 i;
    printk( "%s line %d\n", __FUNCTION__, __LINE__ );
    
    for ( i = 0; i < LED_NUM; i++ )
        device_destroy(led_drv_class, MKDEV(major, i));
    
    class_destroy(led_drv_class);
    unregister_chrdev( major, mod_name );
}

