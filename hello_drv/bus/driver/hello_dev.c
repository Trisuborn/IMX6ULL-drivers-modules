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
#define DEV_NAME HELLO_MOD_NAME

/**
 * @brief functions declare
 */
void hello_dev_release(struct device *dev);

static int __init hello_dev_init( void );
static void __exit hello_dev_exit( void );

/**
 * @brief module's info
 */
module_init( hello_dev_init );
module_exit( hello_dev_exit );
MODULE_AUTHOR( "Trisuborn <ttowfive@gmail.com>" );
MODULE_DESCRIPTION( "Hello device resource." );
MODULE_LICENSE( "GPL" );

/************************************************
 * @brief resource list
 * 
 * @param: .name    : name
 * @param: .start   : GPIOx
 * @param: .end     : PINx
 ************************************************/
static struct resource hello_resrc[] = {
    /* 1 */
    {
        .name  = "hello_ebf6ull",
        .start = 1,
        .end   = 4,
        .flags = IORESOURCE_REG,
    },
    /* 2 */
    {
        .name  = "hello_ebf6ull",
        .start = 4,
        .end   = 20,
        .flags = IORESOURCE_REG,
    },
    /* 3 */
    {
        .name  = "hello_ebf6ull",
        .start = 4,
        .end   = 19,
        .flags = IORESOURCE_REG,
    },
    /* 4 */
    {
        .name  = "hello_ebf6ull",
        .start = 5,
        .end   = 3,
        .flags = IORESOURCE_REG,
    },
};

static struct platform_device hello_dev_s = {
    .name = DEV_NAME,
    .resource = hello_resrc,
    .num_resources = ARRAY_SIZE( hello_resrc ),
    .dev = {
        .release = hello_dev_release,
    },
};

void hello_dev_release(struct device *dev)
{
    printk( "device: %s released.\n", DEV_NAME );
}

static int __init hello_dev_init( void )
{
    int err;
    printk( "%s Register device: %s\n", __FUNCTION__, hello_dev_s.name );
    err = platform_device_register( &hello_dev_s );
    if (err) {
		pr_warn("Could not register hello");
		goto register_fail;
	}
    
    return 0;

register_fail:
    platform_device_unregister( &hello_dev_s );

}

static void __exit hello_dev_exit( void )
{
    printk( "%s Unregister device: %s\n", __FUNCTION__, hello_dev_s.name );
    platform_device_unregister( &hello_dev_s );
}

