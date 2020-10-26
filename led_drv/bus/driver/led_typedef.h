
#ifndef __LED_TYPEDEF_H__
#define __LED_TYPEDEF_H__


#include "imx6ull_common_inc.h"
#include <asm-generic/int-ll64.h>

/************************
 * @brief define
 ************************/
#define LED_NUM     4

typedef enum LED_OPT_DEF {
    LED_OPT_OFF    = 0,
    LED_OPT_ON        ,
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
    /* GPIO */
    __IO u32  *REMAP_CCM_CCGRx ;
    __IO u32  *REMAP_PAD       ;
    GPIO_Type *REMAP_GPIOx     ;
} led_reg_typedef;

typedef struct {
    u8  status;
    u8  conf_status;
    u32 dr_oft;
    u32 clk_oft;
    led_reg_typedef reg;
} led_property_typedef;


typedef struct {
    void (*init)     ( u8 which_led );
    void (*ctl)      ( u8 which_led, LED_OPT_DEF opt );
    u8   (*g_stat)   ( u8 which_led );
    void (*s_init)   ( void );
    void (*pwm_init) ( u8 which_led, u32 pwm_freq );
    void (*pwm_ctl)  ( u8 which_led, u32 pwm_freq, LED_PWM_OPT_DEF opt );
} led_ctl_typedef;

#endif // !__LED_TYPEDEF_H__