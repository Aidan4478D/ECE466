%option noyywrap
%{

#include "tokens.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

YYSTYPE yylval;

char* file_name = NULL; 
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
        buf_size <<= 1;
        buf = realloc(buf, buf_size);
        if(!buf) {
            fprintf(stderr, "Out of memory: Wanted %d bytes!\n", buf_size);
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
%}

%x INT_SUFFIX
%x FLOAT_SUFFIX
%x STRING
%x CHAR
%x PREPROC

LETTER      [A-Za-z]
DIGITS      [0-9]+
HEX_DIGIT   [0-9A-Fa-f]
HEX         0[Xx]{HEX_DIGIT}+
DEC_FLOAT   (({DIGITS}"."{DIGITS}?|\.{DIGITS}|{DIGITS}([eE][+-]?{DIGITS}))+)
HEX_FLOAT   0[xX]{HEX_DIGIT}+(\.{HEX_DIGIT}*)?[pP][+-]?{DIGITS}

%%
[ \t\v\f\r]+    /* do nothing for whitespace */


^#   { BEGIN(PREPROC); }
<PREPROC>{
	\"([^"\\]|\\.)*\"	{
        yytext[strlen(yytext)-1] = '\0';
        file_name = strdup(yytext+1);
        line_num = 1;
    }
	\n	{BEGIN(INITIAL);}
	.   {}
}

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

[1-9][0-9]*|0 {
    yylval.number._int = atoll(yytext); 
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
    [^uUlL] {
        unput(yytext[0]);
        yylval.number.type = INT_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
}

{DEC_FLOAT} {
    yylval.number._double = strtod(yytext, NULL);
    yylval.number.type = DOUBLE_T;
    yylval.number.sign = SIGNED;
    BEGIN(FLOAT_SUFFIX);
}

{HEX_FLOAT} {
    yylval.number._double = strtod(yytext, NULL);
    yylval.number.type = DOUBLE_T;
    yylval.number.sign = SIGNED;
    BEGIN(FLOAT_SUFFIX);
}

<FLOAT_SUFFIX>{
    [fF] {
        yylval.number._float = (float) yylval.number._double; 
        yylval.number.type = FLOAT_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [lL] {
        yylval.number._double = (long double) yylval.number._double; 
        yylval.number.type = LONGDOUBLE_T;
        yylval.number.sign = SIGNED;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [^uUlL] {
        unput(yytext[0]);
        BEGIN(INITIAL);
        return NUMBER;
    }
}

L?\"  {init_buf(); yylval.string.type = STRING_T; BEGIN(STRING);}
L?\'  {init_buf(); yylval.string.type = CHAR_T; BEGIN(CHAR);}


<STRING,CHAR>{
    \'|\"  {
        if(yylval.string.type == STRING_T) {
            yylval.string.string_literal = strdup(buf);

            //fprintf(stderr, "buf is %s.\n", yylval.string.string_literal); 
            reset_buf();
            BEGIN(INITIAL);
            return STRING;
        }
        if(yylval.string.type == CHAR_T) {

            if(buf_len > 1) fprintf(stderr, "multi-character constant at line %d\n", line_num);
            yylval.string.char_literal = buf_len ? buf[0] : '\0';

            //fprintf(stderr, "buf is %c.\n", yylval.string.char_literal); 
            reset_buf();
            BEGIN(INITIAL);
            return STRING;
        }
    }
    \\\\    { append_buf('\\'); }
    \\0     { append_buf('\\'); append_buf('0'); } /* is this cheating */
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
        if(n > 255) fprintf(stderr, "octal escape sequence out of range\n"); 
        n = (n > 255 ? 255 : n);
        append_buf((char) (n > 255 ? 255 : n));
    }
    \\[Xx][0-9A-Fa-f]+ {
        int n = (int) strtoll(yytext + 2, NULL, 16);
        if(n > 255) fprintf(stderr, "hex escape sequence out of range\n"); 
        append_buf((char) (n > 255 ? 255 : n));
    }

    \n { fprintf(stderr, "String not terminated on line: %d\n", line_num); exit(-1); }

    . { append_buf(yytext[0]); }
}

[_A-Za-z]+[_A-Za-z0-9]*     {yylval.string.string_literal = strdup(yytext); return IDENT;}

\+=     { return PLUSEQ; }
-=      { return MINUSEQ; }
\*=     { return MULTEQ; }
\/=     { return DIVEQ; }
%=      { return MODEQ; }
\<\<=   { return SLEQ; }
\>\>=   { return SREQ; }
&=      { return ANDEQ; }
\^=     { return XOREQ; }
\|=     { return OREQ; }

->      { return POINT; }
\+\+    { return PLUSPLUS; }
--      { return MINMIN; }
\<\<    { return SL; }
\>\>    { return SR; }
\<=     { return LTEQ; }
\>=     { return GTEQ; }
==      { return EQEQ; }
!=      { return NOTEQ; }
&&      { return ANDAND; }
\|\|    { return OROR; }

\.\.\.  { return ELLIPSIS; }
\<%     { return LTMOD; }
\>%     { return GTMOD; }
\<:     { return LTCOL; }
:\>     { return COLGT; }
%:      { return MODCOL; }
%:%:    { return MCMC; }

!   { return '!'; }
%   { return '%'; }
\^  { return '^'; }
&   { return '&'; }
\*  { return '*'; }
-   { return '-'; }
\+  { return '+'; }
=   { return '='; }
~   { return '~'; }
\|  { return '|'; }
\.  { return '.'; }
\<  { return '<'; }
\>  { return '>'; }
\/  { return '/'; }
\?  { return '?'; }

\(  { return '('; }
\)  { return ')'; }
\[  { return '['; }
\]  { return ']'; }
\{  { return '{'; }
\}  { return '}'; }
,   { return ','; }
;   { return ';'; }
:   { return ':'; }


<<EOF>>     { return TOKEOF; }

.   {fprintf(stderr, "Error: unknown token %s\n", yytext);}

%%

/*
if(buf)
    fprintf(stderr, "buf is %s, len is %d, size is %d\n", buf, buf_len, buf_size);
else
    fprintf(stderr, "buf is NULL!\n");
*/

void print_escaped_char(char c) {
    switch (c) {
        case '\0': printf("\\0"); break;
        case '\\': printf("\\"); break;
        case '\n': printf("\\n"); break;
        case '\t': printf("\\t"); break;
        case '\b': printf("\\b"); break;
        case '\r': printf("\\r"); break;
        case '\f': printf("\\f"); break;
        case '\v': printf("\\v"); break;
        case '\a': printf("\\a"); break;
        case '\?': printf("?"); break;
        case '\'': printf("\\\'"); break;
        case '\"': printf("\\\""); break;

        default:
            if (isprint((unsigned char)c)) putchar(c);
            else printf("\\%0.3o", (unsigned char)c);
    }
}

int main() {
    int t;
    while(t=yylex()) {

        if(file_name) printf("%s\t", file_name);
        else printf("<STDIN>\t");

        printf("%d\t", line_num);

        switch(t) {
            //case HEX: printf("HEX: val = %d\n", yylval.integer); break;
            case NUMBER: 
                switch(yylval.number.type) {
                    case INT_T: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED INT: %lld\n", yylval.number._int); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED INT: %llu\n", yylval.number._int); 
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
                    case CHAR_T: printf("CHAR: "); print_escaped_char(yylval.string.char_literal); printf("\n"); break;
                    case STRING_T: 
                        printf("STRING: "); 
                        for(char *p = yylval.string.string_literal; *p; p++) {
                            print_escaped_char(*p);
                        }
                        printf("\n");
                        break;
                }
                break;

            case IDENT: printf("IDENTIFIER: %s\n", yylval.string.string_literal); break;

            case AUTO: printf("AUTO\n"); break;
            case BREAK: printf("BREAK\n"); break;
            case CASE: printf("CASE\n"); break;
            case CHAR: printf("CHAR\n"); break;
            case CONST: printf("CONST\n"); break;
            case CONTINUE: printf("CONTINUE\n"); break;
            case DEFAULT: printf("DEFAULT\n"); break;
            case DO: printf("DO\n"); break;
            case DOUBLE: printf("DOUBLE\n"); break;
            case ELSE: printf("ELSE\n"); break;
            case ENUM: printf("ENUM\n"); break;
            case EXTERN: printf("EXTERN\n"); break;
            case FLOAT: printf("FLOAT\n"); break;
            case FOR: printf("FOR\n"); break;
            case GOTO: printf("GOTO\n"); break;
            case IF: printf("IF\n"); break;
            case INLINE: printf("INLINE\n"); break;
            case INT: printf("INT\n"); break;
            case LONG: printf("LONG\n"); break;
            case REGISTER: printf("REGISTER\n"); break;
            case RETURN: printf("RETURN\n"); break;
            case SHORT: printf("SHORT\n"); break;
            case SIGNED: printf("SIGNED\n"); break;
            case SIZEOF: printf("SIZEOF\n"); break;
            case STATIC: printf("STATIC\n"); break;
            case STRUCT: printf("STRUCT\n"); break;
            case SWITCH: printf("SWITCH\n"); break;
            case TYPEDEF: printf("TYPEDEF\n"); break;
            case UNION: printf("UNION\n"); break;
            case UNSIGNED: printf("UNSIGNED\n"); break;
            case VOID: printf("VOID\n"); break;
            case VOLATILE: printf("VOLATILE\n"); break;
            case WHILE: printf("WHILE\n"); break;

            case PLUSEQ: printf("PLUSEQ\n"); break;
            case MINUSEQ: printf("MINUSEQ\n"); break;
            case MULTEQ: printf("MULTEQ\n"); break;
            case DIVEQ: printf("DIVEQ\n"); break;
            case MODEQ: printf("MODEQ\n"); break;
            case SLEQ: printf("SLEQ\n"); break;
            case SREQ: printf("SREQ\n"); break;
            case ANDEQ: printf("ANDEQ\n"); break;
            case XOREQ: printf("XOREQ\n"); break;
            case OREQ: printf("OREQ\n"); break;

            case POINT: printf("POINT\n"); break;
            case PLUSPLUS: printf("PLUSPLUS\n"); break; 
            case MINMIN: printf("MINMIN\n"); break;
            case SL: printf("SL\n"); break;
            case SR: printf("SR\n"); break;
            case LTEQ: printf("LTEQ\n"); break;
            case GTEQ: printf("GTEQ\n"); break;
            case EQEQ: printf("EQEQ\n"); break;
            case NOTEQ: printf("NOTEQ\n"); break;
            case ANDAND: printf("ANDAND\n"); break;
            case OROR: printf("OROR\n"); break;

            case ELLIPSIS: printf("ELLIPSIS\n"); break;
            case LTMOD: printf("LTMOD\n"); break;
            case GTMOD: printf("GTMOD\n"); break;
            case LTCOL: printf("LTCOL\n"); break;
            case COLGT: printf("COLGT\n"); break;
            case MODCOL: printf("MODCOL\n"); break;
            case MCMC: printf("MCMC\n"); break;

            case '!': printf("!\n"); break;
            case '%': printf("%\n"); break;
            case '^': printf("^\n"); break;
            case '&': printf("&\n"); break;
            case '*': printf("*\n"); break;
            case '-': printf("-\n"); break;
            case '+': printf("+\n"); break;
            case '=': printf("=\n"); break;
            case '~': printf("~\n"); break;
            case '|': printf("|\n"); break;
            case '.': printf(".\n"); break;
            case '<': printf("<\n"); break;
            case '>': printf(">\n"); break;
            case '/': printf("/\n"); break;
            case '?': printf("?\n"); break;

            case '(': printf("(\n"); break;
            case ')': printf(")\n"); break;
            case '[': printf("[\n"); break;
            case ']': printf("]\n"); break;
            case '{': printf("{\n"); break;
            case '}': printf("}\n"); break;
            case ',': printf(",\n"); break;
            case ';': printf(";\n"); break;
            case ':': printf(":\n"); break;

            case TOKEOF: printf("EOF\n"); break;
        }
    }
}

