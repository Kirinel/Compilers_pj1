#ifndef LLVM_H
#define LLVM_H

#include "def.h"

/* The Main workfunction to generate LLVM IR
 *  compile_llvm
 *
 *  args:
 *    (Node): n, the rootnode
 *    (char *): out, the output file
 *    (int): if need to write the llvm IR to file
 *  returns:
 *    void
 *  others:
 *    use JIT to compile the code
 */
void compile_llvm(Node *n, char *out, int write_to_file, int optimizer, double *cl_args, int cl_len);

#endif
