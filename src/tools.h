#ifndef TOOLS_H
#define TOOLS_H

#include "ast.h"
#include "symbol_table.h"




/* Pre-scan the tree and prints potential errors */

// load_all: a recursive function to reach from *prog* to *extern* and *func*,
//  invokes the following helper functions.
int load_all(FUNC_table *, node *);
// load_func: add a *func* node into the hash table. Returns a nonzero value when encountered an error.
int load_func(FUNC_table *, node *);
// load_args: load the arguments as local variables into the table
int load_args(VAR_table *, node *);
// load_func_blk: a recursive function to go through the *blk* of the function.
//	Returns a nonzero value when encountered an error.
//	the function will be executed one time for each function
int load_func_blk(FUNC_table *, VAR_table *, node *);
// add_local_var: A function to add the declared local variable into the variable table.
int add_local_var(VAR_table *, node *);
// get_type: A function to get the type of the *vdecl* node, as a pointer parameter.
int get_declared_type(node *, char **, char *);

int load_vars(FUNC_table *, node *);

/* Functions still in development */
/* Part I APIs */
int check_vdecl(node *);
/* Part II APIs - dev */
int check_ref_type(node *);
/* Part III APIs */
int check_func_call(FUNC_table *t, char *globid);
/* Part IV APIs */
int check_func_ret_type(FUNC_table *t, char *globid);
/* Part V APIs */
int check_ref_var_init(node *, VAR_table *t);
/* Others */
int declare_local_vars(FUNC_table *, char *, char *, node *);
int check_valid_vars(FUNC_table *t, char *globid);

/* Functions that have been implemented */
/* Part VI APIs */
int check_run(FUNC_table *);


/* Directly calls from main function */
void parse_tree(char *, node **);
void process_tree(node *);
void emit_ast(char *, char *, node *);



#endif
