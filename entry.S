# This is where the kernel will start executing.



# _start symbol is the ELF entry point

.globl _start
_start = V2p_WO(entry)

.globl
entry:
	# Turn on page size extension for 4Mbyte pages
	movl    %cr4, %eax
	orl     $(CR4_PSE), %eax
	movl    %eax, %cr4
	# Set page directory
	movl    $(V2P_WO(entrypgdir)), %eax
	movl    %eax, %cr3
	# Turn on paging.
	movl    %cr0, %eax
	orl     $(CR0_PG|CR0_WP), %eax
	movl    %eax, %cr0

	# Set up the stack pointer.
	movl $(stack + KSTACKSIZE), %esp

	# Jump to main(), and switch to executing at
	# high addresses. The indirect call is needed because
	# the assembler produces a PC-relative instruction
	# for a direct jump.
	mov $main, %eax
	jmp *%eax


.comm stack, KSTACKSIZE

