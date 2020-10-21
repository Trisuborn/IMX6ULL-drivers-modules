
/************************
 * @file hello_app.c
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

#define APP_BUF_LEN 512

static int fd = 0;
static char app_buf[APP_BUF_LEN] = {0};
static const char *dev_def_path = "/dev/hello"; 
static const char *def_text     = "Hello world !"; 

typedef struct {
    int         cmd_num ;
    const char *cmd_opt ;
    const char *dev_path;
    const char *user_str;
} cmd_s;
cmd_s input_cmd = {
    .cmd_num    = 0,
    .cmd_opt    = NULL,
    .dev_path   = NULL,
    .user_str   = NULL,
};

typedef enum {
    HELLO_WRIT  = 0,
    HELLO_READ     ,
    HELLO_HELP     ,
    HELLO_ERR      ,
} HELLO_OPT_ENUM;



/************************
 * @brief analysis command input.
 * 
 * @param void 
 * @return HELLO_OPT_ENUM 
 ************************/
HELLO_OPT_ENUM analysis_cmd( void ) 
{
    HELLO_OPT_ENUM flag = HELLO_HELP;


    if ( input_cmd.cmd_num == 1 ) {
        input_cmd.cmd_opt  = "-h";
    } else if ( strcmp( "-r", input_cmd.cmd_opt ) == 0 ) {
        input_cmd.cmd_opt  = "-r";
        flag = HELLO_READ;
    } else if ( strcmp( "-w", input_cmd.cmd_opt ) == 0 ) {
        input_cmd.cmd_opt  = "-w";
        flag = HELLO_WRIT;
    } else if ( strcmp( "-h", input_cmd.cmd_opt ) == 0 ) {
        input_cmd.cmd_opt  = "-h";
        flag = HELLO_HELP;
    } else if ( input_cmd.cmd_opt == NULL || (input_cmd.cmd_num < 3) ) {
        input_cmd.cmd_opt  = "-h";
        flag = HELLO_HELP;
    }
        
    if ( input_cmd.dev_path == NULL || (input_cmd.cmd_num < 3) )
        input_cmd.dev_path = dev_def_path; 
    if ( input_cmd.user_str == NULL || (input_cmd.cmd_num < 3) )
        input_cmd.user_str = def_text;

    return flag;
}

/************************
 * @brief inplement command input.
 * 
 * @param which_opt 
 ************************/
int implement_cmd( HELLO_OPT_ENUM which_opt )
{

    if ( which_opt == HELLO_ERR )
        return -1;

    fd = open( input_cmd.dev_path, O_RDWR );
    if ( fd == -1 ) {
        printf( "open %p error.\n", NULL );
        return -1;
    }

    switch (which_opt) {
    case HELLO_WRIT:
        write( fd, input_cmd.user_str, strlen(input_cmd.user_str) );
        printf( "write: \"%s\" done.\n", input_cmd.user_str );
        break;
    case HELLO_READ:
        read( fd, app_buf, APP_BUF_LEN );
        printf( "read : \"%s\"\n", app_buf );
        break;
    case HELLO_HELP:
        printf( "Usage: %s ( -w | -r ) dev string\n", "./hello_app" );
        break;
    default:
    case HELLO_ERR:
        close(fd);
        return -1;
        break;
    }

    close(fd);
    return 0;
}

int main(int argc, char const *argv[])
{
    input_cmd.cmd_num  = argc   ;
    if ( input_cmd.cmd_num == 1 ) {
        implement_cmd(analysis_cmd());
    } else {
        if ( argc == 2 )
            input_cmd.cmd_opt  = argv[1];
        if ( argc == 3 )
            input_cmd.dev_path = argv[2];
        if ( argc == 4 )
            input_cmd.user_str = argv[3];
        implement_cmd(analysis_cmd());
    }

    return 0;
}

