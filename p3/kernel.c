#include "hype.h"



void
kernel( unsigned int core )
{
	#include "initf.auto"
	return;
}


extern unsigned int krecv();

void
incoming_kmsg()
{
	unsigned int msg = krecv();
	int id;

	if (MSG_VALID(msg) && (id = MSG_SENDER(msg)) != 0) {
        
		msg = MSG_DATA(msg);
		msg += 1;
		send(id, MSG_DATA(msg) );
	}
    return;
}

