#include "syscall.h"
#include "hype.h"

int syscall( int function, int device_ID, char *args, int argsize )
{
    /* re-allign params */
    int retNum = syscall_helper( args, argsize, device_ID, function );

    /* call some print to print Hello World */
    /* the thing from r0 */ 
    return retNum;
}

int syscall_helper( char * buf, int bufsize, int device_ID, int function )
{
    asm("svc #0");
}

/* TODO : Within swi_handler, b [Handler_Table,+#4]
 */

int Sys_Null(char *buf, int bufsize, int device_ID) {

    return 0;
}

int Sys_Read(char *buf, int bufsize, int device_ID) {
    int retNum = 0;
    switch( device_ID )
    {
        case DEVICE_SDCARD:
        /* TODO
            retNum = GET32();
            */
            break;
        case DEVICE_CLOCKS:
        /* TODO
            retNum = GET32();
            */
            break;
        default:
            break;
    }
    return retNum;
}

int Sys_Write(char *buf, int bufsize, int device_ID) {
    switch( device_ID )
    {
        case DEVICE_LED:
        /* TODO
         * PUT32( xxx, buf ); */
            break;
        case DEVICE_MONITOR:
        /* TODO
         * PUT32( xxx, buf ); */
            break;
        case DEVICE_SDCARD:
        /* TODO
         * PUT32( xxx, buf ); */
            break;
        case DEVICE_CLOCKS:
        /* TODO
         * PUT32( xxx, buf ); */
            break;
        default:
            break;
    }
    return 0;
}

int Sys_NewProc(char *buf, int bufsize, int device_ID) {
    return 0;
}

int Sys_NewThread(char *buf, int bufsize, int device_ID) {
    return 0;
}

const Trap_Handler Handler_Table[] = {
    Sys_Null,
    Sys_Read,
    Sys_Write,
    Sys_NewProc,
    Sys_NewThread
};


