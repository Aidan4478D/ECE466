/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENT = 258,                   /* IDENT  */
    STRING = 259,                  /* STRING  */
    NUMBER = 260,                  /* NUMBER  */
    PLUSEQ = 261,                  /* PLUSEQ  */
    MINUSEQ = 262,                 /* MINUSEQ  */
    MULTEQ = 263,                  /* MULTEQ  */
    MODEQ = 264,                   /* MODEQ  */
    DIVEQ = 265,                   /* DIVEQ  */
    SLEQ = 266,                    /* SLEQ  */
    SREQ = 267,                    /* SREQ  */
    ANDEQ = 268,                   /* ANDEQ  */
    XOREQ = 269,                   /* XOREQ  */
    OREQ = 270,                    /* OREQ  */
    POINT = 271,                   /* POINT  */
    PLUSPLUS = 272,                /* PLUSPLUS  */
    MINMIN = 273,                  /* MINMIN  */
    SL = 274,                      /* SL  */
    SR = 275,                      /* SR  */
    LTEQ = 276,                    /* LTEQ  */
    GTEQ = 277,                    /* GTEQ  */
    EQEQ = 278,                    /* EQEQ  */
    NOTEQ = 279,                   /* NOTEQ  */
    ANDAND = 280,                  /* ANDAND  */
    OROR = 281,                    /* OROR  */
    ELLIPSIS = 282,                /* ELLIPSIS  */
    AUTO = 283,                    /* AUTO  */
    BREAK = 284,                   /* BREAK  */
    CASE = 285,                    /* CASE  */
    CHAR = 286,                    /* CHAR  */
    CONST = 287,                   /* CONST  */
    CONTINUE = 288,                /* CONTINUE  */
    DEFAULT = 289,                 /* DEFAULT  */
    DO = 290,                      /* DO  */
    ELSE = 291,                    /* ELSE  */
    ENUM = 292,                    /* ENUM  */
    EXTERN = 293,                  /* EXTERN  */
    FOR = 294,                     /* FOR  */
    GOTO = 295,                    /* GOTO  */
    IF = 296,                      /* IF  */
    INLINE = 297,                  /* INLINE  */
    LONG = 298,                    /* LONG  */
    REGISTER = 299,                /* REGISTER  */
    RETURN = 300,                  /* RETURN  */
    SHORT = 301,                   /* SHORT  */
    SIGNED = 302,                  /* SIGNED  */
    SIZEOF = 303,                  /* SIZEOF  */
    STATIC = 304,                  /* STATIC  */
    STRUCT = 305,                  /* STRUCT  */
    SWITCH = 306,                  /* SWITCH  */
    TYPEDEF = 307,                 /* TYPEDEF  */
    UNION = 308,                   /* UNION  */
    UNSIGNED = 309,                /* UNSIGNED  */
    VOID = 310,                    /* VOID  */
    VOLATILE = 311,                /* VOLATILE  */
    WHILE = 312,                   /* WHILE  */
    INT = 313,                     /* INT  */
    FLOAT = 314,                   /* FLOAT  */
    DOUBLE = 315                   /* DOUBLE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IDENT 258
#define STRING 259
#define NUMBER 260
#define PLUSEQ 261
#define MINUSEQ 262
#define MULTEQ 263
#define MODEQ 264
#define DIVEQ 265
#define SLEQ 266
#define SREQ 267
#define ANDEQ 268
#define XOREQ 269
#define OREQ 270
#define POINT 271
#define PLUSPLUS 272
#define MINMIN 273
#define SL 274
#define SR 275
#define LTEQ 276
#define GTEQ 277
#define EQEQ 278
#define NOTEQ 279
#define ANDAND 280
#define OROR 281
#define ELLIPSIS 282
#define AUTO 283
#define BREAK 284
#define CASE 285
#define CHAR 286
#define CONST 287
#define CONTINUE 288
#define DEFAULT 289
#define DO 290
#define ELSE 291
#define ENUM 292
#define EXTERN 293
#define FOR 294
#define GOTO 295
#define IF 296
#define INLINE 297
#define LONG 298
#define REGISTER 299
#define RETURN 300
#define SHORT 301
#define SIGNED 302
#define SIZEOF 303
#define STATIC 304
#define STRUCT 305
#define SWITCH 306
#define TYPEDEF 307
#define UNION 308
#define UNSIGNED 309
#define VOID 310
#define VOLATILE 311
#define WHILE 312
#define INT 313
#define FLOAT 314
#define DOUBLE 315

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 50 "parser.y"
 
    NUMTYPE number;
    STRTYPE string;
    ast_node_t* ast_node;
    SYMBOL* symbol;
    int TOKEN;

#line 195 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
