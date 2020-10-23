
#ifndef __LED_TYPEDEF_H__
#define __LED_TYPEDEF_H__


#include "common_inc.h"
#include <asm-generic/int-ll64.h>

/************************
 * @brief define
 ************************/
#define LED_NUM     4

typedef enum LED_OPT_DEF {
    LED_OPT_LOW    = 0,
    LED_OPT_HIGH      ,
    LED_OPT_PWM       ,
} LED_OPT_DEF;

typedef enum LED_PWM_OPT_DEF {
    LED_PWM_OPT_OFF    = 0,
    LED_PWM_OPT_ON        ,
} LED_PWM_OPT_DEF;

typedef enum LED_Dx{
    LED_D4  = 0 ,
    LED_D5      ,
    LED_D6      ,
    LED_D7      ,
} LED_Dx;

/************************
 * @brief LED class typedef
 ************************/
typedef struct {
    __IO u32 *REMAP_CCM_CCGRx  ;
    __IO u32 *REMAP_GPIOx_PIN  ;
    __IO u32 *REMAP_GPIOx_DR   ;
    __IO u32 *REMAP_GPIOx_GDIR ;
} led_reg_typedef;

typedef struct led_property_typedef{
    u8 num;
    u8 status;
    u8 conf_status;
    u8 reserve;
    led_reg_typedef reg;
} led_property_typedef;


typedef struct led_ctl_typedef{
    void (*init)    ( u8 which_led );
    void (*ctl)     ( u8 which_led, LED_OPT_DEF opt );
    u8   (*g_stat)  ( u8 which_led );
    void (*s_init)  ( void );
} led_ctl_typedef;

#endif // !__LED_TYPEDEF_H__