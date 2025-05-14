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
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	call	__x86.get_pc_thunk.cx
	addl	$_GLOBAL_OFFSET_TABLE_, %ecx
	movl	$3, a@GOTOFF(%ecx)
	movl	a@GOTOFF(%ecx), %eax
	movl	%eax, 8(%ebp)
	movl	a@GOTOFF(%ecx), %eax
	addl	$2, %eax
	movl	%eax, 8(%ebp)
	movl	a@GOTOFF(%ecx), %edx
	movl	8(%ebp), %eax
	addl	%edx, %eax
	movl	%eax, -4(%ebp)
	movl	a@GOTOFF(%ecx), %eax
	movl	8(%ebp), %edx
	imull	%edx, %eax
	movl	%eax, -4(%ebp)
	movl	a@GOTOFF(%ecx), %eax
	subl	8(%ebp), %eax
	movl	%eax, -4(%ebp)
	movl	a@GOTOFF(%ecx), %eax
	cltd
	idivl	8(%ebp)
	movl	%eax, -4(%ebp)
	movl	a@GOTOFF(%ecx), %eax
	cltd
	idivl	8(%ebp)
	movl	%edx, -4(%ebp)
	movl	-4(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	f, .-f
	.section	.text.__x86.get_pc_thunk.cx,"axG",@progbits,__x86.get_pc_thunk.cx,comdat
	.globl	__x86.get_pc_thunk.cx
	.hidden	__x86.get_pc_thunk.cx
	.type	__x86.get_pc_thunk.cx, @function
__x86.get_pc_thunk.cx:
.LFB1:
	.cfi_startproc
	movl	(%esp), %ecx
	ret
	.cfi_endproc
.LFE1:
	.ident	"GCC: (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0"
	.section	.note.GNU-stack,"",@progbits
