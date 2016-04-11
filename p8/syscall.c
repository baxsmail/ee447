#include "syscall.h"
#include "hype.h"
/* #include "led.h" */

const Syscall g_syscallTable[] = {
    Sys_Null,
    Sys_WriteLED,
    Sys_ReadLED,
    Sys_WriteSD,
    Sys_ReadSD,
    Sys_WriteClock,
    Sys_ReadClock,
    Sys_NewProc,
    Sys_NewThread,
    Sys_Exit,
};

int Sys_Null()
{
    return SUCCESS;
}

int Sys_WriteLED(int SyscallNum, int address, int value)
{
    return SUCCESS;
}

int Sys_ReadLED(int SyscallNum, int address)
{
    /* not sure if we can directly read from LED */
    return SUCCESS;
}

int Sys_WriteSD(int SyscallNum, int address, int value)
{
    /*
     * some check for safety reason
     * PUT32(address, value);
     */
    return SUCCESS;
}

int Sys_ReadSD(int SyscallNum, int address)
{
    return SUCCESS;
}

int Sys_WriteClock(int SyscallNum, int ClockNum, int value )
{
    /*
     * some check for safety reason
     * PUT32(address, value);
     */
    return SUCCESS;
}

int Sys_ReadClock(int SyscallNum, int ClockNum, int value )
{
    return SUCCESS;
}

int Sys_NewProc(int SyscallNum)
{
    /* dont know if we need other arguments */
    return SUCCESS;
}

int Sys_NewThread(int SyscallNum)
{
    /* dont know if we need other arguments */
    return SUCCESS;
}

int Sys_Exit(int SyscallNum)
{
    return SUCCESS;
}
