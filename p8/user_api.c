#include "user_api.h"
#include "syscall.h"

int read (unsigned int device_type, int id, char *buf, int len)
{
    int rc = -1;
    switch( device_type )
    {
        case DEVICE_SDCARD:
        case DEVICE_CLOCKS:
            rc = syscall( DEVCALL_READ, device_type, buf, len );
            break;
        case DEVICE_NULL:
            rc = 0;
            break;
        default:
            Uerror("Invalid Read\n");
            break;
    }
    return rc;
}

int write (unsigned int device_type, int id, char *buf, int len)
{
    int rc = 0;
    switch( device_type )
    {
        case DEVICE_NULL:
            break;
        default:
            rc = syscall( DEVCALL_WRITE, device_type, buf, len );
            break;
    }
    return rc;
}

int new_process (char *executable)
{
    int rc = 0;
    rc = syscall( SYSCALL_NEWPROC, DEVICE_NULL, executable, /* nil */ 0 );
    return rc;
}

int new_thread (pfv_t entrypoint)
{
    int rc = 0;
    rc = syscall( SYSCALL_NEWTHREAD, DEVICE_NULL, (int) entrypoint, /* nil */ 0 );
    return rc;
}

void yield()
{
    /* no idead what is this about */
}

void Uerror( char* str)
{
    /* print to screen */
}
