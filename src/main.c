#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <llvm-c/Core.h>
#include "tools.h"
#include "../temp/bison.h"
#include "../temp/flex.h"

// parse_tree: parse the given input using Bison
void parse_tree(char *in, Node **root_p)
{
	yyin = fopen(in, "r");

  if (yyin == NULL) {
    puts("Error: No input files given");
    exit(-1);
  }

	fseek(yyin, 0L, SEEK_END);
	int sz = ftell(yyin);
	if (sz < 1) {
		fprintf(stderr, "Error: Input file cannot be empty!\n");
		return;
	}
	fseek(yyin, 0L, SEEK_SET);

	/* parse the given source file */
	int err = yyparse(root_p);
	if (err != 0)	{
		fprintf(stderr, "Error: Wrong Syntax!\n");
		exit(-1);
	}

	return;
}


int main(int argc, char *argv[]) {
	//dependencies of optget
	extern char *optarg;
	extern int optind;
	extern int opterr;
	extern int optopt;

	int c, hflag=0, vflag=0, Oflag=0, llvmflag=0, astflag=0, outflag=0, jitflag=0;
	char *inf=NULL, *outf=NULL;

	while ((c = getopt(argc, argv, "h?vOe:o:j:")) != -1) {
		switch (c) {
			//printf("optind: %d\n", optind);
			case 'h':
			case '?':
				hflag = 1;
				break;
			case 'v':
				vflag = 1;
				break;
			case 'O':
				Oflag = 1;
				break;
			case 'e':
				if (!strcmp(optarg, "mit-ast"))
					astflag = 1;
					//printf("emit-ast\n");
				if (!strcmp(optarg, "mit-llvm")) {
					llvmflag = 1;
					//printf("activated\n");
				}
				break;
			case 'j':
				if (!strcmp(optarg, "it"))
					jitflag = 1;
				break;
			case 'o':
				outflag = 1;
				outf = optarg;
				inf = argv[optind];
				// printf("%d %s %s\n", optind, argv[optind-1], argv[optind]);
				break;
			default:
				printf("Unknown Option: %c", (char) optopt);
				break;
		}
	}

	// Generate the commandline arguments
	double *cl_args = NULL;
	int len_args = argc-optind-1;
	for (int i=0; i<len_args; ++i) {
			cl_args = realloc(cl_args, (i+1)*sizeof(double));
			cl_args[i] = atof(argv[i+optind+1]);
	}

	// check the commadline arguments are generated correctly
	// for (int i=0; i<len_args; ++i) {
	// 	printf("%f\n", cl_args[i]);
	// }

	// printf("args: %d, %s, total:%d\n", optind, argv[optind], argc);
	//printf("%s %s\n", inf, outf);

	if (hflag) {
		printf("The Extended-Kaleidoscope Compiler (Beta v-0.3.1)\n");
		printf("Authors: Chuanqi Yan & Jiang Wang\n");
		printf("Help information of ");
		printf("commandline arguments:\n");
		printf("./bin/ekcc [-h|-?] [-v] [-O] [-emit-ast|-emit-llvm] -o <output-file> <input-file>\n");
		printf("-h | -? : Show help information\n");
		printf("-v : Verbose mode\n");
		printf("-O : Enables optimizations\n");
		printf("-emit-ast : Output file will contain the serialized format for the AST\n");
		printf("-emit-llvm : LLVM IR will be produced (unoptimized, unless -O is provided)\n");
		printf("-o <output-file> : Names the output file (required)\n");
		printf("<input-file> : Names in input source code (required)\n");
	}

	if (vflag)
		printf("Verbose mode is currently disabled.\n");

	if (Oflag)
		printf("Optimization (level 3) is enabled.\n");

	if (!outflag) {
		fprintf(stderr, "\nError: The output filename must be specified!\nExiting the program...\n");
		exit(-1);
	}

	if (!strcmp(inf, outf)) {
		fprintf(stderr, "Error: The input filename must be specified!\nExiting the program...\n");
		exit(-1);
	}

	//Generate the tree on the stack
	Node *root;
	parse_tree(inf, &root);
	process_tree(root);

	// emit ast
	if (astflag)
		emit_ast(inf, outf, root);

	// emit llvm
	if (jitflag)
		// emit llvm
		if (llvmflag)
			// Compile the whole program
			compile_llvm(root, outf, 1, Oflag, cl_args, len_args);
		else
			compile_llvm(root, "./testcases/test.ll", 0, Oflag, cl_args, len_args);
	else
		fprintf(stderr, "Warning: only jit option is enabled for this program.\n");

	if (llvmflag)
		printf("Exporting LLVM IR......\n");

	/* House Keeping */
	free_ast(root);
	free(cl_args);

	// printf("Exiting the Program...\n");

	// Exit
	return 0;
}
