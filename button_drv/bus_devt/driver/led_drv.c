
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
#include <linux/of.h>
#include <linux/platform_device.h>

/************************************************
 * @brief user header files
 ************************************************/
#include "imx6ull_common_inc.h"
#include "led_dev.h"
#include "led_typedef.h"
#include "led_handler_ebf6ull.h"

/************************************************
 * @brief user define
 ************************************************/
#define DRV_NAME    LED_MOD_NAME

/************************************************
 * @brief extern functions
 ************************************************/
extern void led_drv_core_device_create ( u8 minor );
extern void led_drv_core_device_destroy( u8 minor );
extern void register_led_opt( led_ctl_typedef *this_led_opt );

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
MODULE_SOFTDEP( "preload: led_core" );
MODULE_LICENSE("GPL");


/************************************************
 * @brief extern var
 ************************************************/
extern led_ctl_typedef ebf6ull_led_opr_s;

/************************************************
 * @brief static var
 ***********************************************/
static const struct of_device_id of_led_ebf6ull_match_table[] = {
	{ .compatible = "imx6ull,led" },
    { .compatible = "ebf6ull,led" },
	{ }
};
MODULE_DEVICE_TABLE(of, of_led_ebf6ull_match_table);

static struct platform_driver led_driver_s = {
    .probe  = led_drv_probe,
    .remove = led_drv_remove,
    .driver = {
		.name = DRV_NAME,
        .of_match_table = of_led_ebf6ull_match_table,
	},
};


u8 dev_num = 0;
u32 dev_param[20][2]  = {0};

/************************************************
 * @brief function realized
 ************************************************/
static int led_drv_probe(struct platform_device *pdev)
{
    struct device_node *dev_np = NULL;
    int err;
    
    dev_np = pdev->dev.of_node;
    if ( dev_np == NULL ) {
        pr_info( "probe node_get_error" );
        return -1;
    }

    err = of_property_read_u32( dev_np, "gpiox", &dev_param[dev_num][0] );
    err = of_property_read_u32( dev_np, "pinx" , &dev_param[dev_num][1] );
    pr_info( "probe err: %d\n", err );

    led_drv_core_device_create( dev_num );

    pr_info( "The %s's %s %d has been create.\n", pdev->name, dev_np->name, dev_num );

    dev_num++;

    return 0;

}

static int led_drv_remove(struct platform_device *pdev)
{
    u8 i;
    struct device_node *dev_np = NULL;
    int err;

    u32 gpiox = 0;
    u32 pinx  = 0;
    
    dev_np = pdev->dev.of_node;
    if ( dev_np == NULL ) {
        pr_info( "probe node_get_error" );
        return -1;
    }
    
    err = of_property_read_u32( dev_np, "gpiox", &gpiox );
    err = of_property_read_u32( dev_np, "pinx" , &pinx );

    for ( i = 0; i < dev_num; i++ ) {
        if ( (gpiox == dev_param[i][0]) && (pinx == dev_param[i][1]) ) {
            led_drv_core_device_destroy( i );
            pr_info( "%d %d\n", dev_param[i][0], dev_param[i][1] );
            break;
        }
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

    pr_info("ebf6ull_led_opr_get");
    register_led_opt( ebf6ull_led_opr_get() );
    return 0;

register_fail:
    platform_driver_unregister( &led_driver_s );
    return -1;
}

static void __exit led_drv_exit( void )
{
    
    platform_driver_unregister( &led_driver_s );
}
