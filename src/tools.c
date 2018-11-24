#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "tools.h"

/* emit_ast: write out the ast to file
 */
void emit_ast(char *in, char *out, Node *rootnode)
{
	FILE *outfile = fopen(out, "w");

	/* print out the AST */
	fprintf(outfile, "---\n");
	print_ast(rootnode, 0, outfile);
	fprintf(outfile, "...\n");

	//fclose(yyin);
	fclose(outfile);

	printf("Success: The AST has been written to the given filename.\n");

	return;
}

/* fill_exp_type: Fill all the expressions in the AST with types (no ref type)
 *  This is a recursive function.
 *		args:
 *			n: One node of the AST
 *			ft: The pointer to the function table
 *			vt: The pointer to the variable table
 *		returns:
 *			the (value) type of the node (as enum exp_type)
 */
enum exp_type fill_exp_type(Node *n, FUNC_table *ft, VAR_table **vt)
{
	if (n==NULL) return UNDEF;

	VAR_entry *vent = NULL;
	FUNC_entry *fent = NULL;
	enum exp_type type_l, type_r;
	int err;

  switch (n->tag) {
    case T_PROG:
      fill_exp_type(n->node_prog.funcs, ft, vt);
      return 0;

    case T_FUNCS:
      fill_exp_type(n->node_funcs.left, ft, vt);
      if (n->node_funcs.right != NULL)
        fill_exp_type(n->node_funcs.right, ft, vt);
      return 0;

    case T_FUNC:
			assert( find_func(ft, n->node_func.globid->node_globid.funcname, &fent) == 0);
			*vt = fent->local_vars;
      fill_exp_type(n->node_func.blk, ft, vt);
      return 0;

    case T_BLK:
      if (n->node_blk.stmts != NULL) {
        fill_exp_type(n->node_blk.stmts, ft, vt);
      }
      return 0;

    case T_STMTS:
      fill_exp_type(n->node_stmts.left, ft, vt);
      if (n->node_stmts.right != NULL)
        fill_exp_type(n->node_stmts.right, ft, vt);
      return 0;

    case T_STMT:
      fill_exp_type(n->node_stmt.oc_stmt, ft, vt);
      return 0;

    case T_OPEN_STMT:
    case T_CLOSED_STMT:
      fill_exp_type(n->node_open_stmt.exp, ft, vt);
      fill_exp_type(n->node_open_stmt.stmt1, ft, vt);
      if (n->node_open_stmt.stmt2 != NULL)
        fill_exp_type(n->node_open_stmt.stmt2, ft, vt);
      return 0;

    case T_SIMPLE_STMT:
      switch (n->node_simple_stmt.simple_stmt_tag) {
        case STMT_BLK:
          fill_exp_type(n->node_simple_stmt.blk, ft, vt);
          break;

        case STMT_RETURN_VOID:
          break;

        case STMT_VARDECL:
          // type_to_exptype(n->node_simple_stmt.vdecl->node_vdecl.type);
					fill_exp_type(n->node_simple_stmt.exp, ft, vt);
          break;

        case STMT_RETURN:
        case STMT_EXPSTMT:
        case STMT_PRINT:
          fill_exp_type(n->node_simple_stmt.exp, ft, vt);
          break;
        case STMT_PRINTSLIT:
          fill_exp_type(n->node_simple_stmt.slit, ft, vt);
          break;
      }
      return 0;

    case T_SLIT:
      n->node_slit.exp_type = STRING;
      return STRING;

    case T_EXP:
      switch (n->node_exp.exp_tag) {
        case EXP_EXP:
          n->node_exp.exp_type = fill_exp_type(n->node_exp.exp, ft, vt);
          break;

        case EXP_BINOP:
          n->node_exp.exp_type = fill_exp_type(n->node_exp.binop, ft, vt);
          break;

        case EXP_UOP:
          n->node_exp.exp_type = fill_exp_type(n->node_exp.uop, ft, vt);
          break;

        case EXP_LIT:
          n->node_exp.exp_type = fill_exp_type(n->node_exp.lit, ft, vt);
          break;

        case EXP_VAR:
          n->node_exp.exp_type = fill_exp_type(n->node_exp.var, ft, vt);
          break;

        case EXP_GLOBID:
					err = find_func(ft, n->node_exp.globid->node_globid.funcname, &fent);
					if (err == NOT_FOUND) {
						fprintf(stderr, "error: Function (%s) must be declared before use (code: 003).\n", n->node_exp.globid->node_globid.funcname);
						exit(FUNC_NO_DECL);
					} else if (err) {
						fprintf(stderr, "error: Unknown error (code: 099).\n");
						exit(99);
					}
					// assign value
					n->node_exp.exp_type = fent->ret_type;

					// recurse <exps>
					fill_exp_type(n->node_exp.exps, ft, vt);

					break;
      }
      // assert(n->node_exp.exp_type != 0);
      return n->node_exp.exp_type;

    case T_BINOP:
			switch (n->node_binop.op) {
				case BINOP_ASSIGN:
					fill_exp_type(n->node_binop.var, ft, vt);
					n->node_binop.exp_type = fill_exp_type(n->node_binop.exp_right, ft, vt);
					break;

				case BINOP_ADD:
				case BINOP_MUL:
				case BINOP_SUB:
				case BINOP_DIV:

					// generate the left and right hand type
					type_r = fill_exp_type(n->node_binop.exp_right, ft, vt);
					type_l = fill_exp_type(n->node_binop.exp_left, ft, vt);

					// apply promotion rules
					if (type_l == FLOAT || type_r == FLOAT) {
						n->node_binop.exp_type = FLOAT;
						n->node_binop.exp_left->node_exp.exp_type = FLOAT;
						n->node_binop.exp_right->node_exp.exp_type = FLOAT;
					} else if (type_l == SFLOAT || type_r == SFLOAT) {
						n->node_binop.exp_type = SFLOAT;
						n->node_binop.exp_left->node_exp.exp_type = SFLOAT;
						n->node_binop.exp_right->node_exp.exp_type = SFLOAT;
					} else if (type_l == INT || type_r == INT) {
						n->node_binop.exp_type = INT;
						n->node_binop.exp_left->node_exp.exp_type = INT;
						n->node_binop.exp_right->node_exp.exp_type = INT;
					} else if (type_l == CINT || type_r == CINT) {
						n->node_binop.exp_type = CINT;
						n->node_binop.exp_left->node_exp.exp_type = CINT;
						n->node_binop.exp_right->node_exp.exp_type = CINT;
					}
					break;

				default:

					// generate the left and right hand type
					type_r = fill_exp_type(n->node_binop.exp_right, ft, vt);
					type_l = fill_exp_type(n->node_binop.exp_left, ft, vt);

					// apply promotion rules
					if (type_l == FLOAT || type_r == FLOAT) {
						n->node_binop.exp_left->node_exp.exp_type = FLOAT;
						n->node_binop.exp_right->node_exp.exp_type = FLOAT;
					} else if (type_l == SFLOAT || type_r == SFLOAT) {
						n->node_binop.exp_left->node_exp.exp_type = SFLOAT;
						n->node_binop.exp_right->node_exp.exp_type = SFLOAT;
					} else if (type_l == INT || type_r == INT) {
						n->node_binop.exp_left->node_exp.exp_type = INT;
						n->node_binop.exp_right->node_exp.exp_type = INT;
					} else if (type_l == CINT || type_r == CINT) {
						n->node_binop.exp_left->node_exp.exp_type = CINT;
						n->node_binop.exp_right->node_exp.exp_type = CINT;
					}

					n->node_binop.exp_type = INT;

					break;

			}

			return n->node_exp.exp_type;

		case T_UOP:
			if (n->node_uop.op == UOP_MINUS) {
				n->node_uop.exp_type = fill_exp_type(n->node_uop.exp, ft, vt);
			} else {
				fill_exp_type(n->node_uop.exp, ft, vt);
				n->node_uop.exp_type = INT;
			}

			return n->node_uop.exp_type;

		case T_EXPS:

			// recurse
			fill_exp_type(n->node_exps.left, ft, vt);
			if (n->node_exps.right != NULL) fill_exp_type(n->node_exps.right, ft, vt);

			break;

		case T_LIT:
			if (n->node_lit.lit_type == LIT) {
				n->node_lit.exp_type = INT;
			} else {
				n->node_lit.exp_type = FLOAT;
			}

			return n->node_lit.exp_type;

		case T_VAR:
			err = find_local_var(*vt, n->node_var.varname, &vent);
			if (err == NOT_FOUND) {
				fprintf(stderr, "error: local variable (%s) is not defined (code: 008).\n", n->node_var.varname);
			} else if (err) {
				fprintf(stderr, "error: Unknown error (code: 099).\n");
				exit(99);
			}
			n->node_var.exp_type = vent->valtype;

			return n->node_var.exp_type;

		default:
			fprintf(stderr, "unknown type.\n");
			return UNDEF;
  }
	return UNDEF;
}

/* scan_AST: a recursive function to reach from *prog* to *extern* and *func*,
 * 	invokes the following helper functions.
 * 	returns a nonzero value when encountered an error.
 *
 *	args: Node tree
 *	returns: void
 */
void scan_AST(FUNC_table *t, Node *n)
{
	if (n==NULL)	return;
	VAR_table *vt = NULL;
	FUNC_entry *entry = NULL;

	switch (n->tag) {
		case T_PROG:
			scan_AST(t, n->node_prog.externs);
			scan_AST(t, n->node_prog.funcs);
			break;

		case T_FUNCS:
			scan_AST(t, n->node_funcs.left);
			scan_AST(t, n->node_funcs.right);
			break;

		case T_EXTERNS:
			scan_AST(t, n->node_externs.left);
			scan_AST(t, n->node_externs.right);
			break;

		case T_EXTERN:
			scan_tdecls(n->node_extern.tdecls);
			scan_func(t, n);
			break;

		case T_FUNC:
			// Step1: Add the function to the symbol table
			scan_func(t, n);
			// Step2: Find the variable table for the function
			assert( find_func(t, n->node_func.globid->node_globid.funcname, &entry) == 0);
			// printf("%p\n", (void *)entry);
			vt = entry->local_vars;
			// Step3: Load the arguments into the variable table
			scan_args(vt, n->node_func.vdecls);
			// Scan through the *blk* of the function
			scan_fblk(t, vt, n->node_func.blk);
			break;

		default:
			break;
	}
	return;
}

/* add_func: add a function from the source file to the symbol table
 *	args:
 *		t: A pointer to the function table
 *		n: An AST node
 *	returns:
 *		error code:
 *			FUNC_RET_REF: The return type of the function cannot be ref type
 */
void scan_func(FUNC_table *t, Node *n)
{
	// confirm the correct input
	assert(n->tag==T_EXTERN || n->tag==T_FUNC);

	// Check return type, no need to check ref ref or ref void
	enum type_tag ret_type = n->node_func.type->node_type.type_tag;
	if (ret_type == TYPE_NOALIAS_REF || ret_type == TYPE_REF) {
		fprintf(stderr, "error: The function (%s) may not return a ref type (code: 004).\n", n->node_func.globid->node_globid.funcname);
	  exit(FUNC_RET_REF);
	}

	int err = add_table_func(t, n->node_func.globid->node_globid.funcname, n);
	if (err == FUNC_DUP) {
		fprintf(stderr, "error: The function (%s) has already been declared (code: 012).\n", n->node_func.globid->node_globid.funcname);
		exit(FUNC_DUP);
	}
}

/* verify_type: verify the given type node is not ref ref or ref void
 *	args:
 *		n: Node <type>
 *	Use recursion to check the case of ref ref, and ref void
 */
void verify_type(Node *n)
{
	assert(n->tag == T_TYPE);

	if (n->node_type.type_tag == TYPE_REF || n->node_type.type_tag == TYPE_NOALIAS_REF) {
		// Check recursive type
		Node *ttype = n->node_type.type;
		// Check the case of [ref] [ref]
		if (ttype->node_type.type_tag == TYPE_REF ||
				ttype->node_type.type_tag == TYPE_NOALIAS_REF) {
			fprintf(stderr, "error: A ref type may not contain type [ref] (code: 002).\n");
			exit(REF_NO_REF_VOID);
		}
		// Check the case of [ref] [void]
		if (ttype->node_type.type_tag == TYPE_VOID) {
			fprintf(stderr, "error: A ref type may not contain type [void] (code: 002).\n");
			exit(REF_NO_REF_VOID);
		}
	}
}

/* verify_vdecl: verify the given node is <vdecl> is valid
 *	args: a node <vdecl>
 */
void verify_vdecl(Node *n)
{
	assert(n->tag == T_VDECL);

	Node *ttype = n->node_vdecl.type;
	if (ttype->node_type.type_tag == VOID) {
		fprintf(stderr, "error: variable (%s) cannot be declared with type [void] (code: 001).\n", n->node_vdecl.var->node_var.varname);
		exit(VDECL_NO_REF);
	}

	verify_type(ttype);
}

/* verify_vdeclstmt: verify the given node <vdeclstmt> is valid*/
void verify_vdeclstmt(Node *n, VAR_table *t)
{
	int err = 0;
	assert(n->node_simple_stmt.simple_stmt_tag == STMT_VARDECL);

	Node *vdecl = n->node_simple_stmt.vdecl;
	Node *exp = n->node_simple_stmt.exp;
	VAR_entry *var_ent;

	// verify type and vdecl type
	verify_vdecl(n->node_simple_stmt.vdecl);
	// verify rule 5
	enum type_tag decl_type = vdecl->node_vdecl.type->node_type.type_tag;

	if (decl_type == TYPE_REF || decl_type == TYPE_NOALIAS_REF) {
		if (exp->node_exp.exp_tag != EXP_VAR) {
			fprintf(stderr, "error: ref var initializer must be a variable (code: 005).\n");
			exit(REF_INIT_VAR);
		}

		Node *var = exp->node_exp.var;

		// verify the variable at the right hand side exits in variable table.
		err = find_local_var(t, var->node_var.varname, &var_ent);
		if (err == NOT_FOUND) {
			fprintf(stderr, "error: local variable (%s) is not defined (code: 008).\n", var->node_var.varname);
			exit(NOT_FOUND);
		}
	}
	// end of function
}

/* scan_tdecls: scan the type declaration of external functions
 * 	args:
 *		node <tdecls> or <type>
 *	Use recursion to check all the type.
 */
void scan_tdecls(Node *n)
{
	if (n == NULL)	return;

	assert(n->tag == T_TDECLS || n->tag == T_TYPE);

	switch (n->tag) {
		case T_TDECLS:
			scan_tdecls(n->node_tdecls.left);
			scan_tdecls(n->node_tdecls.right);
			break;

		case T_TYPE:
			verify_type(n);
			break;

		default:
			break;
	}
}

/* scan_args: scan the arguments into the local variable table
 *	args:
 *		a node vdecls
 */
void scan_args(VAR_table *t, Node *n)
{
	if (n == NULL)	return;

	char *varname;
	assert(n->tag == T_VDECLS || n->tag == T_VDECL);

	switch (n->tag) {

		case T_VDECLS:
			scan_args(t, n->node_vdecls.left);
			scan_args(t, n->node_vdecls.right);
			break;

		case T_VDECL:
			verify_vdecl(n);
			varname = n->node_vdecl.var->node_var.varname;
			if (add_table_local_var(t, varname, n, NULL, 0) == VAR_DUP) {
				fprintf(stderr, "error: local variable (%s) in function (%s) has already been declared (code: 011).\n", varname, t->func_name);
				exit(VAR_DUP);
			}
			break;

		default:
			break;
	}
}

/* scan_fblk: a recursive function to go through the *blk* of the function.
 * 	args:
 *		ft: function table pointer
 *		vt: variable table pointer
 *		n: Node n
 * purpose: load local variables into the table
 *					and filter invalid function calls
 */
void scan_fblk(FUNC_table *ft, VAR_table *vt, Node *n)
{
	if (n==NULL)	return;

	int err = 0;
	FUNC_entry *ent;
	char *funcname;

	switch (n->tag) {
		case T_BLK:
			scan_fblk(ft, vt, n->node_blk.stmts);
			break;

		case T_STMTS:
			scan_fblk(ft, vt, n->node_stmts.left);
			scan_fblk(ft, vt, n->node_stmts.right);
			break;

		case T_STMT:
			scan_fblk(ft, vt, n->node_stmt.oc_stmt);
			break;

		case T_OPEN_STMT:
			scan_fblk(ft, vt, n->node_open_stmt.exp);
			scan_fblk(ft, vt, n->node_open_stmt.stmt1);
			scan_fblk(ft, vt, n->node_open_stmt.stmt2);
			break;

		case T_CLOSED_STMT:
			scan_fblk(ft, vt, n->node_closed_stmt.exp);
			scan_fblk(ft, vt, n->node_closed_stmt.stmt1);
			scan_fblk(ft, vt, n->node_closed_stmt.stmt2);
			break;

		case T_SIMPLE_STMT:

			// Check if it is a vardeclstmt
			if (n->node_simple_stmt.simple_stmt_tag == STMT_VARDECL) {
				verify_vdeclstmt(n, vt);
				// Add local variable
				// printf("%s added\n", n->node_simple_stmt.vdecl->node_vdecl.var->node_var.varname);

				add_table_local_var(vt, n->node_simple_stmt.vdecl->node_vdecl.var->node_var.varname, n->node_simple_stmt.vdecl, NULL, 0);
			}
			scan_fblk(ft, vt, n->node_simple_stmt.blk);
			scan_fblk(ft, vt, n->node_simple_stmt.exp);
			scan_fblk(ft, vt, n->node_simple_stmt.slit);
			break;

		case T_EXPS:
			scan_fblk(ft, vt, n->node_exps.left);
			scan_fblk(ft, vt, n->node_exps.right);
			break;

		case T_EXP:

			//Check funccall, make sure the function has been declared
			if (n->node_exp.exp_tag == EXP_GLOBID) {
				//Check if the previous function has been declared
				funcname = n->node_exp.globid->node_globid.funcname;
				err = find_func(ft, funcname, &ent);

				if (err == NOT_FOUND)	{
					fprintf(stderr, "error: Function (%s) must be declared before use (code: 003).\n", funcname);
					exit(FUNC_NO_DECL);
				}
				/*Further, check if argument type matches with prototype
				if (ent->func_node->tag == T_FUNC) {
					err = check_call_args(ent->func_node->vdecls, n->right);
				} else {
					err = check_call_args(ent->func_node->tdecls, n->right);
				}*/
			}
			scan_fblk(ft, vt, n->node_exp.exp);
			scan_fblk(ft, vt, n->node_exp.binop);
			scan_fblk(ft, vt, n->node_exp.uop);
			scan_fblk(ft, vt, n->node_exp.lit);
			scan_fblk(ft, vt, n->node_exp.var);
			scan_fblk(ft, vt, n->node_exp.exps);
			break;

		case T_SLIT:
		case T_BINOP:
		case T_UOP:
		case T_VAR:
		case T_LIT:
		case T_TYPE:
			break;

		default:
			fprintf(stderr, "unknown node at scan_fblk()\n");
	}
}

/* check_run: The function to find if the run function has been correctly defined.
  	raise errors and exit the program within this function.
 */
void check_run(FUNC_table *t)
{
	int err;
	FUNC_entry *entry;
	char run[] = "run";

	err = find_func(t, run, &entry);
	if (err == NOT_FOUND) {
		fprintf(stderr, "error: The function (run) is not defined (code: 006).\n");
		exit(NO_FUNC_RUN);
	} else {
		if (entry->ret_type != INT) {
			fprintf(stderr, "error: The function (run) does not have return type [int] (code: 006).\n");
			exit(NO_FUNC_RUN);
		}
		if (entry->func_node->node_func.vdecls != NULL) {
			fprintf(stderr, "error: The function (run) must take no arguments (code: 006)!\n");
			exit(NO_FUNC_RUN);
		}
	}
	// end of function
}

// process_tree: The entry point of this file.
//	will do the 6 checks needed.
void process_tree(Node *root)
{
	FUNC_table *f_table = init_func_table(INIT_SIZE);
	VAR_table *v_table = NULL;
	//Load all the functions into the hash table
	scan_AST(f_table, root);
	//Check if the run function exists and has the correct return type;
	check_run(f_table);
	// Fill the exps with expression tags
	fill_exp_type(root, f_table, &v_table);

	// Load the expression tree
	free_func_table(f_table);

}
