%{
#include <stdio.h>
#include "helpers.h"
#include "stack.h"
#include "symtable.h"

void yyerror(char *s);
int yylex();

ast_node_t* ast_root = NULL;
void print_ast_tree(ast_node_t *node, int indent);
int in_struct_union = 0;

// create a new stack and initialze it
stack_t* scope_stack;

//int yydebug = 1;

/* Although this is great, it does not include: 
    - Function specifiers (inline)
    - Typdef support
    - Qualifiers like const, volatile, and restrict
    - Initialized declarations
    - Arrays are declarators are assumed to be arr[] or arr[NUMBER] (no variable length arrays)

*/
%}

%code requires {
    #include "symtable.h"
}


%union { 
    NUMTYPE number;
    STRTYPE string;
    ast_node_t* ast_node;
    SYMBOL* symbol;
    int TOKEN;
}

%token<string> IDENT 
%token<string> STRING
%token<number> NUMBER

%token<TOKEN> PLUSEQ MINUSEQ MULTEQ MODEQ DIVEQ SLEQ SREQ ANDEQ XOREQ OREQ
%token<TOKEN> POINT PLUSPLUS MINMIN SL SR LTEQ GTEQ EQEQ NOTEQ ANDAND OROR
%token<TOKEN> ELLIPSIS AUTO BREAK CASE CHAR CONST CONTINUE DEFAULT DO ELSE
%token<TOKEN> ENUM EXTERN FOR GOTO IF INLINE LONG REGISTER RETURN SHORT
%token<TOKEN> SIGNED SIZEOF STATIC STRUCT SWITCH TYPEDEF UNION UNSIGNED VOID VOLATILE WHILE
%token<TOKEN> INT FLOAT DOUBLE


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
%type<symbol> direct_declarator declarator
%type<symbol> pointer_declarator
%type<symbol> function_declarator array_declarator
%type<string> simple_declarator
%type<ast_node> pointer constant_expression
%type<ast_node> identifier_list

%type<symbol> init_declarator_list init_declarator


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

declaration_or_fndef    : declaration           { ast_root = $1; /*print_ast_tree(ast_root, 0); print_sym_table(stack_peek(scope_stack));*/ }
                        | function_definition   {}
                        ;

declaration : decl_specifiers ';'   { $$ = $1; }
            | decl_specifiers init_declarator_list ';'  { 
                                                            SYMBOL *sym = $2;
                                                            SYMTABLE *cur_scope = stack_peek(scope_stack); 

                                                            while (sym != NULL) {
                                                                fprintf(stderr, "Symbol Type: %s\n", get_symbol_type(sym->type));
                                                                for (ast_node_t *spec = $1; spec != NULL; spec = spec->list.next) {
                                                                    //fprintf(stderr, "  DeclSpec: decl_type=%d, stg_class=%d\n", 
                                                                    //spec->decl_spec.decl_type, spec->decl_spec.stg_class);
                                                                    
                                                                    if(spec->decl_spec.stg_class) sym->stg_class = spec->decl_spec.stg_class; 
                                                                    if(spec->decl_spec.decl_type) sym->node = combine_nodes(spec, sym->node);
                                                                }
                                                                fprintf(stderr, "Storage Class: %s\n", get_storage_class(sym->stg_class)); 

                                                                print_ast_tree(sym->node, 0);

                                                                //st_install(cur_scope, sym);
                                                                //print_sym_table(cur_scope);
                                                                  
                                                                //fprintf(stderr, "Declarators:\n");
                                                                //fprintf(stderr, "Symbol: %s\n", sym->key);
                                                                sym = sym->next;
                                                            }
                                                        }
            ;

init_declarator_list : init_declarator
                     | init_declarator_list ',' init_declarator {
                                                                    SYMBOL* p = $1;
                                                                    while (p->next != NULL)
                                                                        p = p->next;
                                                                    p->next = $3; 
                                                                    $$ = $1;
                                                                }

init_declarator : declarator
                ; 

function_definition     : decl_specifiers declarator compound_statement {   
                                                                            fprintf(stderr, "Entering function scope\n"); 
                                                                            //ast_node_t* final_type = combine_nodes($1, $2);
                                                                            //SYMTABLE* global = stack_peek(scope_stack);
                                                    
                                                                            // assume identifier comes from direct_declarator
                                                                            //char* funct_name = "";
                                                                            //if($2 && $2->type == IDENT_N) funct_name = $2->ident.name;

                                                                            //SYMBOL* funct_sym = st_new_symbol(funct_name, final_type, GENERAL_NS, FUNCT_SYM, EXTERN_SC); 
                                                                            //st_install(global, funct_sym);
                                                                            //print_sym_table(global);

                                                                            //stack_push(scope_stack, st_create(FUNCT_SCOPE, global));
                                                                            
                                                                        }
                        ;


compound_statement  : '{'   {
                                fprintf(stderr, "Entering block or function scope\n");
                                SYMTABLE* current = stack_peek(scope_stack);
                                stack_push(scope_stack, st_create((current->scope == FILE_SCOPE ? FUNCT_SCOPE : BLOCK_SCOPE), current));
                            } 
                      '}'   {
                                SYMTABLE* st = stack_pop(scope_stack);
                                fprintf(stderr, "Exiting block/funct scope\n"); 
                            }
                    | '{'   {
                                fprintf(stderr, "Entering block or function scope\n");
                                SYMTABLE* current = stack_peek(scope_stack);
                                stack_push(scope_stack, st_create((current->scope == FILE_SCOPE ? FUNCT_SCOPE : BLOCK_SCOPE), current));
                            } 
                      '}'   {
                                SYMTABLE* st = stack_pop(scope_stack);
                                fprintf(stderr, "Exiting block/funct scope\n"); 
                            }

                    ;

decl_or_stmt_list   : decl_or_stmt
                    | decl_or_stmt_list decl_or_stmt
                    ;

decl_or_stmt    : declaration {fprintf(stderr, "reducing a declaration\n"); }
                | statement {fprintf(stderr, "reducing a statement\n"); }
                ;

decl_specifiers : stg_class_specifier                   { $$ = $1; }
                | stg_class_specifier decl_specifiers   { $$ = append_item($2, $1); }
                | type_specifier                        { $$ = $1; }
                | type_specifier decl_specifiers        { $$ = append_item($2, $1); }
                ;

stg_class_specifier : EXTERN    { $$ = new_decl_spec(0, EXTERN_SC); }
                    | STATIC    { $$ = new_decl_spec(0, STATIC_SC); }
                    | AUTO      { $$ = new_decl_spec(0, AUTO_SC); }
                    | REGISTER  { $$ = new_decl_spec(0, REG_SC); }
                    ;

type_specifier  : FLOAT     { $$ = new_decl_spec(FLOAT_DT, 0);   }
                | DOUBLE    { $$ = new_decl_spec(DOUBLE_DT, 0);  }
                | INT       { $$ = new_decl_spec(INT_DT, 0);     }
                | LONG      { $$ = new_decl_spec(LONG_DT, 0);    }
                | SHORT     { $$ = new_decl_spec(SHORT_DT, 0);   }
                | CHAR      { $$ = new_decl_spec(CHAR_DT, 0);    }
                | VOID      { $$ = new_decl_spec(VOID_DT, 0);    }
                | SIGNED    { $$ = new_decl_spec(SIGNED_DT, 0);  }
                | UNSIGNED  { $$ = new_decl_spec(UNSIGNED_DT, 0); }
                | struct_union_specifier
                ; 



struct_union_specifier : struct_or_union IDENT '{' struct_declaration_list '}'
                       | struct_or_union '{' struct_declaration_list '}'
                       | struct_or_union IDENT


struct_or_union : STRUCT
                | UNION
                ;


struct_declaration_list : struct_declaration
                        | struct_declaration_list struct_declaration
                        ;

struct_declaration  : specifier_list struct_declarator_list ';'
                    ;


specifier_list  : type_specifier                { $$ = $1; }
                | type_specifier specifier_list { $$ = append_item($1, $2); }
                ;


struct_declarator_list  : struct_declarator                             { $$ = $1; }
                        | struct_declarator_list ',' struct_declarator  { $$ = append_item($1, $3); }

struct_declarator   : declarator {$$ = $1; }
                    ;


declarator  : pointer_declarator  { $$ = $1; }
            | direct_declarator   { $$ = $1; }
            ;


direct_declarator   : simple_declarator     { $$ = st_new_symbol($1.string_literal, NULL, GENERAL_NS, VAR_SYM, UNKNOWN_SC); }
                    | '(' declarator ')'    { $$ = $2; }
                    | function_declarator
                    | array_declarator
                    ;


simple_declarator   : IDENT
                    ; 

pointer_declarator  : pointer direct_declarator   { $$ = $2; $2->node = $1; fprintf(stderr, "adding ptr\n");  }
                    ;

pointer : '*'           { $$ = new_pointer(NULL); }
        | '*' pointer   { $$ = new_pointer($2);   }
        ;

array_declarator    : direct_declarator '[' ']'                         { 
                                                                            ast_node_t* temp = new_array($1->node, NULL);
                                                                            
                                                                            SYMBOL* sym = $1;
                                                                            if(sym->node) sym->node = combine_nodes(sym->node, temp);
                                                                            else sym->node = temp;

                                                                            $$ = sym;
                                                                        }
                    | direct_declarator '[' constant_expression ']'     { 
                                                                            ast_node_t* temp = new_array($1->node, $3); 

                                                                            SYMBOL* sym = $1; 

                                                                            if(sym->node) sym->node = combine_nodes(sym->node, temp);
                                                                            else sym->node = temp;

                                                                            $$ = sym;
                                                                        }
                    ;

constant_expression : conditional_expression    {$$ = $1;}
                    ;



function_declarator : direct_declarator '(' ')'                     {   
                                                                        //fprintf(stderr, "function declarator detected\n"); 
                                                                        ast_node_t* temp = new_function($1->node, NULL); 

                                                                        SYMBOL* sym = $1;

                                                                        if(sym->type == VAR_SYM) sym->type = FUNCT_SYM;

                                                                        if(sym->node) sym->node = combine_nodes(sym->node, temp);
                                                                        else sym->node = temp;

                                                                        $$ = sym;
                                                                    }
                    | direct_declarator '(' identifier_list ')'     { 
                                                                        ast_node_t* temp = new_function($1->node, $3); 
                                                                        SYMBOL* sym = $1;

                                                                        if(sym->type == VAR_SYM) sym->type = FUNCT_SYM;

                                                                        if(sym->node) sym->node = combine_nodes(sym->node, temp);
                                                                        else sym->node = temp;

                                                                        $$ = sym;
                                                                    }
                    ;


identifier_list : IDENT
                | identifier_list ',' IDENT
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


type_name : INT
          | CHAR
          | FLOAT
          | DOUBLE
          | LONG
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
    scope_stack = (stack_t*) malloc(sizeof(stack_t));
    stack_init(scope_stack);
    stack_push(scope_stack, st_create(FILE_SCOPE, NULL)); 
    yyparse(); 
    return 0;
}


void yyerror(char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
}

