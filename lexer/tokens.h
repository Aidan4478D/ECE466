#ifndef _TOKENS_H
#define _TOKENS_H

typedef union {
    char* string_literal;
    int integer;
    //float _float;
} YYSTYPE;

// compound assignment operators

enum tokens {
    IDENT=257,
    DEC,
    OCT,
    HEX,

    PLUSEQ = 257,
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
