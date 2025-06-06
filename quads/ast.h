#ifndef _AST_H
#define _AST_H

#define MAX_CHILDREN 4096

//#include "symtable.h"
extern int line_num;
extern char* file_name;


// forward declaration of struct symbol (from symtable.h)
struct symbol;
typedef struct symbol SYMBOL;

struct sym_table;
typedef struct sym_table SYMTABLE;

enum num_type {
    INT_T=1024, //set to 1024 to guarentee no collision
    LONG_T,
    LONGLONG_T,
    FLOAT_T,
    DOUBLE_T,
    LONGDOUBLE_T
};

enum num_sign {
    UNSIGNED_T=1030,
    SIGNED_T,
};

enum str_type {
    STRING_T=1032,
    CHAR_T,
};


typedef enum decl_spec {
    FLOAT_DT=1,
    DOUBLE_DT,
    INT_DT,
    LONG_DT,
    SHORT_DT,
    CHAR_DT,
    VOID_DT,
    SIGNED_DT,
    UNSIGNED_DT
} DECLTYPE; 

typedef struct {
    enum num_type type;
    enum num_sign sign;

    long long int _int;
    long double _double;
    float _float;
} NUMTYPE;


// struct to store info relating to words (chars, strings)
typedef struct {
    enum str_type type;
    char* string_literal;
    char char_literal;
} STRTYPE;


// this also includes typdefs but that's not required
typedef enum storage_class {
    REG_SC,
    AUTO_SC,
    EXTERN_SC,
    STATIC_SC,
    UNKNOWN_SC
} STGCLASS;


typedef enum node_type {
    IDENT_N, //primary
    STRING_N, //primary
    NUMBER_N,
    POSTFIX_N,
    UNOP_N,
    BINOP_N,
    TERNOP_N,
    ASSIGNOP_N,
    COMPOP_N,
    FUNCT_N,
    FUNCTCALL_N,
    LIST_N,
    ELEMENT_N,
    LOGOP_N,
    
    // assignment 3
    POINTER_N,
    ARRAY_N,
    DECLSPEC_N,
    PARAM_N,
    STRUCT_N,
    UNION_N,

    // assignment 4
    IF_N,
    WHILE_N,
    DOWHILE_N,
    FOR_N,
    LABEL_N, // also a symbol
    GOTO_N,
    CONTINUE_N,
    BREAK_N,
    SWITCH_N,
    RETURN_N,
    CASE_N,
    DEFAULT_N,
    DECL_N,

    // assignment 5
    TEMP_N
} NODETYPE;

// assignment 2 stuff
struct ast_node;
typedef struct ast_node ast_node_t;


typedef struct ast_node_ident {
    char* name;
} ast_node_ident_t;


typedef struct ast_node_string {
    STRTYPE str_meta;
} ast_node_string_t;


typedef struct ast_node_number {
    NUMTYPE num_meta; 
} ast_node_number_t;


typedef struct ast_node_unop {
    int op;
    ast_node_t* node;
} ast_node_unop_t; 


typedef struct ast_node_ternop {
    ast_node_t* left;
    ast_node_t* center;
    ast_node_t* right;
} ast_node_ternop_t;

// general operator
// - binary op
// - comparison op
// - logical op
typedef struct ast_node_genop {
    int op; 
    ast_node_t* left;
    ast_node_t* right;
} ast_node_genop_t;


typedef struct ast_node_function {
    char* name;
    ast_node_t* return_type;  //function types: return type
    ast_node_t* params; //function types: params
} ast_node_function_t;

typedef struct ast_node_function_call {
    ast_node_t* name;
    ast_node_t* params;
} ast_node_function_call_t;


// elements for the list
typedef struct ast_node_element {
    ast_node_t* arg;
} ast_node_element_t;


typedef struct ast_node_list {
    ast_node_t* head;
    ast_node_t* next;
} ast_node_list_t;


/* ASSIGNMENT 3 STUFF */

typedef struct ast_node_pointer {
    ast_node_t* next;
} ast_node_pointer_t;


typedef struct ast_node_declspec {
    DECLTYPE decl_type;
    STGCLASS stg_class;
} ast_node_declspec_t;

typedef struct ast_node_array {
    int size;
    ast_node_t* element_type;
    //ast_node_list_t* list;

    // next decl spec
} ast_node_array_t;

typedef struct ast_node_param {
    ast_node_t* ident;  // ident node
    ast_node_t* type;   // decl specs type
} ast_node_param_t; 

typedef struct ast_node_struct_union {
    SYMBOL* sym;
} ast_node_struct_union_t; 


/* ASSIGNMENT 4 STUFF */

typedef struct ast_node_if {
    ast_node_t* condition;
    ast_node_t* then_statement;
    ast_node_t* else_statement;
} ast_node_if_t;

typedef struct ast_node_for {
    ast_node_t* init;
    ast_node_t* condition;
    ast_node_t* increment;
    ast_node_t* body;
} ast_node_for_t; 

typedef struct ast_node_while {
    ast_node_t* condition;
    ast_node_t* body;
} ast_node_while_t;

typedef struct ast_node_switch {
    ast_node_t* expression;
    ast_node_t* statement;
} ast_node_switch_t; 

typedef struct ast_node_label {
    SYMBOL* sym;
    ast_node_t* statement;
} ast_node_label_t;

typedef struct ast_node_goto {
    SYMBOL* sym;
} ast_node_goto_t;

typedef struct ast_node_return {
    ast_node_t* expression;
} ast_node_return_t;

typedef struct ast_node_switch_label {
    ast_node_t* name;
    ast_node_t* statement; 
} ast_node_switch_label_t;


typedef struct ast_node_decl {
    ast_node_t* specifiers;  // type specifiers
    SYMBOL* declarators;     // list of declarators
} ast_node_decl_t;


struct ast_node {
    NODETYPE type;
    union {
        ast_node_ident_t ident; 
        ast_node_string_t string;
        ast_node_number_t number;
        ast_node_unop_t unop;
        ast_node_ternop_t ternop;
        ast_node_genop_t genop;
        ast_node_function_t function;
        ast_node_list_t list;
        ast_node_pointer_t pointer;
        ast_node_declspec_t decl_spec;
        ast_node_array_t array;
        ast_node_param_t parameter;
        ast_node_struct_union_t struct_union;

        ast_node_if_t if_node;
        ast_node_for_t for_node;
        ast_node_while_t while_node;
        ast_node_switch_t switch_node;
        ast_node_label_t label_node;
        ast_node_goto_t goto_node;
        ast_node_return_t return_node;
        ast_node_switch_label_t switch_label;
        ast_node_function_call_t funct_call;
        ast_node_decl_t decl;
    };
};

// assignment 2
ast_node_t* new_ident(char* str); 
ast_node_t* new_string(STRTYPE str);
ast_node_t* new_number(NUMTYPE num);
ast_node_t* new_unop(int op, ast_node_t* node); 
ast_node_t* new_ternop(ast_node_t* left, ast_node_t* center, ast_node_t* right); 
ast_node_t* new_genop(NODETYPE type, int op, ast_node_t* left, ast_node_t* right); 
ast_node_t* new_function(char* name, ast_node_t* left, ast_node_t* right);
ast_node_t* new_function_call(ast_node_t* name, ast_node_t* params); 
ast_node_t* new_element(ast_node_t* entry);
ast_node_t* new_list(ast_node_t* head);
ast_node_t* new_pointer(ast_node_t* next);
ast_node_t* append_item(ast_node_t* head, ast_node_t* entry);

// assignment 3
ast_node_t* new_decl_spec(DECLTYPE decl_type, STGCLASS stgclass);
ast_node_t* combine_nodes(ast_node_t* base, ast_node_t* decl);
ast_node_t* new_array(ast_node_t* element_type, int size);
ast_node_t* new_param(ast_node_t* type, ast_node_t* ident); 
ast_node_t* new_struct_union(int token, SYMBOL* sym);
ast_node_t* attach_ident(ast_node_t* node, char* ident);
ast_node_t* extract_ident(ast_node_t* node);

// assignment 4 additions
ast_node_t* new_if(ast_node_t* condition, ast_node_t* then_statement, ast_node_t* else_statement);
ast_node_t* new_for(ast_node_t* init, ast_node_t* condition, ast_node_t* increment, ast_node_t* body);
ast_node_t* new_while(NODETYPE type, ast_node_t* condition, ast_node_t* body); 
ast_node_t* new_switch(ast_node_t* exp, ast_node_t* statement); 
ast_node_t* new_label(SYMBOL* label, ast_node_t* statement); 
ast_node_t* new_switch_label(NODETYPE type, ast_node_t* name, ast_node_t* statement); 
ast_node_t* new_goto(SYMBOL* sym); 
ast_node_t* new_return(ast_node_t* exp); 
ast_node_t* new_continue_break(NODETYPE type);
ast_node_t* new_decl(ast_node_t* specifiers, SYMBOL* declarators);

// assignment 5
int compare_types(ast_node_t* type1, ast_node_t* type2);
void process_declaration(SYMTABLE *cur_scope, SYMBOL *sym, ast_node_t *spec);
ast_node_t* get_pointed_to_type(ast_node_t* node);
int get_type_size(ast_node_t* type_node);
#endif 

