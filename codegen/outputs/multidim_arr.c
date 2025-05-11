---------------------------------------------
[in FILE_SCOPE scope starting at <stdin>:1]
<stdin>:1
symbol: f, scope: FILE_SCOPE, namespace = GENERAL, type = FUNCTION, storage = UNKNOWN
AST Tree for your symbol:
---------------------------------------------
FUNCTION 'f'
	RETURN TYPE: INT
---------------------------------------------

---------------------------------------------
[in FILE_SCOPE scope starting at <stdin>:1]
<stdin>:1
symbol: f, scope: FILE_SCOPE, namespace = GENERAL, type = FUNCTION, storage = UNKNOWN
AST Tree for your symbol:
---------------------------------------------
FUNCTION 'f'
	RETURN TYPE: INT
---------------------------------------------

---------------------------------------------
[in FUNCT_SCOPE scope starting at <stdin>:1]
<stdin>:3
symbol: x, scope: FUNCT_SCOPE, namespace = GENERAL, type = VARIABLE, storage = AUTO
AST Tree for your symbol:
---------------------------------------------
TYPE: ARRAY of size 20 of ARRAY of size 10 of INT
---------------------------------------------


---------------------------------------------
AST Dump for function f
---------------------------------------------
LIST:
LIST ELEMENT #1:
	DECLARATION
		SPECIFIERS: INT 
		DECLARATOR #1
			IDENT: x
			TYPE: ARRAY of size 20 of ARRAY of size 10 of INT


LIST ELEMENT #2:
	RETURN
		NUM (numtype=int)	1	SIGNED


---------------------------------------------
QUAD generation for function f
---------------------------------------------
.BB0.0 
	RETURN $1

---------------------------------------------
ASM generation for BB .BB.0.0
---------------------------------------------
.BB.0.0:
	pushq %rbp		# associate rbp with symbol .BB.0.0
	movq %rsp, %rbp		# set up stack frame pointer

	# QUAD ANALYZED: 	RETURN $1

	popq %rbp




