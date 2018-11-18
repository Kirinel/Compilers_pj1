#ifndef AST_H
#define AST_H

#include "def.h"
#include <stdio.h>

/* Add a prog node
 *  externs:
 *  funcs:
 */
Node *add_prog(Node *externs, Node *funcs);

/* Add a externs node (Recursive)
 *  left:
 *  right:
 */
Node *add_externs(Node *left, Node *right);

/* Add a extern node
 *  type: return type
 *  globid: function name
 *  tdecls: type declarations
 */
Node *add_extern(Node *type, Node *globid, Node *tdecls);

/* Add a funcs node (Recursive)
 *  left:
 *  right:
 */
Node *add_funcs(Node *left, Node *right);

/* Add a func node
 *  type: return type of the function
 *  globid: function name
 *  vdecls: varaible declarations
 *  blk:  function block
 */
Node *add_func(Node *type, Node *globid, Node *vdecls, Node *blk);

/* Add a blk node
 *  stmts: statements
 */
Node *add_blk(Node *stmts);

/* Add a stmts node (recursive type)
 *  left:
 *  right:
 */
Node *add_stmts(Node *left, Node *right);

/* Add a stmt node
 *  either a open statement, or a closed statement
 */
Node *add_stmt(Node *oc_stmt);

/* Add a open_stmt node
 *  ocstmt_tag: IF, WHILE
 *  exp: condition
 *  stmt1: first statement
 *  stmt2: second statement
 */
Node *add_open_stmt(enum ocstmt_tag ocstmt_tag, Node *exp, Node *stmt1, Node *stmt2);

/* Add a closed_stmt node
 *  ocstmt_tag: IF, WHILE
 *  exp: condition
 *  stmt1: first statement
 *  stmt2: second statement
 */
Node *add_closed_stmt(enum ocstmt_tag ocstmt_tag, Node *exp, Node *stmt1, Node *stmt2);

/* Add a simple_stmt node
 *  simple_stmt_tag:
 *  blk: this statement is a block (scoping!)
 *  exp: expression type (multiple)
 *  vdecl: variable declaration
 *  slit: printslit
 */
Node *add_simple_stmt(enum simple_stmt_tag simple_stmt_tag, Node *blk, Node *exp, Node *vdecl, Node *slit);

/* Add a exps type (Recursive)
 *  left:
 *  right:
 */
Node *add_exps(Node *left, Node *right);

/* Add a exp type
 *  exp_tag: the tag of the expression
 *  exp_type: the type of the expression (no ref type) - No value at initialization
 *  exp: expression
 *  binop: binary op
 *  uop: unary op
 *  lit: number
 *  var: variable
 *  globid: function call
 */
Node *add_exp(enum exp_tag exp_tag, Node *exp, Node *binop, Node *uop, Node *lit, Node *var, Node *globid, Node *exps);


/* Add a binop node
 *  op: operator
 *  exp_type: the type of the expression (no ref type)
 *  exp_left: the expression on the left hand side
 *  exp_right: the expression on the right hand side
 *  var: variable assignment
 */
Node *add_binop(enum binop_tag op, Node *exp_left, Node *var, Node *exp_right);

/* Add a uop node
 *  op: operator
 *  exp: expression
 */
Node *add_uop(enum uop_tag op, Node *exp);

/* Add a lit type (Check between float and int)
 *  lit_type: type of the number, LIT, FLIT
 *  exp_type: the type of the expression //not initialized
 *  value: the value of the number, as a double
 */
Node *add_lit(char *valstr);

/* Add a slit node
 *  str: type of the string
 */
Node *add_slit(char *str);

/* Add a var node
 *  varname: the name of the variable
 */
Node *add_var(char *varname);

/* Add a globid node
 *  globid: The function name
 */
Node *add_globid(char *funcname);

/* Add a type node
 *  type_tag: The type of the tag, use strncmp to assign
 *  type: The type being referenced
 */
Node *add_type(char *typename, Node *type);

/* Add a vdecls node (recursive)
 *  left:
 *  right:
 */
Node *add_vdecls(Node *left, Node *right);

/* Add a tdecls node (recursive)
 *  left:
 *  right:
 */
Node *add_tdecls(Node *left, Node *right);

/* Add a vdecl node
 *  type: the type to declare
 *  var: the variable declared
 */
Node *add_vdecl(Node *type, Node *var);

/* Free function (Recursive)
 */
void free_ast(Node *n);

/* print_ast: generate the AST and write it to STDIN
 */
void print_ast(Node *n, int dent, FILE *out);

/* print_exp_type: print out the type for each expression
 */
void print_exp_type(enum exp_type exptype, int dent, FILE *out);

#endif
