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
  LLVMTypeRef function_type;
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
F_entry *new_fentry(char *, LLVMValueRef, LLVMTypeRef);
int add_fentry(F_table *, char *, LLVMValueRef, LLVMTypeRef);
int find_fentry(F_table *, char *, LLVMValueRef *);
int find_ftype(F_table *, char *, LLVMTypeRef *);
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

static void generate_extern_T(LLVMModuleRef module, Node *n, F_table *ft, double *cl_args, int cl_len);

static void generate_extern(LLVMModuleRef module, Node *n, F_table *ft, double *cl_args, int cl_len);

static void generate_getarg(LLVMModuleRef module, F_table *ft, LLVMValueRef function, double *cl_args, int cl_len, int getfloat);

static void generate_function_T(LLVMModuleRef module, Node *n, F_table *ft);

static void generate_function(LLVMModuleRef module, Node *n, F_table *ft);

static void declare_variable(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static LLVMValueRef generate_varptr(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level);

static void generate_args_T(LLVMTypeRef **param_types, unsigned int *nargs, Node *n, int **noalias);

static void generate_args(LLVMTypeRef **param_types, unsigned int *nargs, Node *n, int **noalias);

static void generate_funcargs_T(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function, int *argi);

static void generate_funcargs(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function, int *argi);

static void enter_function_block(LLVMBuilderRef builder, LLVMValueRef function, Node *n, F_table *ft, V_table *vt, unsigned int level);

static void generate_statement_T(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static void generate_simple_statement(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static void generate_block(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static LLVMValueRef generate_expression(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static LLVMValueRef generate_call(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static void generate_callargs(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function, LLVMValueRef **Args, int *nargs, LLVMTypeRef *args_types);

static LLVMValueRef generate_var(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static LLVMValueRef generate_binop(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static LLVMValueRef assign_variable(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static LLVMValueRef generate_uop(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static LLVMValueRef generate_lit(LLVMBuilderRef builder, Node *n);

static LLVMValueRef generate_constant_int(int value);

static LLVMValueRef generate_constant_float(double value);

static LLVMTypeRef get_llvm_type(Node *n);

int generate_llvm(Node *n, char *out);

LLVMValueRef generate_string(char *str);

static void generate_if_statement(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);

static void generate_while_statement(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function);
