

#include "ebf6ull_led_handler.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/ioctl.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <linux/of.h>
#include <linux/cdev.h>
#include <asm/io.h>

/************************
 * @brief user's includes
 ************************/
#include "MCIMX6Y2.h"


static void ebf6ull_led_init( u8 which_led );
static void ebf6ull_led_ctl ( u8 which_led, u8 opt );

static led_ctl_typedef ebf6ull_led_opr_s = {
    .init   = ebf6ull_led_init,
    .ctl    = ebf6ull_led_ctl,
};

static void ebf6ull_led_init( u8 which_led )
{
    printk( "init led%d\n", which_led );
}

static void ebf6ull_led_ctl ( u8 which_led, u8 opt )
{
    GPIO_Type gpio;

    
}

led_ctl_typedef *ebf6ull_led_opr_get(void) 
{
    return &ebf6ull_led_opr_s;
}

