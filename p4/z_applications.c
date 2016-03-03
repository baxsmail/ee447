#include "hype.h"

#define DEBUG 1

int PID;



void	threadX_blinker ( unsigned int color )
{
	unsigned int now, then, num;

	num = 0;
	then = now_usec();

	while (1) {
		now = now_usec();
		if ((now - then) > (1 * ONE_SEC)) {
			then = now;

			flash_led(1, color, num+1);

			num = (num + 1) & 0x3;
		}
	}
}


void	entry_t1 ()
{
	while (1) {
		threadX_blinker(RED);
		flash_lonum(3);
	}
}

void	entry_t0 ()
{
	while (1) {
		threadX_blinker(GRN);
		flash_lonum(3);
	}
}

void scheduler_init()
{
    PID = 0;
    entry_t0();
}

void c_hang()
{
    while(1);
}

int scheduler()
{
    if( PID == 0 )
    {
        PID = 2;
        return 1;
    }
    if( PID == 1 )
    {
        PID = 2;
        return 2;
    }
    if( PID == 2 )
    {
        PID = 1;
        return 3;
    }
    return 4;
}

void blink_on_one()
{
    if( PID == 1 )
        blink_led(RED);
    return;
}
