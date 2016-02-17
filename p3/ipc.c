/*
 * IPC
 */

#include "hype.h"


#define	VALIDBIT	0x80000000
#define	IDMASK		0x30000000
#define	MSGBITS		0x0FFFFFFF

/* set bits - write-only */
#define INT_SET_BASE			0x40000080
/* clear bits - read/write */
#define INT_CLR_BASE			0x400000C0

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
}

//
// returns the message received in MBOX
// return NACK (zero) if a timeout occurs - i.e., if
// time expires and there is still no valid message to read
//
unsigned int
recv( unsigned int timeout )
{
	// your code goes here
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
}

void _init_ipc()
{
	unsigned int i;

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

