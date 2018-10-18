#ifndef AST_H
#define AST_H

typedef struct astnode node;

enum {
  T_ADD, T_MUL, T_SUB, T_DIV,
  T_ASSIGN, T_EQ, T_LT, T_GT, T_AND, T_OR, T_BOP, T_UOP,
  T_VAL, T_VAR, T_ID, T_GLOB,
  T_LIT,
  T_STMT, T_EXP,

  T_PRINT,
  T_BINOP, T_UNARY,
  T_BLK, T_STMTS,
  T_RETURN, T_WHILE, T_IF, T_IFELSE,
  T_NOALIAS, T_REF, T_VDECL, T_TDECL, T_TYPE,
  T_PROG, T_FUNCS, T_EXTERNS, T_FUNC
};

struct astnode {
  char name[10];
  char *str;
  int type;
  int value;
  struct astnode *left;
  struct astnode *right;
};

node *add_node(node *left, int type, node *right, char *name);
node *add_lit(int);
node *add_var(char *);

void print_ast(node *, int);

#endif
