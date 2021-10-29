


.code16
.globl start
start:
	cli

	xorw %ax, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss

	ljmp $(SEG_KCODE<<3), $start32

.code32
start32:
	movw %ax, %ds

	call bootloader


