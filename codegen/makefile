CC = gcc -g -O0
CFLAGS = -Wall

all: parser clean_turn

parser: y.tab.o lex.yy.o ast.o printing.o symtable.o hash.o stack.o quads.o linklist.o codegen.o
	$(CC) $(CFLAGS) -o p y.tab.o lex.yy.o ast.o printing.o symtable.o hash.o stack.o quads.o linklist.o codegen.o -lfl

y.tab.c y.tab.h: parser.y
	yacc -d parser.y

y.tab.o: y.tab.c y.tab.h ast.h symtable.h
	$(CC) $(CFLAGS) -c y.tab.c -ll -ly

lex.yy.c: lexer.l y.tab.h
	flex lexer.l

lex.yy.o: lex.yy.c y.tab.h ast.h
	$(CC) $(CFLAGS) -c lex.yy.c

ast.o: ast.c ast.h symtable.h
	$(CC) $(CFLAGS) -c ast.c


symtable.o: ast.o symtable.c symtable.h ast.h
	$(CC) $(CFLAGS) -c symtable.c

quads.o: quads.c quads.h symtable.h
	$(CC) $(CFLAGS) -c quads.c

codegen.o: codegen.c codegen.h quads.h
	$(CC) $(CFLAGS) -c codegen.c



linklist.o: helpers/linklist.c helpers/linklist.h
	$(CC) $(CFLAGS) -c helpers/linklist.c

hash.o: helpers/hash.c helpers/hash.h
	$(CC) $(CFLAGS) -c helpers/hash.c

stack.o: helpers/stack.c helpers/stack.h symtable.h
	$(CC) $(CFLAGS) -c helpers/stack.c

printing.o: helpers/printing.c helpers/printing.h
	$(CC) $(CFLAGS) -c helpers/printing.c

assemble:
	gcc -m32 file_out.S -o compiled.exe

clean_turn:
	rm -f *.o *.gch

clean:
	rm -f p y.tab.c y.tab.h lex.yy.c *.o y.output *.gch *.out

.PHONY: all clean

