#ifndef AST_H
#define AST_H

typedef struct astnode node;

enum {
  T_PROG,
  T_FUNC, T_EXTERN,
  T_LIT, T_SLIT, T_TDECLS, T_VDECL,
  T_ADD, T_MUL, T_SUB, T_DIV,
  T_ASSIGN, T_EQ, T_LT, T_GT, T_AND, T_OR, T_BOP, T_UOP,
  T_VAL, T_VAR, T_ID, T_GLOB,
  T_STMT, T_EXP,
  T_PRINT,
  T_BINOP, T_UNARY,
  T_BLK, T_STMTS,
  T_RETURN, T_WHILE, T_IF, T_IFELSE,
  T_NOALIAS, T_REF, T_VDECLS, T_TDECL, T_TYPE,
  T_FUNCS, T_EXTERNS, T_EXPS
};

struct astnode {
  int tag;
  char name[32];
  char *str;
  double val;
  struct astnode *left;
  struct astnode *right;
  struct astnode *type;
  struct astnode *globid;
  struct astnode *tdecls;
  struct astnode *vdecls;
  struct astnode *blk;
};

node *add_node(int tag, char *name, char *str, double val, node *left, node *right, node *type, node *globid, node *tdecls, node *vdecls, node *blk);
node *add_astnode(int, char *, node *, node *);
node *add_func(node *, node *, node *, node *);
node *add_extern(node *, node *, node *);
node *add_lit(double);
node *add_slit(char *);
node *add_var(char *);
node *add_globid(char *);
node *add_type(char *);
void free_node(node *);

void print_ast(node *, int);

#endif
