## ECE466 - Compilers

All work done under the supervision of Professor Jeff Hakner during the Cooper Union Spring 2025 semester. [Link to course site](http://faculty.cooper.edu/hak/ece466/)

The following code is a fully functioning C compiler based on the C99 language standard. The source code is `C` and the target assembly output is `x86-32`. 

There are six directories corresponding to each of the assignments within the course. 
- Assignment 1: `lexer` directory
- Assignment 2: `parser` directory
- Assignment 3: `symtable` directory
- Assignment 4: `frontend` directory
- Assignment 5: `quads` directory
- Assignment 6: `codegen` directory (final submission)

To run the compiler, enter the `codegen` directory and follow the subsequent steps.

### How to run:
1. Compile the compiler
```console
make
```
- This compiles the lexer, parser, symbol table, quads generation, and code generation code
- Output executable called `acc` (Aidan's C Compiler)

2. Feed your program into the compiler
```console
cat {file_name}.c | ./acc
```
- This produces two files:
    - `file_out.S`: Contains the x86-32 assembly associated with your program {file_name.c}
    - `file_out.txt`: Contains the AST tree and quad generation output associated with each function

3. Compile the assembly
```console
make assemble
```
- Compiled using `gcc -m32`
- Output executable called `compiled.out`

4. Run the program!
```console
./compiled.out
```

### Additional notes
- You can run `make clean` to remove output files produced by the compiler
- The `tests/` directory includes a couple of (non-exhaustive) test programs that can be run
