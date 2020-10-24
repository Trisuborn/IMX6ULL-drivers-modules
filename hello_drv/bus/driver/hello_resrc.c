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


#include "imx6ull_common_inc.h"
#include "hello_resrc.h"

/* The one */
static u8 gpiox_pinx_arr[HELLO_NUM][2] = {
    { 1, 4 },
    { 4, 20 },
    { 4, 19 },
    { 5, 3 },
};

/* The two */
static struct resource hello_resrc_arr[HELLO_NUM] = {
    /* 1 */
    {
        .name  = "hello_ebf6ull";
        .start = GPIOx_PINx( 1, 4 );
        .flags = IORESOURCE_IO;
    },
    /* 2 */
    {
        .name  = "hello_ebf6ull";
        .start = GPIOx_PINx( 4, 20 );
        .flags = IORESOURCE_IO;
    },
    /* 3 */
    {
        .name  = "hello_ebf6ull";
        .start = GPIOx_PINx( 4, 19 );
        .flags = IORESOURCE_IO;
    },
    /* 4 */
    {
        .name  = "hello_ebf6ull";
        .start = GPIOx_PINx( 5, 3 );
        .flags = IORESOURCE_IO;
    },
};

static struct platform_device hello_resrc = {
    .name = HELLO_DEV_DRV;
    .resource = hello_resrc_arr;
    .num_resources = ARRAY_SIZE( hello_resrc_arr );
};

platform_device *hello_get_resrc( void ) 
{   

    return &hello_resrc;
}
