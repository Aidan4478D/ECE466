#ifndef _AST_H
#define _AST_H

#define MAX_CHILDREN 4096

//#include "symtable.h"

// stuff that was in the lexer .h file

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
    FLOAT_DT,
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
    LIST_N,
    ELEMENT_N,
    LOGOP_N,
    
    //new stuff
    POINTER_N,
    ARRAY_N,
    DECLSPEC_N,
    FUNCT_DECL_N
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
// - assign op
// - comparison op
// - logical op
typedef struct ast_node_genop {
    int op; 
    ast_node_t* left;
    ast_node_t* right;
} ast_node_genop_t;


typedef struct ast_node_function {
    ast_node_t* left;  //function types: return type
    ast_node_t* right; //function types: params
} ast_node_function_t;


// elements for the list
typedef struct ast_node_element {
    ast_node_t* arg;
} ast_node_element_t;


// List of Arguments
typedef struct ast_node_list {
    ast_node_t* head;
    ast_node_t* next;
} ast_node_list_t;


typedef struct ast_node_pointer {
    ast_node_t* next;
} ast_node_pointer_t;


typedef struct ast_node_declspec {
    DECLTYPE decl_type;
    STGCLASS stg_class;
} ast_node_declspec_t;

typedef struct ast_node_array {
    int size;
    ast_node_list_t* list;
} ast_node_array_t; 

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
    };
};

ast_node_t* new_ident(char* str); 
ast_node_t* new_string(STRTYPE str);
ast_node_t* new_number(NUMTYPE num);
ast_node_t* new_unop(int op, ast_node_t* node); 
ast_node_t* new_ternop(ast_node_t* left, ast_node_t* center, ast_node_t* right); 
ast_node_t* new_genop(NODETYPE type, int op, ast_node_t* left, ast_node_t* right); 
ast_node_t* new_function(ast_node_t* left, ast_node_t* right); 
ast_node_t* new_element(ast_node_t* entry);
ast_node_t* new_list(ast_node_t* head);
ast_node_t* new_pointer(ast_node_t* next);
ast_node_t* append_item(ast_node_t* head, ast_node_t* entry);

ast_node_t* new_decl_spec(DECLTYPE decl_type, STGCLASS stgclass);
ast_node_t* combine_nodes(ast_node_t* base, ast_node_t* decl);
ast_node_t* new_array(ast_node_t* base, int size);
ast_node_t* new_function_decl(ast_node_t* base, ast_node_t* params);

#endif 

