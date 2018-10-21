DESCRIPTION:
  The program can now correctly parse a source code into an AST.
    Remeber to install flex and bison on your computer (by apt-get).

  print_ast() in ast.c provides a handy output for inspecting the tree.
    This function can be a reference when you translate the AST into a .yaml file.

EXECUTE the program:
  1. make
  2. make test1 for translating test1.ek
  3. make test2 for translating test2.ek

  0. make clean to remove output files

RULES of nodes (still updating):
  node_name     tag         name        str       val         left      right     other children            others

  input                                                                                                     root, allocated on the stack
  prog          T_PROG      "prog"      -         -           funcs     externs   -
  externs       T_EXTERNS   "externs"   -         -           externs   extern    -                         recursive
  extern        T_EXTERN    "extern"    -         -           -         -         type globid tdecls
  funcs         T_FUNCS     "funcs"     -         -           funcs     func      -                         recursive
  func          T_FUNC      "func"      -         -           -         -         type globid vdecls blk
  blk           T_BLK       "blk"       -         -           stmts     -         -
  stmts         T_STMTS     "stmts"     -         -           stmts     stmt      -
  stmt          T_STMT      ?           -         -           ?         ?         -                         multiple possibilities of types
  ifelse        T_STMT      "if_stmt"   -         -           stmt      stmt      -
                            "stmt"
  exps          T_EXPS      "exps"      -         -           exp       exps      -
  exp           T_EXP       "?"         -         ?           ?         ?         -                         multiple possibilities
  binop         T_BINOP     op          -         -           exp (var) exp       -
  uop           T_UOP       op          -         -           exp       -         -
  slit          T_SLIT      -           STRING    -           -         -         -                         only combined with print
  var           T_var       "varval"    "var_name"-           -         -         -
  globid        T_GLOB      "globid"    func_name -           -         -         -                         stores function name
  type          T_TYPE      "type"      type_name -           type?     -         -
  vdecls        T_VDECLS    "vdecls"    -         -           vdecl     vdecls    -
  tdecls        T_TDECLS    "tdecls"    -         -           type      tdecls    -
  vdecl         T_VDECL     "vdecl"     -         -           type      var       -
