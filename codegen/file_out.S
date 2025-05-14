	.file "stdin"
	.text
	.section .rodata
.LC0:
	.string "hello\n"
	.text
	.globl main
	.type main, @function
main:
.BB.0.0:
	pushl %ebp		# associate ebp with symbol .BB.0.0
	movl %esp, %ebp		# set up stack frame pointer

	subl $15, %esp
	# QUAD ANALYZED: 	i{lvar} = 	MOV $0
	movl $0, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	BR .BB.0.1
	jmp .BB.0.1

.BB.0.1:
	# QUAD ANALYZED: 	CMP i{lvar}, $5
	movl $5, %ecx
	cmpl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	BRLT .BB.0.2, .BB.0.4
	jl .BB.0.2
	jmp .BB.0.4

.BB.0.2:
	# QUAD ANALYZED: 	ARG 0, "hello\n"
	# pushed arg #0 into arg list

	# QUAD ANALYZED: 	%T0 = 	CALL printf, 1
	pushl $.LC0
	call printf
	addl $4, %esp
	movl %eax, %ebx

	# QUAD ANALYZED: 	BR .BB.0.3
	jmp .BB.0.3

.BB.0.3:
	# QUAD ANALYZED: 	%T1 = 	ADD $1, i{lvar}
	movl $1, %edx
	movl -4(%ebp), %eax
	addl %edx, %eax
	movl %eax, %edi

	# QUAD ANALYZED: 	i{lvar} = 	MOV %T1
	movl %edi, %ecx
	movl %ecx, -4(%ebp)

	# QUAD ANALYZED: 	BR .BB.0.1
	jmp .BB.0.1

.BB.0.4:
	# QUAD ANALYZED: 	RETURN 
	leave
	ret

