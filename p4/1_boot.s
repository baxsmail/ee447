.globl _start
_start:
    b reset
    b hang
    b hang
    b hang
    b hang
    b hang
    b irq_nop
    b hang


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
	beq     hang
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

@	mov 	r2, # USR_mode
	mov 	r2, # SYS_mode
	msr		cpsr_c, r2

	bl		enable_irq

	mov		sp, # USTACK1
	bl		entry_t0
	b hang

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

	bl		kernel
	b hang


			.word 0
			.word 0
threadsave:	.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0
			.word 0

save_r13_irq: .word 0
save_r14_irq: .word 0



badval:	.word	0xdeadbeef


@
@ based on code from rpi discussion boards
@
irq_nop:
	str		r13, save_r13_irq			@ save the IRQ stack pointer
	ldr		r13, =threadsave			@ load the IRQ stack pointer with address of TCB
	add		r13, r13, #56				@ jump to middle of TCB for store up and store down
	stmia	sp, {sp, lr}^				@ store the USR stack pointer & link register, upwards
	push	{r0-r12, lr}				@ store USR regs 0-12 and IRQ link register (r14), downwards

	@ regs saved, we can now destroy stuff

	@@@
	@@@ clear interrupt if necessary
	@@@
	mov r0, #1
	bl	clear_interrupt

	@ next - this is optional - announce that we're here (works best when interrupts < 100 times per sec)
@	mov		r0, #2
@	bl		blink_led

	@ clobber the user stack - simulates effect of another thread running
	@ clobber the user stack - simulates effect of another thread running
	@ clobber the user stack - simulates effect of another thread running
	mov 	r2, # SYS_mode
	msr		cpsr_c, r2
	ldr		r0,badval
	ldr		r1,badval
	ldr		r2,badval
	ldr		r3,badval
	ldr		r4,badval
	ldr		r5,badval
	ldr		r6,badval
	ldr		r7,badval
	ldr		r8,badval
	ldr		r9,badval
	ldr		r10,badval
	ldr		r11,badval
	ldr		r12,badval
	ldr		r13,badval
	ldr		r14,badval
	mov 	r2, # IRQ_mode
	msr		cpsr_c, r2
	@ clobber the user stack - simulates effect of another thread running
	@ clobber the user stack - simulates effect of another thread running
	@ clobber the user stack - simulates effect of another thread running

	ldr		r13, =threadsave			@ load the IRQ stack pointer with address of TCB
	pop		{r0-r12, lr}				@ load USR regs 0-12 and IRQ link register (r14), upwards
	ldmia	sp, {sp, lr}^				@ load the USR stack pointer & link register, upwards
	nop									@ evidently it's a god idea to put a NOP after a LDMIA
	ldr		r13, save_r13_irq			@ restore the IRQ stack pointer from way above
	subs	pc, lr, #4					@ return from exception

