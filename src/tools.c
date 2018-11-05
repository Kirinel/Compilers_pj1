#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../temp/bison.h"
#include "../temp/flex.h"
//#include "symbol_table.h"
#include "tools.h"

#define TABLESIZE 32
#define MAX_TYPE	32

// load_all: a recursive function to reach from *prog* to *extern* and *func*,
//  invokes the following helper functions.
//	returns a nonzero value when encountered an error.
int load_all(FUNC_table *t, node *n)
{
	if (n==NULL)	return 0;
	int err;
	VAR_table *vt;
	FUNC_entry *entry;

	switch (n->tag) {
		case T_PROG:
			err = load_all(t, n->right);
			err = load_all(t, n->left) || err;
			break;
		case T_FUNCS:
		case T_EXTERNS:
			err = load_all(t, n->left);
			err = load_all(t, n->right) || err;
			break;
		case T_EXTERN:
			load_targs(n->tdecls);
			//printf("act\n");
			err = load_func(t, n);
			//err = find_func(t, n->globid->str, &entry) || err;
			//vt = entry->local_vars;
			//Load the arguments into the variable table
			break;
		case T_FUNC:
			err = load_func(t, n);
			//Find the variable table for the function
			err = find_func(t, n->globid->str, &entry) || err;
			vt = entry->local_vars;
			//Load the arguments into the variable table
			load_args(vt, n->vdecls);
			//Scan through the *blk* of the function
			load_func_blk(t, vt, n->blk);
			break;
	}
	return err;
}

// load_func: add a *func* node into the hash table.
//  Checks if the return type of the function is a ref type. (If ref type, the program will exit)
//	Returns a nonzero value when encountered an error.
int load_func(FUNC_table *t, node *n)
{
	int err;

	assert(n->tag==T_EXTERN || n->tag==T_FUNC);
	//Check the function does not have a ref return type
	err = check_func_ret_type(n->globid->str, n->type);
	//add the function (also checks if duplicated entries occur)
	err = add_table_func(t, n->globid->str, n->type->str, n) || err;

	return err;
}

// check_func_ret_type: Check the function return type cannot be [ref] type
int check_func_ret_type(char *globid, node *n)
{
	int err = 0;

	assert(n->tag == T_TYPE);

	// check the return type is not a ref type
	if (strcmp(n->name, "type") != 0) {
		// check the type of not ref ref or ref void
		err = check_ref_type(n);
		// the function cannot return a ref type
		fprintf(stderr, "error: The function (%s) may not return a ref type (code: 004).\n", globid);
		exit(4);
		// int err = 4;
	}
	return err;
}

// load_args: load the arguments as local variables into the table
//	also, conduct type check
int load_args(VAR_table *t, node *n)
{
	int err;

	if (n == NULL)	return 0;

	assert(n->tag == T_VDECLS || n->tag == T_VDECL);

	switch (n->tag) {
		case T_VDECLS:
			err = load_args(t, n->left);
			if (n->right != NULL)	err = load_args(t, n->right) || err;
			break;
		case T_VDECL:
			err = check_vdecl(n) || err;
			err = add_local_var(t, n);
			break;
	}

	return err;
}

// load_targs: check the type declarations of a type argument
int load_targs(node *n)
{
	int err = 0;

	if (n == NULL)	return 0;

	assert(n->tag == T_TDECLS || n->tag == T_TYPE);

	switch (n->tag) {
		case T_TDECLS:
			err = load_targs(n->left);
			if (n->right != NULL)	err = load_targs(n->right) || err;
			break;
		case T_TYPE:
			if (strcmp(n->name, "type") != 0) {
				err = check_ref_type(n);
			}
			break;
	}

	return err;
}

// load_func_blk: a recursive function to go through the *blk* of the function.
//	Returns a nonzero value when encountered an error.
//	the function will be executed one time for each function
int load_func_blk(FUNC_table *ft, VAR_table *vt, node *n)
{
	if (n==NULL)	return 0;

	int err = 0;
	FUNC_entry *ent;

	switch (n->tag) {
		case T_BLK:
			err = load_func_blk(ft, vt, n->left);
			break;
		case T_STMTS:
			err = load_func_blk(ft, vt, n->left);
			err = load_func_blk(ft, vt, n->right) || err;
			break;
		case T_STMT:
			err = load_func_blk(ft, vt, n->left);
			break;
		case T_OPEN_STMT:
			err = load_func_blk(ft, vt, n->expr);
			err = load_func_blk(ft, vt, n->stmt1) || err;
			err = load_func_blk(ft, vt, n->stmt2) || err;
			break;
		case T_CLOSED_STMT:
			err = load_func_blk(ft, vt, n->expr);
			err = load_func_blk(ft, vt, n->stmt1) || err;
			err = load_func_blk(ft, vt, n->stmt2) || err;
			break;
		case T_SIMPLE_STMT:
			if (!strcmp(n->name, "vardeclstmt")) {
				// Rule5: ref var initializer must be a variable.
				if (strcmp(n->left->left->name, "type") != 0) {
					if (strcmp(n->right->name, "var") != 0) {
						fprintf(stderr, "error: ref var initializer must be a variable (code: 005).\n");
						exit(5);
					}
					// Find the assignment variable, if it does not exit, complain.
					VAR_entry *var_ent;
					err = find_local_var(vt, n->right->left->str, &var_ent);
					if (err) {
						fprintf(stderr, "error: local variable (%s) is not defined (code: 008).\n", n->right->left->str);
						exit(8);
					}
				}
				// Add local variable
				err = add_local_var(vt, n->left) || err;
				err = load_func_blk(ft, vt, n->right) || err;
			} else {
				err = load_func_blk(ft, vt, n->left);
				if (n->right != NULL) err = load_func_blk(ft, vt, n->right) || err;
			}
			break;
		case T_EXPS:
			err = load_func_blk(ft, vt, n->left) || err;
			err = load_func_blk(ft, vt, n->right) || err;
		case T_EXP:
			//For expression, recurse first
			// if (n->left != NULL) load_func_blk(n->left, vt, ft);
			// if (n->right != NULL) load_func_blk(n->right, vt, ft);
			// assign the type of the expression
			n->exptype = check_exp_type(n, vt, ft);
			//Check funccall, make sure the function has been declared
			if (!strcmp(n->name, "globid")) {
				//Check if the previous function has been declared
				err = find_func(ft, n->left->str, &ent);
				//printf("activated!\n");
				if (err)	{
					fprintf(stderr, "error: Function (%s) must be declared before use (code: 003).\n", n->left->str);
					exit(3);
				}
				//Further, check if argument type matches with prototype
				if (ent->func_node->tag == T_FUNC) {
					err = check_call_args(ent->func_node->vdecls, n->right);
				} else {
					err = check_call_args(ent->func_node->tdecls, n->right);
				}
				err = load_func_blk(ft, vt, n->right) || err;
			}
			break;
			/*
		case T_BINOP:
			// assign the type of the expression
			n->exptype = check_exp_type(n, vt, ft);
			if (n->left != NULL) check_exp_type(n->left, vt, ft);
			if (n->right != NULL) check_exp_type(n->right, vt, ft);
			*/
		case T_SLIT:
			break;
		default:
			printf("unknown node %s\n", n->name);
	}
	return 0;
}

// Check the arguments when making a function call
//	especially cannot assign values to a ref type
int check_call_args(node *def_node, node *call_node)
{
	int err = 0;

	if (def_node == NULL && call_node == NULL)	return 0;

	//printf("%d %d\n", def_node == NULL, call_node == NULL);

	//printf("1\n");
	if (def_node != NULL && call_node != NULL) {

		assert(def_node->tag == T_VDECLS || def_node->tag == T_VDECL ||
					 def_node->tag == T_TDECLS || def_node->tag == T_TYPE);
		assert(call_node->tag == T_EXPS || call_node->tag == T_EXP);

			// case1: both non_terminals
		if ((def_node->tag == T_VDECLS || def_node->tag == T_TDECLS) &&
				call_node->tag == T_EXPS) {
			err = check_call_args(def_node->left, call_node->left) || err;
			err = check_call_args(def_node->right, call_node->right) || err;
		} else if (def_node->tag == T_VDECL && call_node->tag == T_EXP) {
			// case2: both terminals, vdecl
			if (strcmp(def_node->left->name, "type") != 0) {
				if (strcmp(call_node->name, "var") != 0) {
					fprintf(stderr, "error: ref var initializer must be a variable in funccall (code: 005).\n");
					exit(5);
				}
			}
			// err = 0;
		} else if (def_node->tag == T_TYPE && call_node->tag == T_EXP) {
			//printf("activated!\n");
			// case3: both terminals, type
			if (strcmp(def_node->name, "type") != 0) {
				if (strcmp(call_node->name, "var") != 0) {
					fprintf(stderr, "error: ref var initializer must be a variable in funccall (code: 005).\n");
					exit(5);
				}
			}
		} else {
			// printf("activated 1!\n");
			fprintf(stderr, "error: invalid function call, number of arguments doesn't match (code: 009).\n");
			exit(9);
		}
		return err;
	}

	// printf("activated 2!\n");
	fprintf(stderr, "error: invalid function call, number of arguments doesn't match (code: 009).\n");
	exit(9);

	return err;
}

// assign_exp_type: Assign the type of expressions


// add_local_var: A function to add the declared local variable into the variable table.
//	also checks the vdecl type does not have a void type
int add_local_var(VAR_table *t, node *n)
{
	assert(n->tag == T_VDECL);

	int err;
	char *type, *varname = n->right->str;

	//Rule 1: Check the declared type is not void
	err = check_vdecl(n);

	err = get_declared_type(n->left, &type, n->right->str);
	err = add_table_local_var(t, varname, type, n) || err;
	if (err) {
		fprintf(stderr, "error: local variable (%s) in function (%s) has already been declared (code: 008).\n", varname, t->func_name);
		exit(8);
	}


	return err;
}

// get_declared_type: A function to get the type of the *vdecl* node, as a pointer parameter.
int get_declared_type(node *n, char **type_p, char *varname)
{
	assert(n->tag == T_TYPE);

	char type[MAX_TYPE];
	int err;

	if (!strcmp(n->name, "ref") || !strcmp(n->name, "noalias ref")) {
		// Further needs here to check the type of the right hand size expression
		if (!strcmp(n->left->name, "ref") || !strcmp(n->left->name, "noalias ref")) {
			fprintf(stderr, "error: variable (%s) cannot be declared with [ref] [ref] type (code: 002).\n", varname);
			exit(2);

			sprintf(type, "undefined");
			err = 2;
		} else if (!strcmp(n->left->str, "void")) {
			fprintf(stderr, "error: variable (%s) cannot be declared with [ref] [void] type (code: 002).\n", varname);
			exit(2);

			sprintf(type, "undefined");
			err = 2;
		} else {
			sprintf(type, "%s %s", n->name, n->left->str);
			err = 0;
		}
	} else if (!strcmp(n->str, "void")) {
		fprintf(stderr, "error: variable (%s) cannot be declared with [type: %s] (code: 001).\n", varname, n->str);
		exit(1);

		sprintf(type, "undefined");
		err = -1;
	} else {
		sprintf(type, "%s", n->str);
		err = 0;
	}

	*type_p = strdup(type);
	return err;
}

// check_vdecl: Check the node vdecl does not have a declared type void
int check_vdecl(node *n)
{
	int err = 0;

	assert(n->tag == T_VDECL);

	if(strcmp(n->left->name, "type") != 0) {
		// Check ref ref or ref void (Case 2)
		err = check_ref_type(n->left);

		//if the left child is a ref, given that no "ref ref" occurs,
		// the left child must be type
		node *p = n->left;
		if (strcmp(p->left->str,"void") == 0) {
			fprintf(stderr, "error: A ref type cannot cannot contain type [void] (code: 002).\n");
			exit(2);
			//return -1;
		} else if (strcmp(n->left->left->name, "type") != 0) {
			fprintf(stderr, "error: A ref type cannot cannot contain type [ref] (code: 002).\n");
			exit(2);
		}
	}
	else {
		if (strcmp(n->left->str, "void") == 0) {
			fprintf(stderr, "error: variable (%s) cannot be declared with type [void] (code: 001).\n", n->right->str);
			exit(1);
			//return -1;
		}
	}
	return err;
}

int declare_local_vars(FUNC_table *t, char *globid, char *varname, node *n)
{
	//Find the hash table for the given function
	FUNC_entry *f_ent;
	int err;

	err = find_func(t, globid, &f_ent);
	if (!err) {
		fprintf(stderr, "internal error: func (%s) not found!\n", globid);
		exit(-1);
	}

	VAR_table *vt = f_ent->local_vars;
	err = add_table_local_var(vt, varname, n->type->str, n) || err;
	if (err) {
		fprintf(stderr, "error: local variable (%s) in function (%s) has already been declared (code: 008).\n", varname, globid);
		exit(8);
	}

	return err;
}

// check_run: The function to find if the run function has been correctly defined.
//  raise errors and exit the program within this function.
int check_run(FUNC_table *t)
{
	int err;
	FUNC_entry *entry;

	err = find_func(t, "run", &entry);
	if (err) {
		fprintf(stderr, "error: The function (run) is not defined (code: 006).\n");
		exit(6);
	} else {
		if (strcmp(entry->ret_type, "int") != 0) {
			fprintf(stderr, "error: The function (run) [type: %s] does not have return type [int] (code: 006).\n", entry->ret_type);
			exit(6);

			err = -1;
		}
		if (entry->func_node->vdecls != NULL) {
			fprintf(stderr, "error: The function (run) must take no arguments (code: 006)!\n");
			exit(6);

			err = -1;
		}
	}
	return err;
}

// process_tree: The entry point of this file.
//	will do the 6 checks needed.
void process_tree(node *root)
{
	int err;
	FUNC_table *f_table = init_func_table(TABLESIZE);
	//Load all the functions into the hash table
	err = load_all(f_table, root);
	//Check if the run function exists and has the correct return type;
	err = check_run(f_table) || err;
	//printf("%d\n", err);

	free_func_table(f_table);


	if (err) exit(err);
}

// check_ref_type: Once encountered a ref type, check it is not *ref ref* or *ref void*
int check_ref_type(node *n)
{
	int err = 0;

	assert(n->tag == T_TYPE);
	//Make sure the input is a ref type
	assert(strcmp(n->name, "ref") == 0 || strcmp(n->name, "noalias ref") == 0);

	if(strcmp(n->left->str, "void") == 0) {
		fprintf(stderr, "error: A ref type may not contain type [void] (code: 002).\n");
		exit(2);
	} else if (strcmp(n->left->name, "ref") == 0 || strcmp(n->left->name, "noalias ref") == 0) {
		// printf("activated!\n");
		fprintf(stderr, "error: A ref type may not contain type [ref] (code: 002).\n");
		exit(2);
	}

	return err;
}

// input: type name as string;
//	output: type name as enum exptype
enum exptype type_convert(char *type)
{
	if (strcmp(type, "lit") == 0 || strcmp(type, "int") == 0 ||
			strcmp(type, "ref lit") == 0 || strcmp(type, "ref int") == 0 ||
			strcmp(type, "noalias ref lit") == 0 || strcmp(type, "noalias ref int") == 0) {
				return INT;
	}
	else if (strcmp(type, "flit") == 0 || strcmp(type, "float") == 0 ||
					 strcmp(type, "ref flit") == 0 || strcmp(type, "ref float") == 0 ||
				 	 strcmp(type, "noalias ref flit") == 0 || strcmp(type, "noalias ref float") == 0) {
						 return FLOAT;
	}
	else if (strcmp(type, "cint") == 0 ||
					 strcmp(type, "ref cint") == 0 ||
				 	 strcmp(type, "noalias ref cint") == 0) {
						 return CINT;
	}
	else if (strcmp(type, "sfloat") == 0 ||
					 strcmp(type, "ref sfloat") == 0 ||
				 	 strcmp(type, "noalias ref sfloat") == 0) {
						 return SFLOAT;
	}
	else if (strcmp(type, "void") == 0) {
		return VOID;
	} else {
		fprintf(stderr, "unknown type %s\n", type);
		return UNDEF;
	}
}

// Assign the type of the expression
enum exptype check_exp_type(node *n, VAR_table *tv, FUNC_table *tf)
{
	assert(n->tag == T_EXP || n->tag == T_BINOP || n->tag == T_UOP ||
				 n->tag == T_LIT || n->tag == T_VAR || n->tag == T_EXPS || n->tag == T_GLOB);

	int err = 0;
	VAR_entry *ent;
	VAR_entry *temp_var;
	FUNC_entry *temp_func;

	switch (n->tag) {
		case T_LIT:
			//printf("activated!\n");

			if (n->exptype == UNDEF) {
				// printf("%s\n", n->name);
				n->exptype = type_convert(n->name);
				// printf("%d\n", n->exptype);
			}
			return n->exptype;
			// break;
		case T_VAR:
			//printf("activated!\n");

			if (n->exptype == UNDEF) {
				err = find_local_var(tv, n->str, &ent);
				if (err) {
					fprintf(stderr, "error: local variable (%s) has not been defined (code: 008).\n", n->str);
					exit(8);
				}
				// assert(err == 0);
				//printf("%s\n", ent->type);
				n->exptype = type_convert(ent->type);
			}
			return n->exptype;
			//break;
		case T_BINOP:
			if (n->exptype != UNDEF)
				return n->exptype;

			//printf("activated!\n");

			enum exptype t_left = check_exp_type(n->left, tv, tf);
			enum exptype t_right = check_exp_type(n->right, tv, tf);

			//printf("%d %d\n", t_left, t_right);

			if (strcmp(n->name, "add") == 0 ||
				 strcmp(n->name, "mul") == 0 ||
				 strcmp(n->name, "sub") == 0 ||
				 strcmp(n->name, "div") == 0) {

					if (t_left == FLOAT) {
						//n->right->exptype = FLOAT; //no convert
						n->exptype = FLOAT;
						return FLOAT;
					}
					else if (t_right == FLOAT) {
						//n->left->exptype = FLOAT; //no convert
						n->exptype = FLOAT;
						return FLOAT;
					}
					else if (t_left == SFLOAT) {
						//n->right->exptype = SFLOAT; //no convert
						n->exptype = SFLOAT;
						return SFLOAT;
					}
					else if (t_right == SFLOAT) {
						//n->left->exptype = SFLOAT; //no convert
						n->exptype = SFLOAT;
						return SFLOAT;
					}
					else if (t_left == INT) {
						//n->right->exptype = INT; //no convert
						n->exptype = INT;
						return INT;
					}
					else if (t_right == INT) {
						//n->left->exptype = INT; //no convert
						n->exptype = INT;
						return INT;
					}
					else if (t_left == CINT) {
						//n->right->exptype = CINT; //no convert
						n->exptype = CINT;
						return CINT;
					}
					else if (t_right == CINT) {
						//n->left->exptype = CINT; //no convert
						n->exptype = CINT;
						return CINT;
					}
					else {
						fprintf(stderr, "error: encountered void(undefined) type in binary operation!\n");
						n->exptype = UNDEF;
						return UNDEF;
					}
			} else if (strcmp(n->name, "assign") == 0) {
				n->exptype = t_right;
				return n->exptype;
			} else {
				n->exptype = INT;
				return INT;
			}
			// break;
		case T_UOP:
			if (n->exptype != UNDEF) {
				return n->exptype;
			}
			if (strcmp(n->name, "minus") == 0) {
				n->exptype = check_exp_type(n->left, tv, tf);
				return n->exptype;
			}
			else {
				n->exptype = INT;
				return INT;
			}
			//break;
		case T_EXP:
			if (n->exptype != UNDEF)
				return n->exptype;

			if(strcmp(n->name, "exp") == 0) {
				n->exptype = check_exp_type(n->left, tv, tf);
				return n->exptype;
			}
			else if(strcmp(n->name, "binop") == 0) {

				// printf("activated!\n");

				n->exptype = check_exp_type(n->left, tv, tf);

				// printf("%d\n", n->exptype);
				return n->exptype;
			}
			else if(strcmp(n->name, "uop") == 0) {
				n->exptype = check_exp_type(n->left, tv, tf);
				return n->exptype;
			}
			else if(strcmp(n->name, "lit") == 0) {
				n->exptype = check_exp_type(n->left, tv, tf);
				return n->exptype;
			}
			else if(strcmp(n->name, "var") == 0) {
				/*
				err = find_local_var(tv, n->left->str, &temp_var);
				if (err) {
					fprintf(stderr, "error: local variable (%s) is not defined (code: 008).\n", n->right->left->str);
					exit(8);
				}
				*/
				//printf("%s\n", temp_var->type);
				n->exptype = check_exp_type(n->left, tv, tf);
				return n->exptype;
			}
			else if(strcmp(n->name, "globid") == 0) {
				err = find_func(tf, n->left->str, &temp_func);
				if (err) {
					fprintf(stderr, "error: The function (%s) is not defined (code: 003).\n", n->left->str);
					exit(3);
				}
				n->exptype = type_convert(temp_func->ret_type);
				return n->exptype;
			}
			break;
		case T_GLOB:
			return UNDEF;
			//break;
		case T_EXPS:
			check_exp_type(n->left, tv, tf);
			check_exp_type(n->left, tv, tf);
			return UNDEF;
			//break;
		default:
			fprintf(stderr, "unknown node (%s)", n->name);
			break;
			//end of switch
		}
	fprintf(stderr, "unknown node (%s).\n", n->name);
	exit(-1);
	//end of function
}

// parse_tree: parse the given input using Bison
void parse_tree(char *in, node **root_p)
{

	yyin = fopen(in, "r");

	fseek(yyin, 0L, SEEK_END);
	int sz = ftell(yyin);
	if (sz < 1) {
		fprintf(stderr, "Error: Input file cannot be empty!\n");
		return;
	}
	fseek(yyin, 0L, SEEK_SET);

	if (yyin == NULL) {
		puts("Error: No input files given");
		exit(-1);
	}

	/* parse the given source file */
	int err = yyparse(root_p);
	if (err != 0)	{
		fprintf(stderr, "Error: Wrong Syntax!\n");
		exit(-1);
	}

	return;
}

// emit_ast: write out the ast to file
void emit_ast(char *in, char *out, node *rootnode)
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
