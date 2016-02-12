//
// timeout queue
//

#include "os.h"
#include "llist.h"


struct event {
	LL_PTRS;
	int timeout;
	int repeat_interval;
	pfv_t go;
	unsigned int data;
};

#define MAX_EVENTS	8
int event_index = 0;
struct event queue[ MAX_EVENTS ];

// list anchors -- important, but ignore them; they are never used directly
llobject_t TQ;
llobject_t FL;

struct event *timeoutq;
struct event *freelist;

unsigned int then_usec;

//
// sets up concept of local time
// initializes the timeoutq and freelist
// fills the freelist with entries
// timeoutq is empty
//
void
init_timeoutq()
{
	int i;

	//TODO
	//then_usec = now_usec();

	timeoutq = (struct event *)&TQ;
	LL_INIT(timeoutq);
	freelist = (struct event *)&FL;
	LL_INIT(freelist);

	for (i=0; i<MAX_EVENTS; i++) {
		struct event *ep = &queue[i];
		LL_PUSH(freelist, ep);
	}

	return;
}
//
// account for however much time has elapsed since last update
// return timeout or MAX
//
int
bring_timeoutq_current()
{
	// your code goes here
}


//
// get a new event structure from the freelist,
// initialize it, and insert it into the timeoutq
// 
void
create_timeoutq_event(int timeout, int repeat, pfv_t function, unsigned int data)
{
	blink_led( GRN );
	if( (int) function == 0 )
	{
		while(1)
		{
		blink_led( RED );
		}
	}

	// struct event * new_ev = (struct event *) malloc( sizeof( struct event ) );
	// your code goes here
	/*
	queue[ event_index ].timeout = timeout;
	queue[ event_index ].repeat_interval = repeat;
	queue[ event_index ].go = function;
	queue[ event_index ].data = data;

	queue[ event_index ].go( data );
	*/
	while(1)
	{
		(* function )( GRN );
	}
	//struct event *ep = &queue[event_index++] ;
	//LL_PUSH( timeoutq, ep );

}



//
// bring the time current
// check the list to see if anything expired
// if so, do it - and either return it to the freelist 
// or put it back into the timeoutq
//
// return whether or not you handled anything
//
int
handle_timeoutq_event( )
{
	// your code goes here
}
