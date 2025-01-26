%option noyywrap
%{

#include "tokens.h"
#include "stdlib.h"

YYSTYPE yylval;

int line_num = 1;
%}

letter  [A-Za-z]
digit   [0-9]
hex     0[Xx][0-9A-Fa-f]+

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

[_{letter}][_{letter}{digit}]*     {yylval.string_literal = yytext; return IDENT;}


[-]?[1-9]{digit}*           {
                                yylval.number._int = atoi(yytext); 
                                yylval.number.type = INT;
                                yylval.number.sign = SIGNED;
                                return NUMBER;
                            }

[1-9]{digit}*[uU]           {
                                yylval.number._int = atoi(yytext); 
                                yylval.number.type = INT;
                                yylval.number.sign = UNSIGNED;
                                return NUMBER;
                            }

[-]?[1-9]{digit}*[lL]       {
                                yylval.number._int = atoi(yytext); 
                                yylval.number.type = LONG;
                                yylval.number.sign = SIGNED;
                                return NUMBER;
                            }

[1-9]{digit}*([uU][lL]|[lL][uU])      {
                                            yylval.number._int = atoi(yytext); 
                                            yylval.number.type = LONG;
                                            yylval.number.sign = UNSIGNED;
                                            return NUMBER;
                                        }

[-]?[1-9]{digit}*[lL][lL]   {
                                yylval.number._int = atoi(yytext); 
                                yylval.number.type = LONGLONG;
                                yylval.number.sign = SIGNED;
                                return NUMBER;
                            }

[-]?[1-9]{digit}*([uU][lL][lL]|[lL][lL][uU])  {
                                                    yylval.number._int = atoi(yytext); 
                                                    yylval.number.type = LONGLONG;
                                                    yylval.number.sign = SIGNED;
                                                    return NUMBER;
                                                }

{hex}                       {
                                yylval.number._int = (int) strtoll(yytext + 2, NULL, 16); 
                                yylval.number.type = INT; 
                                yylval.number.sign = SIGNED;
                                return NUMBER;
                            }

{hex}[uU]                   {
                                yylval.number._int = (int) strtoll(yytext + 2, NULL, 16); 
                                yylval.number.type = INT; 
                                yylval.number.sign = UNSIGNED;
                                return NUMBER;
                            }

0[0-7]*                     {
                                yylval.number._int = (int) strtoll(yytext + 1, NULL, 8); 
                                yylval.number.type = INT; 
                                yylval.number.sign = SIGNED; 
                                return NUMBER;
                            }

0[0-7]*[uU]                 {
                                yylval.number._int = (int) strtoll(yytext + 1, NULL, 8); 
                                yylval.number.type = INT; 
                                yylval.number.sign = UNSIGNED; 
                                return NUMBER;
                            }


[-]?{digit}+(\.{digit}+)    {
                                yylval.number._double = strtod(yytext, NULL); 
                                yylval.number.type = DOUBLE; 
                                yylval.number.sign = SIGNED; 
                                return NUMBER;
                            }

{digit}+(\.{digit}+)[lL]    {
                                yylval.number._double = strtod(yytext, NULL); 
                                yylval.number.type = LONGDOUBLE; 
                                yylval.number.sign = SIGNED; 
                                return NUMBER;
                            }



.   {fprintf(stderr, "Error: unknown token %s\n", yytext);}

%%

//0[xX]{hex}+(\.{hex}+)?([pP][+-]?{digit}+)?      {yylval.number._float = atof(yytext); yylval.number.type = FLOAT; return NUMBER;}

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
                    case INT: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED INT: val = %lld\n", yylval.number._int); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED INT: val = %llu\n", yylval.number._int); 
                        break;
                    case FLOAT: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED FLOAT: val = %f\n", yylval.number._float); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED FLOAT: val = %f\n", yylval.number._float); 
                        break;
                    case DOUBLE: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED DOUBLE: val = %Lf\n", yylval.number._double); 
                        break;
                    case LONGDOUBLE:
                        if(yylval.number.sign == SIGNED) printf("SIGNED LONG DOUBLE: val = %Lf\n", yylval.number._double); 
                        break;
                    case LONG: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED LONG: val = %lld\n", yylval.number._int); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED LONG: val = %llu\n", yylval.number._int); 
                        break;
                    case LONGLONG: 
                        if(yylval.number.sign == SIGNED) printf("SIGNED LONGLONG: val = %lld\n", yylval.number._int); 
                        if(yylval.number.sign == UNSIGNED) printf("UNSIGNED LONGLONG: val = %llu\n", yylval.number._int); 
                        break;
                }
        }
    }
    printf("EOF\n");
}
