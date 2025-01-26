%option noyywrap
%{

#include "tokens.h"

YYSTYPE yylval;
int line_num = 1;
%}

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

[_a-zA-Z][_a-zA-Z]*     {yylval.string_literal = yytext; return IDENT;}

[1-9][0-9]*             {yylval.integer = atoi(yytext); return DEC;}
0[xX][0-9A-Fa-f]+       {yylval.integer = (int) strtoll(yytext + 2, NULL, 16); return HEX;}
0[0-7]*                 {yylval.integer = (int) strtoll(yytext + 1, NULL, 8); return OCT;}


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
            case HEX: printf("HEX: val = %d\n", yylval.integer); break;
            case DEC: printf("DEC: val = %d\n", yylval.integer); break;
        }
    }
    printf("EOF\n");
}
