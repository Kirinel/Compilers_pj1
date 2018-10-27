#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bison.h"
#include "flex.h"
#include "ast.h"

void emit_ast(char *in, char *out) {
	//printf("%s %s\n", in, out);
	yyin = fopen(in, "r");

	FILE *outfile = fopen(out, "w");

	fseek(yyin, 0L, SEEK_END);
	int sz = ftell(yyin);
	if (sz < 1) {
		fprintf(stderr, "Error: Input file cannot be empty!\n");
		exit(-1);
	}
	fseek(yyin, 0L, SEEK_SET);

	if (yyin == NULL) {
		puts("No input files given");
		printf("\n>>> ");
	}
	node *rootnode;

	/* parse the given source file */
	yyparse(&rootnode);


	/* print out the AST */
	fprintf(outfile, "---\n");
	print_ast(rootnode, 0, outfile);
	fprintf(outfile, "...\n");

	/* House Keeping */
	free_node(rootnode);

	//fclose(yyin);
	fclose(outfile);

	return;
}

int main(int argc, char *argv[]) {

	if (argc < 4) {
		fprintf(stderr, "Incorrect number of commandline arguments!\n");
		exit(-1);
	}

	char *input = argv[argc-1];
	char *output = argv[argc-2];
	char *o = argv[argc-3];
	if (strcmp(o, "-o") != 0) {
		fprintf(stderr, "Incorrect commandline argument!\n");
		exit(-1);
	}

	emit_ast(input, output);

	return 0;
}
