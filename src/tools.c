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
			err = load_func(t, n);
			err = find_func(t, n->globid->str, &entry) || err;
			vt = entry->local_vars;
			//Load the arguments into the variable table
			load_args(vt, n->vdecls);
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
//	Returns a nonzero value when encountered an error.
int load_func(FUNC_table *t, node *n)
{
	assert(n->tag==T_EXTERN || n->tag==T_FUNC);
	int err = add_table_func(t, n->globid->str, n->type->str, n);
		//default:
			//fprintf(stderr, "Encounter unknown node: %s", n->name);
	//printf("%d\n", err);
	return err;
}
// load_args: load the arguments as local variables into the table
int load_args(VAR_table *t, node *n)
{
	if (n == NULL)	return 0;

	int err;
	assert(n->tag == T_VDECLS || n->tag == T_VDECL);

	switch (n->tag) {
		case T_VDECLS:
			err = load_args(t, n->left);
			if (n->right != NULL)	err = load_args(t, n->right) || err;
			break;
		case T_VDECL:
			err = add_local_var(t, n);
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

	int err;
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
			err = load_func_blk(ft, vt, n->stmt1);
			err = load_func_blk(ft, vt, n->stmt2) || err;
			break;
		case T_CLOSED_STMT:
			err = load_func_blk(ft, vt, n->stmt1);
			err = load_func_blk(ft, vt, n->stmt2) || err;
			break;
		case T_SIMPLE_STMT:
			if (!strcmp(n->name, "vardeclstmt")) {
				err = add_local_var(vt, n->left);
				err = load_func_blk(ft, vt, n->right) || err;
			} else {
				err = load_func_blk(ft, vt, n->left);
				if (n->right != NULL) err = load_func_blk(ft, vt, n->right) || err;
			}
			break;
		case T_EXP:
			if (!strcmp(n->name, "globid")) {
				//Check if the previous function has been declared
				err = find_func(ft, n->left->str, &ent);
				//printf("activated!\n");
				if (err)	fprintf(stderr, "err: implicit declaration of function (%s)\n", n->left->str);
				//Further, check if argument type matches with prototype
				/******/
			}
		case T_SLIT:
			break;
		default:
			printf("unknown node %s\n", n->name);
	}
	return 0;
}
// add_local_var: A function to add the declared local variable into the variable table.
int add_local_var(VAR_table *t, node *n)
{
	assert(n->tag == T_VDECL);
	int err;
	char *type, *varname = n->right->str;

	err = get_declared_type(n->left, &type, n->right->str);
	err = add_table_local_var(t, varname, type, n) || err;
	if (err) fprintf(stderr, "error: local variable (%s) in function (%s) has already been declared.\n", varname, t->func_name);


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
			fprintf(stderr, "error: variable (%s) cannot be declared with [ref] [ref] type\n", varname);
			sprintf(type, "undefined");
			err = -1;
		} else if (!strcmp(n->left->str, "void")) {
			fprintf(stderr, "error: variable (%s) cannot be declared with [ref] [void] type\n", varname);
			sprintf(type, "undefined");
			err = -1;
		} else {
			sprintf(type, "%s %s", n->name, n->left->name);
			err = 0;
		}
	} else if (!strcmp(n->str, "void")) {
		fprintf(stderr, "error: variable (%s) cannot be declared with [type: %s]\n", varname, n->str);
		sprintf(type, "undefined");
		err = -1;
	} else {
		sprintf(type, "%s", n->str);
		err = 0;
	}

	*type_p = strdup(type);
	return err;
}

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







int load_vars(FUNC_table *t, node *n)
{
	if (n==NULL) return 0;
	return 0;
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
	if (err) fprintf(stderr, "error: local variable (%s) in function (%s) has already been declared.\n", varname, globid);

	return err;
}

int check_run(FUNC_table *t)
{
	int err;
	FUNC_entry *entry;

	err = find_func(t, "run", &entry);
	if (err) {
		fprintf(stderr, "error: The function (run) is not found.\n");
	} else {
		if (strcmp(entry->ret_type, "int") != 0) {
			fprintf(stderr, "error: The function (run) [type: %s] does not have return type [int].\n", entry->ret_type);
			err = -1;
		}
		if (entry->func_node->vdecls != NULL) {
			fprintf(stderr, "error: The function (run) must take no arguments!\n");
			err = -1;
		}
	}
/* test find_local_var()
	VAR_entry *varentry;
	if (!err) {
		int tt = find_local_var(entry->local_vars, "pdd", &varentry);
		printf("%d\n", tt);
	}
*/
	return err;
}


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

int check_vdecl(node *n) {
	assert(n->tag == T_VDECL);
	if(strcmp(n->left->name, "type") != 0) {
		//if the left child is a ref, given that no "ref ref" occurs,
		// the left child must be type
		node * p = n->left;
		if(strcmp(p->left->str,"void") == 0) {
			fprintf(stderr, "vdecl has a void type\n");
			return -1;
		}
	}
	else {
		if(strcmp(n->left->str, "void") != 0) {
			fprintf(stderr, "vdecl has a void type\n");
			return -1;
		}
	}
	return 0;
}

int check_ref_type(node *n) {
	assert(n->tag == T_TYPE);
	assert(strcmp(n->name, "ref") == 0 || strcmp(n->name, "noalias ref") == 0)
	if(strcmp(n->left->str, "void") == 0 || strcmp(n->left->name, "ref") == 0 || strcmp(n->name, "noalias ref") == 0) {
		fprintf(stderr, "ref or void after ref\n");
		return 1;
	}
	return 0;
}

int check_ref_var_init(node *n, VAR_table *t) {
	assert(n->tag == T_SIMPLE_STMT);
	assert(n->left->left->name == "ref" || n->left->left->name == "noalias ref");
	// check right expr is a variable
	return 0;
}

enum exptype type_convert(char *type) {
	if (strcmp(type, "lit") == 0) {
		return INT;
	}
	else if(strcmp(type, "flit") == 0) {
		return FLOAT;
	}
	else if(strcmp(type, "cint") == 0) {
		return CINT;
	}
	else if(strcmp(type, "sfloat") == 0) {
		return SFLOAT;
	}
	else if(strcmp(type, "void") == 0) {
		return VOID;
	}
}

enum exptype check_exp_type(node *n, VAR_table *tv, FUNC_table *tf) {
	assert(n->tag == T_EXP || n->tag == T_BINOP || n->tag == T_UOP);
	switch (n->tag) {
		case T_EXP/* value */:
			if(strcmp(n->name, "exp") == 0) {
				return check_exp_type(n->left, tv, tf);
			}
			else if(strcmp(n->name, "binop") == 0) {
				return check_exp_type(n->left, tv, tf);
			}
			else if(strcmp(n->name, "uop") == 0) {
				return check_exp_type(n->left, tv, tf);
			}
			else if(strcmp(n->name, "lit") == 0) {
				return INT;
			}
			else if(strcmp(n->name, "var") == 0) {
				VAR_entry *temp_var;
				find_local_var(tv, n->str, &temp_var);
				return type_convert(temp_var->type);
			}
			else if(strcmp(n->name, "globid") == 0) {
				FUNC_entry *temp_func;
				find_func(tf, n->str, &temp_func);
				return type_convert(temp_func->ret_type);
			}
			break;
		case T_BINOP:
			if(strcmp(n->name, "add") == 0 || strcmp(n->name, "mul") == 0 || strcmp(n->name, "sub") == 0 || strcmp(n->name, "div") == 0) {
				if(check_exp_type(n->left, tv, tf) == FLOAT) {
					n->right->exptype = FLOAT;
					return FLOAT;
				}
				else if(check_exp_type(n->right, tv, tf) == FLOAT) {
					n->left->exptype = FLOAT;
					return FLOAT;
				}
				if(check_exp_type(n->left, tv, tf) == SFLOAT) {
					n->right->exptype = SFLOAT;
					return SFLOAT;
				}
				else if(check_exp_type(n->right, tv, tf) == SFLOAT) {
					n->left->exptype = SFLOAT;
					return SFLOAT;
				}
				if(check_exp_type(n->left, tv, tf) == INT) {
					n->right->exptype = INT;
					return INT;
				}
				else if(check_exp_type(n->right, tv, tf) == INT) {
					n->left->exptype = INT;
					return INT;
				}
				if(check_exp_type(n->left, tv, tf) == CINT) {
					n->right->exptype = CINT;
					return CINT;
				}
				else if(check_exp_type(n->right, tv, tf) == CINT) {
					n->left->exptype = CINT;
					return CINT;
				}
			}
			else if(strcmp(n->name, "assign") == 0) {
				return check_exp_type(n->right, tv, tf);
			}
			else {
				return INT;
			}
			break;
		case T_UOP:
			if(strcmp(n->name, "minus") == 0) {
				return check_exp_type(n->left, tv, tf);
			}
			else return INT;
	}

}
