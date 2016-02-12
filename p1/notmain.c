//
// hello, world!!!
//

#include "os.h"


extern void init_timeoutq( void );
extern int bring_timeoutq_current( void );
extern void create_timeoutq_event(int timeout, int repeat, pfv_t function, unsigned int data);
extern int handle_timeoutq_event( void );


void
notmain()
{
	int timeout;

	//flash_led(1, RED|GRN, 4);

	init_timeoutq();

	create_timeoutq_event( ONE_SEC, 2 * ONE_SEC, blink_led, RED );
	//create_timeoutq_event( 6 * ONE_SEC, 2 * ONE_SEC, blink_led, GRN );
	//create_timeoutq_event( 11 * ONE_SEC + 500 * ONE_MSEC, ONE_SEC, blink_led, GRN | RED);

	/*
	while (1) 
	{
		if (handle_timeoutq_event()) {
			continue;
		}
		timeout = bring_timeoutq_current();
		//TODO
		//wait(timeout);
	}
	*/

}
