	.file "stdin"
	.text
	.globl main
	.type main, @function
main:
.BB.0.0:
	pushl %ebp		# associate ebp with symbol .BB.0.0
	movl %esp, %ebp		# set up stack frame pointer

	subl $16, %esp

	pushl %ebx
	pushl %edi
	pushl %esi

	# QUAD ANALYZED: 	%T0 = 	MUL $3, $0
	movl $3, %eax
	movl $0, %edx
	imull %edx, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T1 = 	LEA a{global}
	leal a, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	%T2 = 	ADD %T1, %T0
	movl %edi, %edx
	movl %ebx, %eax
	addl %edx, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	STORE $2, %T2
	movl $2, %edx
	movl %esi, %eax
	movl %edx, (%eax)

	# QUAD ANALYZED: 	%T3 = 	MUL $3, $0
	movl $3, %eax
	movl $0, %edx
	imull %edx, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T4 = 	LEA a{global}
	leal a, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	%T5 = 	ADD %T4, %T3
	movl %edi, %edx
	movl %ebx, %eax
	addl %edx, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	%T6 = 	LOAD  [%T5]
	movl %esi, %edx
	movl (%edx), %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	x{lvar} = 	MOV %T6
	movl %ebx, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	%T7 = 	MUL $5, $0
	movl $5, %eax
	movl $0, %edx
	imull %edx, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	%T8 = 	LEA a{global}
	leal a, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	%T9 = 	ADD %T8, %T7
	movl %esi, %edx
	movl %edi, %eax
	addl %edx, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	p{lvar} = 	MOV %T9
	movl %ebx, %ecx
	movl %ecx, -8(%ebp)

	# QUAD ANALYZED: 	%T10 = 	ADD x{lvar}, $5
	movl -4(%ebp), %edx
	movl $5, %eax
	addl %edx, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	b{lvar} = 	MOV %T10
	movl %edi, %ecx
	movl %ecx, -12(%ebp)

	# QUAD ANALYZED: 	ARG 1, b{lvar}
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, "b is %d\n"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T11 = 	CALL printf, 2
	pushl -12(%ebp)
	pushl $.LC0
	call printf
	addl $8, %esp
	movl %eax, %esi

	# QUAD ANALYZED: 	ARG 1, x{lvar}
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, "x is %d\n"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T12 = 	CALL printf, 2
	pushl -4(%ebp)
	pushl $.LC1
	call printf
	addl $8, %esp
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T13 = 	LEA x{lvar}
	leal -4(%ebp), %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	%T14 = 	ADD $3, %T13
	movl $3, %edx
	movl %edi, %eax
	addl %edx, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	p{lvar} = 	MOV %T14
	movl %esi, %ecx
	movl %ecx, -8(%ebp)

	popl %ebx
	popl %edi
	popl %esi

	# QUAD ANALYZED: 	RETURN 
	leave
	ret

	.section .bss
	.global a
	.type a, @object
	.size a, 40
a:
	.zero 40
	.text
	.section .rodata
.LC0:
	.string "b is %d\n"
.LC1:
	.string "x is %d\n"
