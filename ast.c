#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

/* function to generate nodes */
node *add_node(int tag, char *name, char *str, double val, node *left, node *right, node *type, node *globid, node *tdecls, node *vdecls, node *blk, node *expr, node *stmt1, node *stmt2)
{
	node *n = calloc(1, sizeof(node));
	n->tag = tag;
	strcpy(n->name, name);

	switch (tag) {
		// special node extern
		case T_EXTERN:
			n->type = type;
			n->globid = globid;
			n->tdecls = tdecls;
			break;
		// special node func
		case T_FUNC:
			n->type = type;
			n->globid = globid;
			n->vdecls = vdecls;
			n->blk = blk;
			break;
		// special node number
		case T_LIT:
			n->val = val;
			break;
		// special nodes: name stored in n->str
		case T_SLIT:
		case T_VAR:
		case T_GLOB:
		case T_TYPE:
			n->str = strdup(str);
			n->left = left;
			break;
		// a normal node
		case T_OPEN_STMT:
		case T_CLOSED_STMT:
			n->expr = expr;
			n->stmt1 = stmt1;
			n->stmt2 = stmt2;
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
	return add_node(tag, name, "", 0, left, right, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
/* create a function node */
node *add_func(node *type, node *globid, node *vdecls, node *blk)
{
	return add_node(T_FUNC, "func", "", 0, NULL, NULL, type, globid, NULL, vdecls, blk, NULL, NULL, NULL);
}
/* create an extern node */
node *add_extern(node *type, node *globid, node *tdecls)
{
	return add_node(T_EXTERN, "extern", "", 0, NULL, NULL, type, globid, tdecls, NULL, NULL, NULL, NULL, NULL);
}
/* create a open/closed statement node */
node *add_ocstmt(int tag, char *name, node *expr, node *stmt1, node *stmt2)
{
		return add_node(tag, name, "", 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, expr, stmt1, stmt2);
}
/* create a lit or flit node */
node *add_lit(double val)
{
	//fprintf(stderr, "%f\n", val);
	if (val == (int)val)
		return add_node(T_LIT, "lit", "", val, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	else
		return add_node(T_LIT, "flit", "", val, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

/* create a slit node */
node *add_slit(char *str)
{
	return add_node(T_SLIT, "slit", str, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
/* create a variable identifier node */
node *add_var(char *varname)
{
	return add_node(T_VAR, "varval", varname, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
/* create a function identifier node */
node *add_globid(char *globname)
{
	return add_node(T_GLOB, "globid", globname, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}
/* create a type node */
node *add_type(char *typename)
{
	return add_node(T_TYPE, "type", typename, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
}

//add RETURN node

// put_indent: generate the indent based on the input
void put_indent(int dent)
{
	for (int i=0; i<dent; ++i)
		printf("  ");
}

// print out the string by the given indent
void put_str(char *str1, char *str2, int dent)
{
	put_indent(dent);
	if (strlen(str1)==0) printf("%s\n", str2);
	else printf("%s%s\n", str1, str2);
}

// print out the string (value) by the given indent
void put_strn(char *str, int value, int dent)
{
	put_indent(dent);
	printf("%s: %d\n", str, value);
}

// print out the type declaration of the the given type node
void print_typedec(node *n)
{
	assert(n->tag == T_TYPE);
	//none terminal node
	//printf("%s\n", n->name);
	if (strcmp(n->name, "type") != 0) {
		printf("%s ", n->name);
		print_typedec(n->left);
	} else {
		printf("%s\n", n->str);
	}
	//printf("%s %s %d\n", n->name, n->str, n->left);
	return;
}

// print out the type declaration of the given var node
void print_varname(node *n, int dent)
{
	assert(n->tag == T_VAR);
	put_str("var: ", n->str, dent);
	return;
}

// print_ast: generate the AST and write it to STDIN
void print_ast(node *n, int dent)
{
	// Case of Constants
	switch (n->tag) {

		case T_PROG: //name:prog
			put_str("name: ", n->name, dent);
			put_str("", "funcs:", dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent); //extern no indent
			break;

		case T_EXTERNS: //name: externs
			//i) the recursive case
			if(n->left->tag == T_EXTERNS) {

				if (n->left != NULL) 	print_ast(n->left, dent);

				if (n->right != NULL) {
					put_str("", "-", dent+2);
					print_ast(n->right, dent+3);
				}
				break;
			}
			//ii) the terminal case
			//		last externs to recurse, put the header
			//		recurse on left only
			else if(n->left->tag == T_EXTERN) {
				put_str("", "externs:", dent);
				put_str("name: ", n->name, dent+1);
				put_str("", "externs:", dent+1);
				put_str("", "-", dent+2);
				if (n->left != NULL) print_ast(n->left, dent+3);
				//if (n->right != NULL) print_ast(n->right, dent+1);
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

		case T_TDECLS: //name: tdecls
			//Since it is a recursive type, use similar methods to print
			//i)terminal case
			if(n->left->tag == T_TYPE) {
				put_str("", "tdecls:", dent);
				put_str("name: ", n->name, dent+1);
				put_str("", "types: ", dent+1);

				if (n->left != NULL) {
					put_indent(dent+2);
					printf("- ");
					print_typedec(n->left);
				}
				break;
			}
			//ii) recursive case
			else if(n->left->tag == T_TDECLS) {
				if (n->left!= NULL) print_ast(n->left, dent);
				//printf("activated\n");
				if (n->right!= NULL) {
					put_indent(dent+2);
					printf("- ");
					print_typedec(n->right);
				}
				break;
			}

		case T_FUNCS: //name: funcs
			if(n->left->tag == T_FUNC) {
				put_str("name: ", n->name, dent);
				put_str("", "funcs:", dent);
				put_str("", "-", dent+1);
				if (n->left != NULL) print_ast(n->left, dent+2);
				//if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else {
				if (n->left != NULL) print_ast(n->left, dent);
				put_str("", "-", dent+1);
				if (n->right != NULL) print_ast(n->right, dent+2);
				break;
			}

		case T_FUNC: //name: func
			put_str("name: ", n->name, dent);
			put_str("ret_type: ",n->type->str, dent);
			// if (n->type != NULL) print_ast(n->type, dent+1);
			if (n->globid != NULL) print_ast(n->globid, dent);
			put_str("", "blk:", dent);
			if (n->blk != NULL) print_ast(n->blk, dent+1);
			//printf("activated!\n");
			if (n->vdecls != NULL) print_ast(n->vdecls, dent);
			//printf("activated!\n");
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
				put_str("", "-", dent+1);
				if (n->left != NULL) print_ast(n->left, dent+2);
				// if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else {
				if (n->left != NULL) print_ast(n->left, dent);

				if (n->right != NULL) {
					put_str("", "-", dent+1);
					print_ast(n->right, dent+2);
				}
				break;
			}

		case T_STMT: //name: stmt
			// recurse with no change
			if (n->left != NULL) print_ast(n->left, dent);
			break;

		case T_OPEN_STMT: //name: open_stmt
			//case "if"
			if(strcmp(n->name, "if") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("name: ", n->name, dent);
				put_str("", "cond:", dent);
				if (n->expr != NULL) print_ast(n->expr, dent+1);
				put_str("", "stmt:", dent);
				if (n->stmt1 != NULL)	print_ast(n->stmt1, dent+1);

				// check if the second statement exists
				if (n->stmt2 != NULL)	{
					put_str("", "else_stmt:", dent);
					print_ast(n->stmt2, dent+1);
				}
				break;
			}
			//case "while"
			else if(strcmp(n->name, "while") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("name: ", n->name, dent);
				put_str("", "cond:", dent);
				if (n->expr != NULL) print_ast(n->expr, dent+1);
				put_str("", "stmt:", dent);
				if (n->stmt1 != NULL) print_ast(n->stmt1, dent+1);
				break;
			}

		case T_CLOSED_STMT: //name: closed_stmt
			//case "if"
			if(strcmp(n->name, "if") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("name: ", n->name, dent);
				put_str("", "cond:", dent);
				if (n->expr != NULL) print_ast(n->expr, dent+1);
				put_str("", "stmt:", dent);
				if (n->stmt1 != NULL)	print_ast(n->stmt1, dent+1);

				// check if the second statement exists
				if (n->stmt2 != NULL)	{
					put_str("", "else_stmt:", dent);
					print_ast(n->stmt2, dent+1);
				}
				break;
			}
			//case "while"
			else if(strcmp(n->name, "while") == 0) {
				// put_str("", "-", dent);
				// dent++;
				put_str("name: ", n->name, dent);
				put_str("", "cond:", dent);
				if (n->expr != NULL) print_ast(n->expr, dent+1);
				put_str("", "stmt:", dent);
				//printf("activated!\n");
				if (n->stmt1 != NULL) print_ast(n->stmt1, dent+1);
				break;
			}
			//case "simple", proceed with no change
			else if (strcmp(n->name, "simple") == 0) {
				//printf("activated!\n");
				if (n->stmt1 != NULL) print_ast(n->stmt1, dent);
				break;
			}

		case T_SIMPLE_STMT: //name: simple_stmt
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
				put_str("", "exp:", dent);
				if (n->right != NULL) print_ast(n->right, dent+1);//align exp with vardeclstmt
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
				if (n->left != NULL) print_ast(n->left, dent+1);
				if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else if(strcmp(n->name, "blk") == 0) {
				if (n->left != NULL) print_ast(n->left, dent);
				break;
			} else {
				printf("undefined!\n");
				break;
			}

		case T_EXPS: // name: exps
			if(n->left->tag == T_EXP) {
				put_str("name: ", n->name, dent);
				put_str("", "exps:", dent);
				put_str("", "-", dent+1);
				if (n->left != NULL) print_ast(n->left, dent+2);
				if (n->right != NULL) print_ast(n->right, dent+2);
				break;
			}
			else {
				if (n->left != NULL) print_ast(n->left, dent);

				if (n->right != NULL) {
					put_str("", "-", dent+1);
					print_ast(n->right, dent+2);
				}
				break;
			}
			break;

		case T_EXP: //name: exp
			if(strcmp(n->name, "exp") == 0) {
				//put_str("", "exp:", dent);
				if (n->left != NULL) print_ast(n->left, dent);
				//if (n->right != NULL) print_ast(n->right, dent);
				break;
			}
			else if(strcmp(n->name, "binop") == 0 || strcmp(n->name, "uop") == 0) {
				//printf("activated!\n");
				if (n->left != NULL) print_ast(n->left, dent);
				break;
			}
			else if(strcmp(n->name, "var") == 0) {
				// put_str("var: ", n->str, dent);
				if (n->left != NULL) print_ast(n->left, dent);
				break;
			}
			else if(strcmp(n->name, "globid") == 0) {
				//put_str("", "exp:", dent);
				put_str("","name: funccall", dent+1);
				if (n->left != NULL) print_ast(n->left, dent+1);
				put_str("", "params:", dent+1);
				if (n->right != NULL) print_ast(n->right, dent+2);
			break;
			}
			else if (strcmp(n->name, "lit") == 0) {
				//put_str("", "exp:", dent);
				if (n->left != NULL) print_ast(n->left, dent+1);
				break;
			}
			break;

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
					//if(strcmp(n->left->name, "var") == 0) {
					//	put_str("", "name: varval", dent+1);
					//}
					print_ast(n->left, dent+1);
				}
				put_str("", "rhs: ", dent);
				if (n->right != NULL) {
					//if(strcmp(n->right->name, "var") == 0) {
						//put_str("", "name: varval", dent+1);
					//}
					//printf("activated!\n");
					print_ast(n->right, dent+1);
				}
				break;
			}
			break;

		case T_UOP: //name: uop
			put_str("", "name: uop", dent); //newly inserted
			put_str("op: ", n->name, dent);
			put_str("", "exp:", dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			//if (n->right != NULL) print_ast(n->right, dent+1);
			break;

		case T_SLIT: //name: slit
			put_str("string: ", n->str, dent);
			if (n->left != NULL) print_ast(n->left, dent+1);
			if (n->right != NULL) print_ast(n->right, dent+1);
			break;

		case T_LIT: //name: lit
			put_str("name: ", n->name, dent);
			put_indent(dent);
			if (strcmp("lit", n->name) == 0) {
				printf("value: %.0lf\n", n->val);
			} else if (strcmp("flit", n->name) == 0) {
				printf("value: %.6g\n", n->val);
			}
			//if (n->left != NULL) print_ast(n->left, dent+1);
			//if (n->right != NULL) print_ast(n->right, dent+1);
			break;

		case T_VAR: //name: var
			put_str("", "name: varval", dent);
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

		case T_VDECLS: //name: vdecls
			//Since it is a recursive type, use similar methods to vdecls:
			//i)terminal case
			if(n->left->tag == T_VDECL) {
				put_str("", "vdecls:", dent);
				put_str("name: ", n->name, dent+1);
				put_str("", "vars:", dent+1);
				put_str("", "-", dent+2);

				if (n->left != NULL) print_ast(n->left, dent+3);

				break;
			}
			//ii) recursive case
			else if(n->left->tag == T_VDECLS) {
				if (n->left!= NULL) print_ast(n->left, dent);
				//printf("activated\n");
				put_str("", "-", dent+2);
				if (n->right != NULL) print_ast(n->right, dent+3);

				break;
			}
			/*
			put_str("", "vdecls:", dent);
			put_str("name: ", n->name, dent+1);
			put_str("", "vars:", dent+1);
			put_str("", "-", dent+2);
			if (n->left != NULL) print_ast(n->left, dent+3);
			if (n->right != NULL) print_ast(n->right, dent+3);
			break;
			*/
			break;

		case T_VDECL: //name: vdecl
			//fprintf(stderr, "%s\n", n->left->name);

			put_str("node: ", n->name, dent);
			if (n->left != NULL) print_ast(n->left, dent);

			if (n->right != NULL) {
				//fprintf(stderr, "%s\n", n->right->name);
				print_varname(n->right, dent);
			}
			break;
		// case T_RETURN

		case T_TYPE: //name: type
			put_indent(dent);
			printf("type: ");
			print_typedec(n);
			break;
			/*
			if(strcmp(n->name, "ref") == 0) {

				// printf("type: %s %s\n", n->name, n->left->name);
				if (n->left != NULL) print_ast(n->left, dent+1);
				//if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else if(strcmp(n->name, "noalias ref") == 0) {
				put_indent(dent);
				//printf("type: %s %s\n", n->name, n->left->str);
				if (n->left != NULL) print_ast(n->left, dent+1);
				//if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
			else {
				put_str("type: ", n->str, dent);
				//if (n->left != NULL) print_ast(n->left, dent+1);
				//if (n->right != NULL) print_ast(n->right, dent+1);
				break;
			}
		*/

		default:
			printf("unrecognized\n");
			break;
		}

		return;
}

// free_node: free the AST generated by bison
void free_node(node *n)
{
	if (n==NULL) return;

	free_node(n->left);
	free_node(n->right);
	free_node(n->type);
	free_node(n->globid);
	free_node(n->tdecls);
	free_node(n->vdecls);
	free_node(n->blk);
	free_node(n->expr);
	free_node(n->stmt1);
	free_node(n->stmt2);
	free(n);
	return;
}
