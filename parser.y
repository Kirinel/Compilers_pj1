%output  "bison.c"
%defines "bison.h"
%parse-param {struct astnode **rootnode}

%code{
#include "flex.h"
#include "ast.h"
void yyerror (struct astnode **rootnode, char const *s);
}

/* YYLVAL types*/
%union{
  double num;
  char var;
  char *str;
  struct astnode *node;
}


/* Terminal Tokens and Type Declaration */
%token <num> NUMBER
%token <str> ID STRINGS TYPES
%token ADD SUB MUL DIV
%token OPREN CPREN CBRACK OBRACK
%token SEMICOL COMMA DOLLAR NOT
%token ASSIGN EQ LT GT AND OR
%token RETURN IF WHILE ELSE PRINT
%token NOALIAS REF EXTERN DEF
%token EOL

/* Non Terminal Type Declaration */
%type <node> prog externs extern funcs
%type <node> func blk stmts stmt open_stmt closed_stmt simple_stmt
%type <node> exps exp binop uop
%type <node> var globid type
%type <node> vdecls tdecls vdecl
%type <node> lit slit


/* Precedence */
%left ASSIGN
%left AND OR
%left EQ LT GT
%left ADD SUB
%left MUL DIV
%left NEG
%left OPREN CPREN NOT


/* Grammar Section */
%%
input
    : prog { *rootnode = $1;}
    ;

prog
		: externs funcs     { $$ = add_astnode(T_PROG, "prog", $2, $1); }
		| funcs             { $$ = add_astnode(T_PROG, "prog", $1, NULL); }
		;

externs
		: externs extern    { $$ = add_astnode(T_EXTERNS, "externs", $1, $2); }
		| extern            { $$ = add_astnode(T_EXTERNS, "externs", $1, NULL); }
		;

extern
		:	EXTERN type globid OPREN tdecls CPREN SEMICOL   { $$ = add_extern($2, $3, $5); }
		| EXTERN type globid OPREN CPREN SEMICOL          { $$ = add_extern($2, $3, NULL); }
		;

funcs
		: funcs func      { $$ = add_astnode(T_FUNCS, "funcs", $1, $2); }
		| func            { $$ = add_astnode(T_FUNCS, "funcs", $1, NULL); }
		;

func
		: DEF type globid OPREN vdecls CPREN blk  { $$ = add_func($2, $3, $5, $7); }
		| DEF type globid OPREN CPREN blk         { $$ = add_func($2, $3, NULL, $6); }
		;

blk
		: OBRACK stmts CBRACK   { $$ = add_astnode(T_BLK, "blk", $2, NULL); }
		| OBRACK CBRACK         { $$ = add_astnode(T_BLK, "blk", NULL, NULL); }
		;

stmts
		: stmts stmt  { $$ = add_astnode(T_STMTS, "stmts", $1, $2); }
		| stmt        { $$ = add_astnode(T_STMTS, "stmts", $1, NULL); }
		;

stmt
    : open_stmt     { $$ = add_astnode(T_STMT, "stmt_raw", $1, NULL); }
    | closed_stmt   { $$ = add_astnode(T_STMT, "stmt_raw", $1, NULL); }
    ;

open_stmt
    : IF OPREN exp CPREN stmt                           { $$ = add_ocstmt(T_OPEN_STMT, "if", $3, $5, NULL); }
    | IF OPREN exp CPREN closed_stmt ELSE open_stmt     { $$ = add_ocstmt(T_OPEN_STMT, "if", $3, $5, $7); }
    | WHILE OPREN exp CPREN open_stmt                   { $$ = add_ocstmt(T_OPEN_STMT, "while", $3, $5, NULL); }
    ;

closed_stmt
    : simple_stmt                                       { $$ = add_ocstmt(T_CLOSED_STMT, "simple", NULL, $1, NULL); }
    | IF OPREN exp CPREN closed_stmt ELSE closed_stmt   { $$ = add_ocstmt(T_CLOSED_STMT, "if", $3, $5, $7); }
    | WHILE OPREN exp CPREN closed_stmt                 { $$ = add_ocstmt(T_CLOSED_STMT, "while", $3, $5, NULL); }
    ;

simple_stmt
		: blk                         { $$ = add_astnode(T_SIMPLE_STMT, "blk", $1, NULL); }
		| RETURN exp SEMICOL          { $$ = add_astnode(T_SIMPLE_STMT, "return", $2, NULL); }
		| RETURN SEMICOL              { $$ = add_astnode(T_SIMPLE_STMT, "return", NULL, NULL); }
		| vdecl ASSIGN exp SEMICOL    { $$ = add_astnode(T_SIMPLE_STMT, "vardeclstmt", $1, $3); }
		| exp SEMICOL                 { $$ = add_astnode(T_SIMPLE_STMT, "expstmt", $1, NULL); }
		| PRINT exp SEMICOL           { $$ = add_astnode(T_SIMPLE_STMT, "print", $2, NULL); }
		| PRINT slit SEMICOL          { $$ = add_astnode(T_SIMPLE_STMT, "printslit", $2, NULL); }
		;

exps
		: exps COMMA exp  { $$ = add_astnode(T_EXPS, "exps", $1, $3); }
		| exp             { $$ = add_astnode(T_EXPS, "exps", $1, NULL); }
		;

exp
		: OPREN exp CPREN   { $$ = add_astnode(T_EXP, "exp", $2, NULL); }
    | binop             { $$ = add_astnode(T_EXP, "binop", $1, NULL); }
		| uop               { $$ = add_astnode(T_EXP, "uop", $1, NULL); }
    | lit               { $$ = add_astnode(T_EXP, "lit", $1, NULL);  }
		| var               { $$ = add_astnode(T_EXP, "var", $1, NULL); }
		| globid OPREN exps CPREN { $$ = add_astnode(T_EXP, "globid", $1, $3); }
		| globid OPREN CPREN      { $$ = add_astnode(T_EXP, "globid", $1, NULL); }
    ;

binop
		:	exp ADD exp     { $$ = add_astnode(T_BINOP, "add", $1, $3); }
		| exp MUL exp     { $$ = add_astnode(T_BINOP, "mul", $1, $3); }
		| exp SUB exp     { $$ = add_astnode(T_BINOP, "sub", $1, $3); }
		| exp DIV exp     { $$ = add_astnode(T_BINOP, "div", $1, $3); }
		| var ASSIGN exp  { $$ = add_astnode(T_BINOP, "assign", $1, $3); }
		|	exp EQ exp      { $$ = add_astnode(T_BINOP, "eq", $1, $3); }
		| exp LT exp      { $$ = add_astnode(T_BINOP, "lt", $1, $3); }
		| exp GT exp      { $$ = add_astnode(T_BINOP, "gt", $1, $3); }
		| exp AND exp     { $$ = add_astnode(T_BINOP, "and", $1, $3); }
		| exp OR exp      { $$ = add_astnode(T_BINOP, "or", $1, $3); }
		;

uop
		: NOT exp { $$ = add_astnode(T_UOP, "not", $2, NULL); }
		| SUB exp { $$ = add_astnode(T_UOP, "minus", $2, NULL); }
		;

lit
    : NUMBER  { $$ = add_lit($1); }
    ;

slit
		: STRINGS { $$ = add_slit($1); }
		;

var
		: DOLLAR ID { $$ = add_var($2); }
		;

globid
		: ID { $$ = add_globid($1); }
		;

type
		: NOALIAS REF type  { $$ = add_astnode(T_TYPE, "noalias ref", $3, NULL); }
		| REF type          { $$ = add_astnode(T_TYPE, "ref", $2, NULL); }
		| TYPES             { $$ = add_type($1); }
		;

vdecls
		: vdecls COMMA vdecl  { $$ = add_astnode(T_VDECLS, "vdecls", $1, $3); }
		| vdecl               { $$ = add_astnode(T_VDECLS, "vdecls", $1, NULL); }
		;

tdecls
		: tdecls COMMA type { $$ = add_astnode(T_TDECLS, "tdecls", $1, $3); }
		| type              { $$ = add_astnode(T_TDECLS, "tdecls", $1, NULL); }
		;

vdecl
		: type var { $$ = add_astnode(T_VDECL, "vdecl", $1, $2); }
		;


%%

/* Error handling - this is the default function reccomended by Bison docs */
void yyerror (struct astnode **rootnode, char const *s){
	fprintf (stderr, "%s\n", s);
}
