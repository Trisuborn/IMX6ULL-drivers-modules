

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
#include "common_inc.h"
#include "led_typedef.h"

static void ebf6ull_led_struct_init ( void );
static void ebf6ull_led_init( u8 which_led );
static void ebf6ull_led_ctl ( u8 which_led, LED_OPT_DEF opt );
static u8 ebf6ull_led_get_stat ( u8 which_led );

static void ebf6ull_led_pwm_init( u8 which_led, u32 pwm_freq );
static void ebf6ull_led_pwm_ctl( u8 which_led, u32 pwm_freq, LED_PWM_OPT_DEF opt );

static led_property_typedef ledx[LED_NUM];

static CCM_Type  *REMAP_CCM      = NULL;
static GPIO_Type *REMAP_GPIO1    = NULL;
static GPIO_Type *REMAP_GPIO4    = NULL;
static GPIO_Type *REMAP_GPIO5    = NULL;

static led_ctl_typedef ebf6ull_led_opr_s = {
    .init       = ebf6ull_led_init,
    .ctl        = ebf6ull_led_ctl,
    .g_stat     = ebf6ull_led_get_stat,
    .s_init     = ebf6ull_led_struct_init,
    .pwm_init   = ebf6ull_led_pwm_init,
    .pwm_ctl    = ebf6ull_led_pwm_ctl,
};

static void ebf6ull_led_struct_init ( void )
{
    u8 i;
    for ( i = 0; i < LED_NUM; i++ ) {
        ledx[i].status = 0;
        ledx[i].conf_status = 0;
    }

    if ( REMAP_CCM == NULL ) {
        REMAP_CCM   = (CCM_Type *) ioremap( (u32)CCM  , sizeof( CCM_Type  ) );
        REMAP_GPIO1 = (GPIO_Type *)ioremap( (u32)GPIO1, sizeof( GPIO_Type ) );
        REMAP_GPIO4 = (GPIO_Type *)ioremap( (u32)GPIO4, sizeof( GPIO_Type ) );
        REMAP_GPIO5 = (GPIO_Type *)ioremap( (u32)GPIO5, sizeof( GPIO_Type ) );
    }

}

static void ebf6ull_led_init( u8 which_led )
{

    /* 检查是否配置过led引脚 */
    if ( ledx[which_led].conf_status )
        return;

    printk( "error led config.\n" );

    switch ( which_led ) {
    case LED_D4:     // EBF6ULL的 LED_D4
        ledx[which_led].reg.REMAP_GPIOx         = REMAP_GPIO1;
        ledx[which_led].reg.REMAP_CCM_CCGRx     = &REMAP_CCM->CCGR1;
        ledx[which_led].reg.REMAP_PAD           = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x6C) , 4 );

        *ledx[which_led].reg.REMAP_CCM_CCGRx    |= 3 << 26;
        *ledx[which_led].reg.REMAP_PAD          =  0x05;
        ledx[which_led].oft = 4;
        break;
    case LED_D5:     // EBF6ULL的 LED_D5
        ledx[which_led].reg.REMAP_GPIOx         = REMAP_GPIO4;
        ledx[which_led].reg.REMAP_CCM_CCGRx     = &REMAP_CCM->CCGR3;
        ledx[which_led].reg.REMAP_PAD           = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x1E0) , 4 );

        *ledx[which_led].reg.REMAP_CCM_CCGRx    |= 3 << 12;
        *ledx[which_led].reg.REMAP_PAD          =  0x05;
        ledx[which_led].oft = 20;
        break;
    case LED_D6:     // EBF6ULL的 LED_D6
        ledx[which_led].reg.REMAP_GPIOx         = REMAP_GPIO4;
        ledx[which_led].reg.REMAP_CCM_CCGRx     = &REMAP_CCM->CCGR3;
        ledx[which_led].reg.REMAP_PAD           = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x1DC) , 4 );

        *ledx[which_led].reg.REMAP_CCM_CCGRx    |= 3 << 12;
        *ledx[which_led].reg.REMAP_PAD          =  0x05;
        ledx[which_led].oft = 19;
        break;
    case LED_D7:     // EBF6ULL的 LED_D7
        ledx[which_led].reg.REMAP_GPIOx         = REMAP_GPIO5;
        ledx[which_led].reg.REMAP_CCM_CCGRx     = &REMAP_CCM->CCGR1;
        ledx[which_led].reg.REMAP_PAD           = (__IO u32 *)ioremap( (IOMUXC_SNVS_BASE + 0x14) , 4 );

        *ledx[which_led].reg.REMAP_CCM_CCGRx    |= 3 << 30;
        *ledx[which_led].reg.REMAP_PAD          =  0x05;
        ledx[which_led].oft = 3;
        break;
    default:
        printk( "led error.\n" );
        return;
    }

    ledx[which_led].reg.REMAP_GPIOx->GDIR   |= 1 << ledx[which_led].oft;

    ledx[which_led].conf_status = 1;

}

static void ebf6ull_led_ctl ( u8 which_led, LED_OPT_DEF opt )
{
    /* LED 负极拉低时 LED电亮 */
    if ( opt == LED_OPT_ON ) {
        ledx[which_led].reg.REMAP_GPIOx->DR &=~ (1 << ledx[which_led].oft);
        ledx[which_led].status = 1;
    } else if ( opt == LED_OPT_OFF ) {
        ledx[which_led].reg.REMAP_GPIOx->DR |=  (1 << ledx[which_led].oft);
        ledx[which_led].status = 0;
    }
}

static u8 ebf6ull_led_get_stat ( u8 which_led )
{
    printk( "led_status_buf: %d\n ", ledx[which_led].status );
    return ledx[which_led].status;
}



/************************************************
 * @brief   
 * 
 * @param which_led  
 * @param pwm_freq 
 * @param opt 
 ************************************************/
static void ebf6ull_led_pwm_init( u8 which_led, u32 pwm_freq )
{
    printk( "led%d into pwm mode. \npwm_freq:(%d)kHz\n", which_led+4, pwm_freq*100 );
}

static void ebf6ull_led_pwm_ctl( u8 which_led, u32 pwm_freq, LED_PWM_OPT_DEF opt )
{

}

led_ctl_typedef *ebf6ull_led_opr_get(void) 
{
    return &ebf6ull_led_opr_s;
}

