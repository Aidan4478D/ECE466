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

%type<ast_node> expression
%type<ast_node> primary_expression postfix_expression

%type<ast_node> subscript_expression component_selection_expression 
%type<ast_node> function_call expression_list assignment_expression
%type<ast_node> post_increment_expression post_decrement_expression
%type<ast_node> compound_literal type_name initializer_list

%type<ast_node> cast_expression unary_expression
%type<ast_node> sizeof-expression
%type<ast_node> unary_minus_expression
%type<ast_node> unary_plus_expression
%type<ast_node> logical_negation_expression
%type<ast_node> bitwise_negation_expression
%type<ast_node> address_expression
%type<ast_node> indirection_expression
%type<ast_node> preincrement_expression
%type<ast_node> predecrement_expression

%type<ast_node> multiplicative_expression
%type<ast_node> additive_expression
%type<ast_node> shift_expression
%type<ast_node> relational_expression
%type<ast_node> equality_expression
%type<ast_node> bitwise_and_expression
%type<ast_node> bitwise_xor_expression
%type<ast_node> bitwise_or_expression

%type<ast_node> logical_or_expression
%type<ast_node> logical_and_expression

%type<ast_node> conditional_expression

%type<ast_node> assignment_expression

%type<ast_node> comma_expression
%type<ast_node> expression


%start parser

%%

parser  : primary_expression
        | parser primary_expression  { fprintf(stderr, "parsing a primary expression!\n"); }
        ;

primary_expression  : IDENT { $$ = new_ident($1.string_literal); }
                    | NUMBER { $$ = new_number($1); }
                    | STRING { $$ = new_string($1); }
                    | '(' expression ')' { $$ = $2; } /* can be any expression supposedly */
                    ;



postfix_expression  : primary_expression
                    | subscript_expression
                    | component_selection_expression
                    | function_call
                    | post_increment_expression
                    | post_decrement_expression
                    | compound_literal
                    ;

subscript_expression : postfix_expression '[' expression ']' /* same thing as A[B] == *((A) + (B)) */
                     ;


component_selection_expression : postfix_expression '.' IDENT /* do stuff */
                               | postfix_expression POINT IDENT /* do stuff */
                               ;

function_call : postfix_expression '(' expression_list ')' /* do stuff */
              | postfix_expression '(' ')'
              ;

expression_list : assignment_expression
                | expresison_list ',' assignment_expression /* do stuff */
                ;

post_increment_expression : postfix_expression PLUSPLUS /* do stuff */
                          ;

post_decrement_expression : postfix_expression MINMIN /* do stuff */
                          ;

compund_literal : '(' type_name ')' '{' initializer_list ',' '}' /* do stuff */
                | '(' type_name ')' '{' initializer_list '}' /* do stuff */
                ; 




unary_expression : postfix_expression /* have precedence lower than postfix but higher than binary and ternary */
                 | sizeof_expression
                 | unary_minus_expression
                 | unary_plus_expression
                 | logical_negation_expression
                 | bitwise_negation_expression
                 | address_expression
                 | indirection_expression
                 | preincrement_expression
                 | predecrement_expression
                 ;


cast_expression : unary_expression
                | '(' type_name ')' cast_expression
                ;

sizeof_expression : sizeof '(' type_name ')'
                  | sizeof unary_expression
                  ; 

unary_minus_expression : '-' cast_expression
                       ;

unary_plus_expression : '+' cast_expression
                      ;

logical_negation_expression : '!' cast_expression
                            ;

bitwise_negation_expression : '~' cast_expression
                            ;

address_expression : '&' cast_expression
                   ;

preincrement_expression : PLUSPLUS unary_expression
                        ;

predecrement_expression : MINMIN unary_expression
                        ;



multiplicative_expression : cast_expression
                          | multiplicative_expression '*' cast_expression
                          | multiplicative_expression '/' cast_expression
                          | multiplicative_expression '%' cast_expression
                          ;

additive_expression : multiplicative_expression
                    | additive_expression '+' multiplicative_expression
                    | additive_expression '-' multiplicative_expression
                    ;

shift_expression : additive_expression
                 | shift_expression SL additive_expression
                 | shift_expression SR additive_expression
                 ;

relational_expression : shift_expression
                      | relational_expression '<' shift_expression
                      | relational_expression GTEQ shift_expression
                      | relational_expression '>' shift_expression
                      | relational_expression LTEQ shift_expression
                      ; 

equality_expression : relational_expression
                   | equality_expression EQEQ relational_expression
                   | equality_expression NOTEQ relational_expression
                   ; 

bitwise_or_expression : bitwise_xor_expression
                      | bitwise_or_expression '|' bitwise_xor_expression
                      ; 

bitwise_xor_expression : bitwise_and_expression
                       | bitwise_xor_expression '^' bitwise_and_expression
                       ; 

bitwise_and_expression : equality_expression
                       | bitwise_and_expression '&' equality_expression
                       ;



logical_or_expression : logical_and_expression
                      | logical_or_expression OROR logical_and_expression
                      ;

logical_and_expression : bitwise_or_expression
                       | logical_and_expression ANDAND bitwise_or_expression
                       ; 


conditional_expression : logical_or_expression
                       | logical_or_expression '?' expression ':' conditional_expression
                       ;



assignment_expression : conditional_expression
                      | unary_expression '=' assignment_expression
                      | unary_expression PLUSEQ assignment_expression
                      | unary_expression MINUSEQ assignment_expression
                      | unary_expression MULTEQ assignment_expression
                      | unary_expression DIVEQ assignment_expression
                      | unary_expression MODEQ assignment_expression
                      | unary_expression SLEQ assignment_expression
                      | unary_expression SREQ assignment_expression
                      | unary_expression ANDEQ assignment_expression
                      | unary_expression XOREQ assignment_expression
                      | unary_expression OREQ assignment_expression
                      ; 


comma_expression : assignment_expression
                 | comma_expression ',' assignment_expression
                 ;

expression : comma_expression
           ;

%%

int main(void) {
    yyparse();
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
}

