#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_TYPE 32

/* Add a prog node
 *  externs:
 *  funcs:
 */
Node *add_prog(Node *externs, Node *funcs)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_PROG;
  n->node_prog.externs = externs;
  n->node_prog.funcs = funcs;

  return n;
}

/* Add a externs node (Recursive)
 *  left:
 *  right:
 */
Node *add_externs(Node *left, Node *right)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_EXTERNS;
  n->node_externs.left = left;
  n->node_externs.right = right;

  return n;
}

/* Add a extern node
 *  type: return type
 *  globid: function name
 *  tdecls: type declarations
 */
Node *add_extern(Node *type, Node *globid, Node *tdecls)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_EXTERN;
  n->node_extern.type = type;
  n->node_extern.globid = globid;
  n->node_extern.tdecls = tdecls;

  return n;
}

/* Add a funcs node (Recursive)
 *  left:
 *  right:
 */
Node *add_funcs(Node *left, Node *right)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_FUNCS;
  n->node_funcs.left = left;
  n->node_funcs.right = right;

  return n;
}

/* Add a func node
 *  type: return type of the function
 *  globid: function name
 *  vdecls: varaible declarations
 *  blk:  function block
 */
Node *add_func(Node *type, Node *globid, Node *vdecls, Node *blk)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_FUNC;
  n->node_func.type = type;
  n->node_func.globid = globid;
  n->node_func.vdecls = vdecls;
  n->node_func.blk = blk;

  return n;
}

/* Add a blk node
 *  stmts: statements
 */
Node *add_blk(Node *stmts)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_BLK;
  n->node_blk.stmts = stmts;

  return n;
}

/* Add a stmts node (recursive type)
 *  left:
 *  right:
 */
Node *add_stmts(Node *left, Node *right)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_STMTS;
  n->node_stmts.left = left;
  n->node_stmts.right = right;

  return n;
}

/* Add a stmt node
 *  either a open statement, or a closed statement
 */
Node *add_stmt(Node *oc_stmt)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_STMT;
  n->node_stmt.oc_stmt = oc_stmt;

  return n;
}

/* Add a open_stmt node
 *  ocstmt_tag: IF, WHILE
 *  exp: condition
 *  stmt1: first statement
 *  stmt2: second statement
 */
Node *add_open_stmt(enum ocstmt_tag ocstmt_tag, Node *exp, Node *stmt1, Node *stmt2)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_OPEN_STMT;
  n->node_open_stmt.ocstmt_tag = ocstmt_tag;
  n->node_open_stmt.exp = exp;
  n->node_open_stmt.stmt1 = stmt1;
  n->node_open_stmt.stmt2 = stmt2;

  return n;
}

/* Add a closed_stmt node
 *  ocstmt_tag: IF, WHILE
 *  exp: condition
 *  stmt1: first statement
 *  stmt2: second statement
 */
Node *add_closed_stmt(enum ocstmt_tag ocstmt_tag, Node *exp, Node *stmt1, Node *stmt2)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_CLOSED_STMT;
  n->node_closed_stmt.ocstmt_tag = ocstmt_tag;
  n->node_closed_stmt.exp = exp;
  n->node_closed_stmt.stmt1 = stmt1;
  n->node_closed_stmt.stmt2 = stmt2;

  return n;
}

/* Add a simple_stmt node
 *  simple_stmt_tag:
 *  blk: this statement is a block (scoping!)
 *  exp: expression type (multiple)
 *  vdecl: variable declaration
 *  slit: printslit
 */
Node *add_simple_stmt(enum simple_stmt_tag simple_stmt_tag, Node *blk, Node *exp, Node *vdecl, Node *slit)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_SIMPLE_STMT;
  n->node_simple_stmt.simple_stmt_tag = simple_stmt_tag;
  n->node_simple_stmt.blk = blk;
  n->node_simple_stmt.exp = exp;
  n->node_simple_stmt.vdecl = vdecl;
  n->node_simple_stmt.slit = slit;

  return n;
}

/* Add a exps type (Recursive)
 *  left:
 *  right:
 */
Node *add_exps(Node *left, Node *right)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_EXPS;
  n->node_exps.left = left;
  n->node_exps.right = right;

  return n;
}

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
Node *add_exp(enum exp_tag exp_tag, Node *exp, Node *binop, Node *uop, Node *lit, Node *var, Node *globid, Node *exps)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_EXP;
  n->node_exp.exp_tag = exp_tag;
  // n->node_exp.exp_type = exp_type;
  n->node_exp.exp = exp;
  n->node_exp.binop = binop;
  n->node_exp.uop = uop;
  n->node_exp.lit = lit;
  n->node_exp.var = var;
  n->node_exp.globid = globid;
  n->node_exp.exps = exps;

  return n;
}

/* Add a binop node
 *  op: operator
 *  exp_type: the type of the expression (no ref type)
 *  exp_left: the expression on the left hand side
 *  exp_right: the expression on the right hand side
 *  var: variable assignment
 */
Node *add_binop(enum binop_tag op, Node *exp_left, Node *var, Node *exp_right)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_BINOP;
  n->node_binop.op = op;
  n->node_binop.exp_left = exp_left;
  n->node_binop.var = var;
  n->node_binop.exp_right = exp_right;

  return n;
}

/* Add a uop node
 *  op: operator
 *  exp: expression
 */
Node *add_uop(enum uop_tag op, Node *exp)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_UOP;
  n->node_uop.op = op;
  n->node_uop.exp = exp;

  return n;
}

/* Add a lit type (Check between float and int)
 *  lit_type: type of the number, LIT, FLIT
 *  exp_type: the type of the expression //not initialized
 *  value: the value of the number, as a double
 */
Node *add_lit(char *valstr)
{
  int len = strlen(valstr);
  enum lit_tag flit = LIT;

  for (int i=0; i<len; ++i) {
    if (valstr[i] == '.') {
      flit = FLIT;
      break;
    }
  }

  Node *n = calloc(1, sizeof(Node));
  n->tag = T_LIT;
  n->node_lit.lit_type = flit;
  n->node_lit.value = atof(valstr);

  return n;
}

/* Add a slit node
 *  str: type of the string
 */
Node *add_slit(char *str)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_SLIT;
  n->node_slit.str = strdup(str);

  return n;
}

/* Add a var node
 *  varname: the name of the variable
 */
Node *add_var(char *varname)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_VAR;
  n->node_var.varname = strdup(varname);

  return n;
}

/* Add a globid node
 *  globid: The function name
 */
Node *add_globid(char *funcname)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_GLOBID;
  n->node_globid.funcname = strdup(funcname);

  return n;
}

/* Add a type node
 *  type_tag: The type of the tag, use strncmp to assign
 *  type: The type being referenced
 */
Node *add_type(char *typename, Node *type)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_TYPE;
  if (!strncmp(typename, "int", MAX_TYPE)) {
    n->node_type.type_tag = TYPE_INT;
  } else if (!strncmp(typename, "cint", MAX_TYPE)) {
    n->node_type.type_tag = TYPE_CINT;
  } else if (!strncmp(typename, "float", MAX_TYPE)) {
    n->node_type.type_tag = TYPE_FLOAT;
  } else if (!strncmp(typename, "sfloat", MAX_TYPE)) {
    n->node_type.type_tag = TYPE_SFLOAT;
  } else if (!strncmp(typename, "ref", MAX_TYPE)) {
    n->node_type.type_tag = TYPE_REF;
  } else if (!strncmp(typename, "noalias ref", MAX_TYPE)) {
    n->node_type.type_tag = TYPE_NOALIAS_REF;
  } else if (!strncmp(typename, "void", MAX_TYPE)) {
    n->node_type.type_tag = TYPE_VOID;
  } else {
    fprintf(stderr, "unknown type name [%s]", typename);
  }
  n->node_type.type = type;

  return n;
}

/* Add a vdecls node (recursive)
 *  left:
 *  right:
 */
Node *add_vdecls(Node *left, Node *right)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_VDECLS;
  n->node_vdecls.left = left;
  n->node_vdecls.right = right;

  return n;
}

/* Add a tdecls node (recursive)
 *  left:
 *  right:
 */
Node *add_tdecls(Node *left, Node *right)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_TDECLS;
  n->node_tdecls.left = left;
  n->node_tdecls.right = right;

  return n;
}

/* Add a vdecl node
 *  type: the type to declare
 *  var: the variable declared
 */
Node *add_vdecl(Node *type, Node *var)
{
  Node *n = calloc(1, sizeof(Node));
  n->tag = T_VDECL;
  n->node_vdecl.type = type;
  n->node_vdecl.var = var;

  return n;
}

/* Free function (Recursive)
 */
void free_ast(Node *n)
{
  if (n==NULL) return;

  switch(n->tag) {
    case T_PROG:
      free_ast(n->node_prog.externs);
      free_ast(n->node_prog.funcs);
      break;
    case T_EXTERNS:
      free_ast(n->node_externs.left);
      free_ast(n->node_externs.right);
      break;
    case T_EXTERN:
      free_ast(n->node_extern.type);
      free_ast(n->node_extern.globid);
      free_ast(n->node_extern.tdecls);
      break;
    case T_FUNCS:
      free_ast(n->node_funcs.left);
      free_ast(n->node_funcs.right);
      break;
    case T_FUNC:
      free_ast(n->node_func.type);
      free_ast(n->node_func.globid);
      free_ast(n->node_func.vdecls);
      free_ast(n->node_func.blk);
      break;
    case T_BLK:
      free_ast(n->node_blk.stmts);
      break;
    case T_STMTS:
      free_ast(n->node_stmts.left);
      free_ast(n->node_stmts.right);
      break;
    case T_STMT:
      free_ast(n->node_stmt.oc_stmt);
      break;
    case T_OPEN_STMT:
      free_ast(n->node_open_stmt.exp);
      free_ast(n->node_open_stmt.stmt1);
      free_ast(n->node_open_stmt.stmt2);
      break;
    case T_CLOSED_STMT:
      free_ast(n->node_closed_stmt.exp);
      free_ast(n->node_closed_stmt.stmt1);
      free_ast(n->node_closed_stmt.stmt2);
      break;
    case T_SIMPLE_STMT:
      free_ast(n->node_simple_stmt.blk);
      free_ast(n->node_simple_stmt.exp);
      free_ast(n->node_simple_stmt.vdecl);
      free_ast(n->node_simple_stmt.slit);
      break;
    case T_EXPS:
      free_ast(n->node_exps.left);
      free_ast(n->node_exps.right);
      break;
    case T_EXP:
      free_ast(n->node_exp.exp);
      free_ast(n->node_exp.binop);
      free_ast(n->node_exp.uop);
      free_ast(n->node_exp.lit);
      free_ast(n->node_exp.var);
      free_ast(n->node_exp.globid);
      free_ast(n->node_exp.exps);
      break;
    case T_BINOP:
      free_ast(n->node_binop.exp_left);
      free_ast(n->node_binop.exp_right);
      free_ast(n->node_binop.var);
      break;
    case T_UOP:
      free_ast(n->node_uop.exp);
      break;
    case T_LIT:
      break;
    case T_SLIT:
      free(n->node_slit.str);
      break;
    case T_VAR:
      free(n->node_var.varname);
      break;
    case T_GLOBID:
      free(n->node_globid.funcname);
      break;
    case T_TYPE:
      free_ast(n->node_type.type);
      break;
    case T_VDECLS:
      free_ast(n->node_vdecls.left);
      free_ast(n->node_vdecls.right);
      break;
    case T_TDECLS:
      free_ast(n->node_tdecls.left);
      free_ast(n->node_tdecls.right);
      break;
    case T_VDECL:
      free_ast(n->node_vdecl.type);
      free_ast(n->node_vdecl.var);
      break;
  }
  free(n);

  return;
}

/* put_indent: generate the indent based on the input */
void put_indent(int dent, FILE *out)
{
	for (int i=0; i<dent; ++i)
		fprintf(out, "  ");
	return;
}

/* print_type: print out the type declaration of the the given type node
    no indent. no newline.
 */
static void print_type(Node *n, FILE *out)
{
	assert(n->tag == T_TYPE);

  switch (n->node_type.type_tag) {
    case TYPE_INT:
      fprintf(out, "int");
      break;
    case TYPE_CINT:
      fprintf(out, "cint");
      break;
    case TYPE_FLOAT:
      fprintf(out, "float");
      break;
    case TYPE_SFLOAT:
      fprintf(out, "sfloat");
      break;
    case TYPE_VOID:
      fprintf(out, "void");
      break;
    case TYPE_NOALIAS_REF:
      fprintf(out, "noalias ref ");
      print_type(n->node_type.type, out);
      break;
    case TYPE_REF:
      fprintf(out, "ref ");
      print_type(n->node_type.type, out);
      break;
  }
  return;
}

/* print_op: print out the operator of the given type
 *  newline.
 */
static void print_bop(enum binop_tag tag, FILE *out)
{
  switch (tag) {
    case BINOP_ADD:
      fprintf(out, "op: add\n");
      break;
    case BINOP_MUL:
      fprintf(out, "op: mul\n");
      break;
    case BINOP_SUB:
      fprintf(out, "op: sub\n");
      break;
    case BINOP_DIV:
      fprintf(out, "op: div\n");
      break;
    case BINOP_EQ:
      fprintf(out, "op: eq\n");
      break;
    case BINOP_LT:
      fprintf(out, "op: lt\n");
      break;
    case BINOP_GT:
      fprintf(out, "op: gt\n");
      break;
    case BINOP_AND:
      fprintf(out, "op: and\n");
      break;
    case BINOP_OR:
      fprintf(out, "op: or\n");
      break;
    default:
      fprintf(stderr, "unknown tag at <binop>!\n");
  }
}

/* print_ast: generate the AST and write it to STDIN (main printer function)
 */
void print_ast(Node *n, int dent, FILE *out)
{
  if (n==NULL) return;
  
  switch (n->tag) {

    case T_PROG:
      put_indent(dent, out);
      fprintf(out, "name: prog\n");

      put_indent(dent, out);
      fprintf(out, "funcs:\n");

      // recurse to the children
      print_ast(n->node_prog.funcs, dent+1, out);
      print_ast(n->node_prog.externs, dent, out);
      break;

    case T_EXTERNS:
      if (n->node_externs.left->tag == T_EXTERN) {
        // case externs : extern
        put_indent(dent, out);
        fprintf(out, "externs:\n");

        put_indent(dent+1, out);
        fprintf(out, "name: externs\n");

        put_indent(dent+1, out);
        fprintf(out, "externs:\n");

        put_indent(dent+2, out);
        fprintf(out, "-\n");

        // recurse to extern
        print_ast(n->node_externs.left, dent+3, out);
      } else {
        // case externs : externs extern
        // recurse to <externs>
        print_ast(n->node_externs.left, dent, out);
        // recurse to <extern>
        put_indent(dent+2, out);
        fprintf(out, "-\n");

        print_ast(n->node_externs.right, dent+3, out);
      }
      break;

    case T_EXTERN:
      // name: extern
      put_indent(dent, out);
      fprintf(out, "name: extern\n");

      // ret_type: <type>
      put_indent(dent, out);
      fprintf(out, "ret_type: ");

      print_type(n->node_extern.type, out);
      fprintf(out, "\n");

      // <globid>
      print_ast(n->node_extern.globid, dent, out);
      // <tdecls>
      if (n->node_extern.tdecls!= NULL)
        print_ast(n->node_extern.tdecls, dent, out);

      break;

    case T_TDECLS:
      if (n->node_tdecls.left->tag == T_TYPE) {
        // case: tdecls : type

        // tdecls:
        put_indent(dent, out);
        fprintf(out, "tdecls:\n");

        // name: tdecls
        put_indent(dent+1, out);
        fprintf(out, "name: tdecls\n");

        // name: types
        put_indent(dent+1, out);
        fprintf(out, "types:\n");

        // - type
        put_indent(dent+2, out);
        fprintf(out, "- ");

        print_type(n->node_tdecls.left, out);
        fprintf(out, "\n");
      } else {
        // case: tdecls : tdecls type
        print_ast(n->node_tdecls.left, dent, out);

        // - type
        put_indent(dent+2, out);
        fprintf(out, "- ");

        print_type(n->node_tdecls.right, out);
        fprintf(out, "\n");
      }
      break;

    case T_GLOBID:
      // globid: <funcname>
      put_indent(dent, out);
      fprintf(out, "globid: %s\n", n->node_globid.funcname);
      break;

    case T_FUNCS:
      if (n->node_funcs.left->tag == T_FUNC) {
        // first function (case funcs : func)
        put_indent(dent, out);
        fprintf(out, "name: funcs\n");

        put_indent(dent, out);
        fprintf(out, "funcs:\n");

        put_indent(dent+1, out);
        fprintf(out, "-\n");

        // Resurce (only one child)
        print_ast(n->node_funcs.left, dent+2, out);
      } else {
        // case funcs : funcs func
        // left node: funcs
        print_ast(n->node_funcs.left, dent, out);
        // right node: func
        put_indent(dent+1, out);
        fprintf(out, "-\n");

        print_ast(n->node_funcs.right, dent+2, out);
      }
      break;

    case T_FUNC:
      // name: func
      put_indent(dent, out);
      fprintf(out, "name: func\n");

      // ret_type: <type>
      put_indent(dent, out);
      fprintf(out, "ret_type: ");

      print_type(n->node_func.type, out);
      fprintf(out, "\n");

      // <globid>
      print_ast(n->node_func.globid, dent, out);

      // blk:
      put_indent(dent, out);
      fprintf(out, "blk:\n");
      // <blk>
      print_ast(n->node_func.blk, dent+1, out);

      // <vdecls>
      if (n->node_func.vdecls!= NULL)
        print_ast(n->node_func.vdecls, dent, out);

      break;

    case T_VDECLS:
      if (n->node_vdecls.left->tag == T_VDECL) {
        // case - vdecls : vdecl
        // vdecls:
        put_indent(dent, out);
        fprintf(out, "vdecls:\n");
        // name: vdecls
        put_indent(dent+1, out);
        fprintf(out, "name: vdecls\n");
        // vars:
        put_indent(dent+1, out);
        fprintf(out, "vars:\n");
        // -
        put_indent(dent+2, out);
        fprintf(out, "-\n");

        // recurse
        print_ast(n->node_vdecls.left, dent+3, out);
      } else {
        // case - vdecls : vdecls vdecl
        // recurse vdecls
        print_ast(n->node_vdecls.left, dent, out);

        put_indent(dent+2, out);
        fprintf(out, "-\n");
        // recurse vdecl
        print_ast(n->node_vdecls.right, dent+3, out);
      }
      break;

    case T_VDECL:
      // node: vdecl
      put_indent(dent, out);
      fprintf(out, "node: vdecl\n");
      // type: <type>
      put_indent(dent, out);
      fprintf(out, "type: ");
      print_type(n->node_vdecl.type, out);
      fprintf(out, "\n");

      // var: <varname>
      put_indent(dent, out);
      fprintf(out, "var: %s\n", n->node_vdecl.var->node_var.varname);

      break;

    case T_BLK:
      // blk: [stmts]?
      // name: blk
      put_indent(dent, out);
      fprintf(out, "name: blk\n");
      // contents:
      put_indent(dent, out);
      fprintf(out, "contents:\n");

      // recurse stmts
      if (n->node_blk.stmts != NULL)
        print_ast(n->node_blk.stmts, dent+1, out);

      break;

    case T_STMTS:
      if (n->node_stmts.left->tag == T_STMT) {
        // case: stmts : stmt
        // name: stmts
        put_indent(dent, out);
        fprintf(out, "name: stmts\n");
        // stmts:
        put_indent(dent, out);
        fprintf(out, "stmts:\n");
        // -
        put_indent(dent+1, out);
        fprintf(out, "-\n");

        // recurse stmt
        print_ast(n->node_stmts.left, dent+2, out);
      } else {
        // case: stmts : stmts stmt
        // recurse stmts
        print_ast(n->node_stmts.left, dent, out);

        // -
        put_indent(dent+1, out);
        fprintf(out, "-\n");
        // case: stmts : stmt
        print_ast(n->node_stmts.right, dent+2, out);
      }

      break;

    case T_STMT:
      // case: stmt : oc_stmt
      print_ast(n->node_stmt.oc_stmt, dent, out);

      break;

    case T_OPEN_STMT:
      if (n->node_open_stmt.ocstmt_tag == STMT_IF) {
        // case "if"
        // name: if
        put_indent(dent, out);
        fprintf(out, "name: if\n");

        // cond:
        put_indent(dent, out);
        fprintf(out, "cond:\n");
        // recurse <exp>
        print_ast(n->node_open_stmt.exp, dent+1, out);

        // stmt:
        put_indent(dent, out);
        fprintf(out, "stmt:\n");
        // recurse <stmt1>
        print_ast(n->node_open_stmt.stmt1, dent+1, out);

        if (n->node_open_stmt.stmt2 != NULL) {
          // else_stmt:
          put_indent(dent, out);
          fprintf(out, "else_stmt:\n");
          // recurse <stmt2>
          print_ast(n->node_open_stmt.stmt2, dent+1, out);
        }
      } else if (n->node_open_stmt.ocstmt_tag == STMT_WHILE) {
        // case "while"
        // name: while
        put_indent(dent, out);
        fprintf(out, "name: while\n");

        // cond:
        put_indent(dent, out);
        fprintf(out, "cond:\n");
        // recurse <exp>
        print_ast(n->node_open_stmt.exp, dent+1, out);

        // stmt:
        put_indent(dent, out);
        fprintf(out, "stmt:\n");
        // recurse <stmt1>
        print_ast(n->node_open_stmt.stmt1, dent+1, out);
      }

      break;

    case T_CLOSED_STMT:
      if (n->node_closed_stmt.ocstmt_tag == STMT_IF) {
        // case "if"
        // name: if
        put_indent(dent, out);
        fprintf(out, "name: if\n");

        // cond:
        put_indent(dent, out);
        fprintf(out, "cond:\n");
        // recurse <exp>
        print_ast(n->node_closed_stmt.exp, dent+1, out);

        // stmt:
        put_indent(dent, out);
        fprintf(out, "stmt:\n");
        // recurse <stmt1>
        print_ast(n->node_closed_stmt.stmt1, dent+1, out);

        // else_stmt:
        put_indent(dent, out);
        fprintf(out, "else_stmt:\n");
        // recurse <stmt2>
        print_ast(n->node_closed_stmt.stmt2, dent+1, out);

      } else if (n->node_closed_stmt.ocstmt_tag == STMT_WHILE) {
        // case "while"
        // name: while
        put_indent(dent, out);
        fprintf(out, "name: while\n");

        // cond:
        put_indent(dent, out);
        fprintf(out, "cond:\n");
        // recurse <exp>
        print_ast(n->node_closed_stmt.exp, dent+1, out);

        // stmt:
        put_indent(dent, out);
        fprintf(out, "stmt:\n");
        // recurse <stmt1>
        print_ast(n->node_closed_stmt.stmt1, dent+1, out);
      } else {
        // case close_stmt : simple_stmt
        print_ast(n->node_closed_stmt.stmt1, dent, out);
      }

      break;

    case T_SIMPLE_STMT:
      // node: simple_stmt
      // printf("%d\n", n->node_simple_stmt.simple_stmt_tag);
      switch (n->node_simple_stmt.simple_stmt_tag) {
        case STMT_BLK:
          // printf("%p %p %p %p\n", n->node_simple_stmt.blk, n->node_simple_stmt.exp, n->node_simple_stmt.vdecl, n->node_simple_stmt.slit);
          // case: simple_stmt := blk
          // recurse <blk>
          print_ast(n->node_simple_stmt.blk, dent, out);
          break;

        case STMT_RETURN:
        case STMT_RETURN_VOID:
          // case: simple_stmt := RETURN exp SEMICOL
          // name: ret

          put_indent(dent, out);
          fprintf(out, "name: ret\n");
          // exp:
          put_indent(dent, out);
          fprintf(out, "exp:\n");

          // recurse <exp>
          if (n->node_simple_stmt.exp != NULL)
            print_ast(n->node_simple_stmt.exp, dent+1, out);
          break;

        case STMT_VARDECL:
          // case: simple_stmt := vdecl ASSIGN exp SEMICOL

          // name: vardeclstmt
          put_indent(dent, out);
          fprintf(out, "name: vardeclstmt\n");
          // vdecl:
          put_indent(dent, out);
          fprintf(out, "vdecl:\n");
          // recurse <vdecl>
          print_ast(n->node_simple_stmt.vdecl, dent+1, out);

          // exp:
          put_indent(dent, out);
          fprintf(out, "exp:\n");
          // recurse <exp>
          print_ast(n->node_simple_stmt.exp, dent+1, out);
          break;

        case STMT_EXPSTMT:
          // case: simple_stmt := exp SEMICOL

          // name: expstmt
          put_indent(dent, out);
          fprintf(out, "name: expstmt\n");
          // exp:
          put_indent(dent, out);
          fprintf(out, "exp:\n");
          // recurse <exp>
          print_ast(n->node_simple_stmt.exp, dent+1, out);
          break;

        case STMT_PRINT:
          // case: simple_stmt := PRINT exp SEMICOL

          // name: print
          put_indent(dent, out);
          fprintf(out, "name: print\n");
          // exp:
          put_indent(dent, out);
          fprintf(out, "exp:\n");
          // recurse <exp>
          print_ast(n->node_simple_stmt.exp, dent+1, out);
          break;

        case STMT_PRINTSLIT:
          // case: simple_stmt := PRINT slit SEMICOL

          // name: printslit
          put_indent(dent, out);
          fprintf(out, "name: printslit\n");
          // recurse <slit>
          print_ast(n->node_simple_stmt.slit, dent, out);
      }
      break;

    case T_SLIT:
      // node: slit

      // string: <slit>
      put_indent(dent, out);
      fprintf(out, "string: %s\n", n->node_slit.str);
      break;

    case T_EXP:
      // node: exp
      switch (n->node_exp.exp_tag) {
        case EXP_EXP:
          // exp := ( exp )
          print_ast(n->node_exp.exp, dent, out);
          break;

        case EXP_BINOP:
          // exp := binop
          print_ast(n->node_exp.binop, dent, out);
          break;

        case EXP_UOP:
          // exp := uop
          print_ast(n->node_exp.uop, dent, out);
          break;

        case EXP_LIT:
          // exp := lit
          print_ast(n->node_exp.lit, dent, out);
          break;

        case EXP_VAR:
          // exp := var
          print_ast(n->node_exp.var, dent, out);
          break;

        case EXP_GLOBID:
          // exp := globid ( <exps>? )
          // name: funccall
          put_indent(dent, out);
          fprintf(out, "name: funccall\n");

          // recurse <globid>
          print_ast(n->node_exp.globid, dent, out);

          // ADD: print out the expression type
          print_exp_type(n->node_exp.exp_type, dent, out);

          // params:
          put_indent(dent, out);
          fprintf(out, "params:\n");

          // recurse <exps>
          print_ast(n->node_exp.exps, dent+1, out);
      }
      break;

    case T_BINOP:
      // node: binop
      switch (n->node_binop.op) {
        case BINOP_ASSIGN:
          // binop := var ASSIGN exp
          // name: assign
          put_indent(dent, out);
          fprintf(out, "name: assign\n");

          // ADD: print out the expression type
          print_exp_type(n->node_binop.exp_type, dent, out);

          // var: <varname>
          put_indent(dent, out);
          fprintf(out, "var: %s\n", n->node_binop.var->node_var.varname);
          // exp:
          put_indent(dent, out);
          fprintf(out, "exp:\n");
          // recurse <exp_right>
          print_ast(n->node_binop.exp_right, dent+1, out);
          break;

        default:
          // binop := exp OP exp
          // name: binop
          put_indent(dent, out);
          fprintf(out, "name: binop\n");
          // op: <op>
          put_indent(dent, out);
          print_bop(n->node_binop.op, out);

          // ADD: print out the expression type
          print_exp_type(n->node_binop.exp_type, dent, out);

          // lhs:
          put_indent(dent, out);
          fprintf(out, "lhs:\n");
          // recurse <exp_left>
          print_ast(n->node_binop.exp_left, dent+1, out);
          // rhs:
          put_indent(dent, out);
          fprintf(out, "rhs:\n");
          // recurse <exp_right>
          print_ast(n->node_binop.exp_right, dent+1, out);
      }
      break;

    case T_UOP:
      // node: uop
      // name: binop
      put_indent(dent, out);
      fprintf(out, "name: uop\n");
      // op: <op>
      put_indent(dent, out);
      if (n->node_uop.op == UOP_NOT) {
        fprintf(out, "op: not\n");
      } else {
        fprintf(out, "op: minus\n");
      }

      // ADD: print out the expression type
      print_exp_type(n->node_binop.exp_type, dent, out);

      // exp:
      put_indent(dent, out);
      fprintf(out, "exp:\n");
      // recurse <exp>
      print_ast(n->node_uop.exp, dent+1, out);
      break;

    case T_TYPE:
      fprintf(stderr, "invalid access at <type>\n");
      break;

    case T_VAR:
      // node: var

      // name: varval
      put_indent(dent, out);
      fprintf(out, "name: varval\n");
      // var: <varname>
      put_indent(dent, out);
      fprintf(out, "var: %s\n", n->node_var.varname);

      // ADD: print out the expression type
      print_exp_type(n->node_var.exp_type, dent, out);

      break;

    case T_EXPS:
      // node: exps
      if (n->node_exps.left->tag == T_EXP) {
        // case: exps := exp
        // name: exps
        put_indent(dent, out);
        fprintf(out, "name: exps\n");
        // exps:
        put_indent(dent, out);
        fprintf(out, "exps:\n");
        // -
        put_indent(dent+1, out);
        fprintf(out, "-\n");
        // recurse <exp>
        print_ast(n->node_exps.left, dent+2, out);
      } else {
        // case: exps := exps exp
        // recurse <exps>
        print_ast(n->node_exps.left, dent, out);

        // -
        put_indent(dent+1, out);
        fprintf(out, "-\n");

        // recurse <exp>
        print_ast(n->node_exps.right, dent+2, out);
      }
      break;

    case T_LIT:
      // node: lit
      // name: <lit_tag>
      // value: <num>
      if (n->node_lit.lit_type == LIT) {
        put_indent(dent, out);
        fprintf(out, "name: lit\n");
        put_indent(dent, out);
        fprintf(out, "value: %d\n", (int) n->node_lit.value);
      } else {
        put_indent(dent, out);
        fprintf(out, "name: flit\n");
        put_indent(dent, out);
        fprintf(out, "value: %.6g\n", n->node_lit.value);
      }

      // ADD: print out the expression type
      print_exp_type(n->node_lit.exp_type, dent, out);

      break;

    default:
      fprintf(stderr, "unconstructed routine.\n");
  }
  return;
}


/* print_exp_type: print out the type for each expression
 */
void print_exp_type(enum exp_type exptype, int dent, FILE *out)
{
	put_indent(dent, out);
	//printf("%d\n", n->exptype);
	if (exptype == INT) {
		fprintf(out, "expr_type: int\n");
	} else if (exptype == CINT) {
		fprintf(out, "expr_type: cint\n");
	} else if (exptype == FLOAT) {
		fprintf(out, "expr_type: float\n");
	} else if (exptype == SFLOAT) {
		fprintf(out, "expr_type: sfloat\n");
	} else if (exptype == VOID){
		fprintf(out, "expr_type: void\n");
	} else {
		fprintf(out, "expr_type: undefined (%d)\n", exptype);
	}

	return;
}
