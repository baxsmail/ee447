.globl _start
_start:
@
ldr pc, res_handler@ need to set up the jump table here
ldr pc, ud_handler@ need to set up the jump table here
ldr pc, swi_handler@ need to set up the jump table here
ldr pc, prefetch_handler@ need to set up the jump table here
ldr pc, data_handler@ need to set up the jump table here
ldr pc, unused_handler@ need to set up the jump table here
ldr pc, irq_handler@ need to set up the jump table here
ldr pc, fiq_handler@ need to set up the jump table here
@

res_handler: .word reset
ud_handler: .word hang
swi_handler: .word hang
prefetch_handler: .word hang
data_handler: .word hang
unused_handler: .word hang
irq_handler: .word hang
fiq_handler: .word fiq

.equ	USR_mode,	0x10
.equ	FIQ_mode,	0x11
.equ	IRQ_mode,	0x12
.equ	SVC_mode,	0x13
.equ	HYP_mode,	0x1A
.equ	SYS_mode,	0x1F
.equ	No_Int,		0xC0


.include "stacks.auto"


reset:

	mrc p15, 0, r0, c1, c0, 0 @ Read System Control Register
@	orr r0, r0, #(1<<2)       @ dcache enable
	orr r0, r0, #(1<<12)      @ icache enable
	and r0, r0, #0xFFFFDFFF   @ turn on vector table at 0x0000000 (bit 12)
	mcr p15, 0, r0, c1, c0, 0 @ Write System Control Register

@ check core ID
.globl do_over
do_over:
	mrc     p15, 0, r0, c0, c0, 5
	ubfx    r0, r0, #0, #2
	cmp     r0, #0					@ is it core 0?
	beq     core0

	@ it is not core0, so do things that are appropriate for SVC level as opposed to HYP
	@ eg., turn on virtual memory, etc.

	@ set up separate stacks for each core
	mrc     p15, 0, r0, c0, c0, 5
	ubfx    r0, r0, #0, #2
	cmp     r0, #1					@ is it core 1?
	beq     core1
	cmp     r0, #2					@ is it core 2?
	beq     core2
	cmp     r0, #3					@ is it core 3?
	beq     hang

	@ CPU ID is not 0..3 - wtf
	b hang

core1:
	mov 	r2, # No_Int | IRQ_mode
	msr		cpsr_c, r2
	mov		sp, # IRQSTACK1
	mov 	r2, # No_Int | FIQ_mode
	msr		cpsr_c, r2
	mov		sp, # FIQSTACK1
	mov 	r2, # No_Int | SVC_mode
	msr		cpsr_c, r2
	mov		sp, # KSTACK1

	mov 	r2, # SYS_mode
	msr		cpsr_c, r2

	mov		sp, # USTACK1
	bl		client1
	b hang

core2:
	mov 	r2, # No_Int | IRQ_mode
	msr		cpsr_c, r2
	mov		sp, # IRQSTACK2
	mov 	r2, # No_Int | FIQ_mode
	msr		cpsr_c, r2
	mov		sp, # FIQSTACK2
	mov 	r2, # No_Int | SVC_mode
	msr		cpsr_c, r2
	mov		sp, # KSTACK2

	mov 	r2, # SYS_mode
	msr		cpsr_c, r2

	mov		sp, # USTACK2
	bl		client2

hang: b hang

core0:
	mov 	r2, # No_Int | IRQ_mode
	msr		cpsr_c, r2
	mov		sp, # IRQSTACK0

	mov 	r2, # No_Int | FIQ_mode
	msr		cpsr_c, r2
	mov		sp, # FIQSTACK0

	mov 	r2, # No_Int | HYP_mode
	msr		cpsr_c, r2
	mov		sp, # KSTACK0

	bl		enable_fiq
	bl		kernel
	b hang

fiq:
    push {r0,r1,r2,r3,r4,r5,r6,r7,lr}
    bl incoming_kmsg
    pop {r0,r1,r2,r3,r4,r5,r6,r7,lr}
    subs pc, lr, #4

