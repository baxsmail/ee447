#include "syscall.h"
#include "hype.h"

int syscall( int function, int device_ID, char *args, int argsize )
{
    /* TODO : some assembly to re-allign params */

    /* call some print to print Hello World */
    return /* the thing from r0 */ 0;
}

/* TODO : Within swi_handler, b [Handler_Table,+#4]
 */

int Sys_Null(char *buf, int bufsize, int device_ID) {
    return 0;
}

int Sys_Read(char *buf, int bufsize, int device_ID) {
    int rc = 0;
    switch( device_ID )
    {
        case DEVICE_SDCARD:
        /* TODO
            rc = GET32();
            */
            break;
        case DEVICE_CLOCKS:
        /* TODO
            rc = GET32();
            */
            break;
    }
    return rc;
}

int Sys_Write(char *buf, int bufsize, int device_ID) {
    int rc = 0;
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


