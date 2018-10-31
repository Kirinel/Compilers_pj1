#include <stdio.h>
#include <stdlib.h>
#include "../temp/bison.h"
#include "../temp/flex.h"
//#include "symbol_table.h"
#include "tools.h"

#define TABLESIZE 32

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

int load_funcs(FUNC_table *t, node *n)
{
	if (n==NULL)	return 0;
	int err;

	switch(n->tag) {
		case T_PROG:
			err = load_funcs(t, n->right);
			err = load_funcs(t, n->left) + err;
			break;
		case T_FUNCS:
		case T_EXTERNS:
			err = load_funcs(t, n->left);
			err = load_funcs(t, n->right) + err;
			break;
		case T_EXTERN:
		case T_FUNC:
			//printf("%s\n", n->globid->str);
			err = add_table_func(t, n->globid->str, n->type->str, n);
			break;
		default:
			fprintf(stderr, "Encounter unknown node: %s", n->name);
	}
	//printf("%d\n", err);
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

	return err;
}


void process_tree(node *root)
{
	int err;
	FUNC_table *f_table = init_func_table(TABLESIZE);
	//Load all the functions into the hash table
	err = load_funcs(f_table, root);
	//Check if the run function exists and has the correct return type;
	err = check_run(f_table) && err;
	printf("%d\n", err);

	free_func_table(f_table);


	if (err) exit(-1);
}
