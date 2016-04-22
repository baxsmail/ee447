#include "hype.h"
#include "syscall.h"
#include "swi_handler.h"

unsigned char swi_str[50] = "swi_handler in c\n";
unsigned char nondev_str[50] = "Nondev\n";
unsigned char dev_str[50] = "Dev\n";
int swi_handler( char * buf, int bufsize, int device_ID, int function_ID )
{
    /*
    asm("bl printdec");
    
    asm("mov r0,r1");
    asm("bl printdec");

    asm("mov r0,r2");
    asm("bl printdec");

    asm("mov r0,r3");
    asm("bl printdec");

    asm("mov r0,r4");
    asm("bl printdec");

    asm("mov r0,r5");
    asm("bl printdec");

    asm("mov r0,r6");
    asm("bl printdec");

    asm("mov r0,r7");
    asm("bl printdec");

    print_text( swi_str );
    printdec(buf);
    printdec(bufsize);
    printdec(device_ID);
    printdec(function_ID);
    */
    if( device_ID == DEVICE_MAX )
    {
        return Sys_Error();
    }
    else if( device_ID == DEVICE_NULL )
    {
        return nondev_handler(buf, bufsize,device_ID, function_ID);
    }
    else
    {
        return dev_handler(buf, bufsize,device_ID, function_ID);
    }
}

int nondev_handler( char * buf, int bufsize, int device_ID, int function_ID )
{
    print_text( nondev_str );
    switch ( function_ID )
    {
        case SYSCALL_NULL :
            return Sys_Null();
            break;
        case SYSCALL_NEWPROC :
            return Sys_NewProc(buf, bufsize);
            break;
        case SYSCALL_NEWTHREAD :
            return Sys_NewThread(buf, bufsize);
            break;
        default:
            return Sys_Error();
            break;
    }
}

int dev_handler( char * buf, int bufsize, int device_ID, int function_ID )
{
    print_text( dev_str );
    switch( function_ID )
    {
        case DEVCALL_READ :
            return Sys_Read( buf, bufsize, device_ID );
            break;
        case DEVCALL_WRITE :
            return Sys_Write( buf, bufsize, device_ID );
            break;
        case DEVCALL_NULL :
            return Sys_Null();
            break;
        default :
            return Sys_Error();
            break;
    }
}
