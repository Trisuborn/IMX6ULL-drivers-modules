
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

extern u32 dev_param[20][3];
extern u8 dev_num;
static led_btn_property_typedef devx[10];

static CCM_Type  *REMAP_CCM      = NULL;
static GPIO_Type *REMAP_GPIO1    = NULL;
static GPIO_Type *REMAP_GPIO4    = NULL;
static GPIO_Type *REMAP_GPIO5    = NULL;

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

    if ( REMAP_CCM == NULL ) {
        REMAP_CCM   = (CCM_Type *) ioremap( (u32)CCM  , sizeof( CCM_Type  ) );
        REMAP_GPIO1 = (GPIO_Type *)ioremap( (u32)GPIO1, sizeof( GPIO_Type ) );
        REMAP_GPIO4 = (GPIO_Type *)ioremap( (u32)GPIO4, sizeof( GPIO_Type ) );
        REMAP_GPIO5 = (GPIO_Type *)ioremap( (u32)GPIO5, sizeof( GPIO_Type ) );
    }

    for ( i = 0; i < dev_num; i++ ) {
        switch ( dev_param[i][0] ) {
        case 1:
            devx[i].reg.REMAP_CCM_CCGRx = &REMAP_CCM->CCGR1;
            devx[i].reg.REMAP_GPIOx     = REMAP_GPIO1;
            if ( dev_param[i][1] < 10 ) {
                devx[0].reg.REMAP_PAD   = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x5C + (0x4*dev_param[i][1])), 4 );
            }
            devx[i].clk_oft = 26;
            break;
        case 4:
            devx[i].reg.REMAP_CCM_CCGRx = &REMAP_CCM->CCGR3;
            devx[i].reg.REMAP_GPIOx     = REMAP_GPIO4;
            if ( dev_param[i][1] >= 17 ) {
                devx[i].reg.REMAP_PAD       = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x1D4 + (0x4*(dev_param[i][1]-17))), 4 );
            }
            devx[i].clk_oft = 12;
            break;
        case 5:
            devx[i].reg.REMAP_CCM_CCGRx = &REMAP_CCM->CCGR1;
            devx[i].reg.REMAP_GPIOx     = REMAP_GPIO5;
            if ( dev_param[i][1] < 10 ) {
                devx[i].reg.REMAP_PAD   = (__IO u32 *)ioremap( (IOMUXC_SNVS_BASE + 0x8 + (0x4*dev_param[i][1])) , 4 );
            } else {
                devx[i].reg.REMAP_PAD   = (__IO u32 *)ioremap( (IOMUXC_SNVS_BASE + 0x0 + (0x4*(dev_param[i][1]-10))) , 4 );
            }
            devx[i].clk_oft = 30;
            break;
        default:
            break;
        }

        /* 设置时钟及复用、方向和初始状态 */
        *devx[i].reg.REMAP_CCM_CCGRx    |=  3<<(devx[i].clk_oft);
        *devx[i].reg.REMAP_PAD = 0x5;
        devx[i].dr_oft = dev_param[i][1];
        /* 如果设备是LED */
        if ( dev_param[i][2] == DEV_IS_LED ) {
            devx[i].reg.REMAP_GPIOx->DR     |=  (1 << devx[i].dr_oft);
            devx[i].reg.REMAP_GPIOx->GDIR   |=  1 << devx[i].dr_oft;
            devx[i].status = 0;
        }
        /* 如果设备是BTN */ 
        else if ( dev_param[i][2] == DEV_IS_BTN ) {
            devx[i].reg.REMAP_GPIOx->GDIR   &=~  1 << devx[i].dr_oft;
        }

        pr_info( "device %d is %s.\n", i, (dev_param[i][2])?("btn"):("led") );

    }

}

static void ebf6ull_led_btn_init( u8 which_dev )
{

}

static void ebf6ull_led_btn_ctl ( u8 which_dev, LED_OPT_DEF opt )
{
    /* LED 负极拉低时 LED电亮 */
    if ( dev_param[which_dev][2] == DEV_IS_LED ) {
        if ( opt == LED_OPT_ON ) {
            devx[which_dev].reg.REMAP_GPIOx->DR &=~ (1 << devx[which_dev].dr_oft);
            devx[which_dev].status = 1;
        } else if ( opt == LED_OPT_OFF ) {
            devx[which_dev].reg.REMAP_GPIOx->DR |=  (1 << devx[which_dev].dr_oft);
            devx[which_dev].status = 0;
        }
    }

}

static u8 ebf6ull_led_btn_get_stat ( u8 which_dev )
{
    return (devx[which_dev].reg.REMAP_GPIOx->DR & (1 << devx[which_dev].dr_oft))? \
           (0):(1);
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

