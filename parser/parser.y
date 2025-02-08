%{
#include <ctype.h>
%}

%token DIGIT

%%

line    : expr '\n'     { fprintf("%d\n", $1); }
        ;

expr    : expr '+' term { $$ = $1 + $3; }
        | term
        ; 

term    : term '*' factor { $$ = $1 * $3; }
        | factor
        ; 

factor  : '(' expr ')'  { $$ = $2; }
        | DIGIT
        ;

%%

yylex() {
    c = getchar();
    if(isdigit(c)) {
        yylval = c-'0';
        return DIGIT
    }
    return c;
}
