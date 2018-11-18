%output  "./temp/bison.c"
%defines "./temp/bison.h"
%parse-param {struct astnode **rootnode}

%code{
#include "../temp/flex.h"
#include "../src/ast.h"
void yyerror(struct astnode **rootnode, char const *s);
}

/* YYLVAL types*/
%union{
  double num;
  char var;
  char *str;
  struct astnode *node;
}


/* Terminal Tokens and Type Declaration */
%token <str> NUMBER
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


/* Precedence - from the requirement page */
%right  ASSIGN
%left   OR
%left   AND
%left   EQ
%left   LT GT
%left   ADD SUB
%left   MUL DIV
%right  NOT
%left   OPREN CPREN

/* Grammar Section */
%%
input
    : prog { *rootnode = $1;}
    ;

prog
		: externs funcs     { $$ = add_prog($1, $2); }
		| funcs             { $$ = add_prog(NULL, $1); }
		;

externs
		: externs extern    { $$ = add_externs($1, $2); }
		| extern            { $$ = add_externs($1, NULL); }
		;

extern
		:	EXTERN type globid OPREN tdecls CPREN SEMICOL   { $$ = add_extern($2, $3, $5); }
		| EXTERN type globid OPREN CPREN SEMICOL          { $$ = add_extern($2, $3, NULL); }
		;

funcs
		: funcs func      { $$ = add_funcs($1, $2); }
		| func            { $$ = add_funcs($1, NULL); }
		;

func
		: DEF type globid OPREN vdecls CPREN blk  { $$ = add_func($2, $3, $5, $7); }
		| DEF type globid OPREN CPREN blk         { $$ = add_func($2, $3, NULL, $6); }
		;

blk
		: OBRACK stmts CBRACK   { $$ = add_blk($2); }
		| OBRACK CBRACK         { $$ = add_blk(NULL); }
		;

stmts
		: stmts stmt  { $$ = add_stmts($1, $2); }
		| stmt        { $$ = add_stmts($1, NULL); }
		;

stmt
    : open_stmt     { $$ = add_stmt($1); }
    | closed_stmt   { $$ = add_stmt($1); }
    ;

open_stmt
    : IF OPREN exp CPREN stmt                           { $$ = add_open_stmt(STMT_IF, $3, $5, NULL); }
    | IF OPREN exp CPREN closed_stmt ELSE open_stmt     { $$ = add_open_stmt(STMT_IF, $3, $5, $7); }
    | WHILE OPREN exp CPREN open_stmt                   { $$ = add_open_stmt(STMT_WHILE, $3, $5, NULL); }
    ;

closed_stmt
    : simple_stmt                                       { $$ = add_closed_stmt(STMT_SIMPLE_STMT, NULL, $1, NULL); }
    | IF OPREN exp CPREN closed_stmt ELSE closed_stmt   { $$ = add_closed_stmt(STMT_IF, $3, $5, $7); }
    | WHILE OPREN exp CPREN closed_stmt                 { $$ = add_closed_stmt(STMT_WHILE, $3, $5, NULL); }
    ;

simple_stmt
		: blk                         { $$ = add_simple_stmt(STMT_BLK, $1, NULL, NULL, NULL); }
		| RETURN exp SEMICOL          { $$ = add_simple_stmt(STMT_RETURN, NULL, $2, NULL, NULL); }
		| RETURN SEMICOL              { $$ = add_simple_stmt(STMT_RETURN_VOID, NULL, NULL, NULL, NULL); }
		| vdecl ASSIGN exp SEMICOL    { $$ = add_simple_stmt(STMT_VARDECL, NULL, $3, $1, NULL); }
		| exp SEMICOL                 { $$ = add_simple_stmt(STMT_EXPSTMT, NULL, $1, NULL, NULL); }
		| PRINT exp SEMICOL           { $$ = add_simple_stmt(STMT_PRINT, NULL, $2, NULL, NULL); }
		| PRINT slit SEMICOL          { $$ = add_simple_stmt(STMT_PRINTSLIT, NULL, NULL, NULL, $2); }
		;

exps
		: exps COMMA exp  { $$ = add_exps($1, $3); }
		| exp             { $$ = add_exps($1, NULL); }
		;

exp
		: OPREN exp CPREN   { $$ = add_exp(EXP_EXP, $2, NULL, NULL, NULL, NULL, NULL, NULL); }
    | binop             { $$ = add_exp(EXP_BINOP, NULL, $1, NULL, NULL, NULL, NULL, NULL); }
		| uop               { $$ = add_exp(EXP_UOP, NULL, NULL, $1, NULL, NULL, NULL, NULL); }
    | lit               { $$ = add_exp(EXP_LIT, NULL, NULL, NULL, $1, NULL, NULL, NULL);  }
		| var               { $$ = add_exp(EXP_VAR, NULL, NULL, NULL, NULL, $1, NULL, NULL); }
		| globid OPREN exps CPREN { $$ = add_exp(EXP_GLOBID, NULL, NULL, NULL, NULL, NULL, $1, $3); }
		| globid OPREN CPREN      { $$ = add_exp(EXP_GLOBID, NULL, NULL, NULL, NULL, NULL, $1, NULL); }
    ;

binop
		:	exp ADD exp     { $$ = add_binop(BINOP_ADD, $1, NULL, $3); }
		| exp MUL exp     { $$ = add_binop(BINOP_MUL, $1, NULL, $3); }
		| exp SUB exp     { $$ = add_binop(BINOP_SUB, $1, NULL, $3); }
		| exp DIV exp     { $$ = add_binop(BINOP_DIV, $1, NULL, $3); }
		| var ASSIGN exp  { $$ = add_binop(BINOP_ASSIGN, NULL, $1, $3); }
		|	exp EQ exp      { $$ = add_binop(BINOP_EQ, $1, NULL, $3); }
		| exp LT exp      { $$ = add_binop(BINOP_LT, $1, NULL, $3); }
		| exp GT exp      { $$ = add_binop(BINOP_GT, $1, NULL, $3); }
		| exp AND exp     { $$ = add_binop(BINOP_AND, $1, NULL, $3); }
		| exp OR exp      { $$ = add_binop(BINOP_OR, $1, NULL, $3); }
		;

uop
		: NOT exp           { $$ = add_uop(UOP_NOT, $2); }
		| SUB exp %prec NOT { $$ = add_uop(UOP_MINUS, $2); }
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
		: NOALIAS REF type  { char noalias[] = "noalias ref";
                          $$ = add_type(noalias, $3); }
		| REF type          { char ref[] = "ref";
                          $$ = add_type(ref, $2); }
		| TYPES             { $$ = add_type($1, NULL); }
		;

vdecls
		: vdecls COMMA vdecl  { $$ = add_vdecls($1, $3); }
		| vdecl               { $$ = add_vdecls($1, NULL); }
		;

tdecls
		: tdecls COMMA type { $$ = add_tdecls($1, $3); }
		| type              { $$ = add_tdecls($1, NULL); }
		;

vdecl
		: type var { $$ = add_vdecl($1, $2); }
		;


%%

/* Error handling - this is the default function reccomended by Bison docs */
void yyerror (struct astnode **rootnode, char const *s){
	fprintf(stderr, "Error: %s\nPlease check the input\n", s);
}
