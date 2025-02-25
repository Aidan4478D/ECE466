%{
#include <stdio.h>
#include "helpers.h"
#include "ast.h"

void yyerror(char *s);
int yylex();

ast_node_t* ast_root = NULL;
void print_ast_tree(ast_node_t *node, int indent);

//int yydebug = 1;

/* Although this is great, it does not include: 
    - Function specifiers (inline)
    - Typdef support
    - Qualifiers like const, volatile, and restrict
    - Initialized declarations
    - Arrays are declarators are assumed to be arr[] or arr[NUMBER] (no variable length arrays)
*/
%}

%union { 
    NUMTYPE number;
    STRTYPE string;
    ast_node_t* ast_node;
}

%token<string> IDENT 
%token<string> STRING
%token<number> NUMBER

%token PLUSEQ MINUSEQ MULTEQ MODEQ DIVEQ SLEQ SREQ ANDEQ XOREQ OREQ
%token POINT PLUSPLUS MINMIN SL SR LTEQ GTEQ EQEQ NOTEQ ANDAND OROR
%token ELLIPSIS AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO ELSE
%token ENUM EXTERN FOR GOTO IF INLINE LONG REGISTER RETURN SHORT
%token SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE
%token INT FLOAT DOUBLE


%type<ast_node> primary_expression postfix_expression

%type<ast_node> subscript_expression component_selection_expression 
%type<ast_node> function_call expression_list
%type<ast_node> post_increment_expression post_decrement_expression
%type<ast_node> type_name

%type<ast_node> declaration_or_fndef declaration function_definition
%type<ast_node> compound_statement decl_or_stmt_list
%type<ast_node> decl_or_stmt decl_specifiers
%type<ast_node> stg_class_specifier type_specifier 
%type<ast_node> struct_union_specifier struct_or_union struct_declaration_list struct_declaration
%type<ast_node> specifier_list struct_declarator_list struct_declarator
%type<ast_node> declarator direct_declarator simple_declarator pointer_declarator
%type<ast_node> pointer array_declarator constant_expression function_declarator
%type<ast_node> parameter_type_list parameter_list parameter_declaration
%type<ast_node> identifier_list


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


%left ','
%left '=' PLUSEQ MINUSEQ MULTEQ DIVEQ MODEQ SLEQ SREQ ANDEQ XOREQ OREQ
%left '?' ':'
%left OROR
%left ANDAND
%left '|'
%left '^' 
%left '&' 
%left EQEQ NOTEQ
%left '<' '>' LTEQ GTEQ
%left SL SR
%left '+' '-'
%left '*' '/' '%'
%right SIZEOF '~' '!'
%left POINT PLUSPLUS MINMIN '(' ')' '[' ']'


%start parser

%%

parser  : declaration_or_fndef
        | parser declaration_or_fndef
        ;

declaration_or_fndef    : declaration           {}
                        | function_definition   {}
                        ;

declaration : decl_specifiers
            ;

function_definition     : decl_specifiers declarator compound_statement {}
                        ;


compound_statement  : '{' decl_or_stmt_list '}' {}
                    ;

decl_or_stmt_list   : decl_or_stmt                      {}
                    | decl_or_stmt_list decl_or_stmt    {}
                    ;

decl_or_stmt    : declaration   {}
                | statement     {}
                ;

decl_specifiers : stg_class_specifier                  {}
                | stg_class_specifier decl_specifiers  {}
                | type_specifier                        {}
                | type_specifier decl_specifiers        {}
                ;

stg_class_specifier : EXTERN    {}
                    | STATIC    {}
                    | AUTO      {}
                    | REGISTER  {}
                    ;

type_specifier  : FLOAT     {}
                | DOUBLE    {}
                | INT       {}
                | LONG      {}
                | SHORT     {}
                | CHAR      {}
                | VOID      {}
                | SIGNED    {}
                | UNSIGNED  {}
                | struct_union_specifier   {}
                ; 

struct_union_specifier : struct_or_union IDENT '{' struct_declaration_list '}'  {}
                       | struct_or_union '{' struct_declaration_list '}'        {}
                       | struct_or_union IDENT                                  {}

struct_or_union : STRUCT    {}
                | UNION     {}
                ;

struct_declaration_list : struct_declaration {}
                        | struct_declaration_list struct_declaration {}
                        ;

struct_declaration  : specifier_list struct_declarator_list ';' {}
                    ;

specifier_list  : type_specifier {}
                | type_specifier specifier_list {}
                ;

struct_declarator_list  : struct_declarator                             {}
                        | struct_declarator_list ',' struct_declarator  {}

struct_declarator   : declarator    {}
                    | declarator ':' constant_expression {}
                    ;


declarator  : pointer_declarator    {}
            | direct_declarator     {}
            ;


direct_declarator   : simple_declarator     {}
                    | '(' declarator ')'    {}
                    | function_declarator   {}
                    | array_declarator      {}
                    ;


simple_declarator   : IDENT {}
                    ; 

pointer_declarator  : pointer direct_declarator   {}
                    ;

pointer : '*'                               {}
        | '*' pointer                       {}
        ;

array_declarator    : direct_declarator '[' ']' {}
                    | direct_declarator '[' constant_expression ']'    {}
                    ;

constant_expression : conditional_expression    {$$ = $1;}
                    ;



function_declarator : direct_declarator '(' ')'                     {}
                    | direct_declarator '(' identifier_list ')'     {}
                    | direct_declarator '(' parameter_type_list ')' {}
                    ;

parameter_type_list : parameter_list                {}
                    | parameter_list ',' ELLIPSIS   {}
                    ;

parameter_list  : parameter_declaration                     {}
                | parameter_list ',' parameter_declaration  {}
                ;

parameter_declaration   : decl_specifiers                        {}
                        ;

identifier_list : IDENT {}
                | parameter_list ',' IDENT {}
                ;


statement : compound_statement
          | expression ';' { ast_root = $1; print_ast_tree(ast_root, 0); }
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
                                                                    ast_node_t* tmp = new_genop(BINOP_N, '+', $1, $3); 
                                                                    $$ = new_unop('*', tmp);
                                                                }
                                                                /* same thing as A[B] == *(A + B) */
                     ;


component_selection_expression : postfix_expression '.' IDENT   {   
                                                                    ast_node_t* tmp = new_ident($3.string_literal); 
                                                                    $$ = new_genop(BINOP_N, '.', $1, tmp);
                                                                }

                               | postfix_expression POINT IDENT {   
                                                                    ast_node_t* i = new_ident($3.string_literal); 
                                                                    ast_node_t* u = new_unop('*', $1);
                                                                    $$ = new_genop(BINOP_N, '.', u, i); 
                                                                }
                               ;

function_call : postfix_expression '(' expression_list ')' { $$ = new_function($1, $3); }
              | postfix_expression '(' ')'  { $$ = new_function($1, NULL); }
              ;

expression_list : assignment_expression  { $$ = new_list($1); }
                | expression_list ',' assignment_expression  { $$ = append_item($1, $3); }
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
                | '(' type_name ')' cast_expression { /* here for now, ignore! */ }
                ;


sizeof_expression : SIZEOF '(' type_name ')'    { $$ = new_unop(SIZEOF, $3); }
                  | SIZEOF unary_expression     { $$ = new_unop(SIZEOF, $2); }
                  ; 


type_name : INT     { $$ = INT;  /* gonna make this a node later */   }
          | CHAR    { $$ = CHAR;    }
          | FLOAT   { $$ = FLOAT;   }
          | DOUBLE  { $$ = DOUBLE;  }
          | LONG    { $$ = LONG;    }
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

preincrement_expression : PLUSPLUS unary_expression {
                                                        NUMTYPE* tmp = (NUMTYPE*) malloc(sizeof(NUMTYPE));
                                                        tmp->type = INT_T;
                                                        tmp->sign = UNSIGNED_T;
                                                        tmp->_int = 1;

                                                        $$ = new_genop(ASSIGNOP_N, '+', $2, new_number(*tmp)); 
                                                    }
                        ;


predecrement_expression : MINMIN unary_expression {
                                                        NUMTYPE* tmp = (NUMTYPE*) malloc(sizeof(NUMTYPE));
                                                        tmp->type = INT_T;
                                                        tmp->sign = UNSIGNED_T;
                                                        tmp->_int = 1;

                                                        $$ = new_genop(ASSIGNOP_N, '-', $2, new_number(*tmp));
                                                  } 
                        ;



multiplicative_expression : cast_expression { $$ = $1; }
                          | multiplicative_expression '*' cast_expression   { $$ = new_genop(BINOP_N, '*', $1, $3); }
                          | multiplicative_expression '/' cast_expression   { $$ = new_genop(BINOP_N, '/', $1, $3); }
                          | multiplicative_expression '%' cast_expression   { $$ = new_genop(BINOP_N, '%', $1, $3); }
                          ;

additive_expression : multiplicative_expression { $$ = $1; }
                    | additive_expression '+' multiplicative_expression     { $$ = new_genop(BINOP_N, '+', $1, $3); }
                    | additive_expression '-' multiplicative_expression     { $$ = new_genop(BINOP_N, '-', $1, $3); }
                    ;

shift_expression : additive_expression  { $$ = $1; }
                 | shift_expression SL additive_expression  { $$ = new_genop(BINOP_N, SL, $1, $3); }
                 | shift_expression SR additive_expression  { $$ = new_genop(BINOP_N, SR, $1, $3); }
                 ;

relational_expression : shift_expression    { $$ = $1; }
                      | relational_expression '<' shift_expression      { $$ = new_genop(COMPOP_N, '<', $1, $3); }
                      | relational_expression GTEQ shift_expression     { $$ = new_genop(COMPOP_N, GTEQ, $1, $3); }
                      | relational_expression '>' shift_expression      { $$ = new_genop(COMPOP_N, '>', $1, $3); }
                      | relational_expression LTEQ shift_expression     { $$ = new_genop(COMPOP_N, LTEQ, $1, $3); }
                      ; 

equality_expression : relational_expression { $$ = $1; }
                   | equality_expression EQEQ relational_expression     { $$ = new_genop(COMPOP_N, EQEQ, $1, $3); }
                   | equality_expression NOTEQ relational_expression    { $$ = new_genop(COMPOP_N, NOTEQ, $1, $3); }
                   ; 

bitwise_or_expression : bitwise_xor_expression  { $$ = $1; }
                      | bitwise_or_expression '|' bitwise_xor_expression    { $$ = new_genop(BINOP_N, '|', $1, $3); }
                      ; 

bitwise_xor_expression : bitwise_and_expression { $$ = $1; }
                       | bitwise_xor_expression '^' bitwise_and_expression  { $$ = new_genop(BINOP_N, '^', $1, $3); }
                       ; 

bitwise_and_expression : equality_expression { $$ = $1; }
                       | bitwise_and_expression '&' equality_expression { $$ = new_genop(BINOP_N, '&', $1, $3); }
                       ;



logical_or_expression : logical_and_expression { $$ = $1; }
                      | logical_or_expression OROR logical_and_expression { $$ = new_genop(LOGOP_N, OROR, $1, $3); }
                      ;

logical_and_expression : bitwise_or_expression  { $$ = $1; }
                       | logical_and_expression ANDAND bitwise_or_expression { $$ = new_genop(LOGOP_N, ANDAND, $1, $3); }
                       ; 


conditional_expression : logical_or_expression  { $$ = $1; }
                       | logical_or_expression '?' expression ':' conditional_expression { $$ = new_ternop($1, $3, $5); }
                       ;


assignment_expression : conditional_expression { $$ = $1; }
                      | unary_expression '=' assignment_expression          { $$ = new_genop(ASSIGNOP_N, '=', $1, $3); }
                      | unary_expression PLUSEQ assignment_expression       { $$ = new_genop(ASSIGNOP_N, PLUSEQ, $1, $3); }
                      | unary_expression MINUSEQ assignment_expression      { $$ = new_genop(ASSIGNOP_N, MINUSEQ, $1, $3); }
                      | unary_expression MULTEQ assignment_expression       { $$ = new_genop(ASSIGNOP_N, MULTEQ, $1, $3); }
                      | unary_expression DIVEQ assignment_expression        { $$ = new_genop(ASSIGNOP_N, DIVEQ, $1, $3); }
                      | unary_expression MODEQ assignment_expression        { $$ = new_genop(ASSIGNOP_N, MODEQ, $1, $3); }
                      | unary_expression SLEQ assignment_expression         { $$ = new_genop(ASSIGNOP_N, SLEQ, $1, $3); }
                      | unary_expression SREQ assignment_expression         { $$ = new_genop(ASSIGNOP_N, SREQ, $1, $3); }
                      | unary_expression ANDEQ assignment_expression        { $$ = new_genop(ASSIGNOP_N, ANDEQ, $1, $3); }
                      | unary_expression XOREQ assignment_expression        { $$ = new_genop(ASSIGNOP_N, XOREQ, $1, $3); }
                      | unary_expression OREQ assignment_expression         { $$ = new_genop(ASSIGNOP_N, OREQ, $1, $3); }
                      ; 


comma_expression : assignment_expression { $$ = $1; }
                 | comma_expression ',' assignment_expression { $$ = new_genop(BINOP_N, ',', $1, $3); }
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

