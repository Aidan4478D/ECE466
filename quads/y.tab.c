/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>

#include "helpers/printing.h"
#include "helpers/stack.h"
#include "symtable.h"
#include "quads.h"

#define YYDEBUG 1

void yyerror(char *s);
int yylex();
int yydebug = 0;

ast_node_t* ast_root = NULL;
void print_ast_tree(ast_node_t *node, int indent);

// create a new stack and initialze it
stack_t* scope_stack;
stack_t* struct_union_stack;

// this is sketch but a quick fix
int in_function = 0; 
int global_scope_updated = 0;

/* Although this is great, it does not include: 
    - Function specifiers (inline)
    - Typdef support
    - Qualifiers like const, volatile, and restrict
    - Initialized declarations
    - Arrays are declarators are assumed to be arr[] or arr[NUMBER] (no variable length arrays)
    - Does not support K&R function declarations and definitions (ex. int add(a, b) int a; int b; { return a + b; } <== WILL NOT WORK!)
*/

/* known issues: 
    - can enter things like "int double signed char x;" and it would work
    - void (*g(double d))(int, char); mixes up the parameter lists
*/

/* fix:
    - weird abstract types (casts & sizeof)
    - out of order parameters for function pointers
    - functions returning functions are weird af
*/

#line 118 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 299 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_IDENT = 3,                      /* IDENT  */
  YYSYMBOL_STRING = 4,                     /* STRING  */
  YYSYMBOL_NUMBER = 5,                     /* NUMBER  */
  YYSYMBOL_PLUSEQ = 6,                     /* PLUSEQ  */
  YYSYMBOL_MINUSEQ = 7,                    /* MINUSEQ  */
  YYSYMBOL_MULTEQ = 8,                     /* MULTEQ  */
  YYSYMBOL_MODEQ = 9,                      /* MODEQ  */
  YYSYMBOL_DIVEQ = 10,                     /* DIVEQ  */
  YYSYMBOL_SLEQ = 11,                      /* SLEQ  */
  YYSYMBOL_SREQ = 12,                      /* SREQ  */
  YYSYMBOL_ANDEQ = 13,                     /* ANDEQ  */
  YYSYMBOL_XOREQ = 14,                     /* XOREQ  */
  YYSYMBOL_OREQ = 15,                      /* OREQ  */
  YYSYMBOL_POINT = 16,                     /* POINT  */
  YYSYMBOL_PLUSPLUS = 17,                  /* PLUSPLUS  */
  YYSYMBOL_MINMIN = 18,                    /* MINMIN  */
  YYSYMBOL_SL = 19,                        /* SL  */
  YYSYMBOL_SR = 20,                        /* SR  */
  YYSYMBOL_LTEQ = 21,                      /* LTEQ  */
  YYSYMBOL_GTEQ = 22,                      /* GTEQ  */
  YYSYMBOL_EQEQ = 23,                      /* EQEQ  */
  YYSYMBOL_NOTEQ = 24,                     /* NOTEQ  */
  YYSYMBOL_ANDAND = 25,                    /* ANDAND  */
  YYSYMBOL_OROR = 26,                      /* OROR  */
  YYSYMBOL_ELLIPSIS = 27,                  /* ELLIPSIS  */
  YYSYMBOL_AUTO = 28,                      /* AUTO  */
  YYSYMBOL_BREAK = 29,                     /* BREAK  */
  YYSYMBOL_CASE = 30,                      /* CASE  */
  YYSYMBOL_CHAR = 31,                      /* CHAR  */
  YYSYMBOL_CONST = 32,                     /* CONST  */
  YYSYMBOL_CONTINUE = 33,                  /* CONTINUE  */
  YYSYMBOL_DEFAULT = 34,                   /* DEFAULT  */
  YYSYMBOL_DO = 35,                        /* DO  */
  YYSYMBOL_ELSE = 36,                      /* ELSE  */
  YYSYMBOL_ENUM = 37,                      /* ENUM  */
  YYSYMBOL_EXTERN = 38,                    /* EXTERN  */
  YYSYMBOL_FOR = 39,                       /* FOR  */
  YYSYMBOL_GOTO = 40,                      /* GOTO  */
  YYSYMBOL_IF = 41,                        /* IF  */
  YYSYMBOL_INLINE = 42,                    /* INLINE  */
  YYSYMBOL_LONG = 43,                      /* LONG  */
  YYSYMBOL_REGISTER = 44,                  /* REGISTER  */
  YYSYMBOL_RETURN = 45,                    /* RETURN  */
  YYSYMBOL_SHORT = 46,                     /* SHORT  */
  YYSYMBOL_SIGNED = 47,                    /* SIGNED  */
  YYSYMBOL_SIZEOF = 48,                    /* SIZEOF  */
  YYSYMBOL_STATIC = 49,                    /* STATIC  */
  YYSYMBOL_STRUCT = 50,                    /* STRUCT  */
  YYSYMBOL_SWITCH = 51,                    /* SWITCH  */
  YYSYMBOL_TYPEDEF = 52,                   /* TYPEDEF  */
  YYSYMBOL_UNION = 53,                     /* UNION  */
  YYSYMBOL_UNSIGNED = 54,                  /* UNSIGNED  */
  YYSYMBOL_VOID = 55,                      /* VOID  */
  YYSYMBOL_VOLATILE = 56,                  /* VOLATILE  */
  YYSYMBOL_WHILE = 57,                     /* WHILE  */
  YYSYMBOL_INT = 58,                       /* INT  */
  YYSYMBOL_FLOAT = 59,                     /* FLOAT  */
  YYSYMBOL_DOUBLE = 60,                    /* DOUBLE  */
  YYSYMBOL_61_ = 61,                       /* ','  */
  YYSYMBOL_62_ = 62,                       /* '='  */
  YYSYMBOL_63_ = 63,                       /* '?'  */
  YYSYMBOL_64_ = 64,                       /* ':'  */
  YYSYMBOL_65_ = 65,                       /* '|'  */
  YYSYMBOL_66_ = 66,                       /* '^'  */
  YYSYMBOL_67_ = 67,                       /* '&'  */
  YYSYMBOL_68_ = 68,                       /* '<'  */
  YYSYMBOL_69_ = 69,                       /* '>'  */
  YYSYMBOL_70_ = 70,                       /* '+'  */
  YYSYMBOL_71_ = 71,                       /* '-'  */
  YYSYMBOL_72_ = 72,                       /* '*'  */
  YYSYMBOL_73_ = 73,                       /* '/'  */
  YYSYMBOL_74_ = 74,                       /* '%'  */
  YYSYMBOL_75_ = 75,                       /* '~'  */
  YYSYMBOL_76_ = 76,                       /* '!'  */
  YYSYMBOL_77_ = 77,                       /* '('  */
  YYSYMBOL_78_ = 78,                       /* ')'  */
  YYSYMBOL_79_ = 79,                       /* '['  */
  YYSYMBOL_80_ = 80,                       /* ']'  */
  YYSYMBOL_81_ = 81,                       /* ';'  */
  YYSYMBOL_82_ = 82,                       /* '{'  */
  YYSYMBOL_83_ = 83,                       /* '}'  */
  YYSYMBOL_84_ = 84,                       /* '.'  */
  YYSYMBOL_YYACCEPT = 85,                  /* $accept  */
  YYSYMBOL_parser = 86,                    /* parser  */
  YYSYMBOL_declaration_or_fndef = 87,      /* declaration_or_fndef  */
  YYSYMBOL_declaration = 88,               /* declaration  */
  YYSYMBOL_init_declarator_list = 89,      /* init_declarator_list  */
  YYSYMBOL_init_declarator = 90,           /* init_declarator  */
  YYSYMBOL_function_definition = 91,       /* function_definition  */
  YYSYMBOL_92_1 = 92,                      /* $@1  */
  YYSYMBOL_compound_statement = 93,        /* compound_statement  */
  YYSYMBOL_94_2 = 94,                      /* $@2  */
  YYSYMBOL_95_3 = 95,                      /* $@3  */
  YYSYMBOL_decl_or_stmt_list = 96,         /* decl_or_stmt_list  */
  YYSYMBOL_decl_or_stmt = 97,              /* decl_or_stmt  */
  YYSYMBOL_decl_specifiers = 98,           /* decl_specifiers  */
  YYSYMBOL_stg_class_specifier = 99,       /* stg_class_specifier  */
  YYSYMBOL_type_specifier = 100,           /* type_specifier  */
  YYSYMBOL_struct_union_specifier = 101,   /* struct_union_specifier  */
  YYSYMBOL_102_4 = 102,                    /* $@4  */
  YYSYMBOL_103_5 = 103,                    /* $@5  */
  YYSYMBOL_struct_or_union = 104,          /* struct_or_union  */
  YYSYMBOL_struct_declaration_list = 105,  /* struct_declaration_list  */
  YYSYMBOL_struct_declaration = 106,       /* struct_declaration  */
  YYSYMBOL_specifier_list = 107,           /* specifier_list  */
  YYSYMBOL_struct_declarator_list = 108,   /* struct_declarator_list  */
  YYSYMBOL_struct_declarator = 109,        /* struct_declarator  */
  YYSYMBOL_declarator = 110,               /* declarator  */
  YYSYMBOL_direct_declarator = 111,        /* direct_declarator  */
  YYSYMBOL_simple_declarator = 112,        /* simple_declarator  */
  YYSYMBOL_pointer_declarator = 113,       /* pointer_declarator  */
  YYSYMBOL_pointer = 114,                  /* pointer  */
  YYSYMBOL_array_declarator = 115,         /* array_declarator  */
  YYSYMBOL_function_declarator = 116,      /* function_declarator  */
  YYSYMBOL_parameter_list = 117,           /* parameter_list  */
  YYSYMBOL_parameter_declaration = 118,    /* parameter_declaration  */
  YYSYMBOL_identifier_list = 119,          /* identifier_list  */
  YYSYMBOL_type_name = 120,                /* type_name  */
  YYSYMBOL_abstract_declarator = 121,      /* abstract_declarator  */
  YYSYMBOL_direct_abstract_declarator = 122, /* direct_abstract_declarator  */
  YYSYMBOL_statement = 123,                /* statement  */
  YYSYMBOL_conditional_statement = 124,    /* conditional_statement  */
  YYSYMBOL_if_else_statement = 125,        /* if_else_statement  */
  YYSYMBOL_if_statement = 126,             /* if_statement  */
  YYSYMBOL_labeled_statement = 127,        /* labeled_statement  */
  YYSYMBOL_named_label = 128,              /* named_label  */
  YYSYMBOL_switch_label = 129,             /* switch_label  */
  YYSYMBOL_iterative_statement = 130,      /* iterative_statement  */
  YYSYMBOL_while_statement = 131,          /* while_statement  */
  YYSYMBOL_do_statement = 132,             /* do_statement  */
  YYSYMBOL_for_statement = 133,            /* for_statement  */
  YYSYMBOL_switch_statement = 134,         /* switch_statement  */
  YYSYMBOL_break_statement = 135,          /* break_statement  */
  YYSYMBOL_continue_statement = 136,       /* continue_statement  */
  YYSYMBOL_return_statement = 137,         /* return_statement  */
  YYSYMBOL_goto_statement = 138,           /* goto_statement  */
  YYSYMBOL_null_statement = 139,           /* null_statement  */
  YYSYMBOL_constant_expression = 140,      /* constant_expression  */
  YYSYMBOL_primary_expression = 141,       /* primary_expression  */
  YYSYMBOL_postfix_expression = 142,       /* postfix_expression  */
  YYSYMBOL_subscript_expression = 143,     /* subscript_expression  */
  YYSYMBOL_component_selection_expression = 144, /* component_selection_expression  */
  YYSYMBOL_function_call = 145,            /* function_call  */
  YYSYMBOL_expression_list = 146,          /* expression_list  */
  YYSYMBOL_post_increment_expression = 147, /* post_increment_expression  */
  YYSYMBOL_post_decrement_expression = 148, /* post_decrement_expression  */
  YYSYMBOL_unary_expression = 149,         /* unary_expression  */
  YYSYMBOL_cast_expression = 150,          /* cast_expression  */
  YYSYMBOL_sizeof_expression = 151,        /* sizeof_expression  */
  YYSYMBOL_unary_minus_expression = 152,   /* unary_minus_expression  */
  YYSYMBOL_unary_plus_expression = 153,    /* unary_plus_expression  */
  YYSYMBOL_logical_negation_expression = 154, /* logical_negation_expression  */
  YYSYMBOL_bitwise_negation_expression = 155, /* bitwise_negation_expression  */
  YYSYMBOL_address_expression = 156,       /* address_expression  */
  YYSYMBOL_indirection_expression = 157,   /* indirection_expression  */
  YYSYMBOL_preincrement_expression = 158,  /* preincrement_expression  */
  YYSYMBOL_predecrement_expression = 159,  /* predecrement_expression  */
  YYSYMBOL_multiplicative_expression = 160, /* multiplicative_expression  */
  YYSYMBOL_additive_expression = 161,      /* additive_expression  */
  YYSYMBOL_shift_expression = 162,         /* shift_expression  */
  YYSYMBOL_relational_expression = 163,    /* relational_expression  */
  YYSYMBOL_equality_expression = 164,      /* equality_expression  */
  YYSYMBOL_bitwise_or_expression = 165,    /* bitwise_or_expression  */
  YYSYMBOL_bitwise_xor_expression = 166,   /* bitwise_xor_expression  */
  YYSYMBOL_bitwise_and_expression = 167,   /* bitwise_and_expression  */
  YYSYMBOL_logical_or_expression = 168,    /* logical_or_expression  */
  YYSYMBOL_logical_and_expression = 169,   /* logical_and_expression  */
  YYSYMBOL_conditional_expression = 170,   /* conditional_expression  */
  YYSYMBOL_assignment_expression = 171,    /* assignment_expression  */
  YYSYMBOL_comma_expression = 172,         /* comma_expression  */
  YYSYMBOL_expression = 173                /* expression  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  25
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   799

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  89
/* YYNRULES -- Number of rules.  */
#define YYNRULES  208
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  348

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   315


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    76,     2,     2,     2,    74,    67,     2,
      77,    78,    72,    70,    61,    71,    84,    73,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    64,    81,
      68,    62,    69,    63,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    79,     2,    80,    66,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    82,    65,    83,    75,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   164,   164,   165,   168,   169,   172,   173,   193,   194,
     202,   205,   205,   296,   296,   306,   306,   318,   319,   322,
     323,   326,   327,   328,   329,   332,   333,   334,   335,   338,
     339,   340,   341,   342,   343,   344,   345,   346,   347,   352,
     352,   391,   391,   421,   439,   440,   444,   445,   448,   480,
     481,   485,   486,   494,   498,   499,   503,   507,   508,   509,
     513,   516,   523,   524,   527,   541,   558,   572,   583,   599,
     600,   603,   604,   607,   608,   611,   612,   615,   616,   617,
     620,   621,   622,   623,   624,   625,   626,   629,   630,   631,
     632,   633,   634,   635,   636,   637,   638,   639,   642,   643,
     646,   652,   655,   656,   662,   697,   698,   702,   703,   704,
     707,   710,   714,   715,   716,   720,   724,   728,   732,   733,
     737,   766,   770,   774,   775,   776,   777,   781,   782,   783,
     784,   785,   786,   789,   797,   802,   809,   810,   813,   814,
     817,   820,   824,   825,   826,   827,   828,   829,   830,   831,
     832,   833,   837,   838,   842,   843,   848,   851,   854,   857,
     860,   863,   866,   877,   889,   890,   891,   892,   895,   896,
     897,   900,   901,   902,   905,   906,   907,   908,   909,   912,
     913,   914,   917,   918,   921,   922,   925,   926,   931,   932,
     935,   936,   940,   941,   945,   946,   947,   951,   955,   959,
     963,   967,   971,   975,   979,   983,   990,   991,   994
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "IDENT", "STRING",
  "NUMBER", "PLUSEQ", "MINUSEQ", "MULTEQ", "MODEQ", "DIVEQ", "SLEQ",
  "SREQ", "ANDEQ", "XOREQ", "OREQ", "POINT", "PLUSPLUS", "MINMIN", "SL",
  "SR", "LTEQ", "GTEQ", "EQEQ", "NOTEQ", "ANDAND", "OROR", "ELLIPSIS",
  "AUTO", "BREAK", "CASE", "CHAR", "CONST", "CONTINUE", "DEFAULT", "DO",
  "ELSE", "ENUM", "EXTERN", "FOR", "GOTO", "IF", "INLINE", "LONG",
  "REGISTER", "RETURN", "SHORT", "SIGNED", "SIZEOF", "STATIC", "STRUCT",
  "SWITCH", "TYPEDEF", "UNION", "UNSIGNED", "VOID", "VOLATILE", "WHILE",
  "INT", "FLOAT", "DOUBLE", "','", "'='", "'?'", "':'", "'|'", "'^'",
  "'&'", "'<'", "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'", "'!'",
  "'('", "')'", "'['", "']'", "';'", "'{'", "'}'", "'.'", "$accept",
  "parser", "declaration_or_fndef", "declaration", "init_declarator_list",
  "init_declarator", "function_definition", "$@1", "compound_statement",
  "$@2", "$@3", "decl_or_stmt_list", "decl_or_stmt", "decl_specifiers",
  "stg_class_specifier", "type_specifier", "struct_union_specifier", "$@4",
  "$@5", "struct_or_union", "struct_declaration_list",
  "struct_declaration", "specifier_list", "struct_declarator_list",
  "struct_declarator", "declarator", "direct_declarator",
  "simple_declarator", "pointer_declarator", "pointer", "array_declarator",
  "function_declarator", "parameter_list", "parameter_declaration",
  "identifier_list", "type_name", "abstract_declarator",
  "direct_abstract_declarator", "statement", "conditional_statement",
  "if_else_statement", "if_statement", "labeled_statement", "named_label",
  "switch_label", "iterative_statement", "while_statement", "do_statement",
  "for_statement", "switch_statement", "break_statement",
  "continue_statement", "return_statement", "goto_statement",
  "null_statement", "constant_expression", "primary_expression",
  "postfix_expression", "subscript_expression",
  "component_selection_expression", "function_call", "expression_list",
  "post_increment_expression", "post_decrement_expression",
  "unary_expression", "cast_expression", "sizeof_expression",
  "unary_minus_expression", "unary_plus_expression",
  "logical_negation_expression", "bitwise_negation_expression",
  "address_expression", "indirection_expression",
  "preincrement_expression", "predecrement_expression",
  "multiplicative_expression", "additive_expression", "shift_expression",
  "relational_expression", "equality_expression", "bitwise_or_expression",
  "bitwise_xor_expression", "bitwise_and_expression",
  "logical_or_expression", "logical_and_expression",
  "conditional_expression", "assignment_expression", "comma_expression",
  "expression", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-101)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-14)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     739,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,  -101,   517,  -101,  -101,  -101,
      33,   739,   739,  -101,     5,  -101,  -101,  -101,   -62,     6,
    -101,   -24,  -101,   -70,    21,  -101,  -101,    13,  -101,  -101,
    -101,  -101,   -43,  -101,  -101,   -54,     6,  -101,   -30,   481,
       0,    21,  -101,   182,  -101,  -101,  -101,    -4,  -101,  -101,
    -101,     6,   -15,  -101,    -8,    -3,  -101,   182,   182,   542,
    -101,     6,    23,   310,  -101,   739,  -101,   106,  -101,  -101,
     704,  -101,  -101,  -101,   -23,  -101,  -101,  -101,    48,  -101,
    -101,   637,   637,    35,   653,    38,    76,   371,    95,   171,
     103,    51,   669,   104,   105,   653,   653,   653,   653,   653,
     653,   576,  -101,  -101,  -101,   249,  -101,    33,  -101,  -101,
    -101,   161,  -101,   371,   371,  -101,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,  -101,    24,  -101,  -101,  -101,
    -101,  -101,   250,  -101,  -101,  -101,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,    72,    -6,    53,    74,   144,   133,   134,
     132,    18,   176,  -101,  -101,   142,   125,  -101,  -101,  -101,
       6,  -101,  -101,  -101,   653,  -101,  -101,  -101,   143,  -101,
    -101,  -101,  -101,   151,   427,   129,   653,  -101,   136,   576,
    -101,   653,   653,  -101,  -101,  -101,  -101,  -101,  -101,    25,
     137,   140,  -101,  -101,   371,  -101,  -101,   211,  -101,  -101,
     539,   653,   216,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   653,   653,   653,   653,   653,   653,   653,   653,
     653,   653,   653,   653,  -101,  -101,  -101,   145,   139,   146,
     152,  -101,   156,  -101,   165,   167,   168,    52,     2,    36,
    -101,    87,   653,  -101,  -101,  -101,  -101,    -2,  -101,   141,
    -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,    72,    72,    -6,    -6,    53,
      53,    53,    53,    74,    74,   134,   132,   144,   176,   159,
     133,  -101,   653,   172,   653,   653,   371,  -101,   371,   371,
    -101,   173,   169,  -101,    87,   177,     8,  -101,   653,  -101,
    -101,   653,   190,   371,   188,   189,  -101,  -101,  -101,  -101,
    -101,  -101,   191,  -101,  -101,  -101,   192,  -101,   653,   653,
    -101,  -101,   194,   196,   371,   371,  -101,  -101
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    27,    34,    25,    32,    28,    33,    36,    26,    44,
      45,    37,    35,    31,    29,    30,     0,     2,     4,     5,
       0,    21,    23,    38,     0,     1,     3,    60,    62,     0,
       6,     0,     8,    10,    55,    56,    54,     0,    59,    58,
      22,    24,    43,    41,    63,     0,     0,     7,     0,     0,
       0,    61,    39,     0,    57,     9,    10,    15,    12,    73,
      66,    72,     0,    69,     0,     0,    64,     0,    49,     0,
      46,     0,     0,     0,    71,     0,    68,     0,    67,    65,
       0,    50,    42,    47,     0,    51,    53,    14,   123,   125,
     124,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   121,    19,    87,     0,    17,     0,    20,    90,
      99,    98,    89,     0,     0,    91,   108,   107,   109,    92,
      93,    94,    95,    96,    97,   127,   142,   128,   129,   130,
     131,   132,   152,   164,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   168,   171,   174,   179,   186,   190,   182,
     184,   192,   188,   194,   206,   208,     0,    70,    74,    40,
       0,    48,   104,   123,     0,   162,   163,   116,     0,   152,
     122,   117,   106,     0,     0,     0,     0,   118,     0,     0,
     155,     0,     0,   160,   157,   156,   161,   159,   158,    75,
       0,     0,    16,    18,     0,   102,   103,     0,   140,   141,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    88,    52,   105,     0,     0,     0,
       0,   120,     0,   119,     0,     0,     0,     0,     0,    77,
      76,    78,     0,   126,   100,   135,   137,     0,   138,     0,
     134,   196,   197,   198,   200,   199,   201,   202,   203,   204,
     205,   195,   165,   166,   167,   169,   170,   172,   173,   178,
     176,   175,   177,   180,   181,   183,   185,   187,   189,     0,
     191,   207,     0,     0,     0,     0,     0,   154,     0,     0,
      85,     0,     0,    81,    79,     0,     0,   153,     0,   136,
     133,     0,     0,     0,     0,     0,   101,   115,   110,    80,
      82,    86,     0,    83,   139,   193,     0,   112,     0,     0,
      84,   111,     0,     0,     0,     0,   114,   113
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -101,  -101,   208,   -67,  -101,   229,  -101,  -101,   228,  -101,
    -101,  -101,   166,    45,  -101,   -18,  -101,  -101,  -101,  -101,
     218,    -9,   -64,  -101,   116,    14,   264,  -101,  -101,   -26,
    -101,  -101,  -101,   230,  -101,   121,    54,    58,   -97,  -101,
    -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,   -77,   -87,  -101,  -101,  -101,  -101,
    -101,  -101,  -101,  -101,  -101,   -58,   -53,   -98,   -57,    79,
      80,    86,    91,  -101,    93,   -91,   -27,  -101,  -100
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    16,    17,    18,    31,    32,    19,    48,   114,    72,
      73,   115,   116,   117,    21,    22,    23,    67,    53,    24,
      69,    70,    71,    84,    85,    56,    34,    35,    36,    37,
      38,    39,    62,    63,    64,   200,   260,   261,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   178,   135,   136,   137,   138,
     139,   267,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     183,   188,    44,   180,    81,    65,   113,   312,    42,    27,
      28,   201,   -11,   332,   175,   176,    27,   179,   193,   194,
     195,   196,   197,   198,    54,   190,   205,   206,   179,   179,
     179,   179,   179,   179,    33,    68,    27,    46,   170,    52,
     207,   208,   209,    45,   240,    20,    75,   199,   113,    68,
      68,    68,    57,    77,   173,    89,    90,    47,   171,   318,
      83,    20,    68,    76,   227,   228,    40,    41,    91,    92,
      78,    83,   229,   230,   201,    74,   319,    79,    28,   -13,
      66,   241,   313,    29,   250,    86,   252,    43,   333,   201,
      29,   255,   256,    68,    61,   231,   232,    28,    49,   102,
      50,   210,   257,   211,   258,    28,    87,   264,   212,   168,
      29,   269,   172,   257,    30,   258,   177,   249,   105,   181,
      61,   106,   107,   108,    28,   199,   109,   110,   111,   257,
     310,   258,   187,   289,   290,   291,   292,   282,   283,   284,
     182,   299,   233,   234,   224,   225,   226,   179,   179,   179,
     179,   179,   179,   179,   179,   179,   179,   179,   179,   179,
     179,   179,   179,   179,   315,   179,   316,   235,   236,   285,
     286,    68,   184,   259,   185,   317,   287,   288,   293,   294,
     186,   191,   192,   268,    86,   179,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   204,   237,   239,
     238,   242,   322,   243,   324,   325,   244,   246,   247,   326,
     251,   327,   328,     2,   265,   262,   301,   253,   263,   270,
     303,   320,   302,   321,    26,     4,   337,   304,     6,     7,
     335,   259,     9,   305,   306,    10,    11,    12,   342,   343,
      13,    14,    15,   307,   179,   308,   309,   346,   347,   330,
     323,   329,    88,    89,    90,   331,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,    91,    92,   336,   338,
     339,   340,   344,   341,   345,    55,    58,     1,    93,    94,
       2,   203,    95,    96,    97,    80,   245,     3,    98,    99,
     100,   334,     4,     5,   101,     6,     7,   102,     8,     9,
     103,    51,    10,    11,    12,   167,   104,    13,    14,    15,
     254,   311,   223,    88,    89,    90,   105,   314,   297,   106,
     107,   108,   300,   295,   109,   110,   111,    91,    92,   296,
     112,    57,   202,   298,     0,     0,     0,     0,     1,    93,
      94,     2,     0,    95,    96,    97,     0,     0,     3,    98,
      99,   100,     0,     4,     5,   101,     6,     7,   102,     8,
       9,   103,     0,    10,    11,    12,     0,   104,    13,    14,
      15,     0,     0,     0,    88,    89,    90,   105,     0,     0,
     106,   107,   108,     0,     0,   109,   110,   111,    91,    92,
       0,   112,    57,     0,     0,     0,     0,     0,     0,     0,
      93,    94,     0,     0,    95,    96,    97,     0,     0,     0,
      98,    99,   100,     0,     0,     0,   101,     0,     0,   102,
       0,     0,   103,     0,     0,     0,     0,     0,   104,     0,
     173,    89,    90,     0,     0,     0,     0,     0,   105,     0,
       0,   106,   107,   108,    91,    92,   109,   110,   111,     0,
       0,     0,   112,    57,     0,     1,     0,     0,     2,     0,
       0,     0,     0,     0,     0,     3,     0,     0,     0,     0,
       4,     5,     0,     6,     7,   102,     8,     9,     0,     0,
      10,    11,    12,     0,    59,    13,    14,    15,     0,     0,
       0,     0,     0,     0,   105,     0,     0,   106,   107,   108,
       0,     0,   109,   110,   111,     0,     0,     0,   248,     1,
       0,     0,     2,     0,     0,     0,     0,    25,     0,     3,
       0,     0,     0,     0,     4,     5,     0,     6,     7,     0,
       8,     9,     0,     0,    10,    11,    12,     0,     0,    13,
      14,    15,   173,    89,    90,     1,     0,     0,     2,     0,
       0,     0,     0,     0,     0,     3,    91,    92,     0,    60,
       4,     5,     0,     6,     7,     0,     8,     9,     0,     0,
      10,    11,    12,     2,     0,    13,    14,    15,     0,   173,
      89,    90,     0,     0,     0,     4,     0,   102,     6,     7,
       0,     0,     9,    91,    92,    10,    11,    12,     0,     0,
      13,    14,    15,     0,     0,     0,   105,     2,     0,   106,
     107,   108,     0,     0,   109,   110,   111,   266,     0,     4,
       0,     0,     6,     7,   102,    82,     9,     0,     0,    10,
      11,    12,     0,     0,    13,    14,    15,     0,     0,     0,
     173,    89,    90,   105,     0,     0,   106,   107,   108,     0,
       0,   109,   110,   111,    91,    92,   173,    89,    90,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,    92,   173,    89,    90,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   102,    91,    92,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   102,     0,     0,   105,     0,     0,   106,   107,   108,
       0,     0,   109,   110,   174,     0,     0,   102,     0,     0,
     105,     0,     0,   106,   107,   108,     0,     0,   109,   110,
     111,     0,     0,     0,     0,     2,   105,     0,     0,   106,
     107,   108,     0,     0,   109,   110,   189,     4,     0,     0,
       6,     7,     0,     0,     9,     0,     0,    10,    11,    12,
       0,     0,    13,    14,    15,     0,     0,     1,     0,     0,
       2,     0,     0,     0,     0,     0,     0,     3,     0,     0,
       0,     0,     4,     5,     0,     6,     7,   169,     8,     9,
       0,     0,    10,    11,    12,     0,     0,    13,    14,    15
};

static const yytype_int16 yycheck[] =
{
      97,   101,    28,    94,    68,     5,    73,     5,     3,     3,
      72,   111,    82,     5,    91,    92,     3,    94,   105,   106,
     107,   108,   109,   110,    78,   102,   123,   124,   105,   106,
     107,   108,   109,   110,    20,    53,     3,    61,    61,    82,
      16,    17,    18,    29,    26,     0,    61,   111,   115,    67,
      68,    69,    82,    61,     3,     4,     5,    81,    81,    61,
      69,    16,    80,    78,    70,    71,    21,    22,    17,    18,
      78,    80,    19,    20,   174,    61,    78,    80,    72,    83,
      80,    63,    80,    77,   184,    71,   186,    82,    80,   189,
      77,   191,   192,   111,    49,    21,    22,    72,    77,    48,
      79,    77,    77,    79,    79,    72,    83,   204,    84,     3,
      77,   211,    64,    77,    81,    79,    81,   184,    67,    81,
      75,    70,    71,    72,    72,   189,    75,    76,    77,    77,
      78,    79,    81,   231,   232,   233,   234,   224,   225,   226,
      64,   241,    68,    69,    72,    73,    74,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,    77,   242,    79,    23,    24,   227,
     228,   189,    77,   199,     3,   262,   229,   230,   235,   236,
      77,    77,    77,   210,   170,   262,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,    36,    65,    67,
      66,    25,   302,    61,   304,   305,    81,    64,    57,   306,
      81,   308,   309,    31,     3,    78,   243,    81,    78,     3,
      81,    80,    77,    64,    16,    43,   323,    81,    46,    47,
     321,   257,    50,    81,    78,    53,    54,    55,   338,   339,
      58,    59,    60,    78,   321,    78,    78,   344,   345,    80,
      78,    78,     3,     4,     5,    78,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    17,    18,    78,    81,
      81,    80,    78,    81,    78,    46,    48,    28,    29,    30,
      31,   115,    33,    34,    35,    67,   170,    38,    39,    40,
      41,   318,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    37,    53,    54,    55,    75,    57,    58,    59,    60,
     189,   257,    62,     3,     4,     5,    67,   259,   239,    70,
      71,    72,   242,   237,    75,    76,    77,    17,    18,   238,
      81,    82,    83,   240,    -1,    -1,    -1,    -1,    28,    29,
      30,    31,    -1,    33,    34,    35,    -1,    -1,    38,    39,
      40,    41,    -1,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    -1,    53,    54,    55,    -1,    57,    58,    59,
      60,    -1,    -1,    -1,     3,     4,     5,    67,    -1,    -1,
      70,    71,    72,    -1,    -1,    75,    76,    77,    17,    18,
      -1,    81,    82,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    -1,    -1,    33,    34,    35,    -1,    -1,    -1,
      39,    40,    41,    -1,    -1,    -1,    45,    -1,    -1,    48,
      -1,    -1,    51,    -1,    -1,    -1,    -1,    -1,    57,    -1,
       3,     4,     5,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    70,    71,    72,    17,    18,    75,    76,    77,    -1,
      -1,    -1,    81,    82,    -1,    28,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,    -1,    -1,
      43,    44,    -1,    46,    47,    48,    49,    50,    -1,    -1,
      53,    54,    55,    -1,     3,    58,    59,    60,    -1,    -1,
      -1,    -1,    -1,    -1,    67,    -1,    -1,    70,    71,    72,
      -1,    -1,    75,    76,    77,    -1,    -1,    -1,    81,    28,
      -1,    -1,    31,    -1,    -1,    -1,    -1,     0,    -1,    38,
      -1,    -1,    -1,    -1,    43,    44,    -1,    46,    47,    -1,
      49,    50,    -1,    -1,    53,    54,    55,    -1,    -1,    58,
      59,    60,     3,     4,     5,    28,    -1,    -1,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    38,    17,    18,    -1,    78,
      43,    44,    -1,    46,    47,    -1,    49,    50,    -1,    -1,
      53,    54,    55,    31,    -1,    58,    59,    60,    -1,     3,
       4,     5,    -1,    -1,    -1,    43,    -1,    48,    46,    47,
      -1,    -1,    50,    17,    18,    53,    54,    55,    -1,    -1,
      58,    59,    60,    -1,    -1,    -1,    67,    31,    -1,    70,
      71,    72,    -1,    -1,    75,    76,    77,    78,    -1,    43,
      -1,    -1,    46,    47,    48,    83,    50,    -1,    -1,    53,
      54,    55,    -1,    -1,    58,    59,    60,    -1,    -1,    -1,
       3,     4,     5,    67,    -1,    -1,    70,    71,    72,    -1,
      -1,    75,    76,    77,    17,    18,     3,     4,     5,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,     3,     4,     5,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    17,    18,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    -1,    -1,    67,    -1,    -1,    70,    71,    72,
      -1,    -1,    75,    76,    77,    -1,    -1,    48,    -1,    -1,
      67,    -1,    -1,    70,    71,    72,    -1,    -1,    75,    76,
      77,    -1,    -1,    -1,    -1,    31,    67,    -1,    -1,    70,
      71,    72,    -1,    -1,    75,    76,    77,    43,    -1,    -1,
      46,    47,    -1,    -1,    50,    -1,    -1,    53,    54,    55,
      -1,    -1,    58,    59,    60,    -1,    -1,    28,    -1,    -1,
      31,    -1,    -1,    -1,    -1,    -1,    -1,    38,    -1,    -1,
      -1,    -1,    43,    44,    -1,    46,    47,    83,    49,    50,
      -1,    -1,    53,    54,    55,    -1,    -1,    58,    59,    60
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    28,    31,    38,    43,    44,    46,    47,    49,    50,
      53,    54,    55,    58,    59,    60,    86,    87,    88,    91,
      98,    99,   100,   101,   104,     0,    87,     3,    72,    77,
      81,    89,    90,   110,   111,   112,   113,   114,   115,   116,
      98,    98,     3,    82,   114,   110,    61,    81,    92,    77,
      79,   111,    82,   103,    78,    90,   110,    82,    93,     3,
      78,    98,   117,   118,   119,     5,    80,   102,   100,   105,
     106,   107,    94,    95,   110,    61,    78,    61,    78,    80,
     105,   107,    83,   106,   108,   109,   110,    83,     3,     4,
       5,    17,    18,    29,    30,    33,    34,    35,    39,    40,
      41,    45,    48,    51,    57,    67,    70,    71,    72,    75,
      76,    77,    81,    88,    93,    96,    97,    98,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   141,   142,   143,   144,   145,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   118,     3,    83,
      61,    81,    64,     3,    77,   149,   149,    81,   140,   149,
     170,    81,    64,   123,    77,     3,    77,    81,   173,    77,
     149,    77,    77,   150,   150,   150,   150,   150,   150,   107,
     120,   173,    83,    97,    36,   123,   123,    16,    17,    18,
      77,    79,    84,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    62,    72,    73,    74,    70,    71,    19,
      20,    21,    22,    68,    69,    23,    24,    65,    66,    67,
      26,    63,    25,    61,    81,   109,    64,    57,    81,    88,
     173,    81,   173,    81,   120,   173,   173,    77,    79,   114,
     121,   122,    78,    78,   123,     3,    78,   146,   171,   173,
       3,   171,   171,   171,   171,   171,   171,   171,   171,   171,
     171,   171,   150,   150,   150,   160,   160,   161,   161,   162,
     162,   162,   162,   163,   163,   166,   167,   164,   169,   173,
     165,   171,    77,    81,    81,    81,    78,    78,    78,    78,
      78,   121,     5,    80,   122,    77,    79,   150,    61,    78,
      80,    64,   173,    78,   173,   173,   123,   123,   123,    78,
      80,    78,     5,    80,   171,   170,    78,   123,    81,    81,
      80,    81,   173,   173,    78,    78,   123,   123
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    86,    87,    87,    88,    88,    89,    89,
      90,    92,    91,    94,    93,    95,    93,    96,    96,    97,
      97,    98,    98,    98,    98,    99,    99,    99,    99,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   102,
     101,   103,   101,   101,   104,   104,   105,   105,   106,   107,
     107,   108,   108,   109,   110,   110,   111,   111,   111,   111,
     112,   113,   114,   114,   115,   115,   116,   116,   116,   117,
     117,   118,   118,   119,   119,   120,   120,   121,   121,   121,
     122,   122,   122,   122,   122,   122,   122,   123,   123,   123,
     123,   123,   123,   123,   123,   123,   123,   123,   124,   124,
     125,   126,   127,   127,   128,   129,   129,   130,   130,   130,
     131,   132,   133,   133,   133,   134,   135,   136,   137,   137,
     138,   139,   140,   141,   141,   141,   141,   142,   142,   142,
     142,   142,   142,   143,   144,   144,   145,   145,   146,   146,
     147,   148,   149,   149,   149,   149,   149,   149,   149,   149,
     149,   149,   150,   150,   151,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   160,   160,   160,   161,   161,
     161,   162,   162,   162,   163,   163,   163,   163,   163,   164,
     164,   164,   165,   165,   166,   166,   167,   167,   168,   168,
     169,   169,   170,   170,   171,   171,   171,   171,   171,   171,
     171,   171,   171,   171,   171,   171,   172,   172,   173
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     2,     3,     1,     3,
       1,     0,     4,     0,     3,     0,     4,     1,     2,     1,
       1,     1,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       6,     0,     5,     2,     1,     1,     1,     2,     3,     1,
       2,     1,     3,     1,     1,     1,     1,     3,     1,     1,
       1,     2,     1,     2,     3,     4,     3,     4,     4,     1,
       3,     2,     1,     1,     3,     1,     2,     1,     1,     2,
       3,     2,     3,     3,     4,     2,     3,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     5,     2,     2,     2,     3,     2,     1,     1,     1,
       5,     7,     6,     9,     9,     5,     2,     2,     2,     3,
       3,     1,     1,     1,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     4,     3,     1,     3,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     5,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* parser: declaration_or_fndef  */
#line 164 "parser.y"
                                        { ast_root = (yyvsp[0].ast_node); }
#line 1818 "y.tab.c"
    break;

  case 4: /* declaration_or_fndef: declaration  */
#line 168 "parser.y"
                                                {}
#line 1824 "y.tab.c"
    break;

  case 5: /* declaration_or_fndef: function_definition  */
#line 169 "parser.y"
                                                {}
#line 1830 "y.tab.c"
    break;

  case 6: /* declaration: decl_specifiers ';'  */
#line 172 "parser.y"
                                    { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 1836 "y.tab.c"
    break;

  case 7: /* declaration: decl_specifiers init_declarator_list ';'  */
#line 173 "parser.y"
                                                        { 
                                                            SYMBOL *sym = (yyvsp[-1].symbol);
                                                            SYMTABLE *cur_scope = (SYMTABLE*) stack_peek(scope_stack); 
                                                            ast_node_t *spec = (yyvsp[-2].ast_node); //dec specs

                                                            //fprintf(stderr, "current scope is: %s\n", get_scope_name(cur_scope->scope));
                                                            if (!cur_scope) {
                                                                fprintf(stderr, "Error: No current scope available\n");
                                                                exit(1);
                                                            }

                                                            while (sym != NULL) {
                                                                process_declaration(cur_scope, sym, spec);                                                               
                                                                sym = sym->next;
                                                            }
                                                            //fprintf(stderr, "making decl node\n"); 
                                                            (yyval.ast_node) = new_decl(spec, (yyvsp[-1].symbol));
                                                        }
#line 1859 "y.tab.c"
    break;

  case 9: /* init_declarator_list: init_declarator_list ',' init_declarator  */
#line 194 "parser.y"
                                                                {
                                                                    SYMBOL* p = (yyvsp[-2].symbol);
                                                                    while (p->next != NULL)
                                                                        p = p->next;
                                                                    p->next = (yyvsp[0].symbol); 
                                                                    (yyval.symbol) = (yyvsp[-2].symbol);
                                                                }
#line 1871 "y.tab.c"
    break;

  case 11: /* $@1: %empty  */
#line 205 "parser.y"
                                                    {
                                                        SYMBOL* sym = (yyvsp[0].symbol);
                                                        ast_node_t *spec = (yyvsp[-1].ast_node); //dec specs

                                                        if (sym->type == FUNCT_SYM) {

                                                            SYMTABLE* global = (SYMTABLE*) stack_peek(scope_stack);
                                                            sym->node = combine_nodes((yyvsp[-1].ast_node), sym->node);
    
                                                            //install function into global scope
                                                            process_declaration(global, sym, spec);                                                               
                                                            
                                                            // function scope for function body
                                                            SYMTABLE* funct_scope = st_create(FUNCT_SCOPE, global);
                                                            stack_push(scope_stack, funct_scope);


                                                            // prototype scope
                                                            if (sym->type == FUNCT_SYM && sym->node->type == FUNCT_N && sym->node->function.right) {
                                                                SYMTABLE* proto_scope = st_create(PROTO_SCOPE, funct_scope);
                                                                stack_push(scope_stack, proto_scope);
                                                                ast_node_t* param_list = sym->node->function.right;

                                                                // add parameters to proto scope
                                                                int cnt = 1;
                                                                while (param_list) {
                                                                    ast_node_t* param_node = param_list->list.head;
                                                                    if (param_node->type == PARAM_N) {

                                                                        char* key = (param_node->parameter.ident && param_node->parameter.ident->type == IDENT_N) ? param_node->parameter.ident->ident.name : NULL;

                                                                        if(key) {
                                                                            SYMBOL* param_sym = st_new_symbol(key, param_node, GENERAL_NS, VAR_SYM, AUTO_SC, NULL, file_name, line_num);
                                                                            st_install(proto_scope, param_sym);
                                                                        }
                                                                        else fprintf(stderr, "No key associated with input parameter #%d\n", cnt++); 
                                                                    }
                                                                    param_list = param_list->list.next;
                                                                }

                                                                // "promote" symbols within proto scope to funct scope (if there are any)
                                                                ht_t *ht = proto_scope->ht;
                                                                if(ht->filled) {
                                                                    //print_sym_table(proto_scope);
                                                                    for (int i = 0; i < ht->capacity; i++) {
                                                                        hash_item *item = &ht->data[i];
                                                                        if (item->isOccupied && !item->isDeleted) {
                                                                            SYMBOL *proto_sym = (SYMBOL *) item->pv;
                                                                            //fprintf(stderr, "moving key %s from proto scope to funct scope\n", proto_sym->key);
                                                                            st_install(funct_scope, proto_sym); 
                                                                        }
                                                                    }
                                                                }
                                                                
                                                                //pop proto_scope off stack
                                                                stack_pop(scope_stack);
                                                            }
                                                            fprintf(stderr, "\nEntering function scope for '%s'\n", sym->key);
                                                            //print_sym_table(funct_scope);
                                                            print_symbol(global, sym); 
                                                            in_function = 1;
                                                        }
                                                        else {
                                                            fprintf(stderr, "Expected function definition for symbol: %s", sym->key);
                                                            exit(1);
                                                        }
                                                    }
#line 1943 "y.tab.c"
    break;

  case 12: /* function_definition: decl_specifiers declarator $@1 compound_statement  */
#line 272 "parser.y"
                                                                       { 
                                                        //fprintf(stderr, "Exiting function scope\n");
                                                        SYMBOL* sym = (yyvsp[-2].symbol);

                                                        printf("\n---------------------------------------------\n"); 
                                                        printf("AST Dump for function %s\n", sym->key); 
                                                        printf("---------------------------------------------\n"); 
                                                        fprintf(stderr, "=========== AST DUMP =============\n");
                                                        print_ast_tree((yyvsp[0].ast_node), 0);

                                                        printf("\n---------------------------------------------\n"); 
                                                        printf("QUAD generation for function %s\n", sym->key); 
                                                        printf("---------------------------------------------\n"); 
                                                        fprintf(stderr, "=========== GEN QUADS ============\n");
                                                        create_quads((yyvsp[0].ast_node));
                                                        in_function = 0;

                                                        
                                                        SYMTABLE* st = (SYMTABLE*) stack_pop(scope_stack);
                                                        fprintf(stderr, "Exiting %s scope started at: <%s>:%d\n", get_scope_name(st->scope), st->start_file, st->start_line);
                                                    }
#line 1969 "y.tab.c"
    break;

  case 13: /* $@2: %empty  */
#line 296 "parser.y"
                            {
                                if(in_function != 1) {
                                    SYMTABLE* current = (SYMTABLE*) stack_peek(scope_stack);
                                    SYMTABLE* new_scope = st_create(BLOCK_SCOPE, current);
                                    stack_push(scope_stack, new_scope);
                                    fprintf(stderr, "Entering %s scope: <%s>:%d\n", get_scope_name(new_scope->scope), file_name, line_num);
                                }
                                in_function = 0;
                            }
#line 1983 "y.tab.c"
    break;

  case 14: /* compound_statement: '{' $@2 '}'  */
#line 305 "parser.y"
                            { (yyval.ast_node) = new_list(NULL); }
#line 1989 "y.tab.c"
    break;

  case 15: /* $@3: %empty  */
#line 306 "parser.y"
                            {
                                if(in_function != 1) {
                                    SYMTABLE* current = (SYMTABLE*) stack_peek(scope_stack);
                                    SYMTABLE* new_scope = st_create(BLOCK_SCOPE, current);
                                    stack_push(scope_stack, new_scope);
                                    fprintf(stderr, "Entering %s scope: <%s>:%d\n", get_scope_name(new_scope->scope), file_name, line_num);
                                } 
                                in_function = 0;
                            }
#line 2003 "y.tab.c"
    break;

  case 16: /* compound_statement: '{' $@3 decl_or_stmt_list '}'  */
#line 315 "parser.y"
                                              { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 2009 "y.tab.c"
    break;

  case 17: /* decl_or_stmt_list: decl_or_stmt  */
#line 318 "parser.y"
                                                        { (yyval.ast_node) = new_list((yyvsp[0].ast_node)); }
#line 2015 "y.tab.c"
    break;

  case 18: /* decl_or_stmt_list: decl_or_stmt_list decl_or_stmt  */
#line 319 "parser.y"
                                                        { (yyval.ast_node) = append_item((yyvsp[-1].ast_node), (yyvsp[0].ast_node)); }
#line 2021 "y.tab.c"
    break;

  case 19: /* decl_or_stmt: declaration  */
#line 322 "parser.y"
                              { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 2027 "y.tab.c"
    break;

  case 20: /* decl_or_stmt: statement  */
#line 323 "parser.y"
                            { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 2033 "y.tab.c"
    break;

  case 21: /* decl_specifiers: stg_class_specifier  */
#line 326 "parser.y"
                                                        { (yyval.ast_node) = new_list((yyvsp[0].ast_node)); }
#line 2039 "y.tab.c"
    break;

  case 22: /* decl_specifiers: stg_class_specifier decl_specifiers  */
#line 327 "parser.y"
                                                        { (yyval.ast_node) = append_item((yyvsp[0].ast_node), (yyvsp[-1].ast_node)); }
#line 2045 "y.tab.c"
    break;

  case 23: /* decl_specifiers: type_specifier  */
#line 328 "parser.y"
                                                        { (yyval.ast_node) = new_list((yyvsp[0].ast_node)); }
#line 2051 "y.tab.c"
    break;

  case 24: /* decl_specifiers: type_specifier decl_specifiers  */
#line 329 "parser.y"
                                                        { (yyval.ast_node) = append_item((yyvsp[0].ast_node), (yyvsp[-1].ast_node)); }
#line 2057 "y.tab.c"
    break;

  case 25: /* stg_class_specifier: EXTERN  */
#line 332 "parser.y"
                                { (yyval.ast_node) = new_decl_spec(0, EXTERN_SC); }
#line 2063 "y.tab.c"
    break;

  case 26: /* stg_class_specifier: STATIC  */
#line 333 "parser.y"
                                { (yyval.ast_node) = new_decl_spec(0, STATIC_SC); }
#line 2069 "y.tab.c"
    break;

  case 27: /* stg_class_specifier: AUTO  */
#line 334 "parser.y"
                                { (yyval.ast_node) = new_decl_spec(0, AUTO_SC); }
#line 2075 "y.tab.c"
    break;

  case 28: /* stg_class_specifier: REGISTER  */
#line 335 "parser.y"
                                { (yyval.ast_node) = new_decl_spec(0, REG_SC); }
#line 2081 "y.tab.c"
    break;

  case 29: /* type_specifier: FLOAT  */
#line 338 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(FLOAT_DT, 0);   }
#line 2087 "y.tab.c"
    break;

  case 30: /* type_specifier: DOUBLE  */
#line 339 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(DOUBLE_DT, 0);  }
#line 2093 "y.tab.c"
    break;

  case 31: /* type_specifier: INT  */
#line 340 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(INT_DT, 0);     }
#line 2099 "y.tab.c"
    break;

  case 32: /* type_specifier: LONG  */
#line 341 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(LONG_DT, 0);    }
#line 2105 "y.tab.c"
    break;

  case 33: /* type_specifier: SHORT  */
#line 342 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(SHORT_DT, 0);   }
#line 2111 "y.tab.c"
    break;

  case 34: /* type_specifier: CHAR  */
#line 343 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(CHAR_DT, 0);    }
#line 2117 "y.tab.c"
    break;

  case 35: /* type_specifier: VOID  */
#line 344 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(VOID_DT, 0);    }
#line 2123 "y.tab.c"
    break;

  case 36: /* type_specifier: SIGNED  */
#line 345 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(SIGNED_DT, 0);  }
#line 2129 "y.tab.c"
    break;

  case 37: /* type_specifier: UNSIGNED  */
#line 346 "parser.y"
                            { (yyval.ast_node) = new_decl_spec(UNSIGNED_DT, 0); }
#line 2135 "y.tab.c"
    break;

  case 39: /* $@4: %empty  */
#line 352 "parser.y"
                                                   {
                                                        //fprintf(stderr, "Token value: %d (STRUCT=%d, UNION=%d)\n", $1, STRUCT, UNION);
                                                        SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);

                                                        // use another variable to store where to insert symbol as we don't want a struct within a struct
                                                        SYMTABLE* insert_st = st->is_struct_scope ? st->outer : st;
                                                        
                                                        SYMBOL* sym = st_lookup(insert_st, (yyvsp[-1].string).string_literal, TAG_NS);
                                                        fprintf(stderr, "Going to insert in %s <%s>:%d\n", get_scope_name(insert_st->scope), insert_st->start_file, insert_st->start_line); 

                                                        if (!sym) {
                                                            sym = st_new_symbol((yyvsp[-1].string).string_literal, NULL, TAG_NS, ((yyvsp[-2].TOKEN) == STRUCT ? STRUCT_SYM : UNION_SYM), UNKNOWN_SC, insert_st, file_name, line_num);
                                                            st_install(insert_st, sym);
                                                            fprintf(stderr, "installed struct/union with key %s in %s scope: <%s>:%d\n", sym->key, get_scope_name(insert_st->scope), insert_st->start_file, insert_st->start_line);
                                                        }
                                                        if (!sym->mini_st) {
                                                            sym->mini_st = st_create(BLOCK_SCOPE, insert_st);
                                                            sym->mini_st->is_struct_scope = 1;
                                                        }
                                                        stack_push(scope_stack, sym->mini_st);
                                                        stack_push(struct_union_stack, sym);

                                                        //st = (SYMTABLE*) stack_peek(scope_stack);
                                                        //fprintf(stderr, "current scope is %s\n", get_scope_name(st->scope));

                                                        //print_sym_table(st, file_name, line_num);
                                                        print_symbol(st, sym);
                                                    }
#line 2168 "y.tab.c"
    break;

  case 40: /* struct_union_specifier: struct_or_union IDENT '{' $@4 struct_declaration_list '}'  */
#line 380 "parser.y"
                                                    { 
                                                        SYMTABLE* st = (SYMTABLE*) stack_pop(scope_stack);
                                                        SYMBOL* sym = (SYMBOL*) stack_peek(struct_union_stack);

                                                        // set parent to current (if not used) so we can print where struct is defined
                                                        if(!sym->parent_sym) sym->parent_sym = sym;

                                                        sym->is_complete = 1;
                                                        (yyval.ast_node) = new_struct_union((yyvsp[-5].TOKEN), sym);
                                                        //fprintf(stderr, "Exited struct/union scope started at <%s>:%d\n", st->start_file, st->start_line); 
                                                    }
#line 2184 "y.tab.c"
    break;

  case 41: /* $@5: %empty  */
#line 391 "parser.y"
                                                    { 
                                                        SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);

                                                        // use another variable to store where to insert symbol as we don't want a struct within a struct
                                                        SYMTABLE* insert_st = st->is_struct_scope ? st->outer : st;
                                                        //fprintf(stderr, "Entering struct/union scope: <%s>:%d\n", st->start_file, insert_st->start_line); 

                                                        SYMBOL* sym = st_new_symbol(NULL, NULL, TAG_NS, ((yyvsp[-1].TOKEN) == STRUCT ? STRUCT_SYM : UNION_SYM), UNKNOWN_SC, insert_st, file_name, line_num);

                                                        if (!sym->mini_st) {
                                                            sym->mini_st = st_create(BLOCK_SCOPE, insert_st);
                                                            sym->mini_st->is_struct_scope = 1;
                                                        }
                                                        stack_push(scope_stack, sym->mini_st);
                                                        stack_push(struct_union_stack, sym); 

                                                        //print_sym_table(st, file_name, line_num);
                                                        //print_symbol(st, sym);
                                                    }
#line 2208 "y.tab.c"
    break;

  case 42: /* struct_union_specifier: struct_or_union '{' $@5 struct_declaration_list '}'  */
#line 410 "parser.y"
                                                    {
                                                        SYMTABLE* st = (SYMTABLE*) stack_pop(scope_stack);
                                                        SYMBOL* sym = (SYMBOL*) stack_peek(struct_union_stack); 

                                                        // set parent to current (if not used) so we can print where struct is defined
                                                        if(!sym->parent_sym) sym->parent_sym = sym;
                                                        sym->is_complete = 1;

                                                        (yyval.ast_node) = new_struct_union((yyvsp[-4].TOKEN), sym);
                                                        //fprintf(stderr, "Exited struct/union scope started at <%s>:%d\n", st->start_file, st->start_line); 
                                                    }
#line 2224 "y.tab.c"
    break;

  case 43: /* struct_union_specifier: struct_or_union IDENT  */
#line 421 "parser.y"
                                                {
                                                    SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
                                                    SYMTABLE* insert_st = st->is_struct_scope ? st->outer : st;

                                                    SYMBOL* sym = st_lookup_single(insert_st, (yyvsp[0].string).string_literal, TAG_NS);
                                                    //fprintf(stderr, "found sym is: %d | current scope is: %s at <%s>:%d\n", sym ? 1 : 0, get_scope_name(insert_st->scope), insert_st->start_file, insert_st->start_line); 

                                                    if (!sym) {
                                                        sym = st_new_symbol((yyvsp[0].string).string_literal, NULL, TAG_NS, ((yyvsp[-1].TOKEN) == STRUCT ? STRUCT_SYM : UNION_SYM), UNKNOWN_SC, insert_st, file_name, line_num);
                                                        st_install(insert_st, sym);
                                                        fprintf(stderr, "installed struct/union with key %s in %s scope: <%s>:%d\n", sym->key, get_scope_name(insert_st->scope), insert_st->start_file, insert_st->start_line);
                                                    }
                                                    (yyval.ast_node) = new_struct_union((yyvsp[-1].TOKEN), sym);
                                                    //print_symbol(insert_st, sym);
                                                }
#line 2244 "y.tab.c"
    break;

  case 44: /* struct_or_union: STRUCT  */
#line 439 "parser.y"
                            { (yyval.TOKEN) = STRUCT; }
#line 2250 "y.tab.c"
    break;

  case 45: /* struct_or_union: UNION  */
#line 440 "parser.y"
                            { (yyval.TOKEN) = UNION; }
#line 2256 "y.tab.c"
    break;

  case 46: /* struct_declaration_list: struct_declaration  */
#line 444 "parser.y"
                                                                        { (yyval.ast_node) = new_list((yyvsp[0].ast_node)); }
#line 2262 "y.tab.c"
    break;

  case 47: /* struct_declaration_list: struct_declaration_list struct_declaration  */
#line 445 "parser.y"
                                                                        { (yyval.ast_node) = append_item((yyvsp[-1].ast_node), (yyvsp[0].ast_node)); }
#line 2268 "y.tab.c"
    break;

  case 48: /* struct_declaration: specifier_list struct_declarator_list ';'  */
#line 448 "parser.y"
                                                                { 
                                                                    SYMBOL* sym_list = (yyvsp[-1].symbol);
                                                                    ast_node_t* spec = (yyvsp[-2].ast_node);
                                                                    SYMTABLE* member_st = (SYMTABLE*) stack_peek(scope_stack);

                                                                    fprintf(stderr, "going to member st\n"); 

                                                                    while (sym_list) {
                                                                        SYMBOL* sym = sym_list;
                                                                        sym->node = combine_nodes(spec, sym->node);
                                                                        sym->name_space = MEMBER_NS;
                                                                        sym->type = MEMBER_SYM;
                                                                        sym->stg_class = AUTO_SC;

                                                                        //sym->parent_sym = current_struct_union;
                                                                        SYMBOL* current_sym = (SYMBOL*) stack_peek(struct_union_stack);
                                                                        if (!sym->parent_sym) sym->parent_sym = current_sym;
                                                                        
                                                                        if (st_install(member_st, sym) != 0) {
                                                                            fprintf(stderr, "Error: duplicate member %s\n", sym->key);
                                                                            exit(1);
                                                                        }
                                                                        print_symbol(member_st, sym);

                                                                        sym_list = sym_list->next;
                                                                    }
                                                                    //print_sym_table(member_st, file_name, line_num);
                                                                }
#line 2301 "y.tab.c"
    break;

  case 49: /* specifier_list: type_specifier  */
#line 480 "parser.y"
                                                { (yyval.ast_node) = new_list((yyvsp[0].ast_node)); }
#line 2307 "y.tab.c"
    break;

  case 50: /* specifier_list: type_specifier specifier_list  */
#line 481 "parser.y"
                                                { (yyval.ast_node) = append_item((yyvsp[-1].ast_node), (yyvsp[0].ast_node)); }
#line 2313 "y.tab.c"
    break;

  case 51: /* struct_declarator_list: struct_declarator  */
#line 485 "parser.y"
                                                                        { (yyval.symbol) = (yyvsp[0].symbol); }
#line 2319 "y.tab.c"
    break;

  case 52: /* struct_declarator_list: struct_declarator_list ',' struct_declarator  */
#line 486 "parser.y"
                                                                        { 
                                                                            SYMBOL* p = (yyvsp[-2].symbol);
                                                                            while (p->next != NULL) p = p->next;
                                                                            p->next = (yyvsp[0].symbol); 
                                                                            (yyval.symbol) = (yyvsp[-2].symbol);
                                                                        }
#line 2330 "y.tab.c"
    break;

  case 53: /* struct_declarator: declarator  */
#line 494 "parser.y"
                                 {(yyval.symbol) = (yyvsp[0].symbol); }
#line 2336 "y.tab.c"
    break;

  case 54: /* declarator: pointer_declarator  */
#line 498 "parser.y"
                                  { (yyval.symbol) = (yyvsp[0].symbol); }
#line 2342 "y.tab.c"
    break;

  case 55: /* declarator: direct_declarator  */
#line 499 "parser.y"
                                  { (yyval.symbol) = (yyvsp[0].symbol); }
#line 2348 "y.tab.c"
    break;

  case 56: /* direct_declarator: simple_declarator  */
#line 503 "parser.y"
                                            { 
                                                SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
                                                (yyval.symbol) = st_new_symbol((yyvsp[0].string).string_literal, NULL, GENERAL_NS, VAR_SYM, (st->scope == FILE_SCOPE ? UNKNOWN_SC : AUTO_SC), NULL, file_name, line_num); 
                                            }
#line 2357 "y.tab.c"
    break;

  case 57: /* direct_declarator: '(' declarator ')'  */
#line 507 "parser.y"
                                            { (yyval.symbol) = (yyvsp[-1].symbol); }
#line 2363 "y.tab.c"
    break;

  case 61: /* pointer_declarator: pointer direct_declarator  */
#line 516 "parser.y"
                                                {     
                                                    (yyvsp[0].symbol)->node = combine_nodes((yyvsp[-1].ast_node), (yyvsp[0].symbol)->node); 
                                                    (yyval.symbol) = (yyvsp[0].symbol); 
                                                    //fprintf(stderr, "adding ptr\n");  
                                                }
#line 2373 "y.tab.c"
    break;

  case 62: /* pointer: '*'  */
#line 523 "parser.y"
                        { (yyval.ast_node) = new_pointer(NULL); }
#line 2379 "y.tab.c"
    break;

  case 63: /* pointer: '*' pointer  */
#line 524 "parser.y"
                        { (yyval.ast_node) = new_pointer((yyvsp[0].ast_node));   }
#line 2385 "y.tab.c"
    break;

  case 64: /* array_declarator: direct_declarator '[' ']'  */
#line 527 "parser.y"
                                                            { 
                                                                ast_node_t* temp = new_array((yyvsp[-2].symbol)->node, 0);
                                                                SYMBOL* sym = (yyvsp[-2].symbol);

                                                                if(sym->node) {
                                                                    sym->node = combine_nodes(sym->node, temp);
                                                                    fprintf(stderr, "combining nodes for array\n");
                                                                }
                                                                else sym->node = temp;

                                                                //print_ast_tree(sym->node, 0); 

                                                                (yyval.symbol) = sym;
                                                            }
#line 2404 "y.tab.c"
    break;

  case 65: /* array_declarator: direct_declarator '[' NUMBER ']'  */
#line 541 "parser.y"
                                                            { 
                                                                if(!(yyvsp[-1].number)._int) {
                                                                    fprintf(stderr, "Please only use integers for array declarations");
                                                                    exit(0);
                                                                }

                                                                ast_node_t* temp = new_array((yyvsp[-3].symbol)->node, (yyvsp[-1].number)._int); 
                                                                SYMBOL* sym = (yyvsp[-3].symbol); 

                                                                if(sym->node) sym->node = combine_nodes(sym->node, temp);
                                                                else sym->node = temp;

                                                                (yyval.symbol) = sym;
                                                            }
#line 2423 "y.tab.c"
    break;

  case 66: /* function_declarator: direct_declarator '(' ')'  */
#line 558 "parser.y"
                                                                    {   
                                                                        fprintf(stderr, "function declarator detected\n"); 

                                                                        SYMBOL* sym = (yyvsp[-2].symbol);
                                                                        ast_node_t* temp = new_function(sym->key, NULL, NULL); 

                                                                        if(sym->type == VAR_SYM) sym->type = FUNCT_SYM;
                                                                        
                                                                        temp->function.left = sym->node;
                                                                        sym->node = temp;
                                                                        
                                                                        //print_ast_tree(sym->node, 0); 
                                                                        (yyval.symbol) = sym;
                                                                    }
#line 2442 "y.tab.c"
    break;

  case 67: /* function_declarator: direct_declarator '(' identifier_list ')'  */
#line 572 "parser.y"
                                                                    { 
                                                                        SYMBOL* sym = (yyvsp[-3].symbol);
                                                                        ast_node_t* temp = new_function(sym->key, sym->node, (yyvsp[-1].ast_node)); 

                                                                        if(sym->type == VAR_SYM) sym->type = FUNCT_SYM;

                                                                        if(sym->node) sym->node = combine_nodes(sym->node, temp);
                                                                        else sym->node = temp;

                                                                        (yyval.symbol) = sym;
                                                                    }
#line 2458 "y.tab.c"
    break;

  case 68: /* function_declarator: direct_declarator '(' parameter_list ')'  */
#line 583 "parser.y"
                                                                    {
                                                                        SYMBOL* sym = (yyvsp[-3].symbol);
                                                                        ast_node_t* temp = new_function(sym->key, NULL, NULL);

                                                                        if (sym->type == VAR_SYM) sym->type = FUNCT_SYM;

                                                                        temp->function.left = sym->node;
                                                                        temp->function.right = (yyvsp[-1].ast_node);
                                                                        sym->node = temp;

                                                                        //print_ast_tree(sym->node, 0); 

                                                                        (yyval.symbol) = sym;
                                                                    }
#line 2477 "y.tab.c"
    break;

  case 69: /* parameter_list: parameter_declaration  */
#line 599 "parser.y"
                                       { (yyval.ast_node) = new_list((yyvsp[0].ast_node)); }
#line 2483 "y.tab.c"
    break;

  case 70: /* parameter_list: parameter_list ',' parameter_declaration  */
#line 600 "parser.y"
                                                          { (yyval.ast_node) = append_item((yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2489 "y.tab.c"
    break;

  case 71: /* parameter_declaration: decl_specifiers declarator  */
#line 603 "parser.y"
                                                    { (yyval.ast_node) = new_param((yyvsp[-1].ast_node), attach_ident((yyvsp[0].symbol)->node, (yyvsp[0].symbol)->key)); }
#line 2495 "y.tab.c"
    break;

  case 72: /* parameter_declaration: decl_specifiers  */
#line 604 "parser.y"
                                                    { (yyval.ast_node) = new_param((yyvsp[0].ast_node), NULL); }
#line 2501 "y.tab.c"
    break;

  case 73: /* identifier_list: IDENT  */
#line 607 "parser.y"
                                                { (yyval.ast_node) = new_ident((yyvsp[0].string).string_literal); }
#line 2507 "y.tab.c"
    break;

  case 74: /* identifier_list: identifier_list ',' IDENT  */
#line 608 "parser.y"
                                                { (yyval.ast_node) = append_item((yyvsp[-2].ast_node), new_ident((yyvsp[0].string).string_literal)); }
#line 2513 "y.tab.c"
    break;

  case 76: /* type_name: specifier_list abstract_declarator  */
#line 612 "parser.y"
                                                { (yyval.ast_node) = combine_nodes((yyvsp[-1].ast_node), (yyvsp[0].ast_node)); }
#line 2519 "y.tab.c"
    break;

  case 79: /* abstract_declarator: pointer direct_abstract_declarator  */
#line 617 "parser.y"
                                                         { (yyval.ast_node) = combine_nodes((yyvsp[-1].ast_node), (yyvsp[0].ast_node)); }
#line 2525 "y.tab.c"
    break;

  case 80: /* direct_abstract_declarator: '(' abstract_declarator ')'  */
#line 620 "parser.y"
                                                                        { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 2531 "y.tab.c"
    break;

  case 81: /* direct_abstract_declarator: '[' ']'  */
#line 621 "parser.y"
                                                                        { (yyval.ast_node) = new_array(NULL, 0);              }
#line 2537 "y.tab.c"
    break;

  case 82: /* direct_abstract_declarator: '[' NUMBER ']'  */
#line 622 "parser.y"
                                                                        { (yyval.ast_node) = new_array(NULL, (yyvsp[-1].number)._int);        }
#line 2543 "y.tab.c"
    break;

  case 83: /* direct_abstract_declarator: direct_abstract_declarator '[' ']'  */
#line 623 "parser.y"
                                                                        { (yyval.ast_node) = new_array((yyvsp[-2].ast_node), 0);                }
#line 2549 "y.tab.c"
    break;

  case 84: /* direct_abstract_declarator: direct_abstract_declarator '[' NUMBER ']'  */
#line 624 "parser.y"
                                                                        { (yyval.ast_node) = new_array((yyvsp[-3].ast_node), (yyvsp[-1].number)._int);          }
#line 2555 "y.tab.c"
    break;

  case 85: /* direct_abstract_declarator: '(' ')'  */
#line 625 "parser.y"
                                                                        { (yyval.ast_node) = new_function(NULL, NULL, NULL);  }
#line 2561 "y.tab.c"
    break;

  case 86: /* direct_abstract_declarator: direct_abstract_declarator '(' ')'  */
#line 626 "parser.y"
                                                                        { (yyval.ast_node) = new_function(NULL, (yyvsp[-2].ast_node), NULL);    }
#line 2567 "y.tab.c"
    break;

  case 88: /* statement: expression ';'  */
#line 630 "parser.y"
                           { /* expression statement */ }
#line 2573 "y.tab.c"
    break;

  case 100: /* if_else_statement: if_statement ELSE statement  */
#line 646 "parser.y"
                                                { 
                                                    (yyvsp[-2].ast_node)->if_node.else_statement = (yyvsp[0].ast_node);
                                                    (yyval.ast_node) = (yyvsp[-2].ast_node);
                                                }
#line 2582 "y.tab.c"
    break;

  case 101: /* if_statement: IF '(' expression ')' statement  */
#line 652 "parser.y"
                                                { (yyval.ast_node) = new_if((yyvsp[-2].ast_node), (yyvsp[0].ast_node), NULL); }
#line 2588 "y.tab.c"
    break;

  case 102: /* labeled_statement: named_label statement  */
#line 655 "parser.y"
                                                { (yyval.ast_node) = new_label((yyvsp[-1].symbol), (yyvsp[0].ast_node)); }
#line 2594 "y.tab.c"
    break;

  case 103: /* labeled_statement: switch_label statement  */
#line 656 "parser.y"
                                                { 
                                                    (yyvsp[-1].ast_node)->switch_label.statement = (yyvsp[0].ast_node);
                                                    (yyval.ast_node) = (yyvsp[-1].ast_node);
                                                }
#line 2603 "y.tab.c"
    break;

  case 104: /* named_label: IDENT ':'  */
#line 662 "parser.y"
                            {
                                char* key = (yyvsp[-1].string).string_literal;
                                SYMTABLE* st = stack_peek(scope_stack);

                                if (st->scope == FILE_SCOPE) {
                                    fprintf(stderr, "Cannot install label '%s' into file scope!", key); 
                                    exit(0); 
                                }

                                while(st->scope != FUNCT_SCOPE)
                                    st = st->outer;

                                SYMBOL* sym = st_lookup(st, key, LABEL_NS); 

                                // if sym doesn't already exist, install it in function scope and mark it as "seen" (complete)
                                if(!sym) {
                                    sym = st_new_symbol(key, NULL, LABEL_NS, LABEL_SYM, EXTERN_SC, st, file_name, line_num); 
                                    sym->is_complete = 1;
                                    st_install(st, sym); 

                                    fprintf(stderr, "created symbol '%s' in %s\n", sym->key, get_scope_name(st->scope)); 
                                }
                                else {
                                    sym->is_complete = 1;
                                    sym->line_num = line_num;
                                }
                                

                                print_symbol(st, sym); 

                                (yyval.symbol) = sym; //new_label(sym, $3); 
                            }
#line 2640 "y.tab.c"
    break;

  case 105: /* switch_label: CASE constant_expression ':'  */
#line 697 "parser.y"
                                            { (yyval.ast_node) = new_switch_label(CASE_N, (yyvsp[-1].ast_node), NULL); }
#line 2646 "y.tab.c"
    break;

  case 106: /* switch_label: DEFAULT ':'  */
#line 698 "parser.y"
                                            { (yyval.ast_node) = new_switch_label(DEFAULT_N, NULL, NULL); }
#line 2652 "y.tab.c"
    break;

  case 110: /* while_statement: WHILE '(' expression ')' statement  */
#line 707 "parser.y"
                                                        { (yyval.ast_node) = new_while(WHILE_N, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2658 "y.tab.c"
    break;

  case 111: /* do_statement: DO statement WHILE '(' expression ')' ';'  */
#line 710 "parser.y"
                                                         { (yyval.ast_node) = new_while(DOWHILE_N, (yyvsp[-2].ast_node), (yyvsp[-5].ast_node)); }
#line 2664 "y.tab.c"
    break;

  case 112: /* for_statement: FOR '(' ';' ';' ')' statement  */
#line 714 "parser.y"
                                                                                    { (yyval.ast_node) = new_for(NULL, NULL, NULL, (yyvsp[0].ast_node)); }
#line 2670 "y.tab.c"
    break;

  case 113: /* for_statement: FOR '(' expression ';' expression ';' expression ')' statement  */
#line 715 "parser.y"
                                                                                    { (yyval.ast_node) = new_for((yyvsp[-6].ast_node), (yyvsp[-4].ast_node), (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2676 "y.tab.c"
    break;

  case 114: /* for_statement: FOR '(' declaration ';' expression ';' expression ')' statement  */
#line 716 "parser.y"
                                                                                    { (yyval.ast_node) = new_for((yyvsp[-6].ast_node), (yyvsp[-4].ast_node), (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2682 "y.tab.c"
    break;

  case 115: /* switch_statement: SWITCH '(' expression ')' statement  */
#line 720 "parser.y"
                                                        { (yyval.ast_node) = new_switch((yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2688 "y.tab.c"
    break;

  case 116: /* break_statement: BREAK ';'  */
#line 724 "parser.y"
                            { (yyval.ast_node) = new_continue_break(BREAK_N); }
#line 2694 "y.tab.c"
    break;

  case 117: /* continue_statement: CONTINUE ';'  */
#line 728 "parser.y"
                                  { (yyval.ast_node) = new_continue_break(CONTINUE_N); }
#line 2700 "y.tab.c"
    break;

  case 118: /* return_statement: RETURN ';'  */
#line 732 "parser.y"
                                            { (yyval.ast_node) = new_return(NULL); }
#line 2706 "y.tab.c"
    break;

  case 119: /* return_statement: RETURN expression ';'  */
#line 733 "parser.y"
                                            { (yyval.ast_node) = new_return((yyvsp[-1].ast_node)); }
#line 2712 "y.tab.c"
    break;

  case 120: /* goto_statement: GOTO IDENT ';'  */
#line 737 "parser.y"
                                { 
                                    char* key = (yyvsp[-1].string).string_literal;
                                    SYMTABLE* st = stack_peek(scope_stack);

                                    // make sure we look/install in function scope
                                    while(st->scope != FUNCT_SCOPE) {
                                        st = st->outer;
                                    }

                                    SYMBOL* sym = st_lookup(st, key, LABEL_NS); 

                                    // if sym doesn't already exist, install it in function scope and mark it as not "seen" (incomplete)
                                    if(!sym) {

                                        //fprintf(stderr, "insert scope is %s\n", get_scope_name(st->scope)); 
                                        sym = st_new_symbol(key, NULL, LABEL_NS, LABEL_SYM, EXTERN_SC, st, file_name, line_num); 
                                        sym->is_complete = 0;
                                        st_install(st, sym); 

                                        fprintf(stderr, "created symbol '%s' in %s\n", sym->key, get_scope_name(st->scope)); 
                                    }

                                    //fprintf(stderr, "installed %s into %s scope!\n", sym->key, get_scope_name(st->scope));

                                    (yyval.ast_node) = new_goto(sym); 
                                    //fprintf(stderr, "created goto symbol\n");
                                }
#line 2744 "y.tab.c"
    break;

  case 121: /* null_statement: ';'  */
#line 766 "parser.y"
                     { (yyval.ast_node) = NULL; }
#line 2750 "y.tab.c"
    break;

  case 123: /* primary_expression: IDENT  */
#line 774 "parser.y"
                            { (yyval.ast_node) = new_ident((yyvsp[0].string).string_literal); }
#line 2756 "y.tab.c"
    break;

  case 124: /* primary_expression: NUMBER  */
#line 775 "parser.y"
                             { (yyval.ast_node) = new_number((yyvsp[0].number)); }
#line 2762 "y.tab.c"
    break;

  case 125: /* primary_expression: STRING  */
#line 776 "parser.y"
                             { (yyval.ast_node) = new_string((yyvsp[0].string)); }
#line 2768 "y.tab.c"
    break;

  case 126: /* primary_expression: '(' expression ')'  */
#line 777 "parser.y"
                                         { (yyval.ast_node) = (yyvsp[-1].ast_node); }
#line 2774 "y.tab.c"
    break;

  case 133: /* subscript_expression: postfix_expression '[' expression ']'  */
#line 789 "parser.y"
                                                                {   
                                                                    ast_node_t* tmp = new_genop(BINOP_N, '+', (yyvsp[-3].ast_node), (yyvsp[-1].ast_node)); 
                                                                    (yyval.ast_node) = new_unop('*', tmp);
                                                                }
#line 2783 "y.tab.c"
    break;

  case 134: /* component_selection_expression: postfix_expression '.' IDENT  */
#line 797 "parser.y"
                                                                {   
                                                                    ast_node_t* tmp = new_ident((yyvsp[0].string).string_literal); 
                                                                    (yyval.ast_node) = new_genop(BINOP_N, '.', (yyvsp[-2].ast_node), tmp);
                                                                }
#line 2792 "y.tab.c"
    break;

  case 135: /* component_selection_expression: postfix_expression POINT IDENT  */
#line 802 "parser.y"
                                                                {   
                                                                    ast_node_t* i = new_ident((yyvsp[0].string).string_literal); 
                                                                    ast_node_t* u = new_unop('*', (yyvsp[-2].ast_node));
                                                                    (yyval.ast_node) = new_genop(BINOP_N, '.', u, i); 
                                                                }
#line 2802 "y.tab.c"
    break;

  case 136: /* function_call: postfix_expression '(' expression_list ')'  */
#line 809 "parser.y"
                                                           { (yyval.ast_node) = new_function_call((yyvsp[-3].ast_node), (yyvsp[-1].ast_node)); }
#line 2808 "y.tab.c"
    break;

  case 137: /* function_call: postfix_expression '(' ')'  */
#line 810 "parser.y"
                                            { (yyval.ast_node) = new_function_call((yyvsp[-2].ast_node), NULL); }
#line 2814 "y.tab.c"
    break;

  case 138: /* expression_list: assignment_expression  */
#line 813 "parser.y"
                                         { (yyval.ast_node) = new_list((yyvsp[0].ast_node)); }
#line 2820 "y.tab.c"
    break;

  case 139: /* expression_list: expression_list ',' assignment_expression  */
#line 814 "parser.y"
                                                             { (yyval.ast_node) = append_item((yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2826 "y.tab.c"
    break;

  case 140: /* post_increment_expression: postfix_expression PLUSPLUS  */
#line 817 "parser.y"
                                                        {(yyval.ast_node) = new_unop(PLUSPLUS, (yyvsp[-1].ast_node)); }
#line 2832 "y.tab.c"
    break;

  case 141: /* post_decrement_expression: postfix_expression MINMIN  */
#line 820 "parser.y"
                                                      { (yyval.ast_node) = new_unop(MINMIN, (yyvsp[-1].ast_node)); }
#line 2838 "y.tab.c"
    break;

  case 152: /* cast_expression: unary_expression  */
#line 837 "parser.y"
                                    { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 2844 "y.tab.c"
    break;

  case 153: /* cast_expression: '(' type_name ')' cast_expression  */
#line 838 "parser.y"
                                                    { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 2850 "y.tab.c"
    break;

  case 154: /* sizeof_expression: SIZEOF '(' type_name ')'  */
#line 842 "parser.y"
                                                { (yyval.ast_node) = new_unop(SIZEOF, (yyvsp[-1].ast_node)); }
#line 2856 "y.tab.c"
    break;

  case 155: /* sizeof_expression: SIZEOF unary_expression  */
#line 843 "parser.y"
                                                { (yyval.ast_node) = new_unop(SIZEOF, (yyvsp[0].ast_node)); }
#line 2862 "y.tab.c"
    break;

  case 156: /* unary_minus_expression: '-' cast_expression  */
#line 848 "parser.y"
                                                { (yyval.ast_node) = new_unop('-', (yyvsp[0].ast_node)); }
#line 2868 "y.tab.c"
    break;

  case 157: /* unary_plus_expression: '+' cast_expression  */
#line 851 "parser.y"
                                                { (yyval.ast_node) = new_unop('+', (yyvsp[0].ast_node)); }
#line 2874 "y.tab.c"
    break;

  case 158: /* logical_negation_expression: '!' cast_expression  */
#line 854 "parser.y"
                                                    { (yyval.ast_node) = new_unop('!', (yyvsp[0].ast_node)); }
#line 2880 "y.tab.c"
    break;

  case 159: /* bitwise_negation_expression: '~' cast_expression  */
#line 857 "parser.y"
                                                    { (yyval.ast_node) = new_unop('~', (yyvsp[0].ast_node)); }
#line 2886 "y.tab.c"
    break;

  case 160: /* address_expression: '&' cast_expression  */
#line 860 "parser.y"
                                         { (yyval.ast_node) = new_unop('&', (yyvsp[0].ast_node)); }
#line 2892 "y.tab.c"
    break;

  case 161: /* indirection_expression: '*' cast_expression  */
#line 863 "parser.y"
                                             { (yyval.ast_node) = new_unop('*', (yyvsp[0].ast_node)); }
#line 2898 "y.tab.c"
    break;

  case 162: /* preincrement_expression: PLUSPLUS unary_expression  */
#line 866 "parser.y"
                                                    {
                                                        NUMTYPE* tmp = (NUMTYPE*) malloc(sizeof(NUMTYPE));
                                                        tmp->type = INT_T;
                                                        tmp->sign = UNSIGNED_T;
                                                        tmp->_int = 1;

                                                        (yyval.ast_node) = new_genop(ASSIGNOP_N, '+', (yyvsp[0].ast_node), new_number(*tmp)); 
                                                    }
#line 2911 "y.tab.c"
    break;

  case 163: /* predecrement_expression: MINMIN unary_expression  */
#line 877 "parser.y"
                                                  {
                                                        NUMTYPE* tmp = (NUMTYPE*) malloc(sizeof(NUMTYPE));
                                                        tmp->type = INT_T;
                                                        tmp->sign = UNSIGNED_T;
                                                        tmp->_int = 1;

                                                        (yyval.ast_node) = new_genop(ASSIGNOP_N, '-', (yyvsp[0].ast_node), new_number(*tmp));
                                                  }
#line 2924 "y.tab.c"
    break;

  case 164: /* multiplicative_expression: cast_expression  */
#line 889 "parser.y"
                                            { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 2930 "y.tab.c"
    break;

  case 165: /* multiplicative_expression: multiplicative_expression '*' cast_expression  */
#line 890 "parser.y"
                                                                            { (yyval.ast_node) = new_genop(BINOP_N, '*', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2936 "y.tab.c"
    break;

  case 166: /* multiplicative_expression: multiplicative_expression '/' cast_expression  */
#line 891 "parser.y"
                                                                            { (yyval.ast_node) = new_genop(BINOP_N, '/', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2942 "y.tab.c"
    break;

  case 167: /* multiplicative_expression: multiplicative_expression '%' cast_expression  */
#line 892 "parser.y"
                                                                            { (yyval.ast_node) = new_genop(BINOP_N, '%', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2948 "y.tab.c"
    break;

  case 168: /* additive_expression: multiplicative_expression  */
#line 895 "parser.y"
                                                { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 2954 "y.tab.c"
    break;

  case 169: /* additive_expression: additive_expression '+' multiplicative_expression  */
#line 896 "parser.y"
                                                                            { (yyval.ast_node) = new_genop(BINOP_N, '+', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2960 "y.tab.c"
    break;

  case 170: /* additive_expression: additive_expression '-' multiplicative_expression  */
#line 897 "parser.y"
                                                                            { (yyval.ast_node) = new_genop(BINOP_N, '-', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2966 "y.tab.c"
    break;

  case 171: /* shift_expression: additive_expression  */
#line 900 "parser.y"
                                        { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 2972 "y.tab.c"
    break;

  case 172: /* shift_expression: shift_expression SL additive_expression  */
#line 901 "parser.y"
                                                            { (yyval.ast_node) = new_genop(BINOP_N, SL, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2978 "y.tab.c"
    break;

  case 173: /* shift_expression: shift_expression SR additive_expression  */
#line 902 "parser.y"
                                                            { (yyval.ast_node) = new_genop(BINOP_N, SR, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2984 "y.tab.c"
    break;

  case 174: /* relational_expression: shift_expression  */
#line 905 "parser.y"
                                            { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 2990 "y.tab.c"
    break;

  case 175: /* relational_expression: relational_expression '<' shift_expression  */
#line 906 "parser.y"
                                                                        { (yyval.ast_node) = new_genop(COMPOP_N, '<', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 2996 "y.tab.c"
    break;

  case 176: /* relational_expression: relational_expression GTEQ shift_expression  */
#line 907 "parser.y"
                                                                        { (yyval.ast_node) = new_genop(COMPOP_N, GTEQ, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3002 "y.tab.c"
    break;

  case 177: /* relational_expression: relational_expression '>' shift_expression  */
#line 908 "parser.y"
                                                                        { (yyval.ast_node) = new_genop(COMPOP_N, '>', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3008 "y.tab.c"
    break;

  case 178: /* relational_expression: relational_expression LTEQ shift_expression  */
#line 909 "parser.y"
                                                                        { (yyval.ast_node) = new_genop(COMPOP_N, LTEQ, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3014 "y.tab.c"
    break;

  case 179: /* equality_expression: relational_expression  */
#line 912 "parser.y"
                                            { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3020 "y.tab.c"
    break;

  case 180: /* equality_expression: equality_expression EQEQ relational_expression  */
#line 913 "parser.y"
                                                                        { (yyval.ast_node) = new_genop(COMPOP_N, EQEQ, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3026 "y.tab.c"
    break;

  case 181: /* equality_expression: equality_expression NOTEQ relational_expression  */
#line 914 "parser.y"
                                                                        { (yyval.ast_node) = new_genop(COMPOP_N, NOTEQ, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3032 "y.tab.c"
    break;

  case 182: /* bitwise_or_expression: bitwise_xor_expression  */
#line 917 "parser.y"
                                                { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3038 "y.tab.c"
    break;

  case 183: /* bitwise_or_expression: bitwise_or_expression '|' bitwise_xor_expression  */
#line 918 "parser.y"
                                                                            { (yyval.ast_node) = new_genop(BINOP_N, '|', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3044 "y.tab.c"
    break;

  case 184: /* bitwise_xor_expression: bitwise_and_expression  */
#line 921 "parser.y"
                                                { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3050 "y.tab.c"
    break;

  case 185: /* bitwise_xor_expression: bitwise_xor_expression '^' bitwise_and_expression  */
#line 922 "parser.y"
                                                                            { (yyval.ast_node) = new_genop(BINOP_N, '^', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3056 "y.tab.c"
    break;

  case 186: /* bitwise_and_expression: equality_expression  */
#line 925 "parser.y"
                                             { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3062 "y.tab.c"
    break;

  case 187: /* bitwise_and_expression: bitwise_and_expression '&' equality_expression  */
#line 926 "parser.y"
                                                                        { (yyval.ast_node) = new_genop(BINOP_N, '&', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3068 "y.tab.c"
    break;

  case 188: /* logical_or_expression: logical_and_expression  */
#line 931 "parser.y"
                                               { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3074 "y.tab.c"
    break;

  case 189: /* logical_or_expression: logical_or_expression OROR logical_and_expression  */
#line 932 "parser.y"
                                                                          { (yyval.ast_node) = new_genop(LOGOP_N, OROR, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3080 "y.tab.c"
    break;

  case 190: /* logical_and_expression: bitwise_or_expression  */
#line 935 "parser.y"
                                                { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3086 "y.tab.c"
    break;

  case 191: /* logical_and_expression: logical_and_expression ANDAND bitwise_or_expression  */
#line 936 "parser.y"
                                                                             { (yyval.ast_node) = new_genop(LOGOP_N, ANDAND, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3092 "y.tab.c"
    break;

  case 192: /* conditional_expression: logical_or_expression  */
#line 940 "parser.y"
                                                { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3098 "y.tab.c"
    break;

  case 193: /* conditional_expression: logical_or_expression '?' expression ':' conditional_expression  */
#line 941 "parser.y"
                                                                                         { (yyval.ast_node) = new_ternop((yyvsp[-4].ast_node), (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3104 "y.tab.c"
    break;

  case 194: /* assignment_expression: conditional_expression  */
#line 945 "parser.y"
                                               { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3110 "y.tab.c"
    break;

  case 195: /* assignment_expression: unary_expression '=' assignment_expression  */
#line 946 "parser.y"
                                                                            { (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3116 "y.tab.c"
    break;

  case 196: /* assignment_expression: unary_expression PLUSEQ assignment_expression  */
#line 947 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '+', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3125 "y.tab.c"
    break;

  case 197: /* assignment_expression: unary_expression MINUSEQ assignment_expression  */
#line 951 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '-', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=' , (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3134 "y.tab.c"
    break;

  case 198: /* assignment_expression: unary_expression MULTEQ assignment_expression  */
#line 955 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '*', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3143 "y.tab.c"
    break;

  case 199: /* assignment_expression: unary_expression DIVEQ assignment_expression  */
#line 959 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '/', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3152 "y.tab.c"
    break;

  case 200: /* assignment_expression: unary_expression MODEQ assignment_expression  */
#line 963 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '%', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3161 "y.tab.c"
    break;

  case 201: /* assignment_expression: unary_expression SLEQ assignment_expression  */
#line 967 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, SL, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3170 "y.tab.c"
    break;

  case 202: /* assignment_expression: unary_expression SREQ assignment_expression  */
#line 971 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, SR, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3179 "y.tab.c"
    break;

  case 203: /* assignment_expression: unary_expression ANDEQ assignment_expression  */
#line 975 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, ANDAND, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3188 "y.tab.c"
    break;

  case 204: /* assignment_expression: unary_expression XOREQ assignment_expression  */
#line 979 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, '^', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3197 "y.tab.c"
    break;

  case 205: /* assignment_expression: unary_expression OREQ assignment_expression  */
#line 983 "parser.y"
                                                                            { 
                                                                                ast_node_t* tmp = new_genop(BINOP_N, OROR, (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); 
                                                                                (yyval.ast_node) = new_genop(ASSIGNOP_N, '=', (yyvsp[-2].ast_node), tmp); 
                                                                            }
#line 3206 "y.tab.c"
    break;

  case 206: /* comma_expression: assignment_expression  */
#line 990 "parser.y"
                                         { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3212 "y.tab.c"
    break;

  case 207: /* comma_expression: comma_expression ',' assignment_expression  */
#line 991 "parser.y"
                                                              { (yyval.ast_node) = new_genop(BINOP_N, ',', (yyvsp[-2].ast_node), (yyvsp[0].ast_node)); }
#line 3218 "y.tab.c"
    break;

  case 208: /* expression: comma_expression  */
#line 994 "parser.y"
                              { (yyval.ast_node) = (yyvsp[0].ast_node); }
#line 3224 "y.tab.c"
    break;


#line 3228 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 997 "parser.y"


int main(void) {
    scope_stack = (stack_t*) malloc(sizeof(stack_t));
    stack_init(scope_stack);

    struct_union_stack = (stack_t*)malloc(sizeof(stack_t));
    stack_init(struct_union_stack);
    
    SYMTABLE* global = st_create(FILE_SCOPE, NULL);

    stack_push(scope_stack, global); 
    yyparse(); 

    free(scope_stack);
    free(struct_union_stack);
    return 0;
}


void yyerror(char *s) {
    fprintf(stderr, "syntax error: %s\n", s);
}

