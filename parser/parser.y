%{
#include <stdio.h>
#include "ast.h"

void yyerror(char *s);
int yylex();

//TO DO: 
// - list stuff
// - set precedences
// - generally test if it works lol
%}

%code requires { // would not import without this for some reason (ChatGPT came up with this guy)
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

%type<ast_node> primary_expression postfix_expression

%type<ast_node> subscript_expression component_selection_expression 
%type<ast_node> function_call expression_list
%type<ast_node> post_increment_expression post_decrement_expression
%type<ast_node> type_name

%type<ast_node> cast_expression unary_expression
%type<ast_node> sizeof_expression
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

parser  : expression
        | parser expression  { fprintf(stderr, "parsing an expression!\n"); }
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
                    ;

subscript_expression : postfix_expression '[' expression ']'    {   
                                                                    ast_node_t* tmp = new_binop('+', $1, $3); 
                                                                    $$ = new_unop('*', tmp);
                                                                }
                                                                /* same thing as A[B] == *((A) + (B)) */
                     ;


component_selection_expression : postfix_expression '.' IDENT   {   
                                                                    ast_node_t* tmp = new_ident($3.string_literal); 
                                                                    $$ = new_binop('.', $1, tmp);
                                                                }

                               | postfix_expression POINT IDENT {   
                                                                    ast_node_t* i = new_ident($3.string_literal); 
                                                                    ast_node_t* u = new_unop('*', $1);
                                                                    $$ = new_binop('.', u, i); 
                                                                }
                               ;

function_call : postfix_expression '(' expression_list ')' { $$ = new_function($1, $3); }
              | postfix_expression '(' ')'  { $$ = new_function($1, NULL); }
              ;

expression_list : assignment_expression { $$ = $1; }
                | expression_list ',' assignment_expression { $$ = $1; /* I will eventually do something here maybe make a list*/ }
                ;

post_increment_expression : postfix_expression PLUSPLUS {$$ = new_unop(PLUSPLUS, $1); }
                          ;

post_decrement_expression : postfix_expression MINMIN { $$ = new_unop(MINMIN, $1); }
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


cast_expression : unary_expression  { $$ = $1; }
                | '(' type_name ')' cast_expression { /* is this a unary expression? */ }
                ;


sizeof_expression : SIZEOF '(' type_name ')'    { $$ = new_unop(SIZEOF, $3); }
                  | SIZEOF unary_expression     { $$ = new_unop(SIZEOF, $2); }
                  ; 


type_name : INT     { $$ = INT; }
          | CHAR    { $$ = CHAR; }
          | FLOAT   { $$ = FLOAT; }
          | DOUBLE  { $$ = DOUBLE; }
          | LONG    { $$ = LONG; }
          ;


unary_minus_expression : '-' cast_expression    { $$ = new_unop('-', $2); }
                       ;

unary_plus_expression : '+' cast_expression     { $$ = new_unop('+', $2); }
                      ;

logical_negation_expression : '!' cast_expression   { $$ = new_unop('!', $2); }
                            ;

bitwise_negation_expression : '~' cast_expression   { $$ = new_unop('~', $2); }
                            ;

address_expression : '&' cast_expression { $$ = new_unop('&', $2); }
                   ;

indirection_expression : '*' cast_expression { $$ = new_unop('*', $2); }
                    ;

preincrement_expression : PLUSPLUS unary_expression {$$ = new_unop(PLUSPLUS, $2); }
                        ;

predecrement_expression : MINMIN unary_expression {$$ = new_unop(MINMIN, $2); }
                        ;



multiplicative_expression : cast_expression { $$ = $1; }
                          | multiplicative_expression '*' cast_expression   { $$ = new_binop('*', $1, $3); }
                          | multiplicative_expression '/' cast_expression   { $$ = new_binop('/', $1, $3); }
                          | multiplicative_expression '%' cast_expression   { $$ = new_binop('%', $1, $3); }
                          ;

additive_expression : multiplicative_expression { $$ = $1; }
                    | additive_expression '+' multiplicative_expression     { $$ = new_binop('+', $1, $3); }
                    | additive_expression '-' multiplicative_expression     { $$ = new_binop('-', $1, $3); }
                    ;

shift_expression : additive_expression  { $$ = $1; }
                 | shift_expression SL additive_expression  { $$ = new_binop(SL, $1, $3); }
                 | shift_expression SR additive_expression  { $$ = new_binop(SR, $1, $3); }
                 ;

relational_expression : shift_expression    { $$ = $1; }
                      | relational_expression '<' shift_expression      { $$ = new_binop('<', $1, $3); }
                      | relational_expression GTEQ shift_expression     { $$ = new_binop(GTEQ, $1, $3); }
                      | relational_expression '>' shift_expression      { $$ = new_binop('>', $1, $3); }
                      | relational_expression LTEQ shift_expression     { $$ = new_binop(LTEQ, $1, $3); }
                      ; 

equality_expression : relational_expression { $$ = $1; }
                   | equality_expression EQEQ relational_expression     { $$ = new_binop(EQEQ, $1, $3); }
                   | equality_expression NOTEQ relational_expression    { $$ = new_binop(NOTEQ, $1, $3); }
                   ; 

bitwise_or_expression : bitwise_xor_expression  { $$ = $1; }
                      | bitwise_or_expression '|' bitwise_xor_expression    { $$ = new_binop('|', $1, $3); }
                      ; 

bitwise_xor_expression : bitwise_and_expression { $$ = $1; }
                       | bitwise_xor_expression '^' bitwise_and_expression  { $$ = new_binop('^', $1, $3); }
                       ; 

bitwise_and_expression : equality_expression { $$ = $1; }
                       | bitwise_and_expression '&' equality_expression { $$ = new_binop('&', $1, $3); }
                       ;



logical_or_expression : logical_and_expression { $$ = $1; }
                      | logical_or_expression OROR logical_and_expression { $$ = new_binop(OROR, $1, $3); }
                      ;

logical_and_expression : bitwise_or_expression  { $$ = $1; }
                       | logical_and_expression ANDAND bitwise_or_expression { $$ = new_binop(ANDAND, $1, $3); }
                       ; 


conditional_expression : logical_or_expression  { $$ = $1; }
                       | logical_or_expression '?' expression ':' conditional_expression { $$ = new_ternop($1, $3, $5); }
                       ;



assignment_expression : conditional_expression { $$ = $1; }
                      | unary_expression '=' assignment_expression          { $$ = new_binop('=', $1, $3); }
                      | unary_expression PLUSEQ assignment_expression       { $$ = new_binop(PLUSEQ, $1, $3); }
                      | unary_expression MINUSEQ assignment_expression      { $$ = new_binop(MINUSEQ, $1, $3); }
                      | unary_expression MULTEQ assignment_expression       { $$ = new_binop(MULTEQ, $1, $3); }
                      | unary_expression DIVEQ assignment_expression        { $$ = new_binop(DIVEQ, $1, $3); }
                      | unary_expression MODEQ assignment_expression        { $$ = new_binop(MODEQ, $1, $3); }
                      | unary_expression SLEQ assignment_expression         { $$ = new_binop(SLEQ, $1, $3); }
                      | unary_expression SREQ assignment_expression         { $$ = new_binop(SREQ, $1, $3); }
                      | unary_expression ANDEQ assignment_expression        { $$ = new_binop(ANDEQ, $1, $3); }
                      | unary_expression XOREQ assignment_expression        { $$ = new_binop(XOREQ, $1, $3); }
                      | unary_expression OREQ assignment_expression         { $$ = new_binop(OREQ, $1, $3); }
                      ; 


comma_expression : assignment_expression { $$ = $1; }
                 | comma_expression ',' assignment_expression { $$ = new_binop(',', $1, $3); }
                 ;

expression : comma_expression { $$ = $1; }
           ;

%%

int main(void) {
    yyparse();
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
}

