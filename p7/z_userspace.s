	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"z_userspace.c"
	.text
	.align	2
	.global	nada
	.type	nada, %function
nada:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 1, uses_anonymous_args = 0
	@ link register save eliminated.
	str	fp, [sp, #-4]!
	add	fp, sp, #0
	mov	r0, r0	@ nop
	add	sp, fp, #0
	ldmfd	sp!, {fp}
	bx	lr
	.size	nada, .-nada
	.align	2
	.global	userspace
	.type	userspace, %function
userspace:
	@ Function supports interworking.
	@ args = 0, pretend = 0, frame = 8
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #8
.L6:
	mov	r3, #0
	str	r3, [fp, #-8]
	b	.L4
.L5:
	bl	nada
	ldr	r3, [fp, #-8]
	add	r3, r3, #1
	str	r3, [fp, #-8]
.L4:
	ldr	r2, [fp, #-8]
	ldr	r3, .L7
	cmp	r2, r3
	ble	.L5
@ 15 "z_userspace.c" 1
	svc #0
@ 0 "" 2
	b	.L6
.L8:
	.align	2
.L7:
	.word	524287
	.size	userspace, .-userspace
	.ident	"GCC: (GNU) 4.7.2"
