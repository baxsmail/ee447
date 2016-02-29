/*
 * IPC
 */

#include "hype.h"


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
	PUT32(0x40000054, INT_IRQ);		// mboxes 1..3 interrupt via IRQ
	PUT32(0x40000058, INT_IRQ);
	PUT32(0x4000005C, INT_IRQ);

	return;
}

