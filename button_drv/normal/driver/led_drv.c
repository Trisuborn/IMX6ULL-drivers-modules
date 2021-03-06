
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
#include "led_typedef.h"

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
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>  Github: https://github.com/Trisuborn" );
MODULE_DESCRIPTION( "led driver." );
MODULE_LICENSE("GPL");

/************************
 * @brief static var
 ************************/
static u8 major = 0;
static u8 mod_name[] = "my_led_mod";
static struct class *led_drv_class;
static led_btn_ctl_typedef *led_opr_p;
struct file_operations led_fopt = {
    .owner      = THIS_MODULE,
    .read       = led_drv_read,
    .write      = led_drv_writ,
    .open       = led_drv_open,
    .release    = led_drv_release,
};

static ssize_t led_drv_read (struct file *file, char __user * ubuf, size_t size, loff_t *off) 
{

    unsigned long err;
    struct inode * inode = file_inode( file );
    u8 ledx = iminor( inode );
    int ledx_stat = -1;

    ledx_stat = led_opr_p->g_stat( ledx );
    err = copy_to_user( ubuf, &ledx_stat, 1 );

    return 1;
}



static ssize_t led_drv_writ (struct file *file, const char __user *ubuf, size_t size, loff_t *off) 
{
    struct inode * inode = file_inode( file );
    u8 ledx = iminor( inode );
    u8 opt  = 0;
    u8 pwm_freq;       // temp buffer
    unsigned long err;

    err = copy_from_user( &opt, ubuf, 1 );
    /* 判断普通模式还是PWM模式 */
    if ( (LED_OPT_ON == opt) || ( (LED_OPT_OFF == opt) ) )
        led_opr_p->ctl( ledx, opt );
    else if ( LED_OPT_PWM == chrti(opt) ) {     // 传入字符，转换成十进制数
        err = copy_from_user( &pwm_freq, ubuf+1, 1 );
        led_opr_p->pwm_init( ledx, chrti(pwm_freq) );
        pr_info( "ledx:%d pwm_freq:%d", ledx, pwm_freq );
    }
    return 1;
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

    pr_info( "%s line %d\n", __FUNCTION__, __LINE__ );

    major = register_chrdev( 0, mod_name, &led_fopt );

    led_drv_class = class_create(THIS_MODULE, "led_class");
	if (IS_ERR(led_drv_class)) {
        pr_info( "%s line %d class create error\n", __FUNCTION__, __LINE__ );
		res = PTR_ERR(led_drv_class);
        unregister_chrdev( major, mod_name );
		return -1;
	}

    led_opr_p = ebf6ull_led_opr_get();
    led_opr_p->s_init();

    for ( i = 0; i < LED_NUM; i++ )
        device_create(led_drv_class, NULL, MKDEV(major, i), NULL, "LED_D%d", i+4); 

    return 0;
}

static void __exit led_drv_exit(void) 
{
    u8 i;
    pr_info( "%s line %d\n", __FUNCTION__, __LINE__ );
    
    for ( i = 0; i < LED_NUM; i++ )
        device_destroy(led_drv_class, MKDEV(major, i));
    
    class_destroy(led_drv_class);
    unregister_chrdev( major, mod_name );
}

