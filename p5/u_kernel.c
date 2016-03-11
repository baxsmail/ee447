#include "hype.h"

static int PID[3] = {0,1,2};
void ukernel_init()
{
    PID[0] = 4;
    PID[1] = 1;
    PID[2] = 2;
}

void ukernel_start()
{
    if(PID[cpu_id()] == 1 )
        entry_t1();
    if(PID[cpu_id()] == 2 )
        entry_t2();
}

int
ukernel_scheduler()
{
    return PID[cpu_id()] + PID[0];
}

unsigned int
ukernel_status(void)
{
	unsigned int inmsg, outmsg, current_thread;
	current_thread = PID[cpu_id()] & 0x03;

	outmsg = SET_REGMSG( CORE_RUN, current_thread, 0xcafe ) ;

    do
	{
		unsigned int backoff = 1;
		//while (!send(0, outmsg)) {
		send(0, outmsg); 
        /*
        {
			oldwait(backoff);
			backoff *= 2;
		}
        */
        inmsg = recv( USER_TIMEOUT );
	} while ( inmsg == NACK);
    unsigned int new_thread = REGMSG_THREAD(inmsg);
    if( REGMSG_STATUS(inmsg) == CORE_RUN )
    {
        if( new_thread != current_thread )
        {
            if( new_thread != cpu_id() )
            {
            //save this & load that
            }
            else
            {
            //save that & load this
            }
            while(1)
                blink_led(RED);
            PID[0] = 0;
        }
        else
        {
            while(1)
                blink_led(GRN);
        }
    }

    oldwait(20);
	return 0;

	//
	// do whatever the kernel says to do:
	//
	// CORE_RESTART -- only in error situations (optional extra credit)
	// CORE_RUN -- specific thread number
	// 

}
