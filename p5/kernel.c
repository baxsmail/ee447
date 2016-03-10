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
		if (delta > (2 * ONE_SEC)) {
			then = now;
			blink_led( RED );
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
	unsigned int msg = krecv();
	int id, thread;
	int swap = now_usec() & 2;

	if (MSG_VALID(msg) && (id = MSG_SENDER(msg)) != 0) {
		msg = MSG_DATA(msg);
		blink_led(RED);
		return;

		if (REGMSG_STATUS(msg) == CORE_RUN) {
			thread = REGMSG_THREAD(msg);
			if( REGMSG_IPC1000(msg) == 0xcafe )
				blink_led(RED);
			/*
			if (swap) {
				if (thread & 0x1) {
					thread &= 0xfffffffe;
				} else {
					thread |= 0x1;
				}
			}
			msg = SET_REGMSG( CORE_RUN, thread, 0 );
			*/
		} else {
			/*
			msg = SET_REGMSG( CORE_RESET, 0, 0 );
			*/
		}
		/*
		send(id, msg);
		*/
	}
}

