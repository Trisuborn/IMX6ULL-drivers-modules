
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

// #include "led_typedef.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


int i = 0;

int main( int argc, char **args )
{
    
    printf( "Hello %s\n", (args[1]==NULL)?("Trisuborn"):(args[1]) );
    
    int fd = 0;
 
    fd = open( args[1], O_RDWR );
    if ( fd == -1 ) {
        printf( "open %s error.\n", args[1] );
        return -1;
    }
    printf( "open %s successfully.\n", args[1] );

    write( fd, "Hello", 1 );


    close( fd );
    
    return 0;
}
