#include "hype.h"

extern unsigned int interrupt_core( unsigned int );

void
kernel()
{
	unsigned int then, now, delta;

	#include "initf.auto"

	then = now_usec();
	while (1) {
		now = now_usec();
		delta = usec_diff( now, then );
		if (delta > (6 * ONE_SEC)) {
            oldwait(50);
			blink_led( RED );
			then = now;
			interrupt_core(1);
			// try int. core 1 first
			// interrupt_core(2);
		}
	}
}


extern unsigned int krecv();

void
incoming_kmsg()
{
    oldwait(20);
	unsigned int msg = krecv();
    //clear_interrupt(0);
    period_blink();
	int id, thread;
	int swap = now_usec() & 1;

	if (MSG_VALID(msg) && ( MSG_SENDER(msg)) != 0) 
    {
        id = MSG_SENDER(msg);
		msg = MSG_DATA(msg);
		if (REGMSG_STATUS(msg) == CORE_RUN) {
			thread = REGMSG_THREAD(msg);
			if( REGMSG_IPC1000(msg) == 0xcafe )
            {
                oldwait(20);
                blink_led( GRN );
            }
			if (swap) {
				if (thread & 0x1) {
					thread &= 0xfffffffe;
				} else {
					thread |= 0x1;
				}
                thread = 3;
			}
			msg = SET_REGMSG( CORE_RUN, thread, 0 );
		} 
        else 
        {
			msg = SET_REGMSG( CORE_RESET, 0, 0 );
		}
		send(id, msg);
	}
    oldwait(20);
    return;
}

