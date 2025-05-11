	.file	"arr_test.c"
	.text
	.globl	a
	.bss
	.align 32
	.type	a, @object
	.size	a, 40
a:
	.zero	40
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

    # 
	subq	$32, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	movl	12+a(%rip), %eax
    
    
	movl	%eax, -24(%rbp)
	leaq	20+a(%rip), %rax

    # b = x + 5
	movq	%rax, -16(%rbp)
	movl	-24(%rbp), %eax
	addl	$5, %eax
	movl	%eax, -20(%rbp)
	leaq	-24(%rbp), %rax
	addq	$12, %rax
	movq	%rax, -16(%rbp)
	nop
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L2
	call	__stack_chk_fail@PLT
.L2:
	leave
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
