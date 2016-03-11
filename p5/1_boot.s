.equ    FIQSTACK3 , 21504
.equ    IRQSTACK3 , 22528
.equ    KSTACK3   , 38912
.equ    USTACK3   , 71680
.equ    FIQSTACK2 , 21248
.equ    IRQSTACK2 , 22272
.equ    KSTACK2   , 34816
.equ    USTACK2   , 63488
.equ    FIQSTACK1 , 20992
.equ    IRQSTACK1 , 22016
.equ    KSTACK1   , 30720
.equ    USTACK1   , 55296
.equ    FIQSTACK0 , 20736
.equ    IRQSTACK0 , 21760
.equ    KSTACK0   , 26624
.equ    USTACK0   , 47104

.equ	USR_mode,	0x10
.equ	FIQ_mode,	0x11
.equ	IRQ_mode,	0x12
.equ	SVC_mode,	0x13
.equ	HYP_mode,	0x1A
.equ	SYS_mode,	0x1F
.equ	No_Int,		0xC0


.globl _start
_start:
ldr pc, res_handler@ need to set up the jump table here
ldr pc, ud_handler@ need to set up the jump table here
ldr pc, swi_handler@ need to set up the jump table here
ldr pc, prefetch_handler@ need to set up the jump table here
ldr pc, data_handler@ need to set up the jump table here
ldr pc, unused_handler@ need to set up the jump table here
ldr pc, irq_handler@ need to set up the jump table here
ldr pc, fiq_handler@ need to set up the jump table here

res_handler: .word reset
ud_handler: .word hang
swi_handler: .word hang
prefetch_handler: .word hang
data_handler: .word hang
unused_handler: .word hang
irq_handler: .word irq
fiq_handler: .word fiq
 

reset:

	mrc p15, 0, r0, c1, c0, 0 @ Read System Control Register
@	orr r0, r0, #(1<<2)       @ dcache enable
	orr r0, r0, #(1<<12)      @ icache enable
	and r0, r0, #0xFFFFDFFF   @ turn on vector table at 0x0000000 (bit 12)
	mcr p15, 0, r0, c1, c0, 0 @ Write System Control Register

@ check core ID
	mrc     p15, 0, r0, c0, c0, 5
	ubfx    r0, r0, #0, #2
	cmp     r0, #0					@ is it core 0?
	beq     core0

	@ it is not core0, so do things that are appropriate for SVC level as opposed to HYP
	@ like set up separate stacks for each core, etc.

	mrc     p15, 0, r0, c0, c0, 5
	ubfx    r0, r0, #0, #2
	cmp     r0, #1					@ is it core 1?
	beq     core1
	cmp     r0, #2					@ is it core 2?
	beq     hang
	cmp     r0, #3					@ is it core 3?
	beq     hang

	@ CPU ID is not 0..3 - huh?
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

	bl		enable_irq

	mov 	r2, # USR_mode
	msr		cpsr_c, r2
	mov		sp, # USTACK2
    bl ukernel_start
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

	bl		enable_irq

	mov 	r2, # USR_mode
	msr		cpsr_c, r2
	mov		sp, # USTACK1
    bl ukernel_start
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

	bl 		enable_fiq
	bl		kernel
	b hang

irq:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
	bl cpu_id
	bl clear_interrupt
	bl ukernel_status
    pop {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc, lr, #4

save_lr1_fiq: .word 0
save_lr2_fiq: .word 0

fiq:
    str lr, save_lr1_fiq
    push {r0,r1,r2,r3,r4,r5,r6,r7,lr}
    bl incoming_kmsg
    pop {r0,r1,r2,r3,r4,r5,r6,r7,lr}
    ldr lr,save_lr1_fiq
    @ldr lr, = kernel
    subs pc, lr, #4

SWITCH_HDL:
    bl ukernel_scheduler
    cmp r0,#0
    bne ELSE1
    b save_1_run_0

ELSE1:
    cmp r0,#1
    bne ELSE2
    b save_0_run_1

ELSE2:
    cmp r0,#2
    bne ELSE3
    b save_3_run_2

ELSE3:
    cmp r0,#3
    bne ELSE4
    b save_2_run_3

ELSE4:
    cmp r0,#5
    bne ELSE5
    b start_0

ELSE5:
    b start_3

save_1_run_0:
    str		r13, save_r13_1			
	ldr		r13, =threadsave_1			@ load the IRQ stack pointer with address of TCB
	add		r13, r13, #56				@ jump to middle of TCB for store up and store down
	stmia	sp, {sp, lr}^				@ store the USR stack pointer & link register, upwards
	push	{r0-r12, lr}				@ store USR regs 0-12 and IRQ link register (r14), downwards
    ldr		r13, =threadsave_0			@ load the IRQ stack pointer with address of TCB
	pop		{r0-r12, lr}				@ load USR regs 0-12 and IRQ link register (r14), upwards
	ldmia	sp, {sp, lr}^				@ load the USR stack pointer & link register, upwards
	nop									@ evidently it's a god idea to put a NOP after a LDMIA
	ldr		r13, save_r13_0
	sub	pc, lr, #4					@ return from exception

save_0_run_1:
    str		r13, save_r13_0			
	ldr		r13, =threadsave_0			@ load the IRQ stack pointer with address of TCB
	add		r13, r13, #56				@ jump to middle of TCB for store up and store down
	stmia	sp, {sp, lr}^				@ store the USR stack pointer & link register, upwards
	push	{r0-r12, lr}				@ store USR regs 0-12 and IRQ link register (r14), downwards
    ldr		r13, =threadsave_1			@ load the IRQ stack pointer with address of TCB
	pop		{r0-r12, lr}				@ load USR regs 0-12 and IRQ link register (r14), upwards
	ldmia	sp, {sp, lr}^				@ load the USR stack pointer & link register, upwards
	nop									@ evidently it's a god idea to put a NOP after a LDMIA
	ldr		r13, save_r13_1
	sub	pc, lr, #4					@ return from exception


save_2_run_3:
    str		r13, save_r13_2			
	ldr		r13, =threadsave_2			@ load the IRQ stack pointer with address of TCB
	add		r13, r13, #56				@ jump to middle of TCB for store up and store down
	stmia	sp, {sp, lr}^				@ store the USR stack pointer & link register, upwards
	push	{r0-r12, lr}				@ store USR regs 0-12 and IRQ link register (r14), downwards
    ldr		r13, =threadsave_3			@ load the IRQ stack pointer with address of TCB
	pop		{r0-r12, lr}				@ load USR regs 0-12 and IRQ link register (r14), upwards
	ldmia	sp, {sp, lr}^				@ load the USR stack pointer & link register, upwards
	nop									@ evidently it's a god idea to put a NOP after a LDMIA
	ldr		r13, save_r13_3
	sub	pc, lr, #4					@ return from exception


save_3_run_2:
    str		r13, save_r13_3			
	ldr		r13, =threadsave_3			@ load the IRQ stack pointer with address of TCB
	add		r13, r13, #56				@ jump to middle of TCB for store up and store down
	stmia	sp, {sp, lr}^				@ store the USR stack pointer & link register, upwards
	push	{r0-r12, lr}				@ store USR regs 0-12 and IRQ link register (r14), downwards
    ldr		r13, =threadsave_2			@ load the IRQ stack pointer with address of TCB
	pop		{r0-r12, lr}				@ load USR regs 0-12 and IRQ link register (r14), upwards
	ldmia	sp, {sp, lr}^				@ load the USR stack pointer & link register, upwards
	nop									@ evidently it's a god idea to put a NOP after a LDMIA
	ldr		r13, save_r13_2
	sub	pc, lr, #4					@ return from exception

start_0:
    str		r13, save_r13_1			
	ldr		r13, =threadsave_1			@ load the IRQ stack pointer with address of TCB
	add		r13, r13, #56				@ jump to middle of TCB for store up and store down
	stmia	sp, {sp, lr}^				@ store the USR stack pointer & link register, upwards
	push	{r0-r12, lr}				@ store USR regs 0-12 and IRQ link register (r14), downwards

    ldr lr, = entry_t0
    sub pc, lr, #0
 

start_3:
    str		r13, save_r13_2			
	ldr		r13, =threadsave_2			@ load the IRQ stack pointer with address of TCB
	add		r13, r13, #56				@ jump to middle of TCB for store up and store down
	stmia	sp, {sp, lr}^				@ store the USR stack pointer & link register, upwards
	push	{r0-r12, lr}				@ store USR regs 0-12 and IRQ link register (r14), downwards

    ldr lr, = entry_t3
    sub pc, lr, #0
  
			.word 0
			.word 0
threadsave_3:.word 0
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

			.word 0
			.word 0
threadsave_2:.word 0
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


			.word 0
			.word 0
threadsave_1:.word 0
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

			.word 0
			.word 0
threadsave_0:.word 0
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

save_r13_0: .word 0
save_r13_1: .word 0
save_r13_2: .word 0
save_r13_3: .word 0



