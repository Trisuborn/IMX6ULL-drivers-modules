

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

static led_property_typedef ebf6ull_ledx[LED_NUM];

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
        ebf6ull_ledx[i].num = i;
        ebf6ull_ledx[i].status = 0;
        ebf6ull_ledx[i].conf_status = 0;
    }
}

static void ebf6ull_led_init( u8 which_led )
{

    /* 检查是否配置过led引脚 */
    if ( ebf6ull_ledx[which_led].conf_status )
        return;

    switch ( which_led ) {
    case LED_D4:     // EBF6ULL的 LED_D4
        ebf6ull_ledx[which_led].reg.REMAP_CCM_CCGRx   = (__IO u32 *)ioremap( (CCM_BASE + 0x6C)         , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_PIN   = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x6C)      , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_DR    = (__IO u32 *)ioremap( (u32)(&(GPIO1->DR))       , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_GDIR  = (__IO u32 *)ioremap( (u32)(&(GPIO1->GDIR))     , 4 );
        *ebf6ull_ledx[which_led].reg.REMAP_CCM_CCGRx  |= (3 << 26);
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_PIN  = 0x05;
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_GDIR |= (1 << 4);
        break;
    case LED_D5:     // EBF6ULL的 LED_D5
        ebf6ull_ledx[which_led].reg.REMAP_CCM_CCGRx   = (__IO u32 *)ioremap( (CCM_BASE + 0x74)         , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_PIN   = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x1E0)     , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_DR    = (__IO u32 *)ioremap( (u32)(&(GPIO4->DR))       , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_GDIR  = (__IO u32 *)ioremap( (u32)(&(GPIO4->GDIR))     , 4 );
        *ebf6ull_ledx[which_led].reg.REMAP_CCM_CCGRx  |= (3 << 12);
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_PIN  = 0x05;
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_GDIR |= (1 << 20);
        break;
    case LED_D6:     // EBF6ULL的 LED_D6
        ebf6ull_ledx[which_led].reg.REMAP_CCM_CCGRx   = (__IO u32 *)ioremap( (CCM_BASE + 0x74)         , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_PIN   = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x1DC)     , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_DR    = (__IO u32 *)ioremap( (u32)(&(GPIO4->DR))       , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_GDIR  = (__IO u32 *)ioremap( (u32)(&(GPIO4->GDIR))     , 4 );
        *ebf6ull_ledx[which_led].reg.REMAP_CCM_CCGRx  |= (3 << 12);
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_PIN  = 0x05;
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_GDIR |= (1 << 19);
        break;
    case LED_D7:     // EBF6ULL的 LED_D7
        ebf6ull_ledx[which_led].reg.REMAP_CCM_CCGRx   = (__IO u32 *)ioremap( (CCM_BASE + 0x6C)         , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_PIN   = (__IO u32 *)ioremap( (IOMUXC_SNVS_BASE + 0x14) , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_DR    = (__IO u32 *)ioremap( (u32)(&(GPIO5->DR))       , 4 );
        ebf6ull_ledx[which_led].reg.REMAP_GPIOx_GDIR  = (__IO u32 *)ioremap( (u32)(&(GPIO5->GDIR))     , 4 );
        *ebf6ull_ledx[which_led].reg.REMAP_CCM_CCGRx  |= (3 << 30);
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_PIN  = 0x05;
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_GDIR |= (1 << 3);
        break;
    default:
        printk( "led error.\n" );
        return;
    }

    ebf6ull_ledx[which_led].conf_status = 1;

}

static void ebf6ull_led_ctl ( u8 which_led, LED_OPT_DEF opt )
{
    u32 pin_oft = 0;

    /* 引脚偏移 */
    switch (which_led) {
    case LED_D4:
        pin_oft = (1 << 4);
        break;
    case LED_D5:
        pin_oft = (1 << 20);
        break;
    case LED_D6:
        pin_oft = (1 << 19);
        break;
    case LED_D7:
        pin_oft = (1 << 3);
        break;
        
    default:
        break;
    }

    /* LED 负极拉低时 LED电亮 */
    if ( opt == LED_OPT_ON ) {
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_DR &=~ pin_oft;
        ebf6ull_ledx[which_led].status = 1;
    } else if ( opt == LED_OPT_OFF ) {
        *ebf6ull_ledx[which_led].reg.REMAP_GPIOx_DR |=  pin_oft;
        ebf6ull_ledx[which_led].status = 0;
    }
}

static u8 ebf6ull_led_get_stat ( u8 which_led )
{
    printk( "led_status_buf: %d\n ", ebf6ull_ledx[which_led].status );
    return ebf6ull_ledx[which_led].status;
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
    printk( "led%d into pwm mode. \npwm_freq:(%d)\n", which_led+4, pwm_freq );
}

static void ebf6ull_led_pwm_ctl( u8 which_led, u32 pwm_freq, LED_PWM_OPT_DEF opt )
{

}

led_ctl_typedef *ebf6ull_led_opr_get(void) 
{
    return &ebf6ull_led_opr_s;
}

