	.file "stdin"
	.text
	.section .rodata
.LC0:
	.string "value is %d"
	.text
	.globl f
	.type f, @function
f:
.BB.0.0:
	pushl %ebp		# associate ebp with symbol .BB.0.0
	movl %esp, %rbp		# set up stack frame pointer

	# QUAD ANALYZED: 	ARG 1, a{param}
	# pushed arg #1 into arg list

	# QUAD ANALYZED: 	ARG 0, "value is %d"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T0 = 	CALL printf, 2
	pushl 8(%ebp)
	pushl $.LC0
	call printf
	addl $8, %esp

	# QUAD ANALYZED: 	%T1 = 	ADD a{param}, $1
	movl 8(%ebp), %edx
	movl $1, %eax
	addl %edx, %eax
	movl %eax, %%edi

	# QUAD ANALYZED: 	RETURN %T1
	movl %%edi, %eax
	leave
	ret

	.section .rodata
.LC1:
	.string "hello"
.LC2:
	.string "hello, 2"
	.text
	.globl main
	.type main, @function
main:
.BB.1.0:
	pushl %ebp		# associate ebp with symbol .BB.1.0
	movl %esp, %rbp		# set up stack frame pointer

	# QUAD ANALYZED: 	g{lvar} = 	MOV "hello"
	movl $.LC1, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	i{lvar} = 	MOV "hello, 2"
	movl $.LC2, %ecx
	movl %ecx, -8(%ebp)

	# QUAD ANALYZED: 	ARG 0, $2
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T0 = 	CALL f, 1
	pushl $2
	call f
	addl $4, %esp

	# QUAD ANALYZED: 	RETURN %T0
	movl %%ebx, %eax
	leave
	ret

