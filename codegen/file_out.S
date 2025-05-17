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

	# QUAD ANALYZED: 	%T3 = 	MUL $8, $0
	movl $8, %eax
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

	# QUAD ANALYZED: 	STORE $8, %T5
	movl $8, %edx
	movl %esi, %eax
	movl %edx, (%eax)

	# QUAD ANALYZED: 	%T6 = 	MUL $3, $0
	movl $3, %eax
	movl $0, %edx
	imull %edx, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T7 = 	LEA a{global}
	leal a, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	%T8 = 	ADD %T7, %T6
	movl %edi, %edx
	movl %ebx, %eax
	addl %edx, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	%T9 = 	LOAD  [%T8]
	movl %esi, %edx
	movl (%edx), %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	x{lvar} = 	MOV %T9
	movl %ebx, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	ARG 1, x{lvar}
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, "x is %d\n"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T10 = 	CALL printf, 2
	pushl -4(%ebp)
	pushl $.LC0
	call printf
	addl $8, %esp
	movl %eax, %edi

	# QUAD ANALYZED: 	%T11 = 	MUL $5, $0
	movl $5, %eax
	movl $0, %edx
	imull %edx, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	%T12 = 	LEA a{global}
	leal a, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T13 = 	ADD %T12, %T11
	movl %ebx, %edx
	movl %esi, %eax
	addl %edx, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	p{lvar} = 	MOV %T13
	movl %edi, %ecx
	movl %ecx, -8(%ebp)

	# QUAD ANALYZED: 	%T14 = 	ADD x{lvar}, $5
	movl -4(%ebp), %edx
	movl $5, %eax
	addl %edx, %eax
	movl %eax, %esi

	# QUAD ANALYZED: 	b{lvar} = 	MOV %T14
	movl %esi, %ecx
	movl %ecx, -12(%ebp)

	# QUAD ANALYZED: 	CMP x{lvar}, $5
	movl $5, %ecx
	cmpl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	BRLT .BB.0.3, .BB.0.1
	jl .BB.0.3
	jmp .BB.0.1

.BB.0.1:
	# QUAD ANALYZED: 	x{lvar} = 	MOV $100
	movl $100, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	BR .BB.0.2
	jmp .BB.0.2

.BB.0.2:
	# QUAD ANALYZED: 	ARG 1, b{lvar}
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, "b is %d\n"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T1 = 	CALL printf, 2
	pushl -12(%ebp)
	pushl $.LC1
	call printf
	addl $8, %esp
	movl %eax, %edi

	# QUAD ANALYZED: 	%T3 = 	MUL $2, $0
	movl $2, %eax
	movl $0, %edx
	imull %edx, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T4 = 	LEA p{lvar}
	leal -8(%ebp), %eax
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

	# QUAD ANALYZED: 	%T7 = 	LOAD  [%T6]
	movl %ebx, %edx
	movl (%edx), %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	ARG 1, %T7
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, "p[3] is %d\n"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T2 = 	CALL printf, 2
	pushl %edi
	pushl $.LC2
	call printf
	addl $8, %esp
	movl %eax, %esi

	# QUAD ANALYZED: 	ARG 1, x{lvar}
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, "x is now %d\n"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T8 = 	CALL printf, 2
	pushl -4(%ebp)
	pushl $.LC3
	call printf
	addl $8, %esp
	movl %eax, %esi

	# QUAD ANALYZED: 	%T9 = 	LEA x{lvar}
	leal -4(%ebp), %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	%T10 = 	ADD $3, %T9
	movl $3, %edx
	movl %ebx, %eax
	addl %edx, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	p{lvar} = 	MOV %T10
	movl %edi, %ecx
	movl %ecx, -8(%ebp)

	popl %ebx
	popl %edi
	popl %esi

	# QUAD ANALYZED: 	RETURN 
	leave
	ret

.BB.0.3:
	# QUAD ANALYZED: 	%T0 = 	ADD b{lvar}, $5
	movl -12(%ebp), %edx
	movl $5, %eax
	addl %edx, %eax
	movl %eax, %ebx

	# QUAD ANALYZED: 	x{lvar} = 	MOV %T0
	movl %ebx, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	BR .BB.0.2
	jmp .BB.0.2

	.section .bss
	.global a
	.type a, @object
	.size a, 40
a:
	.zero 40
	.text
	.section .rodata
.LC0:
	.string "x is %d\n"
.LC1:
	.string "b is %d\n"
.LC2:
	.string "p[3] is %d\n"
.LC3:
	.string "x is now %d\n"
