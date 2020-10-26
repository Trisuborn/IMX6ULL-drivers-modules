
/************************
 * @file led_drv_core.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief led's driver code
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
#include "imx6ull_common_inc.h"
#include "led_btn_dev.h"
#include "led_btn_typedef.h"

/**
 * @brief functions declare
 */
static ssize_t led_drv_core_read (struct file *file, char __user * ubuf, size_t size, loff_t *off);
static ssize_t led_drv_core_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off);
static int led_drv_core_open (struct inode *inode, struct file *file);
static int led_drv_core_release (struct inode *inode, struct file *file);

static int  __init led_drv_core_init(void);
static void __exit led_drv_core_exit(void);
/************************************************
 * @brief EXPORT_SYMBOL function
 ************************************************/
static void led_drv_core_device_create( u8 minor, EBF_DEV_TYPE dev_type );
static void led_drv_core_device_destroy( u8 minor );
static void register_led_opt( led_btn_ctl_typedef *this_led_opt );

/************************************************
 * @brief module and EXPORT_SYMBOL conf
************************************************/
module_init(led_drv_core_init);
module_exit(led_drv_core_exit);
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>  Github: https://github.com/Trisuborn" );
MODULE_DESCRIPTION( "led driver." );
MODULE_LICENSE("GPL");

EXPORT_SYMBOL( led_drv_core_device_create );
EXPORT_SYMBOL( led_drv_core_device_destroy );
EXPORT_SYMBOL( register_led_opt );

/************************************************
 * @brief static var
 ************************************************/
static u8 major = 0;
static struct class *led_drv_core_class;
static led_btn_ctl_typedef *led_opt;
struct file_operations led_fopt = {
    .owner      = THIS_MODULE,
    .read       = led_drv_core_read,
    .write      = led_drv_core_writ,
    .open       = led_drv_core_open,
    .release    = led_drv_core_release,
};


/************************************************
 * @brief function realized
 ************************************************/
static ssize_t led_drv_core_read (struct file *file, char __user * ubuf, size_t size, loff_t *off) 
{
    unsigned long err;
    struct inode * inode = file_inode( file );
    u8 ledx = iminor( inode );
    int ledx_stat = -1;

    pr_info( "read led_ebf%d status\n", ledx );

    ledx_stat = led_opt->g_stat( ledx );
    err = copy_to_user( ubuf, &ledx_stat, 1 );

    return 1;
}



static ssize_t led_drv_core_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off) 
{
    struct inode * inode = file_inode( file );
    u8 ledx = iminor( inode );
    u8 opt  = 0;
    u8 pwm_freq;
    unsigned long err;

    pr_info( "write into led_ebf%d\n", ledx );

    err = copy_from_user( &opt, ubuf, 1 );
    /* 判断普通模式还是PWM模式 */
    if ( (LED_OPT_ON == opt) || ( (LED_OPT_OFF == opt) ) )
        led_opt->ctl( ledx, opt );
    else if ( LED_OPT_PWM == chrti(opt) ) {     // 传入字符，转换成十进制数
        err = copy_from_user( &pwm_freq, ubuf+1, 1 );
        led_opt->pwm_init( ledx, chrti(pwm_freq) );
        pr_info( "ledx:%d pwm_freq:%d", ledx, pwm_freq );
    }
    
    return 1;
}
 
static int led_drv_core_open (struct inode *inode, struct file *file) 
{
    u8 ledx = iminor( inode );
    pr_info( "init led_ebf%d\n", ledx );
    led_opt->init( ledx );
    return 0;
}

static int led_drv_core_release (struct inode *inode, struct file *file) 
{
    u8 ledx = iminor( inode );
    pr_info( "release led_ebf%d\n", ledx );
    return 0;
}

static int __init led_drv_core_init(void) 
{
    int res = 0;

    pr_info( "%s line %d\n", __FUNCTION__, __LINE__ );

    major = register_chrdev( 0, LED_MOD_NAME, &led_fopt );

    led_drv_core_class = class_create(THIS_MODULE, "led_ebf6ull_class");
	if (IS_ERR(led_drv_core_class)) {
        pr_info( "%s line %d class create error\n", __FUNCTION__, __LINE__ );
		res = PTR_ERR(led_drv_core_class);
        unregister_chrdev( major, LED_MOD_NAME );
		return -1;
	}

    return 0;
}

static void __exit led_drv_core_exit(void) 
{
    pr_info( "%s line %d\n", __FUNCTION__, __LINE__ );
    class_destroy(led_drv_core_class);
    unregister_chrdev( major, LED_MOD_NAME );
}

static void led_drv_core_device_create( u8 minor, EBF_DEV_TYPE dev_type )
{
    static u8 led_num = 4;
    static u8 btn_num = 0;
    
    device_create(  led_drv_core_class, \
                    NULL, MKDEV(major, minor), \
                    NULL, \
                    (dev_type)?("btn_ebf%d"):("led_ebf%d") , \
                    (dev_type)?(btn_num):(led_num) ); 
    
    if ( dev_type == DEV_IS_BTN )
        btn_num++;
    else if ( dev_type == DEV_IS_LED )
        led_num++;

}

static void led_drv_core_device_destroy( u8 minor )
{
    device_destroy(led_drv_core_class, MKDEV(major, minor));
}

static void register_led_opt( led_btn_ctl_typedef *this_led_opt )
{
    led_opt = this_led_opt;
}

