## Compilers Final Submission

Source code: C
Target assembly: x86-32

### How to run:
1. Compile the compilers
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
