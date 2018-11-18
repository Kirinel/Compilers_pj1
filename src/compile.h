#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include "def.h"

#define UNKNOWN_RET_TYPE (21)

// Variable entry
typedef struct v_entry {
  char *varname;
  LLVMValueRef reference;
  char is_ref;
  unsigned int level;
  struct v_entry *next;
} V_entry;

// Variable table
typedef struct v_table {
  size_t num_bucs;
  char *funcname;
  V_entry **bucs;
} V_table;

// Function entry
typedef struct f_entry {
  char *funcname;
  LLVMValueRef reference;
  V_table *vtable;
  struct f_entry *next;
} F_entry;

// Function table
typedef struct f_table {
  size_t num_bucs;
  F_entry **bucs;
} F_table;

/* Function table APIs */
F_table *init_ftable();
void free_ftable(F_table *);
F_entry *new_fentry(char *, LLVMValueRef);
int add_fentry(F_table *, char *, LLVMValueRef);
int find_fentry(F_table *, char *, LLVMValueRef *);
int find_vtable(F_table *, char *, V_table **);
/* Variable table APIs */
V_table *init_vtable(char *);
void free_vtable(V_table *);
V_entry *new_ventry(char *, LLVMValueRef, char, unsigned int);
int add_ventry(V_table *, char *, LLVMValueRef, char, unsigned int);
int find_ventry(V_table *, char *, unsigned int, V_entry **);
void destroy_ventry_bylevel(V_table *t, unsigned int);


// typedef int (*Sum) (int, int);

/* Define the run function type
 *  The run function should take no arguments, and returns an integer value
 */
typedef int (*Run) ();

/* Traverse through the externs node to find extern nodes
 *  args: Node, LLVMModuleRef
 *  returns: void
 */
static void generate_extern_T(LLVMModuleRef module, Node *n, F_table *ft);

/* Generate a extern declaration in the module
 *  args:
 *    Node: A extern node
 *    LLVMModuleRef
 *  returns:
 *    void
 */
static void generate_extern(LLVMModuleRef module, Node *n, F_table *ft);

/* Traverse through the externs node to find extern nodes
 *  args: Node, LLVMModuleRef
 *  returns: void
 */
static void generate_function_T(LLVMModuleRef module, Node *n, F_table *ft);

/* Generate function in the module (pending)
 *  args:
 *    Node: A extern node
 *    LLVMModuleRef
 *  returns:
 *    void
 */
static void generate_function(LLVMModuleRef module, Node *n, F_table *ft);

/* Generate function params as local variables
 */
static void declare_variable(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate the variable pointer, when encountered ref type */
static LLVMValueRef generate_varptr(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate function params types - Traversal
 *  args:
 *    LLVMTypeRef **param_types
 *    int *, which records to number of arguments
 *    Node <vdecls> <vdecl>
 *  returns:
 *    void
 */
static void generate_args_T(LLVMTypeRef **param_types, unsigned int *nargs, Node *n);

/* Generate function params types - Traversal
 *  args:
 *    LLVMTypeRef **param_types
 *    int *, which records to number of arguments
 *    Node <vdecls> <vdecl>
 *  returns:
 *    void
 */
static void generate_args(LLVMTypeRef **param_types, unsigned int *nargs, Node *n);

/* Enter the function block
 *  args:
 *    function (LLVMValueRef)
 *    Node (a blk node)
 *  returns:
 *    void
 */
static void enter_function_block(LLVMBuilderRef builder, LLVMValueRef function, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Search for the statement - Traversal
 *  args:
 *    LLVMBuilderRef: The builder
 *    Node n: The node pointer
 *  returns:
 *    void
 */
static void generate_statement_T(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate a simple statement
 *  args:
 *    LLVMBuilderRef: The builder
 *    Node n: The node pointer
 *  returns:
 *    void
 */
static void generate_simple_statement(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate a block
 *  args:
 *    LLVMBuilderRef: The builder
 *    Node n: The node pointer
 *  returns:
 *    void
 */
static void generate_block(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate and return a expression value
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The value.
 */
static LLVMValueRef generate_expression(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Find the variable in the variable table by the given value */
static LLVMValueRef generate_var(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate and return a binop value
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The LLVMValueRef value.
 */
static LLVMValueRef generate_binop(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

static LLVMValueRef assign_variable(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate and return a uop value (must be an Int32)
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The LLVMValueRef value.
 */
static LLVMValueRef generate_uop(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate a variable vdecl_node */
// static void generate_vdecl(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Generate a LLVM constant
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The value, as a constant.
 */
static LLVMValueRef generate_lit(LLVMBuilderRef builder, Node *n);

/* Generate a constant integer value
 *  args: The integer value to be generated
 *  returns: LLVMValueRef
 */
static LLVMValueRef generate_constant_int(int value);

/* Generate a constant float value
 *  args: The float-point value to be generated
 *  returns: LLVMValueRef
 */
static LLVMValueRef generate_constant_float(double value);

/* Generate the llvm type of a type node
 *  args: Node <type>
 *  returns: LLVMTypeRef
 */
static LLVMTypeRef get_llvm_type(Node *n);

/* Enter the function block
 *  args:
 *    function (LLVMValueRef)
 *    Node (a blk node)
 *  returns:
 *    void
 */
// static void enter_function_block(LLVMValueRef function, Node *n, F_table *ft, V_table *vt, unsigned int level);

/* Main function to generate the LLVM
 */
int generate_llvm(Node *n, char *out);

/* Generate a LLVM string
 *  args: The string to be generated
 *  returns: LLVMValueRef
 */
LLVMValueRef generate_string(char *str);

static void generate_if_statement(LLVMBuilderRef builder, Node *n);

static void generate_while_statement(LLVMBuilderRef builder, Node *n);
