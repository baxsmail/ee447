#include "hype.h"
#include "syscall.h"


unsigned char sys_str[50] = "SystemCall: ";
unsigned char read_str[50] = "----Read";
unsigned char write_str[50] = "----Write";
unsigned char newthread_str[50] = "----NewThread";
unsigned char newproc_str[50] = "----NewProc";
unsigned char null_str[50] = "----Null";
unsigned char error_str[50] = "Invalid operation and error occured";

unsigned char dev_LED[50] = "----LED";
unsigned char dev_MONITOR[50] = "----Monitor";
unsigned char dev_SDCARD[50] = "----SdCard";
unsigned char dev_CLOCKS[50] = "----CLOCK";
unsigned char syscall_str[50] = "en;er syscall";
unsigned char syscallh_str[50] = "enter syscall helper";

unsigned char end_str[50] ="end";

int syscall( int function, int device_ID, char *args, int argsize )
{
    /*
    print_text(syscall_str);
    printdec(args);
    printdec(argsize);
    printdec(device_ID);
    printdec(function);
    print_text(end_str);
    */
    /* re-allign params */
    int retNum = syscall_helper( args, argsize, device_ID, function );

    /* call some print to print Hello World */
    /* the thing from r0 */ 
    return retNum;
}

/*
int syscall_helper( char * buf, int bufsize, int device_ID, int function )
{
    asm("svc #0");
    print_text(syscallh_str);
    printdec( device_ID );
    print_text(end_str);
}
*/


int Sys_Null() {
    print_text(null_str);
    print_text(end_str);
    return 0;
}

int Sys_Read(char *buf, int bufsize, int device_ID) {
    print_text( read_str );
    int retNum = 0;
    switch( device_ID )
    {
        case DEVICE_SDCARD:
        /* TODO */
            print_text( dev_SDCARD );
            retNum = GET32( (int) buf);
            // printdec( retNum );
            break;
        case DEVICE_CLOCKS:
        /* TODO */
            print_text( dev_CLOCKS );
            retNum = GET32(0x4000001C);
            // printdec( retNum );
            break;
        default:
            retNum = Sys_Error();
            break;
    }
    print_text(end_str);
    return retNum;
}

int Sys_Write(char *buf, int bufsize, int device_ID) {
    print_text( write_str );
    switch( device_ID )
    {
        case DEVICE_LED:
        /* TODO
         * PUT32( xxx, buf ); */
            print_text( dev_LED );
            flash_led( 1, GRN | RED , 3 );
            break;
        case DEVICE_MONITOR:
        /* TODO
         * PUT32( xxx, buf ); */
            print_text( dev_MONITOR );
            break;
        case DEVICE_SDCARD:
        /* TODO
         * PUT32( xxx, buf ); */
            print_text( dev_SDCARD );
            break;
        case DEVICE_CLOCKS:
        /* TODO
            PUT32( 0x4000001C, 0 );
            */
            print_text( dev_CLOCKS );
            break;
        default:
            Sys_Error();
            break;
    }
    print_text(end_str);
    return 0;
}

int Sys_NewProc(char *buf, int bufsize) {
    print_text( newproc_str );
    print_text(end_str);
    return 0;
}

int Sys_NewThread(char *buf, int bufsize) {
    print_text( newthread_str );
    print_text(end_str);
    return 0;
}

int Sys_Error() {
    print_text( error_str );
    print_text(end_str);
    return 0;
}
/*
const Trap_Handler Handler_Table[] = {
    Sys_Null,
    Sys_Read,
    Sys_Write,
    Sys_NewProc,
    Sys_NewThread
};

*/
