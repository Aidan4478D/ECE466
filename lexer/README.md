## Assignment 1 - Lexer

The objective of this assignment is to create a C lexer that pre-processes the file to make the life of the parser easier (I think). It was programmed completely in flex.

### How to Run:
Compilation:
```
flex lexer.l
gcc lex.yy.c
```

Run on all tests:
```
gcc -E tests/* | ./a.out
```
