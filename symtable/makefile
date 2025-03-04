CC = gcc
CFLAGS = -Wall

all: parser clean_turn

parser: y.tab.o lex.yy.o ast.o helpers.o symtable.o hash.o stack.o
	$(CC) $(CFLAGS) -o p y.tab.o lex.yy.o ast.o helpers.o symtable.o hash.o stack.o -lfl

y.tab.c y.tab.h: parser.y
	yacc -d parser.y

y.tab.o: y.tab.c y.tab.h ast.h symtable.h
	$(CC) $(CFLAGS) -c y.tab.c -ll -ly

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

lex.yy.o: lex.yy.c y.tab.h ast.h
	$(CC) $(CFLAGS) -c lex.yy.c

ast.o: ast.c ast.h
	$(CC) $(CFLAGS) -c ast.c

helpers.o: helpers.c helpers.h
	$(CC) $(CFLAGS) -c helpers.c

symtable.o: ast.o symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) -c hash.c

stack.o: stack.c stack.h symtable.h
	$(CC) $(CFLAGS) -c stack.c


clean_turn:
	rm -f *.o *.gch

clean:
	rm -f p y.tab.c y.tab.h lex.yy.c *.o y.output *.gch *.out

.PHONY: all clean

