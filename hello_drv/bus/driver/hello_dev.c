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

/**
 * @brief resource list
 */
static struct resource hello_resrc_arr[] = {
    /* 1 */
    {
        // .name  = "hello_ebf6ull",
        .start = GPIOx_PINx( 1, 4 ),
        .flags = IORESOURCE_IO,
    },
    /* 2 */
    {
        // .name  = "hello_ebf6ull",
        .start = GPIOx_PINx( 4, 20 ),
        .flags = IORESOURCE_IO,
    },
    /* 3 */
    {
        // .name  = "hello_ebf6ull",
        .start = GPIOx_PINx( 4, 19 ),
        .flags = IORESOURCE_IO,
    },
    /* 4 */
    {
        // .name  = "hello_ebf6ull",
        .start = GPIOx_PINx( 5, 3 ),
        .flags = IORESOURCE_IO,
    },
};

static struct resource resources[] = {
        {
                .start = GPIOx_PINx(3,1),
                .flags = IORESOURCE_IRQ,
        },
        {
                .start = GPIOx_PINx(5,8),
                .flags = IORESOURCE_IRQ,
        },
};

static struct platform_device hello_dev_s = {
    .name = DEV_NAME,
    .resource = resources,
    .num_resources = ARRAY_SIZE( resources ),
};


static int __init hello_dev_init( void )
{
    int err;
    printk( "%s Register device: %s\n", __FUNCTION__, hello_dev_s.name );
    err = platform_device_register( &hello_dev_s );
    return 0;
}

static void __exit hello_dev_exit( void )
{
    printk( "%s Unregister device: %s\n", __FUNCTION__, hello_dev_s.name );
    platform_device_unregister( &hello_dev_s );
}
