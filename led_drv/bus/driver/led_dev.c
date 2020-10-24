
/************************************************
 * @file led_dev.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-10-24
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************************************/

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


/************************************************
 * @brief user header files
 ************************************************/
#include "imx6ull_common_inc.h"
#include "led_dev.h"

/************************************************
 * @brief user define
 ************************************************/
#define DEV_NAME    LED_MOD_NAME

/************************************************
 * @brief function declare
 ************************************************/
static void led_dev_release(struct device *dev);
static int __init led_dev_init( void );
static int __exit led_dev_exit( void );

/************************************************
 * @brief module config
 ************************************************/
module_init( led_dev_init );
module_init( led_dev_exit );
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>  Github: https://github.com/Trisuborn" );
MODULE_DESCRIPTION( "led device." );
MODULE_LICENSE("GPL");

/************************************************
 * @brief static var
 ************************************************/
static struct resource led_resrc[] = {
    {
        .name  = "led_ebf",
        .start = 1,
        .end   = 4,
        .flags = IORESOURCE_REG,
    },
    {
        .name  = "led_ebf",
        .start = 4,
        .end   = 20,
        .flags = IORESOURCE_REG,
    },
    {
        .name  = "led_ebf",
        .start = 4,
        .end   = 19,
        .flags = IORESOURCE_REG,
    },
    {
        .name  = "led_ebf",
        .start = 5,
        .end   = 3,
        .flags = IORESOURCE_REG,
    },
};

static struct platform_device led_device_s = {
    .name = DEV_NAME,
    .resource = led_resrc,
    .num_resources = ARRAY_SIZE(led_resrc),
    .dev = {
        .release = led_dev_release;
    },
};

/************************************************
 * @brief function realized
 ************************************************/
static void led_dev_release(struct device *dev)
{
    if ( dev ) {
        printk( "device: %s released.\n", DEV_NAME );
        dev = NULL;
    }
}

static int __init led_dev_init( void )
{
    int err;
    err = platform_device_register( led_device_s );
    if (err) {
		pr_warn("Could not register led_device_s");
		goto register_fail;
	}
    
    return 0;

register_fail:
    platform_device_unregister( &led_device_s );
}

static int __exit led_dev_exit( void )
{
    platform_device_unregister( led_device_s );
}

