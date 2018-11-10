#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/tools.h"


int main(int argc, char *argv[]) {
	//dependencies of optget
	extern char *optarg;
	extern int optind;
	extern int opterr;
	extern int optopt;

	int c, hflag=0, vflag=0, Oflag=0, llvmflag=0, astflag=0, outflag=0;
	char *inf=argv[argc-1], *outf=NULL;

	while ((c = getopt(argc, argv, "h?vOe:o:")) != -1) {
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
			case 'o':
				outflag = 1;
				outf = optarg;
				break;
			default:
				printf("Unknown Option: %c", (char) optopt);
				break;
		}
	}

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
		printf("Optimization is currently disabled.\n");

	if (llvmflag)
		printf("llvm mode is currently disabled.\n");

	if (!outflag) {
		fprintf(stderr, "\nError: The output filename must be specified!\nExiting the program...\n");
		exit(-1);
	}

	if (!strcmp(inf, outf)) {
		fprintf(stderr, "Error: The input filename must be specified!\nExiting the program...\n");
		exit(-1);
	}

	//Generate the tree on the stack
	node *root;
	parse_tree(inf, &root);

	process_tree(root);

	if (astflag)
		emit_ast(inf, outf, root);
	else
		printf("Exiting the Program...\n");

	/* House Keeping */
	free_node(root);

	// Exit
	return 0;
}
