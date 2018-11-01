#ifndef AST_H
#define AST_H
#include <stdio.h>
#include <stdbool.h>

typedef struct astnode node;
enum exptype {
  UNASSIGN = 0,
  BOOL,
  INT,
  CINT,
  FLOAT,
  SFLOAT,
  VOID
};

enum {
  // Major nodes, subnodes (possible names)
  T_PROG,// "prog"
  T_EXTERNS,// "externs"
  T_EXTERN,// "extern"
  T_FUNCS,// "funcs"
  T_FUNC,// "func"
  T_BLK,// "blk"
  T_STMTS,// "stmts"
  T_STMT,// "stmt_raw"
  T_OPEN_STMT,
  T_CLOSED_STMT,// "blk" (d), "return", "vardeclstmt", "expstmt", "while", "if", "print", "printslit"
  T_SIMPLE_STMT,
  //T_IFELSE,// "if_stmt", "stmt"
  T_EXPS,// "exps"
  T_EXP,// "exp", "binop", "uop", NUMBER, "var", "globid", "globid"
  T_BINOP,// "add", "mul", "sub", "div", "assign", "eq", "lt", "gt", "and", "or"
  T_UOP,// "not", "neg"
  T_ID,
  T_LIT,
  T_SLIT,
  T_VAR,
  T_GLOB,
  T_TYPE,// "type", "noalias ref", "ref", "type"
  T_VDECLS,// "vdecls"
  T_TDECLS,// "tdecls"
  T_VDECL// "vdecl"
};

struct astnode {
  int tag;
  char name[32];
  char *str;
  double val;
  bool ref_or_not;
  enum exptype exptype; //hidden filed
  struct astnode *left;
  struct astnode *right;
  struct astnode *type;
  struct astnode *globid;
  struct astnode *tdecls;
  struct astnode *vdecls;
  struct astnode *blk;
  struct astnode *expr;
  struct astnode *stmt1;
  struct astnode *stmt2;
};

node *add_node(int tag, char *name, char *str, double val, node *left, node *right, node *type, node *globid, node *tdecls, node *vdecls, node *blk, node *expr, node *stmt1, node *stmt2, bool ref_or_not);
node *add_astnode(int, char *, node *, node *);
node *add_func(node *, node *, node *, node *);
node *add_extern(node *, node *, node *);
node *add_ocstmt(int, char *, node *, node *, node *);
node *add_lit(double);
node *add_flit(double);
node *add_slit(char *);
node *add_var(char *);
node *add_globid(char *);
node *add_type(char *);
node *add_ref_type(char *, node *);
void free_node(node *);

void print_ast(node *, int, FILE *);

#endif
