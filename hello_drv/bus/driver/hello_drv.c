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

/**
 * @brief user define
 */
#define DRV_NAME    HELLO_MOD_NAME

/**
 * @brief functions declare
 */
static int hello_drv_probe(struct platform_device *pdev);
static int hello_drv_remove(struct platform_device *pdev);

static int __init hello_drv_init( void );
static void __exit hello_drv_exit( void );

/**
 * @brief module's info
 */
module_init(hello_drv_init);
module_exit(hello_drv_exit);
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>" );
MODULE_DESCRIPTION( "Hello world driver." );
MODULE_LICENSE("GPL");

/**
 * @brief static var
 */
static struct platform_driver hello_drv_s = {
    .probe = hello_drv_probe,
	.remove = hello_drv_remove,
	.driver = {
		.name = DRV_NAME,
	},
};

/**
 * @brief function's realized
 */

static int hello_drv_probe(struct platform_device *pdev)
{
    printk( "do %s\n", __FUNCTION__ );
    return 0;
}

static int hello_drv_remove(struct platform_device *pdev)
{
    printk( "do %s\n", __FUNCTION__ );
    return 0;
}

static int __init hello_drv_init( void )
{
    int err;
    printk( "%s Register driver: %s\n", __FUNCTION__, hello_drv_s.driver.name );
    err = platform_driver_register( &hello_drv_s );
    return 0;
}

static void __exit hello_drv_exit( void )
{   
    printk( "%s unregister driver: %s\n", __FUNCTION__, hello_drv_s.driver.name );
    platform_driver_unregister( &hello_drv_s );
}

