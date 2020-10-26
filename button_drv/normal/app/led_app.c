
/************************
 * @file led_app.c
 * @author Trisuborn (ttowfive@gmail.com)
 * @brief led app
 * @version 0.1
 * @date 2020-10-21
 * 
 * @copyright Copyright (c) 2020
 * 
 ************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 

static char def_led[] = "LED_D7";
static char def_opt[] = "off";
static char path_prefix[20] = "/dev/";

typedef struct {
    int     param_num;
    char    ledx[8];
    char    ledx_opt[4];
    char    ledx_pwm_param;
} cmd_s;
static cmd_s input_cmd;

typedef enum LED_STAT {
    LED_IS_OFF   = 0,
    LED_IS_ON       ,
    LED_IS_ERR      ,
} LED_STAT;

typedef enum LED_OPT_DEF {
    LED_OPT_OFF    = '0',
    LED_OPT_ON     = '1',
    LED_OPT_PWM    = '2',
} LED_OPT_DEF;

typedef enum {
    PWM_FREQ_32K   = '0',
    PWM_FREQ_100K  = '1',
    PWM_FREQ_200K  = '2',
} PWM_FREQ;

typedef struct {
    unsigned char header;
    unsigned char pwm_freq;       // unit: kHz
} pwm_param_s;
pwm_param_s pwm_param = {
    .header     = LED_OPT_PWM,
    .pwm_freq   = PWM_FREQ_100K,
};

LED_STAT get_led_status( void )
{
    int fd = -1;
    strcat( path_prefix, input_cmd.ledx );
    fd = open( path_prefix, O_RDONLY );
    if ( fd == -1 )
        return -1;
    
    LED_STAT status = LED_IS_ERR;
    read( fd, &status, 1 );
    close(fd);

    if ( status == LED_IS_ON ) 
        printf( "%s is on.\n", input_cmd.ledx );
    if ( status == LED_IS_OFF ) 
        printf( "%s is off.\n", input_cmd.ledx );
    else if ( status == LED_IS_ERR )
        printf( "%s status(%d) get error.\n", input_cmd.ledx, status );

    return status;
}

int analysis_cmd( void ) 
{

    if ( input_cmd.param_num < 2 )
        strcpy( input_cmd.ledx, def_led );
    if ( input_cmd.param_num < 3 )
        strcpy( input_cmd.ledx_opt, def_opt );

    char status = 0;
    if ( 0 == strcmp(input_cmd.ledx_opt, "on") ) {
		status = 1;
	} else if ( 0 == strcmp(input_cmd.ledx_opt, "off") ) {
		status = 0;
	} 

    printf( "%s %s\n", input_cmd.ledx, (status)?("on"):("off") );

    int fd = 0;
    strcat( path_prefix, input_cmd.ledx );
    fd = open( path_prefix, O_RDWR );
    if ( fd == -1 ) {
        printf( "open %s error.\n", input_cmd.ledx );
        return -1;
    }
    write(fd, &status, 1);
    close( fd );

    return 0;
}

void led_set_pwm( void ) 
{
    int fd = 0;
    strcat( path_prefix, input_cmd.ledx );
    fd = open( path_prefix, O_RDWR );
    if ( fd == -1 ) {
        printf( "open %s error.\n", input_cmd.ledx );
        return;
    }

    unsigned char param_buf[2];
    param_buf[0] = LED_OPT_PWM;
    param_buf[1] = input_cmd.ledx_pwm_param;
    write(fd, param_buf, 2);
    close( fd );
}

int main( int argc, char **args )
{
    input_cmd.param_num = argc;
    if ( input_cmd.param_num > 4 )
        return -1;
    if ( input_cmd.param_num >= 2 ){
        if ( !strcmp(args[1], "-h") || (!strcmp( args[1], "-help" )) ) {
            printf( "Usage_1: ./led_app {LED_D4...LED_D7} {on | off}\n" );
            printf( "Usage_2: ./led_app {LED_D4...LED_D7} stat\n" );
            return 0;
        }
        strcpy( input_cmd.ledx, args[1] );
    }
    if ( input_cmd.param_num >= 3 ) {
        if ( !strcmp(args[2], "stat") ) {
            return get_led_status();
        }
        if ( !strcmp(args[2], "pwm") ) {
            if ( input_cmd.param_num == 4 )
                input_cmd.ledx_pwm_param = *args[3];
            else 
                input_cmd.ledx_pwm_param = PWM_FREQ_100K;
            led_set_pwm();
            return 0;
        }
        strcpy( input_cmd.ledx_opt, args[2] );
    }

    return analysis_cmd();
}
