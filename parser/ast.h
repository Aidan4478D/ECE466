#ifndef _AST_H
#define _AST_H

#define MAX_CHILDREN 4096

// stuff that was in the lexer .h file
typedef struct {
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

    enum num_type type;
    enum num_sign sign;

    long long int _int;
    long double _double;
    float _float;

} NUMTYPE;


typedef struct {
    enum str_type {
        STRING_T=1032,
        CHAR_T,
    };

    enum str_type type;

    char* string_literal;
    char char_literal;
} STRTYPE;


typedef enum node_type {
    IDENT_N, //primary
    STRING_N, //primary
    NUMBER_N,
    POSTFIX_N,
    UNARY_N,
    BINOP_N,
    LOGIC_N,
    CONDIT_N,
    ASSIGN_N,
    SEQUENTIAL_N
} node_type;

struct ast_node;
typedef struct ast_node ast_node_t;


typedef struct ast_node_ident {
    char* name;
} ast_node_ident_t;


typedef struct ast_node_string {
    union {
        char* string_literal;
        char char_literal;
    };
} ast_node_string_t;


typedef struct ast_node_number {
    union {
        long long int _int;
        float _float;
        long double _double;
    };
} ast_node_number_t;


typedef struct ast_node_binop {
    char op; 
    ast_node_t* left;
    ast_node_t* right;
} ast_node_binop_t;


typedef struct ast_node_unary {
    char* op; 
    ast_node_t* node;
} ast_node_unary_t; 


struct ast_node {
    node_type type;
    union {
        ast_node_ident_t ident; 
        ast_node_string_t string;
        ast_node_number_t number;
        ast_node_binop_t binop; 
        ast_node_unary_t unary;
    };
};

ast_node_t* new_ident(char* str); 
ast_node_t* new_string(STRTYPE str);
ast_node_t* new_number(NUMTYPE num);
ast_node_t* new_binop(char op, ast_node_t* left, ast_node_t* right); 
ast_node_t* new_unary(char* op, ast_node_t* node); 



#endif 

