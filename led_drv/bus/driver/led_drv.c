
/************************************************
 * @file led_drv.c
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
#define DRV_NAME    LED_MOD_NAME

/************************************************
 * @brief function declare
 ************************************************/
static int __init led_drv_init( void );
static int __exit led_drv_exit( void );

/************************************************
 * @brief module config
 ************************************************/
module_init( led_drv_init );
module_init( led_drv_exit );
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>  Github: https://github.com/Trisuborn" );
MODULE_DESCRIPTION( "led driver." );
MODULE_LICENSE("GPL");

/************************************************
 * @brief static var
 ************************************************/

static struct platform_driver led_driver_s = {
    .probe  = ;
    .remove = ;
    .driver = {
		.name = DRV_NAME,
	},
};

/************************************************
 * @brief function realized
 ************************************************/
static int __init led_drv_init( void )
{

    led_driver_s.probe  = ;
    led_driver_s.remove = ;

    int err;
    err = platform_driver_register( led_driver_s );
    if (err) {
		pr_warn("Could not register led_driver_s");
		goto register_fail;
	}
    
    return 0;

register_fail:
    platform_driver_unregister( &led_driver_s );
}

static int __exit led_drv_exit( void )
{
    platform_driver_unregister( led_driver_s );
}

