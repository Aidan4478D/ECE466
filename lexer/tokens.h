#ifndef _TOKENS_H
#define _TOKENS_H

typedef struct {

    enum num_type {
        INT, //set to 1024 to guarentee no collision
        LONG,
        LONGLONG,
        FLOAT,
        DOUBLE,
        LONGDOUBLE
    };

    enum num_sign {
        UNSIGNED,
        SIGNED,
    };

    enum num_type type;
    enum num_sign sign;

    long long int _int;
    float _float;
    long double _double;

} NUMTYPE;


typedef union {
    char* string_literal;
    NUMTYPE number;
} YYSTYPE;


enum tokens {
    IDENT=257,
    NUMBER,
    LITERAL,

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
    //DOUBLE,
    ELSE,
    ENUM,
    EXTERN,
    //FLOAT,
    FOR,
    GOTO,
    IF,
    INLINE,
    //INT,
    //LONG,
    REGISTER,
    RETURN,
    SHORT,
    //SIGNED,
    SIZEOF,
    STATIC,
    STRUCT,
    SWITCH,
    TYPEDEF,
    UNION,
    //UNSIGNED,
    VOID,
    VOLATILE,
    WHILE
};

#endif //_TOKENS_H
