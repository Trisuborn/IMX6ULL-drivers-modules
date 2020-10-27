
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
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>

/************************
 * @brief user's includes
 ************************/
#include "imx6ull_common_inc.h"
#include "led_btn_typedef.h"
#include "led_btn_handler_ebf6ull.h"

static void ebf6ull_led_btn_struct_init ( void );
static void ebf6ull_led_btn_init( u8 which_dev );
static void ebf6ull_led_btn_ctl ( u8 which_dev, LED_OPT_DEF opt );
static u8 ebf6ull_led_btn_get_stat ( u8 which_dev );

static void ebf6ull_led_pwm_init( u8 which_dev, u32 pwm_freq );
static void ebf6ull_led_pwm_ctl( u8 which_dev, u32 pwm_freq, LED_PWM_OPT_DEF opt );

extern u32 dev_param[10];
extern u8 dev_num;
extern struct gpio_desc *dev_gd[10];
static led_btn_property_typedef devx[10];

static led_btn_ctl_typedef ebf6ull_led_btn_opt_s = {
    .init       = ebf6ull_led_btn_init,
    .ctl        = ebf6ull_led_btn_ctl,
    .g_stat     = ebf6ull_led_btn_get_stat,
    .s_init     = ebf6ull_led_btn_struct_init,
    .pwm_init   = ebf6ull_led_pwm_init,
    .pwm_ctl    = ebf6ull_led_pwm_ctl,
};

static void ebf6ull_led_btn_struct_init ( void )
{

    u8 i;
    for ( i = 0; i < dev_num; i++ ) {
        devx[i].status = 0;
        devx[i].conf_status = 0;
    }

}

static void ebf6ull_led_btn_init( u8 which_dev )
{
    if ( !devx[which_dev].conf_status ) {
        gpiod_direction_output( dev_gd[which_dev], 1 );
        gpiod_set_value( dev_gd[which_dev], 0 );
        devx[which_dev].conf_status = 1;
    }
    
}

static void ebf6ull_led_btn_ctl ( u8 which_dev, LED_OPT_DEF opt )
{
    /* LED 负极拉低时 LED电亮 */
    if ( dev_param[which_dev] == DEV_IS_LED ) {
        if ( opt == LED_OPT_ON ) {
            gpiod_set_value( dev_gd[which_dev], 1 );
            devx[which_dev].status = 1;
        } else if ( opt == LED_OPT_OFF ) {
            gpiod_set_value( dev_gd[which_dev], 0 );
            devx[which_dev].status = 0;
        }
    }
}

static u8 ebf6ull_led_btn_get_stat ( u8 which_dev )
{
    return gpiod_get_value( dev_gd[which_dev] );
}


/************************************************
 * @brief   
 * 
 * @param which_dev  
 * @param pwm_freq 
 * @param opt 
 ************************************************/
static void ebf6ull_led_pwm_init( u8 which_dev, u32 pwm_freq )
{
    pr_info( "led%d into pwm mode. \npwm_freq:(%d)kHz\n", which_dev+4, pwm_freq*100 );
}

static void ebf6ull_led_pwm_ctl( u8 which_dev, u32 pwm_freq, LED_PWM_OPT_DEF opt )
{

}

led_btn_ctl_typedef *ebf6ull_led_opr_get(void) 
{
    ebf6ull_led_btn_struct_init();
    return &ebf6ull_led_btn_opt_s;
}

