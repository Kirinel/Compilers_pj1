#ifndef TOOLS_H
#define TOOLS_H

#include "ast.h"
#include "symbol_table.h"
#include "llvm.h"

#define VDECL_NO_REF    (1)
#define REF_NO_REF_VOID (2)
#define FUNC_NO_DECL    (3)
#define FUNC_RET_REF    (4)
#define REF_INIT_VAR    (5)
#define NO_FUNC_RUN     (6)

/* emit_ast: write out the ast to file
 */
void emit_ast(char *in, char *out, Node *rootnode);

/* fill_exp_type: Fill all the expressions in the AST with types (no ref type)
 *  This is a recursive function.
 *		args:
 *			n: One node of the AST
 *			ft: The pointer to the function table
 *			vt: The pointer to the variable table
 *		returns:
 *			the (value) type of the node (as enum exp_type)
 */
enum exp_type fill_exp_type(Node *n, FUNC_table *ft, VAR_table **vt, unsigned int l);

/* scan_AST: a recursive function to reach from *prog* to *extern* and *func*,
 * 	invokes the following helper functions.
 * 	returns a nonzero value when encountered an error.
 *
 *	args: Node tree
 *	returns: void
 */
void scan_AST(FUNC_table *t, Node *n);

/* add_func: add a function from the source file to the symbol table
 *	args:
 *		t: A pointer to the function table
 *		n: An AST node
 *	returns:
 *		error code:
 *			FUNC_RET_REF: The return type of the function cannot be ref type
 */
void scan_func(FUNC_table *t, Node *n);

/* verify_type: verify the given type node is not ref ref or ref void
 *	args:
 *		n: Node <type>
 *	Use recursion to check the case of ref ref, and ref void
 */
void verify_type(Node *n);

/* verify_vdecl: verify the given node is <vdecl> is valid
 *	args: a node <vdecl>
 */
void verify_vdecl(Node *n);

/* verify_vdeclstmt: verify the given node <vdeclstmt> is valid*/
void verify_vdeclstmt(Node *n, VAR_table *t, unsigned int level);

/* scan_tdecls: scan the type declaration of external functions
 * 	args:
 *		node <tdecls> or <type>
 *	Use recursion to check all the type.
 */
void scan_tdecls(Node *n);

/* scan_args: scan the arguments into the local variable table
 *	args:
 *		a node vdecls
 */
void scan_args(VAR_table *t, Node *n);

/* scan_fblk: a recursive function to go through the *blk* of the function.
 * 	args:
 *		ft: function table pointer
 *		vt: variable table pointer
 *		n: Node n
 * purpose: load local variables into the table
 *					and filter invalid function calls
 */
void scan_fblk(FUNC_table *ft, VAR_table *vt, Node *n, unsigned int level);

/* check_run: The function to find if the run function has been correctly defined.
  	raise errors and exit the program within this function.
 */
void check_run(FUNC_table *t);

// process_tree: The entry point of this file.
//	will do the 6 checks needed.
void process_tree(Node *root);

#endif
