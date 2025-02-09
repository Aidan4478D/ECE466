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

%token<string> IDENT 
%token TOKEOF STRING PLUSEQ MINUSEQ MULTEQ MODEQ DIVEQ SLEQ SREQ ANDEQ XOREQ OREQ
%token POINT PLUSPLUS MINMIN SL SR LTEQ GTEQ EQEQ NOTEQ ANDAND OROR
%token ELLIPSIS AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO ELSE
%token ENUM EXTERN FOR GOTO IF INLINE LONG REGISTER RETURN SHORT
%token SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE
%token INT FLOAT DOUBLE

%token NUMBER

%type<ast_node> primary_expression

%%

primary_expression  : IDENT { 
    fprintf(stderr, "ident: %s\n", $1.string_literal); 
    $$ = new_ident($1.string_literal); 
}
;
%%

int main(void) {
    yyparse();
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
}

