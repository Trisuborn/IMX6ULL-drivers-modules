#ifndef HELLO_DEV_H
#define HELLO_DEV_H

#include <linux/platform_device.h>

#define HELLO_MOD_NAME "hello_world_mod"

/* 两个字节 
*  高字节代表GPIOx
*  低字节代表PINx
*/
#define GPIOx_PINx( gpiox, pinx )       ( (gpiox&0xFF)<<8 | (pinx&0xFF) )
#define GPIOx_GET( gpiox_pinx )         ( (gpiox_pinx>>8)&0xFF )
#define PINx_GET( gpiox_pinx )          ( gpiox_pinx&0xFF )

#endif // !HELLO_H

