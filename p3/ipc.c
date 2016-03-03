/*
 * IPC
 */

#include "hype.h"


#define	VALIDBIT	0x80000000
#define	IDMASK		0x30000000
#define	MSGBITS		0x0FFFFFFF

#define CUR_BOX_NUM 0
#define P( x ) while( x == 0 ); x = 0
#define V( x ) x = 1
#define IS_BOX_BUSY( x ) ( ( mailbox_state >> ( (x)*4 + CUR_BOX_NUM ) ) & 0x01 )
#define SET_BOX_BUSY( x ) P( mutex ) ; mailbox_state = mailbox_state | ( 0x01 << ( (x)*4 + CUR_BOX_NUM ) ); V( mutex )
#define CLR_BOX_BUSY( x ) P( mutex ) ; mailbox_state = mailbox_state ^ ( 0x01 << ( (x)*4 + CUR_BOX_NUM ) ); V( mutex )

/* set bits - write-only */
#define INT_SET_BASE			0x40000080
/* clear bits - read/write */
#define INT_CLR_BASE			0x400000C0

static unsigned int mailbox_state = 0;
static unsigned int mutex = 1;
void 
write_in_core( unsigned int core, unsigned int msg )
{
	PUT32(INT_SET_BASE + ((core & 0xf) << 4), msg);
}

unsigned int
read_mailbox_in_core( unsigned int num_box, unsigned int core )
{
    unsigned result = 
    GET32( INT_CLR_BASE + 
         ( 
            ( ( core & 0xf ) << 4 ) 
            | 
            ( ( num_box & 0xf ) << 2 ) 
         ) );
    return result;
}
void
interrupt_core( unsigned int core )
{
	PUT32(INT_SET_BASE + ((core & 0xf) << 4), 0x1);
}

void
clear_interrupt( unsigned int core )
{
	PUT32(INT_CLR_BASE + ((core & 0xf) << 4), 0xFFFFFFFF);
}

//
// sets the top bit (bit 31) of the outgoing message 					(VALIDBIT)
// sets the sender ID, determined by calling cpu_id(), in bits 28-29	(IDMASK)
// puts the user's message into bits 0-27 								(MSGBITS)
// returns a Boolean 
// 0 - msg not delivered (if dest MBOX is full)		[kernel ignores this]
// 1 - otherwise
//
unsigned int
send( unsigned int dest, unsigned int msg )
{
	// your code goes here
    unsigned int s_msg = ( 0x80000000 | ( cpu_id()  << 28 ) );
    msg = msg & 0x0FFFFFFF ;
    s_msg = s_msg | msg;
    unsigned int ret_bool = 1;


    /* mail box stuff begin */
    if( IS_BOX_BUSY( dest ) == 1 )
    {
        ret_bool = 0;
    }
    else
    {
        write_in_core( dest, s_msg );
        SET_BOX_BUSY( dest );
        /*
        if( IS_BOX_BUSY( dest ) == 1 )
        {
			blink_led(GRN);
            oldwait(100);
        }
        */
    }
    /* mail box stuff end */

    return ret_bool;
}

//
// returns the message received in MBOX
// return NACK (zero) if a timeout occurs - i.e., if
// time expires and there is still no valid message to read
//
unsigned int
recv( unsigned int timeout )
{
    //return read_mailbox_in_core( 0, cpu_id() ); 

	// your code goes here
    unsigned int result = NACK;
    unsigned int time_remain = timeout;
    /* mail box stuff begin */
    do 
    {
        //if( IS_BOX_BUSY ( cpu_id() ) == 1 )
        {
            result = read_mailbox_in_core( CUR_BOX_NUM, cpu_id() ); 
            clear_interrupt( cpu_id() );
            CLR_BOX_BUSY( cpu_id() );
            return result;
        }
        //oldwait(1);
        //time_remain--;
    }while( time_remain > 0 );
    /* mail box stuff end */

    return ( result );
}

//
// this is what the kernel code can call instead of recv, because it has no 
// timeout, and you don't care if it returns a valid message or not
// ... if you think it's simpler, you can just have one recv() function 
// that both kernel and apps call ... for me, this was easier
//
unsigned int
krecv( )
{
	// your code goes here
    unsigned int ret = read_mailbox_in_core( CUR_BOX_NUM, cpu_id() ) ;
    clear_interrupt( 0 ); //cpu_id() );
    if( IS_BOX_BUSY ( cpu_id() ) )
        CLR_BOX_BUSY( cpu_id() );
    return ret;
}

void _init_ipc()
{
	unsigned int i;
    mailbox_state = 0;
    mutex = 1;

	for (i=0; i<NUM_CORES; i++) {
		// clear the mailbox
	}

	#define INT_IRQ	0x0F
	#define INT_FIQ	0xF0
	#define INT_NONE 0

	// mailboxes & interrupts
	PUT32(0x40000050, INT_FIQ);		// mbox 0 interrupts by FIQ; note: IRQ used by timer
	PUT32(0x40000054, INT_NONE);	// mboxes 1..3 *do not* interrupt, for now
	PUT32(0x40000058, INT_NONE);
	PUT32(0x4000005C, INT_NONE);

	return;
}

