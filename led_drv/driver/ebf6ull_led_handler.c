

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

static void ebf6ull_led_init( u8 which_led );
static void ebf6ull_led_deinit ( u8 which_led );
static void ebf6ull_led_ctl ( u8 which_led, u8 opt );
static u8 ebf6ull_led_get_stat ( u8 which_led );

__IO static u32 *REMAP_CCM_CCGRx    = NULL;
__IO static u32 *REMAP_GPIOx_PIN    = NULL;
__IO static u32 *REMAP_GPIOx_DR     = NULL;
__IO static u32 *REMAP_GPIOx_GDIR   = NULL;

/* LED 状态 BUF */
static u8 led_status_buf[LED_NUM] = {0};
/* LED 配置状态 BUF */
static u8 led_conf_status_buf[LED_NUM] = {0};
static led_ctl_typedef ebf6ull_led_opr_s = {
    .init   = ebf6ull_led_init,
    .deinit = ebf6ull_led_deinit,
    .ctl    = ebf6ull_led_ctl,
    .g_stat = ebf6ull_led_get_stat,
};

static void ebf6ull_led_init( u8 which_led )
{
    /* 检查是否配置过led引脚 */
    if ( led_conf_status_buf[which_led] )
        return;
    switch ( which_led ) {
    case LED_D4:     // EBF6ULL的 LED_D4
        REMAP_CCM_CCGRx   = (__IO u32 *)ioremap( (CCM_BASE + 0x6C)         , 4 );
        REMAP_GPIOx_PIN   = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x6C)      , 4 );
        REMAP_GPIOx_DR    = (__IO u32 *)ioremap( (u32)(&(GPIO1->DR))       , 4 );
        REMAP_GPIOx_GDIR  = (__IO u32 *)ioremap( (u32)(&(GPIO1->GDIR))     , 4 );
        *REMAP_CCM_CCGRx  |= (3 << 26);
        *REMAP_GPIOx_PIN  = 0x05;
        *REMAP_GPIOx_GDIR |= (1 << 4);
        break;
    case LED_D5:     // EBF6ULL的 LED_D5
        REMAP_CCM_CCGRx   = (__IO u32 *)ioremap( (CCM_BASE + 0x74)         , 4 );
        REMAP_GPIOx_PIN   = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x1E0)     , 4 );
        REMAP_GPIOx_DR    = (__IO u32 *)ioremap( (u32)(&(GPIO4->DR))       , 4 );
        REMAP_GPIOx_GDIR  = (__IO u32 *)ioremap( (u32)(&(GPIO4->GDIR))     , 4 );
        *REMAP_CCM_CCGRx  |= (3 << 12);
        *REMAP_GPIOx_PIN  = 0x05;
        *REMAP_GPIOx_GDIR |= (1 << 20);
        break;
    case LED_D6:     // EBF6ULL的 LED_D6
        REMAP_CCM_CCGRx   = (__IO u32 *)ioremap( (CCM_BASE + 0x74)         , 4 );
        REMAP_GPIOx_PIN   = (__IO u32 *)ioremap( (IOMUXC_BASE + 0x1DC)     , 4 );
        REMAP_GPIOx_DR    = (__IO u32 *)ioremap( (u32)(&(GPIO4->DR))       , 4 );
        REMAP_GPIOx_GDIR  = (__IO u32 *)ioremap( (u32)(&(GPIO4->GDIR))     , 4 );
        *REMAP_CCM_CCGRx  |= (3 << 12);
        *REMAP_GPIOx_PIN  = 0x05;
        *REMAP_GPIOx_GDIR |= (1 << 19);
        break;
    case LED_D7:     // EBF6ULL的 LED_D7
        REMAP_CCM_CCGRx   = (__IO u32 *)ioremap( (CCM_BASE + 0x6C)         , 4 );
        REMAP_GPIOx_PIN   = (__IO u32 *)ioremap( (IOMUXC_SNVS_BASE + 0x14) , 4 );
        REMAP_GPIOx_DR    = (__IO u32 *)ioremap( (u32)(&(GPIO5->DR))       , 4 );
        REMAP_GPIOx_GDIR  = (__IO u32 *)ioremap( (u32)(&(GPIO5->GDIR))     , 4 );
        *REMAP_CCM_CCGRx  |= (3 << 30);
        *REMAP_GPIOx_PIN  = 0x05;
        *REMAP_GPIOx_GDIR |= (1 << 3);
        break;
    default:
        printk( "led error.\n" );
        return;
    }

    led_conf_status_buf[which_led] = 1;

    printk( "init led%d done.\n", which_led );
}

static void ebf6ull_led_ctl ( u8 which_led, u8 opt )
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
    if ( opt ) {
        *REMAP_GPIOx_DR &=~ pin_oft;
        led_status_buf[which_led] = 1;
    } else {
        *REMAP_GPIOx_DR |=  pin_oft;
        led_status_buf[which_led] = 0;
    }
}

static u8 ebf6ull_led_get_stat ( u8 which_led )
{
    printk( "led_status_buf: %d\n ", led_status_buf[which_led] );
    return led_status_buf[which_led];
}

/************************
 * @brief led_deinit
 * 
 * @param which_led 
 ************************/
static void ebf6ull_led_deinit ( u8 which_led )
{

    u32 clk_oft = 0;

    led_conf_status_buf[which_led] = 0;

    switch (which_led) {
    case LED_D4:
        clk_oft = (3 << 26);
        break;
    case LED_D5:
    case LED_D6:
        clk_oft = (3 << 12);
        break;
    case LED_D7:
        clk_oft = (3 << 30);
        break;
        
    default:
        break;
    }

    *REMAP_CCM_CCGRx    &=~ clk_oft;

    REMAP_CCM_CCGRx    = NULL;
    REMAP_GPIOx_PIN    = NULL;
    REMAP_GPIOx_DR     = NULL;
    REMAP_GPIOx_GDIR   = NULL;
}

led_ctl_typedef *ebf6ull_led_opr_get(void) 
{
    return &ebf6ull_led_opr_s;
}

