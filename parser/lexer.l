%option noyywrap
%{

/* 
Things to fix:
    - The \0
*/

#include "parser.tab.h"
#include "ast.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

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
[\n]        { line_num++; }

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
        yylval.number.sign = UNSIGNED_T;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [lL] {
        yylval.number.type = LONG_T;
        yylval.number.sign = SIGNED_T;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [lL][lL] {
        yylval.number.type = LONGLONG_T;
        yylval.number.sign = SIGNED_T;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [uU][lL]|[lL][uU] {
        yylval.number.type = LONG_T;
        yylval.number.sign = UNSIGNED_T;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [uU][lL][lL]|[lL][lL][uU] {
        yylval.number.type = LONGLONG_T;
        yylval.number.sign = UNSIGNED_T;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [^uUlL] {
        unput(yytext[0]);
        yylval.number.type = INT_T;
        yylval.number.sign = SIGNED_T;
        BEGIN(INITIAL);
        return NUMBER;
    }
}

{DEC_FLOAT} {
    yylval.number._double = strtod(yytext, NULL);
    yylval.number.type = DOUBLE_T;
    yylval.number.sign = SIGNED_T;
    BEGIN(FLOAT_SUFFIX);
}

{HEX_FLOAT} {
    yylval.number._double = strtod(yytext, NULL);
    yylval.number.type = DOUBLE_T;
    yylval.number.sign = SIGNED_T;
    BEGIN(FLOAT_SUFFIX);
}

<FLOAT_SUFFIX>{
    [fF] {
        yylval.number._float = (float) yylval.number._double; 
        yylval.number.type = FLOAT_T;
        yylval.number.sign = SIGNED_T;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [lL] {
        yylval.number._double = (long double) yylval.number._double; 
        yylval.number.type = LONGDOUBLE_T;
        yylval.number.sign = SIGNED_T;
        BEGIN(INITIAL);
        return NUMBER;
    }
    [^lL] {
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
            fprintf(stderr, "got a string in lexer!\n"); 
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
        append_buf((char) n);
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


<<EOF>>     { return YYEOF; }

.   {fprintf(stderr, "Error: unknown token %s\n", yytext);}

%%

/*
if(buf)
    fprintf(stderr, "buf is %s, len is %d, size is %d\n", buf, buf_len, buf_size);
else
    fprintf(stderr, "buf is NULL!\n");
*/

