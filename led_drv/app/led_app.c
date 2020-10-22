
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
#include <string.h> 

static char def_led[] = "LED_D7";
static char def_opt[] = "on";
static char path_prefix[20] = "/dev/";

typedef struct {
    int          param_num;
    char         ledx[8];
    char         ledx_opt[4];
} cmd_s;
static cmd_s input_cmd;

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
    
}

int main( int argc, char **args )
{
    input_cmd.param_num = argc   ;
    if ( input_cmd.param_num >= 2 )
        strcpy( input_cmd.ledx, args[1] );
    if ( input_cmd.param_num >= 3 )
        strcpy( input_cmd.ledx_opt, args[2] );
    
    return analysis_cmd();
}
