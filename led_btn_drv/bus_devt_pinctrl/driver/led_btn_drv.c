
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
#include <linux/gpio/consumer.h>

/************************************************
 * @brief user header files
 ************************************************/
#include "imx6ull_common_inc.h"
#include "led_btn_dev.h"
#include "led_btn_typedef.h"
#include "led_btn_handler_ebf6ull.h"

/************************************************
 * @brief user define
 ************************************************/
#define DRV_NAME    LED_MOD_NAME

/************************************************
 * @brief extern functions
 ************************************************/
extern void led_drv_core_device_create ( u8 minor, EBF_DEV_TYPE dev_type );
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
MODULE_SOFTDEP( "preload: led_core" );
MODULE_LICENSE("GPL");


/************************************************
 * @brief extern var
 ************************************************/
extern led_btn_ctl_typedef ebf6ull_led_btn_opt_s;

/************************************************
 * @brief static var
 ***********************************************/
static const struct of_device_id of_led_ebf6ull_match_table[] = {
	{ .compatible = "imx6ull,led" },
    { .compatible = "ebf6ull,led" },

    { .compatible = "imx6ull,btn" },
    { .compatible = "ebf6ull,btn" },
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

struct gpio_desc *dev_gd[10];
u32 dev_param[10]  = {0};
u8 dev_num = 0;


/************************************************
 * @brief function realized
 ************************************************/
static int led_drv_probe(struct platform_device *pdev)
{
    struct device_node *dev_np = NULL;
    int err;

    dev_gd[dev_num] = gpiod_get( &pdev->dev, "dev", 0 );
    
    if (IS_ERR(dev_gd[dev_num])) {
		dev_err(&pdev->dev, "Failed to get GPIO for led\n");
		return PTR_ERR(dev_gd[dev_num]);
	}

    // dev_np = pdev->dev.of_node;
    // if ( dev_np == NULL ) {
    //     pr_info( "probe node_get_error" );
    //     return -1;
    // }

    // err = of_property_read_u32( dev_np, "gpiox", &dev_param[dev_num][0] );
    // err = of_property_read_u32( dev_np, "pinx" , &dev_param[dev_num][1] );
    // err = of_property_read_u32( dev_np, "ebf_type" , &dev_param[dev_num][2] );

    /* 根据设备号和ebf_type创建设备 */
    dev_param[dev_num] = 0;
    led_drv_core_device_create( dev_num, dev_param[dev_num] );

    pr_info( "The %s has been create.\n", pdev->name );

    dev_num++;

    return 0;

}

static int led_drv_remove(struct platform_device *pdev)
{
    u8 i;
    int err;

    for ( i = 0; i < dev_num; i++ ) {
        led_drv_core_device_destroy( i );
        gpiod_put(dev_gd[i]);
        pr_info( "The %s has been destroied.\n", pdev->name );
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
