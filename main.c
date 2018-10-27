#include <stdio.h>
#include "bison.h"
#include "flex.h"
#include "ast.h"

int main(int argc, char **argv) {

	yyin = fopen(argv[1], "r");

	if (yyin == NULL) {
		puts("No input files given");
		printf("\n>>> ");
	}
	node *rootnode;

	/* parse the given source file */
	yyparse(&rootnode);


	/* print out the AST */
	printf("---\n");
	print_ast(rootnode, 0);
	printf("...\n");

	/* House Keeping */
	free_node(rootnode);
}
