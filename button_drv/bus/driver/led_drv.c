
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
#include "led_typedef.h"
#include "led_btn_handler_ebf6ull.h"

/************************************************
 * @brief user define
 ************************************************/
#define DRV_NAME    LED_MOD_NAME

/************************************************
 * @brief extern functions
 ************************************************/
extern void led_drv_core_device_create ( u8 minor );
extern void led_drv_core_device_destroy( u8 minor );
extern void register_led_opt( led_btn_ctl_typedef *this_led_opt );

/************************************************
 * @brief function declare
 ************************************************/
static int led_drv_probe(struct platform_device *dev);
static int led_drv_remove(struct platform_device *dev);

static int __init led_drv_init( void );
static void __exit led_drv_exit( void );

/************************************************
 * @brief module config
 ************************************************/
module_init( led_drv_init );
module_exit( led_drv_exit );
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>  Github: https://github.com/Trisuborn" );
MODULE_DESCRIPTION( "led driver." );
MODULE_LICENSE("GPL");

/************************************************
 * @brief extern var
 ************************************************/
extern led_btn_ctl_typedef ebf6ull_led_btn_opt_s;

/************************************************
 * @brief static var
 ************************************************/
static struct platform_driver led_driver_s = {
    .probe  = led_drv_probe,
    .remove = led_drv_remove,
    .driver = {
		.name = DRV_NAME,
	},
};

u8 dev_param[20][2] = {0};
u8 dev_num = 0;

/************************************************
 * @brief function realized
 ************************************************/
static int led_drv_probe(struct platform_device *pdev)
{
    struct resource *res;
    u8 i;
    
    dev_num = pdev->num_resources;
    for ( i = 0; i < dev_num ; i++ ) {
        res = platform_get_resource( pdev, IORESOURCE_REG, i );
        
        dev_param[i][0] = res->start;       // GPIOx
        dev_param[i][1] = res->end;         // PINx

        led_drv_core_device_create( i );

        pr_info( "The %s's %s %d has been create.\n", pdev->name, res->name, i );

    }

    return 0;
}

static int led_drv_remove(struct platform_device *pdev)
{
    u8 i;
    for ( i = 0; i < pdev->num_resources; i++ ) {
        led_drv_core_device_destroy( i );
        pr_info( "The %s's %s %d has been destroied.\n", pdev->name, pdev->resource[i].name, i );
    }
    return 0;
}

static int __init led_drv_init( void )
{
    int err;
    err = platform_driver_register( &led_driver_s );
    if (err) {
		pr_warn("Could not register led_driver_s");
		goto register_fail;
	}
    register_led_opt( ebf6ull_led_opr_get() );
    return 0;

register_fail:
    platform_driver_unregister( &led_driver_s );
}

static void __exit led_drv_exit( void )
{
    platform_driver_unregister( &led_driver_s );
}
