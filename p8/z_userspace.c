#include "hype.h"
#include "user_api.h"
#include "syscall.h"

void userspace()
{   
    oldwait(2);
    flash_led(1, RED, 4);
    flash_led(1, GRN, 4);
    read( DEVICE_CLOCKS, 0, NULL, 15 );
    oldwait(2);
    write( DEVICE_SDCARD, 0, NULL, 0 );
    oldwait(2);
    new_process( NULL );
    oldwait(2);
    new_thread( NULL );
    oldwait(2);
    
}
