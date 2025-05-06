%{
#include <stdio.h>
#include <stdlib.h>

#include "helpers/printing.h"
#include "helpers/stack.h"
#include "symtable.h"
#include "quads.h"

#define YYDEBUG 1

void yyerror(char *s);
int yylex();
int yydebug = 0;

ast_node_t* ast_root = NULL;
void print_ast_tree(ast_node_t *node, int indent);

// create a new stack and initialze it
stack_t* scope_stack;
stack_t* struct_union_stack;

// this is sketch but a quick fix
int in_function = 0; 
int global_scope_updated = 0;

/* Although this is great, it does not include: 
    - Function specifiers (inline)
    - Typdef support
    - Qualifiers like const, volatile, and restrict
    - Initialized declarations
    - Arrays are declarators are assumed to be arr[] or arr[NUMBER] (no variable length arrays)
    - Does not support K&R function declarations and definitions (ex. int add(a, b) int a; int b; { return a + b; } <== WILL NOT WORK!)
*/

/* known issues: 
    - can enter things like "int double signed char x;" and it would work
    - void (*g(double d))(int, char); mixes up the parameter lists
*/

/* fix:
    - out of order parameters for function pointers
    - functions returning functions are weird af
*/
%}



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
%type<ast_node> struct_union_specifier struct_declaration_list struct_declaration
%type<ast_node> specifier_list 
%type<ast_node> pointer
%type<ast_node> identifier_list
%type<ast_node> parameter_list parameter_declaration
%type<ast_node> abstract_declarator direct_abstract_declarator

%type<symbol>struct_declarator_list struct_declarator
%type<symbol> direct_declarator declarator
%type<symbol> pointer_declarator
%type<symbol> function_declarator array_declarator
%type<symbol> init_declarator_list init_declarator

%type<TOKEN> struct_or_union
%type<string> simple_declarator

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

%type<ast_node> labeled_statement 
%type<ast_node> conditional_statement if_statement if_else_statement
%type<ast_node> iterative_statement do_statement while_statement for_statement
%type<ast_node> switch_statement
%type<ast_node> break_statement
%type<ast_node> continue_statement
%type<ast_node> return_statement
%type<ast_node> goto_statement
%type<ast_node> null_statement

%type<symbol> named_label
%type<ast_node> switch_label


%type<ast_node> constant_expression
%type<ast_node> statement

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
%left POINT PLUSPLUS MINMIN 
%left '(' ')' '[' ']'
%left IF
%left ELSE


%start parser

%%

parser  : declaration_or_fndef          { ast_root = $1; }
        | parser declaration_or_fndef
        ;

declaration_or_fndef    : declaration           {}
                        | function_definition   {}
                        ;

declaration : decl_specifiers ';'   { $$ = $1; }
            | decl_specifiers init_declarator_list ';'  { 
                                                            SYMBOL *sym = $2;
                                                            SYMTABLE *cur_scope = (SYMTABLE*) stack_peek(scope_stack); 
                                                            ast_node_t *spec = $1; //dec specs

                                                            //fprintf(stderr, "current scope is: %s\n", get_scope_name(cur_scope->scope));
                                                            if (!cur_scope) {
                                                                fprintf(stderr, "Error: No current scope available\n");
                                                                exit(1);
                                                            }

                                                            while (sym != NULL) {
                                                                process_declaration(cur_scope, sym, spec);                                                               
                                                                sym = sym->next;
                                                            }
                                                            //fprintf(stderr, "making decl node\n"); 
                                                            $$ = new_decl(spec, $2);
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

function_definition : decl_specifiers declarator    {
                                                        SYMBOL* sym = $2;
                                                        ast_node_t *spec = $1; //dec specs

                                                        if (sym->type == FUNCT_SYM) {

                                                            SYMTABLE* global = (SYMTABLE*) stack_peek(scope_stack);
                                                            sym->node = combine_nodes($1, sym->node);
    
                                                            //install function into global scope
                                                            process_declaration(global, sym, spec);                                                               
                                                            
                                                            // function scope for function body
                                                            SYMTABLE* funct_scope = st_create(FUNCT_SCOPE, global);
                                                            stack_push(scope_stack, funct_scope);


                                                            // prototype scope
                                                            if (sym->type == FUNCT_SYM && sym->node->type == FUNCT_N && sym->node->function.params) {
                                                                SYMTABLE* proto_scope = st_create(PROTO_SCOPE, funct_scope);
                                                                stack_push(scope_stack, proto_scope);
                                                                ast_node_t* param_list = sym->node->function.params;

                                                                // add parameters to proto scope
                                                                int cnt = 1;
                                                                while (param_list) {
                                                                    ast_node_t* param_node = param_list->list.head;
                                                                    if (param_node->type == PARAM_N) {

                                                                        char* key = (param_node->parameter.ident && param_node->parameter.ident->type == IDENT_N) ? param_node->parameter.ident->ident.name : NULL;

                                                                        if(key) {
                                                                            SYMBOL* param_sym = st_new_symbol(key, param_node, GENERAL_NS, VAR_SYM, AUTO_SC, NULL, file_name, line_num);
                                                                            st_install(proto_scope, param_sym);
                                                                        }
                                                                        else fprintf(stderr, "No key associated with input parameter #%d\n", cnt++); 
                                                                    }
                                                                    param_list = param_list->list.next;
                                                                }

                                                                // "promote" symbols within proto scope to funct scope (if there are any)
                                                                ht_t *ht = proto_scope->ht;
                                                                if(ht->filled) {
                                                                    //print_sym_table(proto_scope);
                                                                    for (int i = 0; i < ht->capacity; i++) {
                                                                        hash_item *item = &ht->data[i];
                                                                        if (item->isOccupied && !item->isDeleted) {
                                                                            SYMBOL *proto_sym = (SYMBOL *) item->pv;
                                                                            //fprintf(stderr, "moving key %s from proto scope to funct scope\n", proto_sym->key);
                                                                            st_install(funct_scope, proto_sym); 
                                                                        }
                                                                    }
                                                                }
                                                                
                                                                //pop proto_scope off stack
                                                                stack_pop(scope_stack);
                                                            }
                                                            fprintf(stderr, "\nEntering function scope for '%s'\n", sym->key);
                                                            //print_sym_table(funct_scope);
                                                            print_symbol(global, sym); 
                                                            in_function = 1;
                                                        }
                                                        else {
                                                            fprintf(stderr, "Expected function definition for symbol: %s", sym->key);
                                                            exit(1);
                                                        }
                                                    } 
                                                    compound_statement { 
                                                        //fprintf(stderr, "Exiting function scope\n");
                                                        SYMBOL* sym = $2;

                                                        printf("\n---------------------------------------------\n"); 
                                                        printf("AST Dump for function %s\n", sym->key); 
                                                        printf("---------------------------------------------\n"); 
                                                        fprintf(stderr, "=========== AST DUMP =============\n");
                                                        print_ast_tree($4, 0);

                                                        printf("\n---------------------------------------------\n"); 
                                                        printf("QUAD generation for function %s\n", sym->key); 
                                                        printf("---------------------------------------------\n"); 
                                                        fprintf(stderr, "=========== GEN QUADS ============\n");
                                                        create_quads($4);
                                                        printf("\n\n\n\n");
                                                        in_function = 0;

                                                        
                                                        SYMTABLE* st = (SYMTABLE*) stack_pop(scope_stack);
                                                        fprintf(stderr, "Exiting %s scope started at: <%s>:%d\n", get_scope_name(st->scope), st->start_file, st->start_line);
                                                    }
                    ;


compound_statement  : '{'   {
                                if(in_function != 1) {
                                    SYMTABLE* current = (SYMTABLE*) stack_peek(scope_stack);
                                    SYMTABLE* new_scope = st_create(BLOCK_SCOPE, current);
                                    stack_push(scope_stack, new_scope);
                                    fprintf(stderr, "Entering %s scope: <%s>:%d\n", get_scope_name(new_scope->scope), file_name, line_num);
                                }
                                in_function = 0;
                            } 
                      '}'   { $$ = new_list(NULL); }
                    | '{'   {
                                if(in_function != 1) {
                                    SYMTABLE* current = (SYMTABLE*) stack_peek(scope_stack);
                                    SYMTABLE* new_scope = st_create(BLOCK_SCOPE, current);
                                    stack_push(scope_stack, new_scope);
                                    fprintf(stderr, "Entering %s scope: <%s>:%d\n", get_scope_name(new_scope->scope), file_name, line_num);
                                } 
                                in_function = 0;
                            }
                      decl_or_stmt_list '}'   { $$ = $3; }
                    ;                 

decl_or_stmt_list   : decl_or_stmt                      { $$ = new_list($1); }
                    | decl_or_stmt_list decl_or_stmt    { $$ = append_item($1, $2); }
                    ;

decl_or_stmt    : declaration { $$ = $1; }
                | statement { $$ = $1; }
                ;

decl_specifiers : stg_class_specifier                   { $$ = new_list($1); }
                | stg_class_specifier decl_specifiers   { $$ = append_item($2, $1); }
                | type_specifier                        { $$ = new_list($1); }
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



struct_union_specifier : struct_or_union IDENT '{' {
                                                        //fprintf(stderr, "Token value: %d (STRUCT=%d, UNION=%d)\n", $1, STRUCT, UNION);
                                                        SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);

                                                        // use another variable to store where to insert symbol as we don't want a struct within a struct
                                                        SYMTABLE* insert_st = st->is_struct_scope ? st->outer : st;
                                                        
                                                        SYMBOL* sym = st_lookup(insert_st, $2.string_literal, TAG_NS);
                                                        fprintf(stderr, "Going to insert in %s <%s>:%d\n", get_scope_name(insert_st->scope), insert_st->start_file, insert_st->start_line); 

                                                        if (!sym) {
                                                            sym = st_new_symbol($2.string_literal, NULL, TAG_NS, ($1 == STRUCT ? STRUCT_SYM : UNION_SYM), UNKNOWN_SC, insert_st, file_name, line_num);
                                                            st_install(insert_st, sym);
                                                            fprintf(stderr, "installed struct/union with key %s in %s scope: <%s>:%d\n", sym->key, get_scope_name(insert_st->scope), insert_st->start_file, insert_st->start_line);
                                                        }
                                                        if (!sym->mini_st) {
                                                            sym->mini_st = st_create(BLOCK_SCOPE, insert_st);
                                                            sym->mini_st->is_struct_scope = 1;
                                                        }
                                                        stack_push(scope_stack, sym->mini_st);
                                                        stack_push(struct_union_stack, sym);

                                                        //st = (SYMTABLE*) stack_peek(scope_stack);
                                                        //fprintf(stderr, "current scope is %s\n", get_scope_name(st->scope));

                                                        //print_sym_table(st, file_name, line_num);
                                                        print_symbol(st, sym);
                                                    }
                       struct_declaration_list '}'  { 
                                                        SYMTABLE* st = (SYMTABLE*) stack_pop(scope_stack);
                                                        SYMBOL* sym = (SYMBOL*) stack_peek(struct_union_stack);

                                                        // set parent to current (if not used) so we can print where struct is defined
                                                        if(!sym->parent_sym) sym->parent_sym = sym;

                                                        sym->is_complete = 1;
                                                        $$ = new_struct_union($1, sym);
                                                        //fprintf(stderr, "Exited struct/union scope started at <%s>:%d\n", st->start_file, st->start_line); 
                                                    }
                       | struct_or_union '{'        { 
                                                        SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);

                                                        // use another variable to store where to insert symbol as we don't want a struct within a struct
                                                        SYMTABLE* insert_st = st->is_struct_scope ? st->outer : st;
                                                        //fprintf(stderr, "Entering struct/union scope: <%s>:%d\n", st->start_file, insert_st->start_line); 

                                                        SYMBOL* sym = st_new_symbol(NULL, NULL, TAG_NS, ($1 == STRUCT ? STRUCT_SYM : UNION_SYM), UNKNOWN_SC, insert_st, file_name, line_num);

                                                        if (!sym->mini_st) {
                                                            sym->mini_st = st_create(BLOCK_SCOPE, insert_st);
                                                            sym->mini_st->is_struct_scope = 1;
                                                        }
                                                        stack_push(scope_stack, sym->mini_st);
                                                        stack_push(struct_union_stack, sym); 

                                                        //print_sym_table(st, file_name, line_num);
                                                        //print_symbol(st, sym);
                                                    }
                        struct_declaration_list '}' {
                                                        SYMTABLE* st = (SYMTABLE*) stack_pop(scope_stack);
                                                        SYMBOL* sym = (SYMBOL*) stack_peek(struct_union_stack); 

                                                        // set parent to current (if not used) so we can print where struct is defined
                                                        if(!sym->parent_sym) sym->parent_sym = sym;
                                                        sym->is_complete = 1;

                                                        $$ = new_struct_union($1, sym);
                                                        //fprintf(stderr, "Exited struct/union scope started at <%s>:%d\n", st->start_file, st->start_line); 
                                                    }
                       | struct_or_union IDENT  {
                                                    SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
                                                    SYMTABLE* insert_st = st->is_struct_scope ? st->outer : st;

                                                    SYMBOL* sym = st_lookup_single(insert_st, $2.string_literal, TAG_NS);
                                                    //fprintf(stderr, "found sym is: %d | current scope is: %s at <%s>:%d\n", sym ? 1 : 0, get_scope_name(insert_st->scope), insert_st->start_file, insert_st->start_line); 

                                                    if (!sym) {
                                                        sym = st_new_symbol($2.string_literal, NULL, TAG_NS, ($1 == STRUCT ? STRUCT_SYM : UNION_SYM), UNKNOWN_SC, insert_st, file_name, line_num);
                                                        st_install(insert_st, sym);
                                                        fprintf(stderr, "installed struct/union with key %s in %s scope: <%s>:%d\n", sym->key, get_scope_name(insert_st->scope), insert_st->start_file, insert_st->start_line);
                                                    }
                                                    $$ = new_struct_union($1, sym);
                                                    //print_symbol(insert_st, sym);
                                                }
                        ;


struct_or_union : STRUCT    { $$ = STRUCT; }
                | UNION     { $$ = UNION; }
                ;


struct_declaration_list : struct_declaration                            { $$ = new_list($1); }
                        | struct_declaration_list struct_declaration    { $$ = append_item($1, $2); }
                        ;

struct_declaration  : specifier_list struct_declarator_list ';' { 
                                                                    SYMBOL* sym_list = $2;
                                                                    ast_node_t* spec = $1;
                                                                    SYMTABLE* member_st = (SYMTABLE*) stack_peek(scope_stack);

                                                                    fprintf(stderr, "going to member st\n"); 

                                                                    while (sym_list) {
                                                                        SYMBOL* sym = sym_list;
                                                                        sym->node = combine_nodes(spec, sym->node);
                                                                        sym->name_space = MEMBER_NS;
                                                                        sym->type = MEMBER_SYM;
                                                                        sym->stg_class = AUTO_SC;

                                                                        //sym->parent_sym = current_struct_union;
                                                                        SYMBOL* current_sym = (SYMBOL*) stack_peek(struct_union_stack);
                                                                        if (!sym->parent_sym) sym->parent_sym = current_sym;
                                                                        
                                                                        if (st_install(member_st, sym) != 0) {
                                                                            fprintf(stderr, "Error: duplicate member %s\n", sym->key);
                                                                            exit(1);
                                                                        }
                                                                        print_symbol(member_st, sym);

                                                                        sym_list = sym_list->next;
                                                                    }
                                                                    //print_sym_table(member_st, file_name, line_num);
                                                                }
                    ;



specifier_list  : type_specifier                { $$ = new_list($1); }
                | type_specifier specifier_list { $$ = append_item($1, $2); }
                ;


struct_declarator_list  : struct_declarator                             { $$ = $1; }
                        | struct_declarator_list ',' struct_declarator  { 
                                                                            SYMBOL* p = $1;
                                                                            while (p->next != NULL) p = p->next;
                                                                            p->next = $3; 
                                                                            $$ = $1;
                                                                        }
                        ;

struct_declarator   : declarator {$$ = $1; }
                    ;


declarator  : pointer_declarator  { $$ = $1; }
            | direct_declarator   { $$ = $1; }
            ;


direct_declarator   : simple_declarator     { 
                                                SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
                                                $$ = st_new_symbol($1.string_literal, NULL, GENERAL_NS, VAR_SYM, (st->scope == FILE_SCOPE ? UNKNOWN_SC : AUTO_SC), NULL, file_name, line_num); 
                                            }
                    | '(' declarator ')'    { $$ = $2; }
                    | function_declarator
                    | array_declarator
                    ;


simple_declarator   : IDENT
                    ; 

pointer_declarator  : pointer direct_declarator {     
                                                    $2->node = combine_nodes($1, $2->node); 
                                                    $$ = $2; 
                                                    //fprintf(stderr, "adding ptr\n");  
                                                }
                    ;

pointer : '*'           { $$ = new_pointer(NULL); }
        | '*' pointer   { $$ = new_pointer($2);   }
        ;

array_declarator    : direct_declarator '[' ']'             { 
                                                                ast_node_t* temp = new_array($1->node, 0);
                                                                SYMBOL* sym = $1;

                                                                if(sym->node) {
                                                                    sym->node = combine_nodes(sym->node, temp);
                                                                    fprintf(stderr, "combining nodes for array\n");
                                                                }
                                                                else sym->node = temp;

                                                                //print_ast_tree(sym->node, 0); 

                                                                $$ = sym;
                                                            }
                    | direct_declarator '[' NUMBER ']'      { 
                                                                if(!$3._int) {
                                                                    fprintf(stderr, "Please only use integers for array declarations");
                                                                    exit(0);
                                                                }

                                                                ast_node_t* temp = new_array($1->node, $3._int); 
                                                                SYMBOL* sym = $1; 

                                                                if(sym->node) sym->node = combine_nodes(sym->node, temp);
                                                                else sym->node = temp;

                                                                $$ = sym;
                                                            }
                    ;


function_declarator : direct_declarator '(' ')'                     {   
                                                                        fprintf(stderr, "function declarator detected\n"); 

                                                                        SYMBOL* sym = $1;
                                                                        ast_node_t* temp = new_function(sym->key, NULL, NULL); 

                                                                        if(sym->type == VAR_SYM) sym->type = FUNCT_SYM;
                                                                        
                                                                        temp->function.return_type = sym->node;
                                                                        sym->node = temp;
                                                                        
                                                                        //print_ast_tree(sym->node, 0); 
                                                                        $$ = sym;
                                                                    }
                    | direct_declarator '(' identifier_list ')'     { 
                                                                        SYMBOL* sym = $1;
                                                                        ast_node_t* temp = new_function(sym->key, sym->node, $3); 

                                                                        if(sym->type == VAR_SYM) sym->type = FUNCT_SYM;

                                                                        if(sym->node) sym->node = combine_nodes(sym->node, temp);
                                                                        else sym->node = temp;

                                                                        $$ = sym;
                                                                    }
                    | direct_declarator '(' parameter_list ')'      {
                                                                        SYMBOL* sym = $1;
                                                                        ast_node_t* temp = new_function(sym->key, NULL, NULL);

                                                                        if (sym->type == VAR_SYM) sym->type = FUNCT_SYM;

                                                                        temp->function.return_type = sym->node;
                                                                        temp->function.params = $3;
                                                                        sym->node = temp;

                                                                        //print_ast_tree(sym->node, 0); 

                                                                        $$ = sym;
                                                                    }
                    ;

parameter_list : parameter_declaration { $$ = new_list($1); }
               | parameter_list ',' parameter_declaration { $$ = append_item($1, $3); }
               ;

parameter_declaration : decl_specifiers declarator  { $$ = new_param($1, attach_ident($2->node, $2->key)); }
                      | decl_specifiers             { $$ = new_param($1, NULL); }


identifier_list : IDENT                         { $$ = new_ident($1.string_literal); }
                | identifier_list ',' IDENT     { $$ = append_item($1, new_ident($3.string_literal)); }
                ;

type_name : specifier_list
          | specifier_list abstract_declarator  { $$ = combine_nodes($1, $2); }
          ;

abstract_declarator : pointer
                    | direct_abstract_declarator
                    | pointer direct_abstract_declarator { $$ = combine_nodes($1, $2); }
                    ;

direct_abstract_declarator : '(' abstract_declarator ')'                { $$ = $2; }
                           | '[' ']'                                    { $$ = new_array(NULL, 0);              }
                           | '[' NUMBER ']'                             { $$ = new_array(NULL, $2._int);        }
                           | direct_abstract_declarator '[' ']'         { $$ = new_array($1, 0);                }
                           | direct_abstract_declarator '[' NUMBER ']'  { $$ = new_array($1, $3._int);          }
                           | '(' ')'                                    { $$ = new_function(NULL, NULL, NULL);  }
                           | direct_abstract_declarator '(' ')'         { $$ = new_function(NULL, $1, NULL);    }
                           ;

statement : compound_statement
          | expression ';' { /* expression statement */ }
          | labeled_statement
          | conditional_statement
          | iterative_statement
          | switch_statement
          | break_statement
          | continue_statement
          | return_statement
          | goto_statement
          | null_statement
          ; 

conditional_statement: if_statement         %prec IF
                     | if_else_statement    %prec ELSE
                     ; 

if_else_statement : if_statement ELSE statement { 
                                                    $1->if_node.else_statement = $3;
                                                    $$ = $1;
                                                }
                  ;

if_statement : IF '(' expression ')' statement  { $$ = new_if($3, $5, NULL); }
             ;

labeled_statement : named_label statement       { $$ = new_label($1, $2); }
                  | switch_label statement      { 
                                                    $1->switch_label.statement = $2;
                                                    $$ = $1;
                                                }
                  ;

named_label : IDENT ':'     {
                                char* key = $1.string_literal;
                                SYMTABLE* st = stack_peek(scope_stack);

                                if (st->scope == FILE_SCOPE) {
                                    fprintf(stderr, "Cannot install label '%s' into file scope!", key); 
                                    exit(0); 
                                }

                                while(st->scope != FUNCT_SCOPE)
                                    st = st->outer;

                                SYMBOL* sym = st_lookup(st, key, LABEL_NS); 

                                // if sym doesn't already exist, install it in function scope and mark it as "seen" (complete)
                                if(!sym) {
                                    sym = st_new_symbol(key, NULL, LABEL_NS, LABEL_SYM, EXTERN_SC, st, file_name, line_num); 
                                    sym->is_complete = 1;
                                    st_install(st, sym); 

                                    fprintf(stderr, "created symbol '%s' in %s\n", sym->key, get_scope_name(st->scope)); 
                                }
                                else {
                                    sym->is_complete = 1;
                                    sym->line_num = line_num;
                                }
                                

                                print_symbol(st, sym); 

                                $$ = sym; //new_label(sym, $3); 
                            }
            ;  


switch_label : CASE constant_expression ':' { $$ = new_switch_label(CASE_N, $2, NULL); }
             | DEFAULT ':'                  { $$ = new_switch_label(DEFAULT_N, NULL, NULL); } 
             ; 


iterative_statement : do_statement
                    | while_statement
                    | for_statement
                    ;

while_statement : WHILE '(' expression ')' statement    { $$ = new_while(WHILE_N, $3, $5); }
                ;

do_statement : DO statement WHILE '(' expression ')' ';' { $$ = new_while(DOWHILE_N, $5, $2); }
             ;


for_statement : FOR '(' ';' ';' ')' statement                                       { $$ = new_for(NULL, NULL, NULL, $6); }
              | FOR '(' expression ';' expression ';' expression ')' statement      { $$ = new_for($3, $5, $7, $9); }
              | FOR '(' declaration ';' expression ';' expression ')' statement     { $$ = new_for($3, $5, $7, $9); }
              ;


switch_statement : SWITCH '(' expression ')' statement  { $$ = new_switch($3, $5); }
                 ;


break_statement : BREAK ';' { $$ = new_continue_break(BREAK_N); } 
                ; 


continue_statement : CONTINUE ';' { $$ = new_continue_break(CONTINUE_N); }
                   ;


return_statement : RETURN ';'               { $$ = new_return(NULL); }
                 | RETURN expression ';'    { $$ = new_return($2); }
                 ; 


goto_statement : GOTO IDENT ';' { 
                                    char* key = $2.string_literal;
                                    SYMTABLE* st = stack_peek(scope_stack);

                                    // make sure we look/install in function scope
                                    while(st->scope != FUNCT_SCOPE) {
                                        st = st->outer;
                                    }

                                    SYMBOL* sym = st_lookup(st, key, LABEL_NS); 

                                    // if sym doesn't already exist, install it in function scope and mark it as not "seen" (incomplete)
                                    if(!sym) {

                                        //fprintf(stderr, "insert scope is %s\n", get_scope_name(st->scope)); 
                                        sym = st_new_symbol(key, NULL, LABEL_NS, LABEL_SYM, EXTERN_SC, st, file_name, line_num); 
                                        sym->is_complete = 0;
                                        st_install(st, sym); 

                                        fprintf(stderr, "created symbol '%s' in %s\n", sym->key, get_scope_name(st->scope)); 
                                    }

                                    //fprintf(stderr, "installed %s into %s scope!\n", sym->key, get_scope_name(st->scope));

                                    $$ = new_goto(sym); 
                                    //fprintf(stderr, "created goto symbol\n");
                                }
               ;

null_statement : ';' { $$ = NULL; }
               ;


constant_expression : conditional_expression
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

function_call : postfix_expression '(' expression_list ')' { $$ = new_function_call($1, $3); }
              | postfix_expression '(' ')'  { $$ = new_function_call($1, NULL); }
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
                | '(' type_name ')' cast_expression { $$ = $4; }
                ;


sizeof_expression : SIZEOF '(' type_name ')'    { $$ = new_unop(SIZEOF, $3); }
                  | SIZEOF unary_expression     { $$ = new_unop(SIZEOF, $2); }
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
                      | unary_expression PLUSEQ assignment_expression       { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '+', $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
                      | unary_expression MINUSEQ assignment_expression      { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '-', $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=' , $1, tmp); 
                                                                            }
                      | unary_expression MULTEQ assignment_expression       { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '*', $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
                      | unary_expression DIVEQ assignment_expression        { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '/', $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
                      | unary_expression MODEQ assignment_expression        { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '%', $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
                      | unary_expression SLEQ assignment_expression         { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, SL, $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
                      | unary_expression SREQ assignment_expression         { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, SR, $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
                      | unary_expression ANDEQ assignment_expression        { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, ANDAND, $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
                      | unary_expression XOREQ assignment_expression        { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '^', $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
                      | unary_expression OREQ assignment_expression         { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, OROR, $1, $3); 
                                                                                $$ = new_genop(ASSIGNOP_N, '=', $1, tmp); 
                                                                            }
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

    struct_union_stack = (stack_t*)malloc(sizeof(stack_t));
    stack_init(struct_union_stack);
    
    SYMTABLE* global = st_create(FILE_SCOPE, NULL);

    stack_push(scope_stack, global); 
    yyparse(); 

    free(scope_stack);
    free(struct_union_stack);
    return 0;
}


void yyerror(char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
}

