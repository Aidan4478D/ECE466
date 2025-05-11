	.file	"assignop.c"
	.text
	.globl	a
	.bss
	.align 4
	.type	a, @object
	.size	a, 4
a:
	.zero	4
	.text
	.globl	f
	.type	f, @function
f:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6

    # a = 3
	movl	%edi, -20(%rbp)
	movl	$3, a(%rip)
    
    # load a into eax
	movl	a(%rip), %eax
	addl	$2, %eax
	movl	%eax, -20(%rbp) # b = 2 + a
    
    # ADDITION
    # load a and b, add them and move into c
	movl	a(%rip), %edx
	movl	-20(%rbp), %eax
	addl	%edx, %eax
	movl	%eax, -4(%rbp)

    # MULTIPLICATION 
    # load a and b, multiply them, move them into c
	movl	a(%rip), %eax
	movl	-20(%rbp), %edx
	imull	%edx, %eax
	movl	%eax, -4(%rbp)

    # SUBTRACTION
    # load a, sub b from a, move into c
	movl	a(%rip), %eax
	subl	-20(%rbp), %eax
	movl	%eax, -4(%rbp)

    # DIVISION
    # load a, convert long to double, divide, move into c
	movl	a(%rip), %eax
	cltd
	idivl	-20(%rbp)
	movl	%eax, -4(%rbp)

    # MOD
    # load a, convert long to double, divide, move edx into c, move c into eax
	movl	a(%rip), %eax
	cltd
	idivl	-20(%rbp)
	movl	%edx, -4(%rbp)
	movl	-4(%rbp), %eax

	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
