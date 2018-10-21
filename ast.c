#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* function to generate nodes */
node *add_node(int tag, char *name, char *str, double val, node *left, node *right, node *type, node *globid, node *tdecls, node *vdecls, node *blk)
{
	node *n = calloc(1, sizeof(node));
	n->tag = tag;
	strcpy(n->name, name);

	switch (tag) {
		case T_EXTERN:
			n->type = type;
			n->globid = globid;
			n->tdecls = tdecls;
			break;
		case T_FUNC:
			n->type = type;
			n->globid = globid;
			n->vdecls = vdecls;
			n->blk = blk;
			break;
		case T_LIT:
			n->val = val;
			break;
		case T_SLIT:
		case T_VAR:
		case T_GLOB:
		case T_TYPE:
			n->str = strdup(str);
			break;
		default:
			n->left = left;
			n->right = right;
		}

		return n;
}

/* create a new AST node */
node *add_astnode(int tag, char *name, node *left, node *right)
{
	return add_node(tag, name, "", 0, left, right, NULL, NULL, NULL, NULL, NULL);
}
/* create a function node */
node *add_func(node *type, node *globid, node *vdecls, node *blk)
{
	return add_node(T_FUNC, "func", "", 0, NULL, NULL, type, globid, NULL, vdecls, blk);
}
/* create an extern node */
node *add_extern(node *type, node *globid, node *tdecls)
{
	return add_node(T_EXTERN, "extern", "", 0, NULL, NULL, type, globid, tdecls, NULL, NULL);
}
/* create a lit node */
node *add_lit(double val)
{
	return add_node(T_LIT, "lit", "", val, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
/* create a slit node */
node *add_slit(char *str)
{
	return add_node(T_SLIT, "slit", str, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
/* create a variable identifier node */
node *add_var(char *name)
{
	return add_node(T_VAR, "varval", name, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
/* create a function identifier node */
node *add_globid(char *name)
{
	return add_node(T_GLOB, "globid", name, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
/* create a type node */
node *add_type(char *name) {
	return add_node(T_TYPE, "type", name, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

void put_indent(int dent) {
	for (int i=0; i<dent; ++i)
		printf("  ");
}

void put_op(char *op, int dent) {
	put_indent(dent);
	printf("op: %s\n", op);
}

void put_str(char *str1, char *str2, int dent) {
	put_indent(dent);
	if (strlen(str1)==0) printf("%s:\n", str2);
	else printf("%s: %s\n", str1, str2);
}

void put_strn(char *str, int value, int dent) {
	put_indent(dent);
	printf("%s: %d\n", str, value);
}

void print_ast(node *n, int dent) {
	// Case of Constants
	switch (n->tag) {
		case T_PROG:
			put_str("name: prog\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_EXTERNS:
			put_str("name: externs\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_EXTERN:
			put_str("name: extern\t", n->name, dent);
			if (n->type != NULL) print_ast(n->type, dent+1);
			if (n->globid != NULL) print_ast(n->globid, dent+1);
			if (n->tdecls != NULL) print_ast(n->tdecls, dent+1);
			break;
		case T_FUNCS:
			put_str("name: funcs\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_FUNC:
			put_str("name: func\t", n->name, dent);
			if (n->type != NULL) print_ast(n->type, dent+1);
			if (n->globid != NULL) print_ast(n->globid, dent+1);
			if (n->vdecls != NULL) print_ast(n->vdecls, dent+1);
			if (n->blk != NULL) print_ast(n->blk, dent+1);
			break;
		case T_BLK:
			put_str("name: blk\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_STMTS:
			put_str("name: stmts\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_STMT:
			put_str("name: stmt\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_EXPS:
			put_str("name: exps\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_EXP:
			put_str("name: exp\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_BINOP:
			put_str("name: binop\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_UOP:
			put_str("name: uop\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_SLIT:
			put_str("name: slit\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_LIT:
			put_str("name: lit\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_VAR:
			put_str("name: var\t", n->str, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_ID:
			put_str("name: identifier\t", n->str, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_GLOB:
			put_str("name: globid\t", n->str, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_TYPE:
			put_str("name: type\t", n->str, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_VDECLS:
			put_str("name: vdecls\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_TDECLS:
			put_str("name: tdecls\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_VDECL:
			put_str("name: vdecl\t", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		default:
			printf("unrecognized\n");
			break;
		}

		return;
}

void free_node(node *n) {
	if (n==NULL) return;

	free_node(n->left);
	free_node(n->right);
	free_node(n->type);
	free_node(n->globid);
	free_node(n->tdecls);
	free_node(n->vdecls);
	free_node(n->blk);
	free(n);
	return;
}
