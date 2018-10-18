/* Flex directives */
%option outfile="flex.c" header-file="flex.h"
%option noyywrap nounput noinput

/* This code is copied verbatim to the .c file generated - includes token defs from bison */
%{
#include "bison.h"
%}

LIT       [0-9]+(\.[0-9]+)?
SLIT      ["][^"]*["]
ID       [a-zA-Z_]+[a-zA-Z0-9_]*
TYPE      "int" | "cint" | "float" | "sfloat" | "void"
NOALIAS   "noalias ref"
REF       "ref"

/* Grammar */
%%
"+"      {return ADD;}
"*"     {return MUL;}
"-"      {return SUB;}
"="      {return ASSIGN;}
"=="     {return EQ;}
"<"      {return LT;}
">"       {return GT;}
"&&"     {return AND;}
"||"     {return OR;}
"("      {return OPREN;}
")"      {return CPREN;}
"{"      {return OBRACK;}
"}"      {return CBRACK;}
";"       {return SEMICOL;}
","      {return COMMA;}
"$"         {return DOLLAR;}
"!"        {return NOT;}
"return"    {return RETURN;}
"if"       {return IF;}
"while"    {return WHILE;}
"else"     {return ELSE;}
"print"    {return PRINT;}
"extern"   {return EXTERN;}
"def"       {return DEF; }
{NOALIAS} {yylval.str = strdup(yytext); return NOALIAS;}
{REF}     {yylval.str = strdup(yytext); return REF;}
{TYPE}    {yylval.str = atoi(yytext); return TYP;}
{LIT}     {yylval.num = atoi(yytext); return NUMBER;}
{SLIT}    {yylval.str = strdup(yytext); return STRINGS;}
{ID}      {yylval.str = strdup(yytext); return ID;}
[ \t]     { /* ignore whitespaces */ }
\n        {return 0;}
.         {printf("Mistery character %s\n", yytext);}
%%
