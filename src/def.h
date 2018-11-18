#ifndef DEF_H
#define DEF_H

/* An enum type:
 *  Document the primary type of the AST node,
 */
typedef enum node_tag {
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
  //T_ID,
  T_LIT,
  T_SLIT,
  T_VAR,
  T_GLOBID,
  T_TYPE,// "type", "noalias ref", "ref", "type"
  T_VDECLS,// "vdecls"
  T_TDECLS,// "tdecls"
  T_VDECL// "vdecl"
} Tag;

/* An enum type:
 *  Document the type of each expression,
 *  not including the ref type.
 */
enum exp_type {
  UNDEF = 0,
  INT,
  CINT,
  FLOAT,
  SFLOAT,
  VOID,
  STRING
};

enum llvm_type {
  LLVM_INT,
  LLVM_CINT,
  LLVM_FLOAT,
  LLVM_SFLOAT,
  LLVM_VOID,
  LLVM_STRING,
  LLVM_REF_INT,
  LLVM_REF_CINT,
  LLVM_REF_FLOAT,
  LLVM_REF_SFLOAT,
  LLVM_NOALIAS_REF_INT,
  LLVM_NOALIAS_REF_CINT,
  LLVM_NOALIAS_REF_FLOAT,
  LLVM_NOALIAS_REF_SFLOAT
};

/* An enum type:
 *  Document the secondary type of a STMT node.
 */
enum simple_stmt_tag {
  STMT_BLK,
  STMT_RETURN,
  STMT_RETURN_VOID,
  STMT_VARDECL,
  STMT_EXPSTMT,
  STMT_PRINT,
  STMT_PRINTSLIT
};

/* An enum type:
 *  Document the secondary type of a EXP node.
 */
enum exp_tag {
  EXP_EXP,
  EXP_BINOP,
  EXP_UOP,
  EXP_LIT,
  EXP_VAR,
  EXP_GLOBID
};

/* An enum type:
 *  Document the secondary type of a BINOP node.
 */
enum binop_tag {
  BINOP_ADD,
  BINOP_MUL,
  BINOP_SUB,
  BINOP_DIV,
  BINOP_ASSIGN,
  BINOP_EQ,
  BINOP_LT,
  BINOP_GT,
  BINOP_AND,
  BINOP_OR
};

/* An enum type:
 *  Document the secondary type of a UOP node
 */
enum uop_tag {
  UOP_NOT,
  UOP_MINUS,
};

/* An enum type:
 *  Document the secondary type of a TYPE node
 */
enum type_tag {
  TYPE_NOALIAS_REF,
  TYPE_REF,
  TYPE_INT,
  TYPE_CINT,
  TYPE_FLOAT,
  TYPE_SFLOAT,
  TYPE_VOID
};

/* Open/closed stmt:
 *  Document between if and while
 */
enum ocstmt_tag {
  STMT_IF,
  STMT_WHILE,
  STMT_SIMPLE_STMT
};

/* lit:
 *  Document between lit and flit
 */
enum lit_tag {
  LIT,
  FLIT
};

/* Struct: Tree node
 *  The expression tree
 */
typedef struct astnode {
  Tag tag;
  union {
    struct {
      struct astnode *externs;
      struct astnode *funcs;
    } node_prog;
    struct {
      struct astnode *left;
      struct astnode *right;
    } node_externs;
    struct {
      struct astnode *type;
      struct astnode *globid;
      struct astnode *tdecls;
    } node_extern;
    struct {
      struct astnode *left;
      struct astnode *right;
    } node_funcs;
    struct {
      struct astnode *type;
      struct astnode *globid;
      struct astnode *vdecls;
      struct astnode *blk;
    } node_func;
    struct {
      struct astnode *stmts;
    } node_blk;
    struct {
      struct astnode *left;
      struct astnode *right;
    } node_stmts;
    struct {
      struct astnode *oc_stmt;
    } node_stmt;
    struct {
      enum ocstmt_tag ocstmt_tag;
      struct astnode *exp;
      struct astnode *stmt1;
      struct astnode *stmt2;
    } node_open_stmt;
    struct {
      enum ocstmt_tag ocstmt_tag;
      struct astnode *exp;
      struct astnode *stmt1;
      struct astnode *stmt2;
    } node_closed_stmt;
    struct {
      enum simple_stmt_tag simple_stmt_tag;
      struct astnode *blk;
      struct astnode *exp;
      struct astnode *vdecl;
      struct astnode *slit;
    } node_simple_stmt;
    struct {
      struct astnode *left;
      struct astnode *right;
    } node_exps;
    struct {
      enum exp_tag exp_tag;
      enum exp_type exp_type;
      struct astnode *exp;
      struct astnode *binop;
      struct astnode *uop;
      struct astnode *lit;
      struct astnode *var;
      struct astnode *globid;
      struct astnode *exps;
    } node_exp;
    struct {
      enum binop_tag op;
      enum exp_type exp_type;
      struct astnode *exp_left;
      struct astnode *var;
      struct astnode *exp_right;
    } node_binop;
    struct {
      enum uop_tag op;
      enum exp_type exp_type;
      struct astnode *exp;
    } node_uop;
    struct {
      enum lit_tag lit_type;
      enum exp_type exp_type;
      double value;
    } node_lit;
    struct {
      enum exp_type exp_type;
      char *str;
    } node_slit;
    struct {
      enum exp_type exp_type;
      char *varname;
    } node_var;
    struct {
      char *funcname;
    } node_globid;
    struct {
      enum type_tag type_tag;
      struct astnode *type;
    } node_type;
    struct {
      struct astnode *left;
      struct astnode *right;
    } node_vdecls;
    struct {
      struct astnode *left;
      struct astnode *right;
    } node_tdecls;
    struct {
      struct astnode *type;
      struct astnode *var;
    } node_vdecl;
  };
} Node;

/* type_to_exptype: Given a <type> node, convert it into a enum exp_tag
 */
enum exp_type type_to_exptype(Node *n_type);

/* type_to_llvmtype: Given a <type> node, convert it into a enum exp_tag
 */
enum llvm_type type_to_llvmtype(Node *n_type);

#endif
