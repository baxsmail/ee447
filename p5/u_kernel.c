#include "hype.h"

unsigned int
ukernel_status(void)
{
	unsigned int inmsg, outmsg, current_thread;
	current_thread = cpu_id() & 0x03;

	outmsg = SET_REGMSG( CORE_RUN, current_thread, 0xcafe ) ;

	{
		unsigned int backoff = 1;
		while (!send(0, outmsg)) {
			oldwait(backoff);
			backoff *= 2;
		}
	} //while (( inmsg = recv(USER_TIMEOUT) ) == NACK);
	return 0;

	//
	// do whatever the kernel says to do:
	//
	// CORE_RESTART -- only in error situations (optional extra credit)
	// CORE_RUN -- specific thread number
	// 

}
