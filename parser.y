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
  int num;
  char var;
	char *str;
  struct astnode *node;
}


/* Terminal Tokens and Type Declaration */
%token <num> NUMBER
%token <str> ID STRINGS TYP
%token ADD SUB MUL DIV
%token OPREN CPREN CBRACK OBRACK
%token SEMICOL COMMA DOLLAR NOT
%token ASSIGN EQ LT GT AND OR
%token RETURN IF WHILE ELSE PRINT
%token NOALIAS REF EXTERN DEF
%token EOL

/* Non Terminal Type Declaration */
%type <node> stmts stmt blk funcs func externs extern globs prog
%type <node> exp exps ifelse type
%type <node> binop vdecls tdecls vdecl
%type <node> var uop globid
%type <node> ident lit slit


/* Precedence */
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%left ASSIGN
%left AND OR NOT
%left EQ LT GT
%left ADD SUB
%left MUL DIV
%left OPREN CPREN


/* Grammar Section */
%%
input
    : stmt { *rootnode = $1;}
    ;

prog
		: externs funcs { $$ = add_node($1, T_FUNCS, $2, "funcs"); }
		| funcs { $$ = add_node($1, T_FUNCS, NULL, "funcs"); }
		;

externs
		: extern { $$ = add_node($1, T_EXTERNS, NULL, "extern"); }
		| extern externs { $$ = add_node($1, T_EXTERNS, $2, "externs"); }
		;

extern
		:	EXTERN type globs { $$ = add_node($2, T_GLOB, $3, "globs"); }
		;

globs
		: globid OPREN tdecls CPREN SEMICOL { $$ = add_node($1, T_TDECL, $3, "tdecls"); }
		;

funcs
		: func { $$ = add_node($1, T_FUNC, NULL, "func"); }
		| func funcs { $$ = add_node($1, T_FUNC, NULL, "funcs"); }
		;

func
		: 


blk
		: OBRACK stmts CBRACK { $$ = add_node($2, T_STMTS, NULL, "stmts"); }
		| OBRACK CBRACK { $$ = add_node(NULL, T_BLK, NULL, "blk"); }
		;

stmts
		: stmt { $$ = add_node($1, T_STMT, NULL, "stmt"); }
		| stmt stmts { $$ = add_node($1, T_STMTS, $2, "stmts"); }
		;

stmt
		: blk { $$ = add_node($1, T_BLK, NULL, "blk"); }
		| RETURN exp SEMICOL { $$ = add_node($2, T_RETURN, NULL, "return"); }
		| RETURN SEMICOL { $$ = add_node(NULL, T_RETURN, NULL, "return"); }
		| exp SEMICOL { $$ = add_node($1, T_STMT, NULL, "exp"); }
		| WHILE OPREN exp CPREN stmt { $$ = add_node($3, T_WHILE, $5, "while"); }
		| IF OPREN exp CPREN ifelse { $$ = add_node($3, T_IF, $5, "if"); }
		| PRINT exp SEMICOL { $$ = add_node($2, T_PRINT, NULL, "exp"); }
		| PRINT slit SEMICOL { $$ = add_node($2, T_PRINT, NULL, "exp"); }
		;

ifelse
		: stmt ELSE stmt { $$ = add_node($1, T_STMT, $3, "stmt"); }
		| stmt %prec LOWER_THAN_ELSE { $$ = add_node($1, T_STMT, NULL, "stmt"); }
		;

exps
		: exp { $$ = add_node($1, T_EXP, NULL, "exp"); }
		| exp COMMA exps { $$ = add_node($1, T_EXP, $3, "exp"); }
		;

exp
		: OPREN exp CPREN { $$ = $2; }
    | binop { $$ = add_node($1, T_BINOP, NULL, "binop"); }
		| uop { $$ = add_node($1, T_UNARY, NULL, "uop"); }
    | lit { $$ = add_node($1, T_LIT, NULL, "lit"); }
		| var { $$ = add_node($1, T_VAR, NULL, "var"); }
		| globid OPREN exps CPREN { $$ = add_node($1, T_GLOB, $3, "globid"); }
		| globid OPREN CPREN { $$ = add_node($1, T_GLOB, NULL, "globid"); }
    ;

binop
		:	exp ADD exp { $$ = add_node($1, T_BOP, $3, "add"); }
		| exp MUL exp { $$ = add_node($1, T_BOP, $3, "mul"); }
		| exp SUB exp { $$ = add_node($1, T_BOP, $3, "sub"); }
		| exp DIV exp { $$ = add_node($1, T_BOP, $3, "div"); }
		| var ASSIGN exp { $$ = add_node($1, T_ASSIGN, $3, "assign"); }
		|	exp EQ exp { $$ = add_node($1, T_BOP, $3, "eq"); }
		| exp LT exp { $$ = add_node($1, T_BOP, $3, "lt"); }
		| exp GT exp { $$ = add_node($1, T_BOP, $3, "gt"); }
		| exp AND exp { $$ = add_node($1, T_BOP, $3, "and"); }
		| exp OR exp { $$ = add_node($1, T_BOP, $3, "or"); }
		;

uop
		: NOT exp { $$ = add_node($2, T_UOP, NULL, "not"); }
		| SUB exp { $$ = add_node($2, T_UOP, NULL, "neg"); }
		;

lit
		: NUMBER { $$ = add_lit($1); }
		;

slit
		: STRINGS { $$ = add_var($1); }
		;

ident
		: ID { $$ = add_var($1); }
		;

var
		: DOLLAR ident { $$ = add_node($2, T_VAR, NULL, "var"); }
		;

globid
		: ident { $$ = add_node($1, T_GLOB, NULL, "globid"); }
		;

type
		: NOALIAS type { $$ = add_node($2, T_NOALIAS, NULL, "noalias"); }
		| REF type { $$ = add_node($2, T_REF, NULL, "ref"); }
		| TYP { $$ = add_var($1); }
		;

vdecls
		: vdecl { $$ = add_node($1, T_VDECL, NULL, "vdecl"); }
		| vdecl COMMA vdecls { $$ = add_node($1, T_VDECL, NULL, "vdecl"); }
		;

tdecls
		: type { $$ = add_node($1, T_TYPE, NULL, "tdecl"); }
		| type COMMA tdecls { $$ = add_node($1, T_TDECL, $3, "tdecl"); }
		;

vdecl
		: type var { $$ = add_node($1, T_VDECL, $2, "vdecl"); }
		;


%%

/* Error handling - this is the default function reccomended by Bison docs */
void yyerror (struct astnode **rootnode, char const *s){
	fprintf (stderr, "%s\n", s);
}
