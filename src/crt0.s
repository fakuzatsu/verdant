	.include "constants/gba_constants.inc"

	.syntax unified

	.arm

	.align 2, 0
Init::
@ Set up location for IRQ stack
	mov r0, #PSR_IRQ_MODE
	msr cpsr_cf, r0
	ldr sp, sp_irq
@ Set up location for system stack
	mov r0, #PSR_SYS_MODE
	msr cpsr_cf, r0
	ldr sp, sp_sys
@ Dispatch memory reset request to hardware
	mov r0, #255 @ RESET_ALL
	svc #1 << 16
@ Fill RAM areas with appropriate data
	bl InitializeWorkingMemory
@ Prepare for interrupt handling
	ldr r1, =INTR_VECTOR
	ldr r0, =IntrMain
	str r0, [r1]
@ Jump to AgbMain
	ldr r1, =AgbMain + 1
	mov lr, pc
	bx r1
@ Re-init if AgbMain exits
	b Init

	.align 2, 0
sp_sys: .word IWRAM_END - 0x1c0
sp_irq: .word IWRAM_END - 0x60

	.pool

	.arm
    .section .iwram.code
	.align 2, 0
IntrMain::
	mov r3, #REG_BASE
	add r3, r3, #OFFSET_REG_IE
	ldr r2, [r3]
	ldrh r1, [r3, #OFFSET_REG_IME - 0x200]
	mrs r0, spsr
	stmfd sp!, {r0-r3,lr}
	mov r0, #0
	strh r0, [r3, #OFFSET_REG_IME - 0x200]
	and r1, r2, r2, lsr #16
	mov r12, #0
	ands r0, r1, #INTR_FLAG_VCOUNT
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	mov r0, 0x1
	strh r0, [r3, #OFFSET_REG_IME - 0x200]
	ands r0, r1, #INTR_FLAG_SERIAL
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_TIMER3
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_HBLANK
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_VBLANK
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_TIMER0
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_TIMER1
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_TIMER2
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_DMA0
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_DMA1
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_DMA2
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_DMA3
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_KEYPAD
	bne IntrMain_FoundIntr
	add r12, r12, 0x4
	ands r0, r1, #INTR_FLAG_GAMEPAK
	strbne r0, [r3, #REG_SOUNDCNT_X - REG_IE]
	bne . @ spin
IntrMain_FoundIntr:
	strh r0, [r3, #OFFSET_REG_IF - 0x200]
	bic r2, r2, r0
	ldr r0, =gSTWIStatus
	ldr r0, [r0]
	ldrb r0, [r0, 0xA]
	mov r1, 0x8
	lsl r0, r1, r0
	orr r0, r0, #INTR_FLAG_GAMEPAK
	orr r1, r0, #INTR_FLAG_SERIAL | INTR_FLAG_TIMER3 | INTR_FLAG_VCOUNT | INTR_FLAG_HBLANK
	and r1, r1, r2
	strh r1, [r3, #OFFSET_REG_IE - 0x200]
	mrs r3, cpsr
	bic r3, r3, #PSR_I_BIT | PSR_F_BIT | PSR_MODE_MASK
	orr r3, r3, #PSR_SYS_MODE
	msr cpsr_cf, r3
	ldr r1, =gIntrTable
	add r1, r1, r12
	ldr r0, [r1]
	stmfd sp!, {lr}
	adr lr, IntrMain_RetAddr
	bx r0
IntrMain_RetAddr:
	ldmfd sp!, {lr}
	mrs r3, cpsr
	bic r3, r3, #PSR_I_BIT | PSR_F_BIT | PSR_MODE_MASK
	orr r3, r3, #PSR_I_BIT | PSR_IRQ_MODE
	msr cpsr_cf, r3
	ldmia sp!, {r0-r3,lr}
	strh r2, [r3, #OFFSET_REG_IE - 0x200]
	strh r1, [r3, #OFFSET_REG_IME - 0x200]
	msr spsr_cf, r0
	bx lr

	.pool

    .text
	.align 2, 0 @ Don't pad with nop.

@ Fills initialized IWRAM and EWRAM sections in RAM from LMA areas in ROM
InitializeWorkingMemory:
	push {r0-r3,lr}
	ldr r0, =__iwram_lma
	ldr r1, =__iwram_start
	ldr r2, =__iwram_end
	cmp r1, r2
	beq skip_iwram_copy
	bl CopyMemory_DMA
skip_iwram_copy:
	ldr r0, =__ewram_lma
	ldr r1, =__ewram_start
	ldr r2, =__ewram_end
	cmp r1, r2
	beq skip_ewram_copy
	bl CopyMemory_DMA
skip_ewram_copy:
	pop {r0-r3,lr}
	bx lr

@ Uses a DMA transfer to load from r0 into r1 until r2
CopyMemory_DMA:
	subs r2, r2, r1
	lsr r2, r2, #2
	mov r4, #0x80000000
	orr r4, r4, #(1 << 26)
	orr r2, r2, r4
	ldr r3, =REG_DMA3
	stmia r3, {r0, r1, r2}
	bx lr

.thumb
@ Called from C code to reinitialize working memory after a link connection failure
ReInitializeEWRAM::
	ldr r0, =__ewram_lma
	ldr r1, =__ewram_start
	ldr r2, =__ewram_end
	cmp r1, r2
	beq EndReinitializeEWRAM
	subs r2, r1
	movs r3, #1
	lsls r3, r3, #26
	orrs r2, r2, r3
	swi 0x0B
EndReinitializeEWRAM:
	bx lr

	.pool

	.align 2, 0 @ Don't pad with nop.
