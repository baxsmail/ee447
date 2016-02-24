#include "hype.h"

void
client1(void)
{
	unsigned int now, inmsg, outmsg, diff;

	while (1) 
    {
		oldwait(50);
		now = now_usec();
		outmsg = now & 0x0FFFFFFF;

		do {
			unsigned int backoff = 1;
			while (!send(0, outmsg)) {
				oldwait(backoff);
				backoff *= 2;
			}
            oldwait(50);
            inmsg = recv( USER_TIMEOUT );
		} while ( inmsg == NACK);


		//diff = MSG_DATA(inmsg) - outmsg;
		diff = MSG_DATA(inmsg) - outmsg;
		if (diff == 1) 
        //if( outmsg == MSG_DATA(inmsg) )
        //if ( MSG_DATA(inmsg) == 0x02 )
        {
			blink_led(GRN);
		}
	}
}

void
client2(void)
{
    unsigned int now, inmsg, outmsg, diff;

    while (1) 
    {
        oldwait(50);
        now = now_usec();
        outmsg = now & 0x0FFFFFFF;

        do {
            unsigned int backoff = 1;
            while (!send(0, outmsg)) {
                oldwait(backoff);
                backoff *= 2;
            }
            oldwait(50);
            inmsg = recv( USER_TIMEOUT );
        } while ( inmsg == NACK);


        //diff = MSG_DATA(inmsg) - outmsg;
        diff = MSG_DATA(inmsg) - outmsg;
        if (diff == 1) 
            //if( outmsg == MSG_DATA(inmsg) )
            //if ( MSG_DATA(inmsg) == 0x02 )
        {
            blink_led(RED);
        }
    }
}
