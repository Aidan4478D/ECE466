CC = gcc
CFLAGS = -std=c99

all: parser

parser: parser.tab.o lex.yy.o ast.o
	$(CC) $(CFLAGS) -o p parser.tab.o lex.yy.o ast.o -lfl

parser.tab.c parser.tab.h: parser.y
	bison -d parser.y

lex.yy.c: lexer.l parser.tab.h
	flex lexer.l

parser.tab.o: parser.tab.c ast.h
	$(CC) $(CFLAGS) -c parser.tab.c

lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c lex.yy.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

clean:
	rm -f parser parser.tab.c parser.tab.h lex.yy.c *.o

.PHONY: all clean

