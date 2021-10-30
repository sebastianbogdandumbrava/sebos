



BOOTSEG = 0X07c0
SYSSEG = 0X1000

.code16
.globl _start

_start:
	movw %ds, %ax
	movw %ax, %es
	
	cld

	movw %ss, %dx
	movw %ax, %dx
	movw %sp, %dx

	sti

	calll main
