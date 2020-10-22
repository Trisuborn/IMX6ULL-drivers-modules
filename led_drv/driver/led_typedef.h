
#ifndef __LED_TYPEDEF_H__
#define __LED_TYPEDEF_H__

/*
static ebf6ull_led_init( u8 which_led );
static ebf6ull_led_ctl ( u8 which_led, u8 opt );
*/
#include <asm-generic/int-ll64.h>

typedef struct led_ctl_typedef{
    void (*init)    ( u8 which_led );
    void (*ctl)     ( u8 which_led, u8 opt );    
} led_ctl_typedef;

typedef enum LED_Dx{
    LED_D4  = 0 ,
    LED_D5      ,
    LED_D6      ,
    LED_D7      ,
} LED_Dx;

#endif // !__LED_TYPEDEF_H__