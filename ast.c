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

//add RETURN node

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
	if (strlen(str1)==0) printf("%s\n", str2);
	else printf("%s%s\n", str1, str2);
}

void put_strn(char *str, int value, int dent) {
	put_indent(dent);
	printf("%s: %d\n", str, value);
}

void print_ast(node *n, int dent) {
	// Case of Constants
	switch (n->tag) {
		case T_PROG: //name:prog
			put_str("name: ", n->name, dent);
			put_str("", "funcs:", dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_EXTERNS: //name: externs
			if(n->left->tag == T_EXTERNS) {
				if (n->left != NULL) print_ast(n->left, dent);
				if (n->right != NULL) print_ast(n->right, dent);
				break;
			}
			else if(n->left->tag == T_EXTERN) {
				put_str("", "externs:", dent);
				put_str("name: ", n->name, dent+1);
				put_str("", "externs:", dent+1);
				if (n->left != NULL) print_ast(n->left, dent+1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}

		case T_EXTERN: //name: extern
			put_str("name: ", n->name, dent);
			if (n->type != NULL) {
				put_indent(dent);
				printf("ret_");
				print_ast(n->type, 0);
			}
			if (n->globid != NULL) print_ast(n->globid, dent);
			if (n->tdecls != NULL) print_ast(n->tdecls, dent);
			break;
		case T_FUNCS: //name: funcs
			if(n->left->tag == T_FUNC) {
				put_str("name: ", n->name, dent);
				put_str("", "funcs:", dent);
				put_str("", "-", dent+1);
				if (n->left != NULL) print_ast(n->left, dent+1);
				//if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else {
				if (n->left != NULL) print_ast(n->left, dent);
				put_str("", "-", dent);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
		case T_FUNC: //name: func
			put_str("name: ", n->name, dent);
			put_str("ret_type: ",n->type->str, dent);
			// if (n->type != NULL) print_ast(n->type, dent+1);
			if (n->globid != NULL) print_ast(n->globid, dent);
			put_str("", "blk:", dent);
			if (n->blk != NULL) print_ast(n->blk, dent+1);
			if (n->vdecls != NULL) print_ast(n->vdecls, dent);
			break;
		case T_BLK: //name: blk
			put_str("name: ", n->name, dent);
			put_str("", "contents:", dent);
			//contents:
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_STMTS: //name:stmts
			if(n->left->tag == T_STMT) {
				put_str("name: ", n->name, dent);
				put_str("", "stmts:", dent);
				put_str("", "-",dent+1);
				if (n->left != NULL) print_ast(n->left, dent+1);
				// if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else {
				if (n->left != NULL) print_ast(n->left, dent);

				if (n->right != NULL) {
					put_str("", "-",dent);
					print_ast(n->right, dent+1);
				}
				break;
			}
		case T_STMT: //name: stmt
			//name == "if", should add "cond:\n" and else_stmt

			if(strcmp(n->name, "return") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("", "name: ret", dent);
				put_str("", "exp: ",dent);
				if (n->left != NULL) print_ast(n->left, dent+1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else if(strcmp(n->name, "vardeclstmt") == 0) {
				put_str("name: ", n->name, dent);
				put_str("", "vdecl:", dent);
				if (n->left != NULL) print_ast(n->left, dent+1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}

			else if(strcmp(n->name, "while") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("name: ", n->name, dent);
				put_str("", "cond:", dent);
				if (n->left != NULL) print_ast(n->left, dent+1);
				put_str("", "stmt:", dent);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else if(strcmp(n->name, "if") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("name: ", n->name, dent);
				put_str("", "cond:", dent);
				if (n->left != NULL) print_ast(n->left, dent+1);
				put_str("", "stmt:", dent);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else if(strcmp(n->name, "if_stmt") == 0) {
				if (n->left != NULL) print_ast(n->left, dent);
				put_str("", "else_stmt:", dent-1);
				if (n->right != NULL) print_ast(n->right, dent);
				break;
			}
			else if(strcmp(n->name, "print") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("name: ", n->name, dent);
				put_str("", "exp:", dent);
				if (n->left != NULL) print_ast(n->left, dent+1);
				break;
			}
			else if(strcmp(n->name, "printslit") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("name: ", n->name, dent);
				if (n->left != NULL) print_ast(n->left, dent);
				if (n->right != NULL) print_ast(n->right, dent);
				break;
			}
			else if(strcmp(n->name, "expstmt") == 0){
				put_str("name: ", n->name, dent);
				put_str("", "exp:", dent);
				if (n->left != NULL) print_ast(n->left, dent +1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}


		case T_EXPS: // name:exps
			put_str("name: ", n->name, dent);
			put_str("", "exps:", dent);
			put_str("", "-", dent+1);
			if (n->left != NULL) print_ast(n->left, dent+2);
			if (n->right != NULL) print_ast(n->right, dent+2);
			break;
		case T_EXP: //name: exp
			if(strcmp(n->name, "exp") == 0) {
				put_str("", "exp:", dent);
				if (n->left != NULL) print_ast(n->left, dent+1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else if(strcmp(n->name, "binop") == 0 || strcmp(n->name, "uop") == 0) {
				//printf("11111\n");
				if (n->left != NULL) print_ast(n->left, dent);
				break;
			}
			else if(strcmp(n->name, "var") == 0) {
				// put_str("var: ", n->str, dent);
				// printf("222222\n");
				if (n->left != NULL) print_ast(n->left, dent);
				// printf("3333333\n");
				break;
			}
			else if(strcmp(n->name, "globid") == 0) {
				put_str("", "exp:", dent);
				put_str("","name: funccall", dent+1);
				if (n->left != NULL) print_ast(n->left, dent+1);
				put_str("", "params:", dent+1);
				if (n->right != NULL) print_ast(n->right, dent+2);
			break;
		}

		case T_BINOP: //name: binop
			if(strcmp(n->name, "assign") == 0) {
				put_str("name: ", n->name, dent);
				put_str("var: ", n->left->str, dent);
				put_str("", "exp:", dent);
				//if (n->left != NULL) print_ast(n->left, dent);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else {
				put_str("", "name: binop", dent);
				put_str("op: ", n->name, dent);
				// printf("BINOP, n->name: %s\n",n->name );
				put_str("", "lhs: ", dent);

				if (n->left != NULL) {
					// printf("%s %d\n", n->left->name, T_VAR);
					if(strcmp(n->left->name, "var") == 0) {
						put_str("", "name: varval", dent+1);
					}
					print_ast(n->left, dent+1);
				}
				put_str("", "rhs: ", dent);
				if (n->right != NULL) {
					if(strcmp(n->right->name, "var") == 0) {
						put_str("", "name: varval", dent+1);
					}
					print_ast(n->right, dent+1);
				}
				break;
			}

		case T_UOP: //name: uop
			put_str("op: ", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_SLIT: //name: slit
			put_str("string: ", n->str, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_LIT: //name: lit
			put_str("name: ", n->name, dent);
			put_indent(dent);
			printf("value: %.0lf\n", n->val);
			//if (n->left != NULL) print_ast(n->left, dent+1);
			//if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_VAR: //name: var
			//put_str("", "name: varval", dent);
			put_str("var: ", n->str, dent);
			//if (n->left != NULL) print_ast(n->left, dent+1);
			//if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_ID: //name: identifier
			put_str("name: ", n->str, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_GLOB: //name: globid
			put_str("globid: ", n->str, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_TYPE: //name: type
			if(strcmp(n->name, "ref") == 0) {
				// put_indent(dent);
				// printf("type: %s %s\n", n->name, n->left->name);
				if (n->left != NULL) print_ast(n->left, dent+1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else if(strcmp(n->name, "noalias ref") == 0) {
				//put_indent(dent);
				//printf("type: %s %s\n", n->name, n->left->str);
				if (n->left != NULL) print_ast(n->left, dent+1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else {
				put_str("type: ", n->str, dent);
				if (n->left != NULL) print_ast(n->left, dent+1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}

		case T_VDECLS: //name: vdecls
			put_str("", "vdecls:", dent);
			put_str("name: ", n->name, dent+1);
			put_str("", "vars:", dent+1);
			put_str("", "-", dent+2);
			if (n->left != NULL) print_ast(n->left, dent+3);
			if (n->right != NULL) print_ast(n->right, dent+3);
			break;
		case T_TDECLS: //name: tdecls
			put_str("", "tdecls:", dent);
			put_str("name: ", n->name, dent+1);
			put_str("", "types: ", dent+1);
			put_str("", "-", dent+2);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;
		case T_VDECL: //name: vdecl
			put_str("node: ", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent);
			if (n->right != NULL) print_ast(n->right, dent);
			break;
		// case T_RETURN

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
