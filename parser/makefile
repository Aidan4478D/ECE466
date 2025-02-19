CC = gcc
CFLAGS = -Wall

all: parser clean_turn

parser: y.tab.o lex.yy.o ast.o helpers.o
	$(CC) $(CFLAGS) -o p y.tab.o lex.yy.o ast.o helpers.o -lfl

y.tab.c y.tab.h: parser.y
	yacc -d parser.y

y.tab.o: y.tab.c y.tab.h ast.h
	$(CC) $(CFLAGS) -c y.tab.c -ll -ly

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

lex.yy.o: lex.yy.c y.tab.h ast.h
	$(CC) $(CFLAGS) -c lex.yy.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c helpers.c

clean_turn:
	rm -f *.o

clean:
	rm -f p y.tab.c y.tab.h lex.yy.c *.o y.output

.PHONY: all clean

