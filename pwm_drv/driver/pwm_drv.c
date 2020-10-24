
/************************
 * @file pwm_drv.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief pwm's driver code
 * @version 0.1
 * @date 2020-10-21
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************/
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/tty.h>
#include <linux/kmod.h>
#include <linux/gfp.h>
#include <asm/io.h>

/************************
 * @brief user header files
 ************************/

#define PWM_NUM     8

/**
 * @brief functions declare
 */
static ssize_t pwm_drv_read (struct file *file, char __user * ubuf, size_t size, loff_t *off);
static ssize_t pwm_drv_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off);
static int pwm_drv_open (struct inode *inode, struct file *file);
static int pwm_drv_release (struct inode *inode, struct file *file);

static int  __init pwm_drv_init(void);
static void __exit pwm_drv_exit(void);

/**
 * @brief module's info
 */
module_init(pwm_drv_init);
module_exit(pwm_drv_exit);
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>  Github: https://github.com/Trisuborn" );
MODULE_DESCRIPTION( "pwm driver." );
MODULE_LICENSE("GPL");

/************************
 * @brief static var
 ************************/
static u8 major = 0;
static u8 mod_name[] = "pwm_mod";
static struct class *pwm_drv_class;
// static led_ctl_typedef *led_opr_p;

struct file_operations pwm_fopt = {
    .owner      = THIS_MODULE,
    .read       = pwm_drv_read,
    .write      = pwm_drv_writ,
    .open       = pwm_drv_open,
    .release    = pwm_drv_release,
};

static ssize_t pwm_drv_read (struct file *file, char __user * ubuf, size_t size, loff_t *off) 
{

    return 1;
}



static ssize_t pwm_drv_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off) 
{

    return 1;
}
 
static int pwm_drv_open (struct inode *inode, struct file *file) 
{

    return 0;
}

static int pwm_drv_release (struct inode *inode, struct file *file) 
{

    return 0;
}

static int __init pwm_drv_init(void) 
{
    int res = 0;
    u8 i;

    printk( "%s line %d\n", __FUNCTION__, __LINE__ );

    major = register_chrdev( 0, mod_name, &pwm_fopt );

    pwm_drv_class = class_create(THIS_MODULE, "pwm_class");
	if (IS_ERR(pwm_drv_class)) {
        printk( "%s line %d class create error\n", __FUNCTION__, __LINE__ );
		res = PTR_ERR(pwm_drv_class);
        unregister_chrdev( major, mod_name );
		return -1;
	}

    for ( i = 0; i < PWM_NUM; i++ )
        device_create(pwm_drv_class, NULL, MKDEV(major, i), NULL, "LED_D%d", i+4); 

    return 0;
}

static void __exit pwm_drv_exit(void) 
{
    u8 i;
    printk( "%s line %d\n", __FUNCTION__, __LINE__ );
    
    for ( i = 0; i < PWM_NUM; i++ )
        device_destroy(pwm_drv_class, MKDEV(major, i));
    
    class_destroy(pwm_drv_class);
    unregister_chrdev( major, mod_name );
}

