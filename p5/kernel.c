#include "hype.h"

extern unsigned int interrupt_core( unsigned int );

void
kernel()
{
	volatile unsigned int then, now, delta;

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
    return;
}



extern unsigned int krecv();

void
incoming_kmsg()
{
    oldwait(50);
    PUT32(0x400000C0, 0x1);
    PUT32(0x400000C4, 0x1);
    PUT32(0x400000C8, 0x1);
    PUT32(0x400000CC, 0x1);

    PUT32(0x400000D0, 0x1);
    PUT32(0x400000D4, 0x1);
    PUT32(0x400000D8, 0x1);
    PUT32(0x400000DC, 0x1);

    PUT32(0x400000E0, 0x1);
    PUT32(0x400000E4, 0x1);
    PUT32(0x400000E8, 0x1);
    PUT32(0x400000EC, 0x1);

    PUT32(0x400000F0, 0x1);
    PUT32(0x400000F4, 0x1);
    PUT32(0x400000F8, 0x1);
    PUT32(0x400000FC, 0x1);

    blink_led(0x03);
    return;
	volatile unsigned int msg = krecv();
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

