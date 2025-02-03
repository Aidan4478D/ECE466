#ifndef _TOKENS_H
#define _TOKENS_H

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


typedef union {
    STRTYPE string;
    NUMTYPE number;
} YYSTYPE;


enum tokens {
    IDENT=257,
    NUMBER,
    STRING,

    // compound assignment operators
    PLUSEQ,
    MINUSEQ,
    MULTEQ,
    DIVEQ,
    MODEQ,
    SHFTLFTEQ,
    SHFTRTEQ,
    ANDEQ,
    NOTEQ,
    OREQ,

    // other compound operators
    POINT,
    PLUSPLUS,
    MINMIN,
    SHFTLFT,
    SHFTRT,
    LESSEQ,
    GRTEQ,
    EQEQ,
    XOREQ,
    ANDAND,
    OROR,

    //keywords
    AUTO,
    BREAK,
    CASE,
    CHAR,
    CONST,
    CONTINUE,
    DEFAULT,
    DO,
    DOUBLE,
    ELSE,
    ENUM,
    EXTERN,
    FLOAT,
    FOR,
    GOTO,
    IF,
    INLINE,
    INT,
    LONG,
    REGISTER,
    RETURN,
    SHORT,
    SIGNED,
    SIZEOF,
    STATIC,
    STRUCT,
    SWITCH,
    TYPEDEF,
    UNION,
    UNSIGNED,
    VOID,
    VOLATILE,
    WHILE
};

#endif //_TOKENS_H
