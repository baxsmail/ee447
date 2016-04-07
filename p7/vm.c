#include "hype.h"

//
// Your task: map it so that led.c works ... 
// it is currently using these two sets of addresses:
//
// #define GPFSEL3 0x3F20000C
// #define GPFSEL4 0x3F200010
// #define GPSET1  0x3F200020
// #define GPCLR1  0x3F20002C
//
// #define V_GPFSEL3 0x0020000C
// #define V_GPFSEL4 0x00200010
// #define V_GPSET1  0x00200020
// #define V_GPCLR1  0x0020002C
//
// ... and only the ones that go to 0x3F2xxxxx work. The "V_" addresses
// do not currently work ... they are going to "virtual" locations that
// end up having nothing to do with the GPIOs.
//
// You are to use virtual memory to map the following:
//
//	0x002xxxxx -> 0x3F2xxxxx	(so the GPIOs work)
//	0x3F2xxxxx -> 0x3F2xxxxx	(so the GPIOs work)
//	0x400xxxxx -> 0x400xxxxx	(so the clock & timer accesses work)
//
// and you'll also want to map this, so that your code & data still work:
//
//	0x000xxxxx -> 0x000xxxxx
//


#define	PT_START	0x00100000
#define PTE_SIZE 4096
unsigned int *pagetable;


//
// sets up a mapping in the page table between the VPN and PFN, which are
// represented by the vaddr and paddr values
// note that io addresses need to have different information in the PTE
//
void	map( unsigned int v_index, unsigned int pa_index, int io_addr )
{
    int domain = 0;
    if( v_index > 128 )
        domain = 1;
    // I don't find io_addr useful for any configuration lol
    unsigned int entry = 0;
    entry |= pa_index << 20 ;
    // a section
    entry |= 2; 
    // enable r/w permission
    entry |= 3 << 10;
    // domain configure
    entry |= domain << 5;
    pagetable[ v_index ] = entry ;

}


// turns on the system
void	enable_vm()
{
    map(0x000,0x000,0x000);
    map(0x3f2,0x3f2,0x000);
    map(0x002,0x3f2,0x000);
    map(0x400,0x400,0x000);

    // writeDACR
    // set domain 0 to manager instead of client (lol)
    writeDACR( 0xf );
    //asm volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (3));

    // writeTTBCR
    // domain 0 and 1 in manager
    writeTTBCR( 7 );

    // set TTBR0 (page table walk non-cacheable, shareable memory)
    writeTTBR0( 2 | PT_START );
    writeTTBR1( 2 | PT_START );
    //asm volatile ("mcr p15, 0, %0, c2, c0, 0" :: "r" (2 | PT_START ));

    // enable MMU
	unsigned int temp;
	temp = readSCTLR();
	temp |= 0x01;
    writeSCTLR( temp );
}

void map_reset( unsigned int v )
{
    pagetable[ v ] = 0;
}

// first set everything to zeroes
void	initialize_table()
{
    unsigned int i ;
    for( i = 0; i < PTE_SIZE ; i ++ )
    {
        // init some 0s 
        map_reset(i);
    }
}


// this is actually run by core0 at startup
void	_init_vm()
{
	pagetable = (unsigned int *)PT_START;
	initialize_table();
}
