	.file "stdin"
	.text
	.globl f
	.type f, @function
f:
.BB.0.0:
	pushl %ebp		# associate ebp with symbol .BB.0.0
	movl %esp, %ebp		# set up stack frame pointer

	subl $12, %esp
	# QUAD ANALYZED: 	a{global} = 	MOV $1
	movl $1, %ecx
	movl %ecx, a

	# QUAD ANALYZED: 	f{lvar} = 	MOV $3
	movl $3, %ecx
	movl %ecx, -8(%ebp)

	# QUAD ANALYZED: 	e{lvar} = 	MOV $2
	movl $2, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	d{param} = 	MOV $1
	movl $1, %ecx
	movl %ecx, 12(%ebp)

	# QUAD ANALYZED: 	c{param} = 	MOV $0
	movl $0, %ecx
	movl %ecx, 8(%ebp)

	# QUAD ANALYZED: 	b{global} = 	MOV $1
	movl $1, %ecx
	movl %ecx, b

	# QUAD ANALYZED: 	%T0 = 	ADD a{global}, b{global}
	movl a, %edx
	movl b, %eax
	addl %edx, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T1 = 	ADD %T0, c{param}
	movl %ebx, %edx
	movl 8(%ebp), %eax
	addl %edx, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	%T2 = 	ADD %T1, d{param}
	movl %edi, %edx
	movl 12(%ebp), %eax
	addl %edx, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	%T3 = 	ADD %T2, e{lvar}
	movl %esi, %edx
	movl -4(%ebp), %eax
	addl %edx, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T4 = 	ADD %T3, f{lvar}
	movl %ebx, %edx
	movl -8(%ebp), %eax
	addl %edx, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	RETURN %T4
	movl %edi, %eax
	leave
	ret

	.globl main
	.type main, @function
main:
.BB.1.0:
	pushl %ebp		# associate ebp with symbol .BB.1.0
	movl %esp, %ebp		# set up stack frame pointer

	subl $8, %esp
	# QUAD ANALYZED: 	ARG 1, $2
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, $1
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T0 = 	CALL f, 2
	pushl $2
	pushl $1
	call f
	addl $8, %esp
	movl %eax, %ebx

	# QUAD ANALYZED: 	z{lvar} = 	MOV %T0
	movl %ebx, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	%T2 = 	ADD a{global}, b{global}
	movl a, %edx
	movl b, %eax
	addl %edx, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	ARG 1, %T2
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, "a+b=%d\n"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T1 = 	CALL printf, 2
	pushl %esi
	pushl $.LC0
	call printf
	addl $8, %esp
	movl %eax, %edi

	# QUAD ANALYZED: 	RETURN z{lvar}
	movl -4(%ebp), %eax
	leave
	ret

	.section .bss
	.global a
	.type a, @object
	.size a, 4
a:
	.zero 4
	.global b
	.type b, @object
	.size b, 4
b:
	.zero 4
	.text
	.section .rodata
.LC0:
	.string "a+b=%d\n"
