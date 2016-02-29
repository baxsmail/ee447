#include "hype.h"

unsigned int
ukernel_status(void)
{
	unsigned int inmsg, outmsg;

	outmsg = SET_REGMSG( CORE_RUN, current_thread, 0xcafe );

	do {
		unsigned int backoff = 1;
		while (!send(0, outmsg)) {
			oldwait(backoff);
			backoff *= 2;
		}
	} while (( inmsg = recv(USER_TIMEOUT) ) == NACK);

	//
	// do whatever the kernel says to do:
	//
	// CORE_RESTART -- only in error situations (optional extra credit)
	// CORE_RUN -- specific thread number
	// 

}
