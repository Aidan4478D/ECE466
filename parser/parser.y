%{
#include <stdio.h>
#include "ast.h"

void yyerror(char *s);
int yylex();
%}

%code requires {
  #include "ast.h"
}

%union { 
    NUMTYPE number;
    STRTYPE string;
    ast_node_t* ast_node;
}

%token<string> IDENT STRING
%token PLUSEQ MINUSEQ MULTEQ MODEQ DIVEQ SLEQ SREQ ANDEQ XOREQ OREQ
%token POINT PLUSPLUS MINMIN SL SR LTEQ GTEQ EQEQ NOTEQ ANDAND OROR
%token ELLIPSIS AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO ELSE
%token ENUM EXTERN FOR GOTO IF INLINE LONG REGISTER RETURN SHORT
%token SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE
%token INT FLOAT DOUBLE

%token<number> NUMBER

%type<ast_node> primary_expression

%start parser

%%

parser  : primary_expression
        | parser primary_expression  { fprintf(stderr, "parsing a primary expression!\n"); }
        ;

primary_expression  : IDENT { $$ = new_ident($1.string_literal); }
                    | NUMBER { fprintf(stderr, "got a number in parser!\n"); $$ = new_number($1); }
                    | STRING { fprintf(stderr, "got a string in parser!\n"); $$ = new_string($1); }
                    ;
%%

int main(void) {
    yyparse();
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
}

