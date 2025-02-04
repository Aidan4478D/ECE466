%option noyywrap
%{

#include "tokens.h"
#include "stdlib.h"

YYSTYPE yylval;

int line_num = 1;

char* buf = NULL;
int buf_size = 0;
int buf_len = 0;

void init_buf() {
    buf_size = 128;
    buf_len = 0;
    buf = malloc(buf_size);
    buf[0] = '\0';
}

void append_buf(char c) {
    // dynamically adjust size of buf;  
    if(buf_len + 1 >= buf_size) {
        buf_size = buf_size << 1;
        buf = realloc(buf, buf_size);
        if(!buf) {
            fprintf(stderr, "Out of memory!\n");
            exit(1);
        }
    }
    buf[buf_len++] = c;
    buf[buf_len] = '\0';
}

void reset_buf() {
    if(buf) free(buf);
    buf = NULL;
    buf_size = 0;
    buf_len = 0;
}

// TO DO:
// - make correct outputs
%}

%x INT_SUFFIX
%x FLOAT_SUFFIX
%x STRING
%x CHAR

letter  [A-Za-z]
digit   [0-9]
hex     0[Xx][0-9A-Fa-f]+

%%
[ \t\v\f\r]+    /* do nothing for whitespace */

auto        {return AUTO;}
break       {return BREAK;}
case        {return CASE;}
char        {return CHAR;}
const       {return CONST;}
continue    {return CONTINUE;}
default     {return DEFAULT;}
do          {return DO;}
double      {return DOUBLE;}
else        {return ELSE;}
enum        {return ENUM;}
extern      {return EXTERN;}
float       {return FLOAT;}
for         {return FOR;}
goto        {return GOTO;}
if          {return IF;}
inline      {return INLINE;}
int         {return INT;}
long        {return LONG;}
register    {return REGISTER;}
return      {return RETURN;}
short       {return SHORT;}
signed      {return SIGNED;}
sizeof      {return SIZEOF;}
static      {return STATIC;}
struct      {return STRUCT;}
switch      {return SWITCH;}
typedef     {return TYPEDEF;}
union       {return UNION;}
unsigned    {return UNSIGNED;}
void        {return VOID;}
volatile    {return VOLATILE;}
while       {return WHILE;}

[\n]        {line_num++;}


[1-9]{digit}* {
    yylval.number._int = atoi(yytext); 
    fprintf(stderr, "Got here in int!\n");
    BEGIN(INT_SUFFIX);
}

0[Xx][A-Fa-f0-9]+ {
    yylval.number._int = (int) strtoll(yytext + 2, NULL, 16); 
    BEGIN(INT_SUFFIX);
}

0[0-7]+ {
    yylval.number._int = (int) strtoll(yytext + 1, NULL, 8); 
    BEGIN(INT_SUFFIX);
}

0[Bb][0-1]+ {
    yylval.number._int = (int) strtoll(yytext + 2, NULL, 2); 
    BEGIN(INT_SUFFIX);
}
<INT_SUFFIX>{
    [uU] {
        yylval.number.type = INT_T;
        yylval.number.sign = UNSIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }

    [lL] {
        yylval.number.type = LONG_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }

    [lL][lL] {
        yylval.number.type = LONGLONG_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }

    [uU][lL]|[lL][uU] {
        yylval.number.type = LONG_T;
        yylval.number.sign = UNSIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }

    [uU][lL][lL]|[lL][lL][uU] {
        yylval.number.type = LONGLONG_T;
        yylval.number.sign = UNSIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [^uUlL\n] {
        unput(yytext[0]);
        yylval.number.type = INT_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
    \n {
        line_num++;
        yylval.number.type = INT_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
}

{digit}+(\.{digit}+)([Ee][+-]?{digit}+)? {
    BEGIN(FLOAT_SUFFIX);
}

0[xX]{hex}+(\.{hex}+)?([pP][+-]?{digit}+)? {
    BEGIN(FLOAT_SUFFIX);
}

<FLOAT_SUFFIX>{
    [fF] {
        yylval.number._float = strtof(yytext, NULL); 
        yylval.number.type = FLOAT_T;
        yylval.number.sign = UNSIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }

    [lL] {
        yylval.number._double = strtold(yytext, NULL); 
        yylval.number.type = LONGDOUBLE_T;
        yylval.number.sign = UNSIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [^uUlL\n] {
        unput(yytext[0]);
        yylval.number.type = DOUBLE_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
    \n {
        line_num++;
        yylval.number._double = strtof(yytext, NULL); 
        yylval.number.type = DOUBLE_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
}


[_A-Za-z]+[_A-ZA-z0-9]*     {yylval.string.string_literal = strdup(yytext); return IDENT;}

L?\"  {init_buf(); yylval.string.type = STRING_T; BEGIN(STRING);}
L?\'  {init_buf(); yylval.string.type = CHAR_T; BEGIN(CHAR);}


<STRING,CHAR>{
    \'|\"  {
        if(yylval.string.type == STRING_T) {
            if(buf)
                fprintf(stderr, "buf is %s, len is %d, size is %d\n", buf, buf_len, buf_size);
            else
                fprintf(stderr, "buf is NULL!\n");
            
            yylval.string.string_literal = strdup(buf);

            fprintf(stderr, "buf is %s.\n", yylval.string.string_literal); 
            reset_buf();
            BEGIN(INITIAL);
            return STRING;
        }
        if(yylval.string.type == CHAR_T) {
            if(buf)
                fprintf(stderr, "buf is %s, len is %d, size is %d\n", buf, buf_len, buf_size);
            else
                fprintf(stderr, "buf is NULL!\n");

            if(buf_len > 1) fprintf(stderr, "multi-character constant at line %d\n", line_num);
            
            yylval.string.char_literal = buf_len ? buf[0] : '\0';

            fprintf(stderr, "buf is %c.\n", yylval.string.char_literal); 
            reset_buf();
            BEGIN(INITIAL);
            return STRING;
        }
    }
    \\      { append_buf('\\'); }
    \\n     { append_buf('\n'); }
    \\t     { append_buf('\t'); }
    \\b     { append_buf('\b'); }
    \\r     { append_buf('\r'); }
    \\f     { append_buf('\f'); }
    \\v     { append_buf('\v'); }
    \\a     { append_buf('\a'); }
    \\?     { append_buf('\?'); }
    \\\'    { append_buf('\''); }
    \\\"    { append_buf('\"'); } 

    \\[^ntbrfva?\'\"] {fprintf(stderr, "invalid escape sequence!\n"); }

    \\[0-7]{1,3} {
        int n = (int) strtoll(yytext + 1, NULL, 8);
        n = (n > 255 ? 255 : n);
        fprintf(stderr, "adding value %d = %c to buf\n", n, (char) n);
        append_buf((char) n);
    }
    \\[Xx][0-9A-Fa-f]+ {
        int n = (int) strtoll(yytext + 2, NULL, 16);
        n = (n > 255 ? 255 : n);
        fprintf(stderr, "adding value %d = %c to buf\n", n, (char) n);
        append_buf((char) n);
    }

    \n { fprintf(stderr, "String not terminated on line: %d\n", line_num); exit(-1); }

    . { append_buf(yytext[0]); }
}



.   {fprintf(stderr, "Error: unknown token %s\n", yytext);}


%%

/*
<ESC_STR>{
    n { append_buf('\n'); BEGIN(STRING); }
    t { append_buf('\t'); BEGIN(STRING); }
    b { append_buf('\b'); BEGIN(STRING); }
    a { append_buf('\a'); BEGIN(STRING); }
    v { append_buf('\v'); BEGIN(STRING); }
    f { append_buf('\f'); BEGIN(STRING); }

    . {
        fprintf(stderr, "Invalid escape sequence %c at line %d\n", yytext[0], line_num);
    }
}*/

/*
<CHAR>{
    \' {
        yylval.string.char_literal = yytext[1]; 
        yylval.string.type = CHAR_T;
        fprintf(stderr, "chr is: %c \n", yylval.string.char_literal); 
        return STRING;
    }
}
*/
/*
\+      {return '+';}
-       {return MINUS;}
*/

int main() {
    int t;
    while(t=yylex()) {
        switch(t) {
            case AUTO: printf("AUTO\n"); break;
            //case HEX: printf("HEX: val = %d\n", yylval.integer); break;
            case NUMBER: 
                switch(yylval.number.type) {
                    case INT_T: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED INT: val = %lld\n", yylval.number._int); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED INT: val = %llu\n", yylval.number._int); 
                        break;
                    case FLOAT_T: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED FLOAT: %f\n", yylval.number._float); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED FLOAT: %f\n", yylval.number._float); 
                        break;
                    case DOUBLE_T: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED DOUBLE: %Lf\n", yylval.number._double); 
                        break;
                    case LONGDOUBLE_T:
                        if(yylval.number.sign == SIGNED) printf("SIGNED LONG DOUBLE: %Lf\n", yylval.number._double); 
                        break;
                    case LONG_T: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED LONG: %lld\n", yylval.number._int); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED LONG: %llu\n", yylval.number._int); 
                        break;
                    case LONGLONG_T: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED LONGLONG: %lld\n", yylval.number._int); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED LONGLONG: %llu\n", yylval.number._int); 
                        break;
                }
                break;

            case STRING: 
                switch(yylval.string.type) {
                    case CHAR_T: printf("CHAR: %c\n", yylval.string.char_literal); break;
                    case STRING_T: printf("STRING: %s\n", yylval.string.string_literal); break;
                }
                break;

            case IDENT: printf("IDENTIFIER: %s\n", yylval.string.string_literal); break;
        }
    }
    printf("EOF\n");
}
