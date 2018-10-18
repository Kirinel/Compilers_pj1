#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

node *add_node(node *left, int type, node *right, char *name) {
  node* n = calloc(1, sizeof(node));
  n->type = type;
  n->left = left;
  n->right = right;
	strcpy(n->name, name);
  return n;
}

node *add_lit(int value) {
  node *n = calloc(1, sizeof(node));
  n->type = T_LIT;
  n->value = value;
	strcpy(n->name, "lit");
  return n;
}

node *add_var(char *name) {
	node *n = calloc(1, sizeof(node));
	n->type = T_ID;
	n->str = strdup(name);
	strcpy(n->name, "varval");
	return n;
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
	switch(n->type) {
		case T_STMT:
			put_str("", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_BINOP:
			put_str("name", n->name, dent);
			print_ast(n->left, dent);
			break;
		case T_BOP:
			put_str("op", n->name, dent);
			put_str("", "lhs", dent);
			print_ast(n->left, dent+1);
			put_str("", "rhs", dent);
			print_ast(n->right, dent+1);
			break;
		case T_LIT:
			put_str("name", n->name, dent);
			put_strn("value", n->value, dent);
			break;
		case T_ASSIGN:
			put_str("name", n->name, dent);
			print_ast(n->left, dent);
			break;
		case T_VAR:
			print_ast(n->left, dent);
			break;
		case T_ID:
			put_str("var", n->str, dent);
			break;


	}

	// Case of expressions
	//put_indent(dent);
	//printf("name: %s\n", n->name);
	/*
	if (n->left != NULL){
		put_indent(dent);
		printf("lhs:\n");
		print_ast(n->left, dent+1);
	}
	if (n->right != NULL){
		put_indent(dent);
		printf("rhs:\n");
		print_ast(n->right, dent+1);
	}
*/

}
