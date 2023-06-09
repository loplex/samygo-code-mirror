/*
 * armboot - Startup Code for OMAP3530/ARM Cortex CPU-core
 *
 * Copyright (c) 2004	Texas Instruments <r-woodruff2@ti.com>
 *
 * Copyright (c) 2001	Marius Gröger <mag@sysgo.de>
 * Copyright (c) 2002	Alex Züpke <azu@sysgo.de>
 * Copyright (c) 2002	Gary Jennejohn <garyj@denx.de>
 * Copyright (c) 2003	Richard Woodruff <r-woodruff2@ti.com>
 * Copyright (c) 2003	Kshitij <kshitij@ti.com>
 * Copyright (c) 2006-2008 Syed Mohammed Khasim <x0khasim@ti.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include "./echo/include/config.h"

//#include <config.h>
//#include <version.h>

.globl _start
_start: b	reset
#ifndef	CONFIG_NAND_SPL
@	ldr	pc, _undefined_instruction @ can't use due to SE auth..
	nop	@secure boot status value, SE fix offset 
	ldr	pc, _software_interrupt
	ldr	pc, _prefetch_abort
	ldr	pc, _data_abort
	ldr	pc, _not_used
	ldr	pc, _irq
	ldr	pc, _fiq

@_undefined_instruction: .word undefined_instruction
 secure_boot_status_value: .word 0x8 @first SE 8Kbytes
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:		.word data_abort
_not_used:		.word not_used
_irq:			.word irq
_fiq:			.word fiq
_pad:			.word 0x12345678 /* now 16*4=64 */
.global _end_vect
#else
	. = _start + 64
#endif
_end_vect:

	.balignl 16,0xdeadbeef
/*************************************************************************
 *
 * Startup Code (reset vector)
 *
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *
 *************************************************************************/

_TEXT_BASE:
	.word	TEXT_BASE

.globl _armboot_start
_armboot_start:
	.word _start

/*
 * These are defined in the board-specific linker script.
 */
.globl _bss_start
_bss_start:
	.word __bss_start

.globl _bss_end
_bss_end:
	.word _end

#ifdef CONFIG_USE_IRQ
/* IRQ stack memory (calculated at run-time) */
.globl IRQ_STACK_START
IRQ_STACK_START:
	.word	0x0badc0de

/* IRQ stack memory (calculated at run-time) */
.globl FIQ_STACK_START
FIQ_STACK_START:
	.word 0x0badc0de
#endif

/*
 * the actual reset code
 */

reset:
	/*
	 * set the cpu to SVC32 mode
	 */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr,r0



/* Delete CONFIG_OMAP34XX CODE */
@ Refer the onrignal [ start.S ]
/*******************************/

/* the mask ROM code should have PLL and others stable */

#if defined(CONFIG_NAND_U_BOOT) /* because mem init once */
	#if defined(CONFIG_NAND_SPL)
	bl	cpu_init_crit
	#endif
#else
	#ifndef CONFIG_SKIP_LOWLEVEL_INIT
		bl	cpu_init_crit
	#endif
#endif

#ifndef CONFIG_SKIP_RELOCATE_UBOOT
relocate:				@ relocate U-Boot to RAM
	adr	r0, _start		@ r0 <- current position of code
	ldr	r1, _TEXT_BASE		@ test if we run from flash or RAM
	cmp	r0, r1			@ don't reloc during debug
	beq	stack_setup

	ldr	r2, _armboot_start
	ldr	r3, _bss_start
	sub	r2, r3, r2		@ r2 <- size of armboot
	add	r2, r0, r2		@ r2 <- source end address

copy_loop:				@ copy 32 bytes at a time
	ldmia	r0!, {r3 - r10}		@ copy from source address [r0]
	stmia	r1!, {r3 - r10}		@ copy to   target address [r1]
	cmp	r0, r2			@ until source end addreee [r2]
	ble	copy_loop
#endif	/* CONFIG_SKIP_RELOCATE_UBOOT */

	/* Set up the stack */
stack_setup:
	ldr	r0, _TEXT_BASE		@ upper 128 KiB: relocated uboot
	sub	r0, r0, #CONFIG_SYS_MALLOC_LEN @ malloc area
	sub	r0, r0, #CONFIG_SYS_GBL_DATA_SIZE @ bdinfo
#ifdef CONFIG_USE_IRQ
	sub	r0, r0, #(CONFIG_STACKSIZE_IRQ + CONFIG_STACKSIZE_FIQ)
#endif
	sub	sp, r0, #12		@ leave 3 words for abort-stack
	bic	sp, sp, #7		@ 8-byte alignment for ABI compliance

	/* Clear BSS (if any). Is below tx (watch load addr - need space) */
clear_bss:
	ldr	r0, _bss_start		@ find start of bss segment
	ldr	r1, _bss_end		@ stop here
	mov	r2, #0x00000000		@ clear value
clbss_l:
	str	r2, [r0]		@ clear BSS location
	cmp	r0, r1			@ are we at the end yet
	add	r0, r0, #4		@ increment clear index pointer
	bne	clbss_l			@ keep clearing till at end


#ifdef CONFIG_SDP_MMC_OP
	/* mmc operates to relocate U-boot program to DDR */ 
	bl	sdp_mmc
#endif

secure_boot_auth_whole_mmc:
    ldr r0, =0x303f0000
    ldr r1, [r0, #0x38]
    ands    r1, r1, #1
    b end_secure_boot
    
    ldr r2, =TEXT_BASE      @ r2 := start addr 
    mov r7, #(128<<10)
    sub r7, r7, #16     @ r7 := size 

    /* cmac := r3 - r6 */
    add r1, r2, r7
    ldm r1, {r3-r6}

    str r2, [r0, #0x420]    @ start addr 
    str r7, [r0, #0x414]    @ size 
    str r7, [r0, #0x464]    @ size 
 @   rsetl   r0, #0x430, r1, 0xc3    @ control
    ldr r1, =0xc3
    str r1, [r0, #0x430]

    add r2, r0, #0x400      @ pre CMAC
    add r2, r2, #0x54
    stm r2, {r3-r6}

@    rsetl   r0, #0x404, r1, 0x23    @ start auth
    ldr r1, =0x23
    str r1, [r0, #0x404]

1:  ldr r1, [r0, #0x400]    @ wait dma done
    movs    r1, r1
    bne 1b

check_auth_whole:
    ldr r1, [r0, #0x494]
    cmp r1, #0x1
    bne check_auth_whole

end_secure_boot:
    nop
    nop

#ifdef CONFIG_NAND_SPL
	/* SPL init, relocate U-Boot from NAND to DDR */
	b nand_boot
#else

	/* normal u-boot */
	/*bl Main */
	ldr pc, _Main
	
#ifdef CONFIG_ECHO_S
@_ddr_b_setup: .word ddr_b_setup
#endif
_Main: .word Main

#endif
/*************************************************************************
 *
 * CPU_init_critical registers
 *
 * setup important registers
 * setup memory timing
 *
 *************************************************************************/
cpu_init_crit:

@ fast lookup disable
	mrc 	p15, 0, r1, c1, c1, 0
	tst 	r1 , #1
	bne		100f

	mrc 	p15, 0, r1, c15, c0, 1
	orr 	r1, r1, #0x40
	mcr		p15, 0, r1, c15, c0, 1
100:
	/*
	 * Invalidate L1 I/D
	 */
	mov	r0, #0			@ set up for MCR
	mcr	p15, 0, r0, c8, c7, 0	@ invalidate TLBs
	mcr	p15, 0, r0, c7, c5, 0	@ invalidate icache

	/*
	 * disable MMU stuff and caches
	 */
	mrc	p15, 0, r0, c1, c0, 0
	bic	r0, r0, #0x00002000	@ clear bits 13 (--V-)
	bic	r0, r0, #0x00000007	@ clear bits 2:0 (-CAM)
	orr	r0, r0, #0x00000002	@ set bit 1 (--A-) Align
	orr	r0, r0, #0x00001800	@ set bit 12 (ZI--) BTB
	mcr	p15, 0, r0, c1, c0, 0

	/*
	 * Jump to board specific initialization...
	 * The Mask ROM will have already initialized
	 * basic memory. Go here to bump up clock rate and handle
	 * wake up conditions.
	 */
	mov	ip, lr			@ persevere link reg across call

	bl	lowlevel_init		@ go setup pll,mux,memory

	mov	lr, ip			@ restore link
	mov	pc, lr			@ back to my caller

/*
 *************************************************************************
 *
 * Interrupt handling
 *
 *************************************************************************
 */
@
@ IRQ stack frame.
@
#define S_FRAME_SIZE	72

#define S_OLD_R0	68
#define S_PSR		64
#define S_PC		60
#define S_LR		56
#define S_SP		52

#define S_IP		48
#define S_FP		44
#define S_R10		40
#define S_R9		36
#define S_R8		32
#define S_R7		28
#define S_R6		24
#define S_R5		20
#define S_R4		16
#define S_R3		12
#define S_R2		8
#define S_R1		4
#define S_R0		0

#define MODE_SVC 0x13
#define I_BIT	 0x80

/*
 * use bad_save_user_regs for abort/prefetch/undef/swi ...
 * use irq_save_user_regs / irq_restore_user_regs for IRQ/FIQ handling
 */

	.macro	bad_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE		@ carve out a frame on current
						@ user stack
	stmia	sp, {r0 - r12}			@ Save user registers (now in
						@ svc mode) r0-r12

	ldr	r2, _armboot_start
	sub	r2, r2, #(CONFIG_SYS_MALLOC_LEN)
	sub	r2, r2, #(CONFIG_SYS_GBL_DATA_SIZE + 8)	@ set base 2 words into abort
						@ stack
	ldmia	r2, {r2 - r3}			@ get values for "aborted" pc
						@ and cpsr (into parm regs)
	add	r0, sp, #S_FRAME_SIZE		@ grab pointer to old stack

	add	r5, sp, #S_SP
	mov	r1, lr
	stmia	r5, {r0 - r3}			@ save sp_SVC, lr_SVC, pc, cpsr
	mov	r0, sp				@ save current stack into r0
						@ (param register)
	.endm

	.macro	irq_save_user_regs
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0-r12
	add	r8, sp, #S_PC			@ !! R8 NEEDS to be saved !!
						@ a reserved stack spot would
						@ be good.
	stmdb	r8, {sp, lr}^			@ Calling SP, LR
	str	lr, [r8, #0]			@ Save calling PC
	mrs	r6, spsr
	str	r6, [r8, #4]			@ Save CPSR
	str	r0, [r8, #8]			@ Save OLD_R0
	mov	r0, sp
	.endm

	.macro	irq_restore_user_regs
	ldmia	sp, {r0 - lr}^			@ Calling r0 - lr
	mov	r0, r0
	ldr	lr, [sp, #S_PC]			@ Get PC
	add	sp, sp, #S_FRAME_SIZE
	subs	pc, lr, #4			@ return & move spsr_svc into
						@ cpsr
	.endm

	.macro get_bad_stack
	ldr	r13, _armboot_start		@ setup our mode stack (enter
						@ in banked mode)
	sub	r13, r13, #(CONFIG_SYS_MALLOC_LEN)	@ move past malloc pool
	sub	r13, r13, #(CONFIG_SYS_GBL_DATA_SIZE + 8) @ move to reserved a couple
						@ spots for abort stack

	str	lr, [r13]			@ save caller lr in position 0
						@ of saved stack
	mrs	lr, spsr			@ get the spsr
	str	lr, [r13, #4]			@ save spsr in position 1 of
						@ saved stack

	mov	r13, #MODE_SVC			@ prepare SVC-Mode
	@ msr	spsr_c, r13
	msr	spsr, r13			@ switch modes, make sure
						@ moves will execute
	mov	lr, pc				@ capture return pc
	movs	pc, lr				@ jump to next instruction &
						@ switch modes.
	.endm

	.macro get_bad_stack_swi
	sub	r13, r13, #4			@ space on current stack for
						@ scratch reg.
	str	r0, [r13]			@ save R0's value.
	ldr	r0, _armboot_start		@ get data regions start
	sub	r0, r0, #(CONFIG_SYS_MALLOC_LEN)	@ move past malloc pool
	sub	r0, r0, #(CONFIG_SYS_GBL_DATA_SIZE + 8)	@ move past gbl and a couple
						@ spots for abort stack
	str	lr, [r0]			@ save caller lr in position 0
						@ of saved stack
	mrs	r0, spsr			@ get the spsr
	str	lr, [r0, #4]			@ save spsr in position 1 of
						@ saved stack
	ldr	r0, [r13]			@ restore r0
	add	r13, r13, #4			@ pop stack entry
	.endm

	.macro get_irq_stack			@ setup IRQ stack
	ldr	sp, IRQ_STACK_START
	.endm

	.macro get_fiq_stack			@ setup FIQ stack
	ldr	sp, FIQ_STACK_START
	.endm


#ifndef CONFIG_NAND_SPL
/*
 * exception handlers
 */
	.align	5
undefined_instruction:
@	get_bad_stack
@	bad_save_user_regs
@	bl	do_undefined_instruction

	.align	5
software_interrupt:
@	get_bad_stack_swi
@	bad_save_user_regs
@	bl	do_software_interrupt

	.align	5
prefetch_abort:
@	get_bad_stack
@	bad_save_user_regs
@	bl	do_prefetch_abort

	.align	5
data_abort:
@	get_bad_stack
@	bad_save_user_regs
@	bl	do_data_abort

	.align	5
not_used:
@	get_bad_stack
@	bad_save_user_regs
@	bl	do_not_used

#ifdef CONFIG_USE_IRQ

	.align	5
irq:
@	get_irq_stack
@	irq_save_user_regs
@	bl	do_irq
@	irq_restore_user_regs

	.align	5
fiq:
@	get_fiq_stack
	/* someone ought to write a more effective fiq_save_user_regs */
@	irq_save_user_regs
@	bl	do_fiq
@	irq_restore_user_regs

#else

	.align	5
irq:
@	get_bad_stack
@	bad_save_user_regs
@	bl	do_irq

	.align	5
fiq:
@	get_bad_stack
@	bad_save_user_regs
@	bl	do_fiq

#endif
#endif/*CONFIG_NAND_SPL*/
