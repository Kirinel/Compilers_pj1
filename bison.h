/* A Bison parser, made by GNU Bison 3.0.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_BISON_H_INCLUDED
# define YY_YY_BISON_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMBER = 258,
    ID = 259,
    STRINGS = 260,
    TYPES = 261,
    ADD = 262,
    SUB = 263,
    MUL = 264,
    DIV = 265,
    OPREN = 266,
    CPREN = 267,
    CBRACK = 268,
    OBRACK = 269,
    SEMICOL = 270,
    COMMA = 271,
    DOLLAR = 272,
    NOT = 273,
    ASSIGN = 274,
    EQ = 275,
    LT = 276,
    GT = 277,
    AND = 278,
    OR = 279,
    RETURN = 280,
    IF = 281,
    WHILE = 282,
    ELSE = 283,
    PRINT = 284,
    NOALIAS = 285,
    REF = 286,
    EXTERN = 287,
    DEF = 288,
    EOL = 289,
    LOWER_THAN_ELSE = 290,
    NEG = 291
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 12 "parser.y" /* yacc.c:1916  */

  int num;
  char var;
  char *str;
  struct astnode *node;

#line 98 "bison.h" /* yacc.c:1916  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (struct astnode **rootnode);

#endif /* !YY_YY_BISON_H_INCLUDED  */
