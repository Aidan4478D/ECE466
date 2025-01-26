%option noyywrap
%{

#include "tokens.h"
#include "stdlib.h"

YYSTYPE yylval;
int line_num = 1;
%}

letter  [A-Za-z]
digit   [0-9]
hex     [0-9A-Fa-f]

%%
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

[_{letter}][_{letter}]*     {yylval.string_literal = yytext; return IDENT;}

[1-9]{digit}*           {yylval.integer = atoi(yytext); return NUMBER;}
0[xX]{hex}+             {yylval.integer = (int) strtoll(yytext + 2, NULL, 16); return NUMBER;}
0[0-7]*                 {yylval.integer = (int) strtoll(yytext + 1, NULL, 8); return NUMBER;}

{digit}+(\.{digit}+)?(E[+-]?{digit}+)?  {yylval.f = atof(yytext); return NUMBER;}


0[xX]{hex}+(\.{hex}+)?([pP][+-]?{digit}+)?  {yylval.f = atof(yytext); return NUMBER;}


.   {fprintf(stderr, "Error: unknown token %s\n", yytext);}

%%


/*
\+      {return '+';}
-       {return MINUS;}
*/

main() {
    int t;
    while(t=yylex()) {
        switch(t) {
            case AUTO: printf("AUTO\n"); break;
            //case HEX: printf("HEX: val = %d\n", yylval.integer); break;
            case NUMBER: printf("DEC: val = %d\n", yylval.integer); break;
        }
    }
    printf("EOF\n");
}
