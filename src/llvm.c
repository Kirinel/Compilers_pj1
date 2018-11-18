#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
#include "llvm.h"
#include "symbol_table.h"
#include "compile.h"

// #define TRUE LLVMConstInt(LLVMInt1Type(), 1, 0);
// #define FALSE LLVMConstInt(LLVMInt1Type(), 0, 0);
#define MAX_NUM_LEN 128

/* Initialize the function table */
F_table *init_ftable() {
  F_table *t = calloc(1, sizeof(F_table));
  t->num_bucs = INIT_SIZE;
  t->bucs = calloc(INIT_SIZE, sizeof(F_entry *));

  return t;
}
/* Free the function table */
void free_ftable(F_table *t) {
  F_entry *curr=NULL, *next=NULL;

  for (size_t i=0; i<t->num_bucs; ++i) {
    curr = t->bucs[i];

    while (curr != NULL) {
      next = curr->next;
      free(curr->funcname);
      free_vtable(curr->vtable);
      free(curr);
      curr = next;
    }
  }

  free(t->bucs);
  free(t);
}
/* Add a new function entry */
F_entry *new_fentry(char *fname, LLVMValueRef ref) {
  F_entry *e = calloc(1, sizeof(F_entry));
  e->funcname = strdup(fname);
  e->reference = ref;
  e->vtable = init_vtable(fname);
  return e;
}
/* Add a new function to the table */
int add_fentry(F_table *t, char *funcname, LLVMValueRef ref) {
  // Search bucket
  hash_t hash_value = hash(funcname);
  size_t target_buc = hash_value % t->num_bucs;
  F_entry *curr = t->bucs[target_buc];
  // Insert if the head pointer is NULL
  if (curr == NULL) {
    t->bucs[target_buc] = new_fentry(funcname, ref);
    return SUCCESS;
  }
  // Search the list
  while (curr->next != NULL) {
    //check duplicate funcnames
    if (!strcmp(curr->funcname, funcname)) {
      return FUNC_DUP;
    }
    curr = curr->next;
  }
  if (!strcmp(curr->funcname, funcname)) {
    return FUNC_DUP;
  }
  curr->next = new_fentry(funcname, ref);
  return SUCCESS;
}
/* Find a function reference */
int find_fentry(F_table *t, char *funcname, LLVMValueRef *func) {
  // find the bucket
  hash_t hash_value = hash(funcname);
  size_t target_buc = hash_value % t->num_bucs;
  // start iterating to find the entry
  F_entry *curr = t->bucs[target_buc];
  if (curr == NULL) return NOT_FOUND;
  while (curr != NULL) {
    //check duplicate globids
    if (!strcmp(curr->funcname, funcname)) {
      *func = curr->reference;
      return SUCCESS;
    }
    curr = curr->next;
  }
  return NOT_FOUND;
}
/* Find a variable table binded with a function */
int find_vtable(F_table *t, char *funcname, V_table **vt) {
  // find the bucket
  hash_t hash_value = hash(funcname);
  size_t target_buc = hash_value % t->num_bucs;
  // start iterating to find the entry
  F_entry *curr = t->bucs[target_buc];
  if (curr == NULL) {
    // printf("search failure!\n");
    return NOT_FOUND;
  }
  while (curr != NULL) {
    //check duplicate globids
    if (!strcmp(curr->funcname, funcname)) {
      // printf("search success!\n");
      *vt = curr->vtable;
      return SUCCESS;
    }
    curr = curr->next;
  }
  // printf("search failure!\n");
  return NOT_FOUND;
}

/* Initialize a variable table */
V_table *init_vtable(char *funcname) {
  V_table *t = calloc(1, sizeof(V_table));
  t->num_bucs = INIT_SIZE;
  t->funcname = strdup(funcname);
  t->bucs = calloc(INIT_SIZE, sizeof(V_entry *));

  return t;
}
/* Free the variable table */
void free_vtable(V_table *t) {
  V_entry *curr=NULL, *next=NULL;

  for (size_t i=0; i<t->num_bucs; ++i) {
    curr = t->bucs[i];

    while (curr != NULL) {
      next = curr->next;
      free(curr->varname);
      free(curr);
      curr = next;
    }
  }

  free(t->bucs);
  free(t->funcname);
  free(t);
}
/* Add a new variable entry */
V_entry *new_ventry(char *varname, LLVMValueRef ref, char is_ref, unsigned int level) {
  V_entry *e = calloc(1, sizeof(V_entry));
  e->varname = strdup(varname);
  e->reference = ref;
  e->is_ref = is_ref;
  e->level = level;

  return e;
}
/* Add a new variable to the table */
int add_ventry(V_table *t, char *varname, LLVMValueRef ref, char is_ref, unsigned int level) {
  // Search bucket
  hash_t hash_value = hash(varname);
  size_t target_buc = hash_value % t->num_bucs;
  V_entry *curr = t->bucs[target_buc];
  // Insert if the head pointer is NULL
  if (curr == NULL) {
    t->bucs[target_buc] = new_ventry(varname, ref, is_ref, level);
    return SUCCESS;
  }
  // Search the list
  while (curr->next != NULL) {
    //check duplicate variable names
    if (strcmp(curr->varname, varname)==0 && curr->level==level) {
      return VAR_DUP;
    }
    curr = curr->next;
  }
  if (strcmp(curr->varname, varname) == 0 && curr->level==level) {
    return VAR_DUP;
  }
  curr->next = new_ventry(varname, ref, is_ref, level);
  return SUCCESS;
}
/* Find a variable reference */
int find_ventry(V_table *t, char *varname, unsigned int level, V_entry **vent) {
  // find the bucket
  hash_t hash_value = hash(varname);
  size_t target_buc = hash_value % t->num_bucs;
  // start iterating to find the entry

  for (unsigned int vlevel = level; vlevel>=0; --vlevel) {
    V_entry *curr = t->bucs[target_buc];
    if (curr == NULL) return NOT_FOUND;
    while (curr != NULL) {
      //check duplicate globids
      if (strcmp(curr->varname, varname)==0 && curr->level==vlevel) {
        *vent = curr;
        return SUCCESS;
      }
      curr = curr->next;
    }
    // End of one iteration.
  }
  return NOT_FOUND;
}
/* Destroy variable by level */
void destroy_ventry_bylevel(V_table *t, unsigned int level) {
  V_entry *curr=NULL, *prev=NULL;

  for (size_t i=0; i<t->num_bucs; ++i) {
    curr = t->bucs[i];
    // skip empty bucket
    if (curr == NULL) continue;

    if (curr->level == level) {
      while (curr != NULL && curr->level == level) {
        // destroy
        t->bucs[i] = curr->next;
        free(curr->varname);
        free(curr);
        curr = t->bucs[i];
      }
    }

    // gets curr that is either NULL, or curr->level != level
    if (curr == NULL) continue;
    // proceed with one unit
    prev = curr;
    curr = curr->next;

    while (curr != NULL) {
      // check the variable to get destroyed
      if (curr->level == level) {
        // Destroy
        prev->next = curr->next;
        free(curr->varname);
        free(curr);
        curr = prev->next;
      }
    }
    // End of one iteration
  }
  // End of function
}

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
void compile_llvm(Node *n, char *out, int write_to_file) {
  char *error = NULL;
  /*  Create module,
   *    One module for one source code
   *  Also create the error function
   */
  LLVMModuleRef module = LLVMModuleCreateWithName("my_module");

  /* Create function table */
  F_table *ft = init_ftable();

  /* Append printf function, and Push it into the hash table*/
  char ppf[] = "printf";
  LLVMTypeRef PrintfArgsTyList[] = { LLVMPointerType(LLVMInt8Type(), 0) };
  LLVMTypeRef PrintfTy = LLVMFunctionType(LLVMInt32Type(), PrintfArgsTyList, 0, 1);
  LLVMValueRef PrintfFunction = LLVMAddFunction(module, ppf, PrintfTy);
  add_fentry(ft, ppf, PrintfFunction);

  /* Recurse into the function blocks
   */
  generate_extern_T(module, n->node_prog.externs, ft);
  generate_function_T(module, n->node_prog.funcs, ft);

  // printf("recursing tree... Complete!\n");

  /* Housekeeping 1 */
  free_ftable(ft);

  /* Verify the module
   *  Make sure the module is consturcted correctly
   */
  error = NULL;
  LLVMVerifyModule(module, LLVMAbortProcessAction, &error);
  LLVMDisposeMessage(error);

  // printf("verify module... Pass!\n");

  /* Set up execution engine,
   *  this program will compile the input source code using JIT
   */
  LLVMExecutionEngineRef engine;
  error = NULL;
  LLVMLinkInMCJIT();

  // printf("gg1\n");

  // Include the functions necessary for executing the JIT
  LLVMInitializeNativeTarget();
  // LLVMInitializeAllTargets();
  // LLVMInitializeAllTargetMCs();
  LLVMInitializeNativeAsmPrinter();
  LLVMInitializeNativeAsmParser();

  // printf("gg2\n");

  // Check the execution engine is working correctly
  if (LLVMCreateExecutionEngineForModule(&engine, module, &error) != 0) {
      fprintf(stderr, "failed to create execution engine\n");
      abort();
  }

  // printf("gg3\n");

  if (error) {
      fprintf(stderr, "error: %s\n", error);
      LLVMDisposeMessage(error);
      exit(EXIT_FAILURE);
  }

  // printf("gg4\n");

  // Execute the function from "run"
  Run run = (Run) LLVMGetFunctionAddress(engine, "run");
  printf("return value: %d\n", run());

  // Sum ss = (Sum) LLVMGetFunctionAddress(engine, "sum");
  // printf("%d\n", ss(4,5));

  /* Write LLVM IR to file
   *  if write_to_file is non_zero
   * Complains when encountered an error
   */
  if (write_to_file) {
    error = NULL;
    LLVMPrintModuleToFile(module, out, &error);
    if (error) {
      fprintf(stderr, "error: %s\n", error);
      LLVMDisposeMessage(error);
      exit(EXIT_FAILURE);
    }
    LLVMDisposeMessage(error);
  }
  //for testing
  LLVMPrintModuleToFile(module, "test.ll", &error);
  LLVMDisposeMessage(error);

  /* The end of function - Housekeeping */
  LLVMRemoveModule(engine, module, &module, &error);
  LLVMDisposeExecutionEngine(engine);

  LLVMDisposeModule(module);
}

/* Main function to generate the LLVM
 */
int generate_llvm(Node *n, char *out) {
  char *err = NULL;

  LLVMModuleRef mod = LLVMModuleCreateWithName("IR_module");

  /* Verify the Module
   */
  LLVMVerifyModule(mod, LLVMAbortProcessAction, &err);
  LLVMDisposeMessage(err);

  /* Generate the llvm_ir to file
   */
  LLVMPrintModuleToFile(mod, out, &err);
  if (err) fprintf(stderr, "Failure: LLVM IR cannot be written to the given file.\n");

  /* House cleaning
   */
  LLVMDisposeModule(mod);

  return 0;
}

/* Traverse through the externs node to find extern nodes
 *  args: Node, LLVMModuleRef
 *  returns: void
 */
static void generate_extern_T(LLVMModuleRef module, Node *n, F_table *ft) {
  if (n==NULL)  return;

  assert(n->tag == T_EXTERNS || n->tag == T_EXTERN);

  if (n->tag == T_EXTERNS) {
    generate_extern_T(module, n->node_externs.left, ft);
    generate_extern_T(module, n->node_externs.right, ft);
  } else if (n->tag == T_EXTERN) {
    generate_extern(module, n, ft);
  }
  // End of function
}

/* Generate a extern declaration in the module (pending)
 *  args:
 *    Node: A extern node
 *    LLVMModuleRef
 *  returns:
 *    void
 */
static void generate_extern(LLVMModuleRef module, Node *n, F_table *ft) {
  assert(n->tag == T_EXTERN);
  return;
}

/* Traverse through the externs node to find extern nodes
 *  args: Node, LLVMModuleRef
 *  returns: void
 */
static void generate_function_T(LLVMModuleRef module, Node *n, F_table *ft) {
  if (n==NULL)  return;

  assert(n->tag == T_FUNCS || n->tag == T_FUNC);

  if (n->tag == T_FUNCS) {
    generate_function_T(module, n->node_funcs.left, ft);
    generate_function_T(module, n->node_funcs.right, ft);
  } else if (n->tag == T_FUNC) {
    generate_function(module, n, ft);
  }
  // End of function
}

/* Generate function in the module (pending)
 *  args:
 *    Node: A extern node
 *    LLVMModuleRef
 *  returns:
 *    void
 */
static void generate_function(LLVMModuleRef module, Node *n, F_table *ft) {
  assert(n->tag == T_FUNC);

  /* Declare the needed function arguments */
  unsigned int ParamCount = 0;
  LLVMTypeRef *param_types = NULL;
  LLVMBool IsVarArg = 0;
  // Get the return type
  LLVMTypeRef ReturnType = get_llvm_type(n->node_func.type);
  // Get the function name
  char *funcname = n->node_func.globid->node_globid.funcname;
  // Declare the LLVM variables to be filled
  LLVMTypeRef ret_type;
  LLVMValueRef function;

  /* Step1: Generate args type
   *  if vdecls != NULL, use recursive function to generate
   *  if vdecls == NULL, nothing needs to do
   */
  if (n->node_func.vdecls != NULL)
    generate_args_T(&param_types, &ParamCount, n->node_func.vdecls);

  /* Step2: Generate the Return type (LLVM object) */
  ret_type = LLVMFunctionType(ReturnType, param_types, ParamCount, IsVarArg);

  /* Step3: Generate the function (as LLVMValueRef) and add it to hash table */
  function = LLVMAddFunction(module, funcname, ret_type);
  add_fentry(ft, funcname, function);

  // Set calling convention
  LLVMSetFunctionCallConv(function, LLVMCCallConv);

  /* Step4: recurse into the function block
   */
  // Generate the variable table and set the level to 0
  V_table *vt = NULL;
  find_vtable(ft, funcname, &vt);

  unsigned level = 0;

  // create entry block
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");

  // create builder and point at the entry block
  // one builder for one function block
  LLVMBuilderRef builder = LLVMCreateBuilder();
  LLVMPositionBuilderAtEnd(builder, entry);

  // generate_args_D1(builder, );

  enter_function_block(builder, function, n->node_func.blk, ft, vt, level);

  // Dispose the builder at the end of the function
  LLVMDisposeBuilder(builder);
}

/* Generate a vdecl node
 */
static void declare_variable(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  if (n==NULL)  return;

  assert(n->tag == T_SIMPLE_STMT);

  Node *vdecl = n->node_simple_stmt.vdecl;
  Node *exp = n->node_simple_stmt.exp;
  Node *var = vdecl->node_vdecl.var;
  char *varname = var->node_var.varname;

  char is_ref = 0;

  LLVMValueRef exp_ptr, exp_val = generate_expression(builder, exp, ft, vt, level);
  LLVMTypeRef ptr_type, exp_type = LLVMTypeOf(exp_val);

  // Generate type
  enum llvm_type vdecl_type = type_to_llvmtype(vdecl->node_vdecl.type);
  // Add the function to the stack
  LLVMTypeRef type = 0;

  switch (vdecl_type) {
    case LLVM_INT:
    case LLVM_CINT:
      type = LLVMInt32Type();
      break;
    case LLVM_FLOAT:
    case LLVM_SFLOAT:
      type = LLVMFloatType();
      break;
    case LLVM_VOID:
      type = LLVMVoidType();
      break;
    case LLVM_STRING:
      fprintf(stderr, "String arguments not allowed!\n");
      break;
    case LLVM_REF_INT:
    case LLVM_REF_CINT:
      exp_ptr = generate_varptr(builder, exp->node_exp.var, ft, vt, level);
      type = LLVMPointerType(LLVMInt32Type(), 0);
      is_ref = 1;
      break;
    case LLVM_REF_FLOAT:
    case LLVM_REF_SFLOAT:
      exp_ptr = generate_varptr(builder, exp->node_exp.var, ft, vt, level);
      type = LLVMPointerType(LLVMFloatType(), 0);
      is_ref = 1;
      break;
    case LLVM_NOALIAS_REF_INT:
    case LLVM_NOALIAS_REF_CINT:
      exp_ptr = generate_varptr(builder, exp->node_exp.var, ft, vt, level);
      type = LLVMPointerType(LLVMInt32Type(), 0);
      is_ref = 1;
      break;
    case LLVM_NOALIAS_REF_FLOAT:
    case LLVM_NOALIAS_REF_SFLOAT:
      exp_ptr = generate_varptr(builder, exp->node_exp.var, ft, vt, level);
      type = LLVMPointerType(LLVMFloatType(), 0);
      is_ref = 1;
      break;
    }

  // Type checker: Check both sides have the same type
  if (is_ref) {
    ptr_type = LLVMTypeOf(exp_ptr);
    if (ptr_type != type) {
      fprintf(stderr, "error: Type unmatch at variable <%s>\n", varname);
      exit(-1);
    }
  } else {
    if (exp_type != type) {
      fprintf(stderr, "error: Type unmatch at variable <%s>\n", varname);
      exit(-1);
    }
  }

  // Allocate Memory
  LLVMValueRef ptr = LLVMBuildAlloca(builder, type, varname);
  // Store value
  if (is_ref) {
    LLVMBuildStore(builder, exp_ptr, ptr);
  } else {
    LLVMBuildStore(builder, exp_val, ptr);
  }

  // Store variable into the variable table
  add_ventry(vt, varname, ptr, is_ref, level);

  // End of function
}

/* Generate the variable pointer, when encountered ref type */
static LLVMValueRef generate_varptr(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  assert(n->tag == T_VAR);

  char *varname = n->node_var.varname;
  V_entry *vent = NULL;
  find_ventry(vt, varname, level, &vent);

  LLVMValueRef ret_val = vent->reference;

  if (vent->is_ref) {
    ret_val = LLVMBuildLoad(builder, vent->reference, "tmp");
  }

  return ret_val;
}

/* Generate function params types - Traversal
 *  args:
 *    LLVMTypeRef **param_types
 *    int *, which records to number of arguments
 *    Node <vdecls> <vdecl>
 *  returns:
 *    void
 */
static void generate_args_T(LLVMTypeRef **param_types, unsigned int *nargs, Node *n) {
  if (n==NULL)  return;

  assert(n->tag == T_VDECLS || n->tag == T_VDECL);

  if (n->tag == T_VDECLS) {
    generate_args_T(param_types, nargs, n->node_vdecls.left);
    generate_args_T(param_types, nargs, n->node_vdecls.right);
  } else if (n->tag == T_VDECL) {
    generate_args(param_types, nargs, n);
  }
  // End of function
}

/* Generate function params types - Traversal
 *  args:
 *    LLVMTypeRef **param_types
 *    int *, which records to number of arguments
 *    Node <vdecls> <vdecl>
 *  returns:
 *    void
 */
static void generate_args(LLVMTypeRef **param_types, unsigned int *nargs, Node *n) {
  assert(n->tag == T_VDECL);

  LLVMTypeRef arg_type = get_llvm_type(n->node_vdecl.type);

  if (*nargs == 0) {
    // if there was no previous arguments, add one argument
    *param_types = calloc(1, sizeof(LLVMTypeRef));
    (*param_types)[0] = arg_type;
  } else {
    // Put data to new memory location
    LLVMTypeRef *new_param_types = calloc((*nargs)+1, sizeof(LLVMTypeRef));
    for (unsigned int i=0; i<(*nargs-1); ++i) {
      new_param_types[i] = (*param_types)[i];
    }
    free(*param_types);
    *param_types = new_param_types;
  }
  // increment
  ++(*nargs);
}

/* Enter the function block
 *  args:
 *    function (LLVMValueRef)
 *    Node (a blk node)
 *  returns:
 *    void
 */
static void enter_function_block(LLVMBuilderRef builder, LLVMValueRef function, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  assert(n->tag == T_BLK);

  generate_statement_T(builder, n->node_blk.stmts, ft, vt, level);
}

/* Search for the statement - Traversal
 *  args:
 *    LLVMBuilderRef: The builder
 *    Node n: The node pointer
 *  returns:
 *    void
 */
static void generate_statement_T(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  if (n==NULL)  return;

  assert(n->tag == T_STMTS || n->tag == T_STMT || n->tag == T_OPEN_STMT ||
         n->tag == T_CLOSED_STMT || n->tag == T_SIMPLE_STMT);

  switch (n->tag) {
    case T_STMTS:
      generate_statement_T(builder, n->node_stmts.left, ft, vt, level);
      generate_statement_T(builder, n->node_stmts.right, ft, vt, level);
      break;
    case T_STMT:
      generate_statement_T(builder, n->node_stmt.oc_stmt, ft, vt, level);
      break;
    case T_OPEN_STMT:
      if (n->node_open_stmt.ocstmt_tag == STMT_IF) {
        // case if
        generate_if_statement(builder, n);
      } else {
        // case while
        generate_while_statement(builder, n);
      }
      break;
    case T_CLOSED_STMT:
      if (n->node_closed_stmt.ocstmt_tag == STMT_IF) {
        // case if
        generate_if_statement(builder, n);
      } else if (n->node_closed_stmt.ocstmt_tag == STMT_WHILE) {
        // case while
        generate_while_statement(builder, n);
      } else {
        generate_statement_T(builder, n->node_closed_stmt.stmt1, ft, vt, level);
      }
      break;
    case T_SIMPLE_STMT:
      generate_simple_statement(builder, n, ft, vt, level);
      break;
    default:
      break;
  }
}

/* Generate a simple statement
 *  args:
 *    LLVMBuilderRef: The builder
 *    Node n: The node pointer
 *  returns:
 *    void
 */
static void generate_simple_statement(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  assert(n->tag == T_SIMPLE_STMT);

  // case STMT_PRINTSLIT:
  char prf[] = "printf";
  char *printslit = NULL, *printspp = NULL;
  size_t len = 0;
  LLVMValueRef callstring, PrintfFunction = 0;
  LLVMValueRef *PrintfArgs;
  // case STMT_PRINT:
  char p_int[] = "%d\n", p_float[] = "%f\n";
  LLVMValueRef print_exp;
  LLVMValueRef PrintInt, PrintFloat;
  LLVMTypeRef print_exp_type;

  switch (n->node_simple_stmt.simple_stmt_tag) {
    case STMT_BLK:
      // Block increase with one level
      generate_block(builder, n->node_simple_stmt.blk, ft, vt, level+1);
      break;
    case STMT_RETURN:
      // Return expression value
      LLVMBuildRet(builder, generate_expression(builder, n->node_simple_stmt.exp, ft, vt, level));
      break;
    case STMT_RETURN_VOID:
      // Return Void
      LLVMBuildRetVoid(builder);
      break;
    case STMT_PRINTSLIT:
      // Add a '\n' at the end of the string, strip "\""
      printslit = n->node_simple_stmt.slit->node_slit.str;
      len = strlen(printslit);
      printspp = calloc(len, sizeof(char *));
      for (size_t i=0; i<len-1; ++i) {
        printspp[i] = printslit[i+1];
      }
      printspp[len-2] = '\n';
      printspp[len-1] = '\0';
      // Create calling string
      callstring = LLVMBuildGlobalStringPtr(builder, printspp, "printslit_str");
      PrintfArgs = calloc(1, sizeof(LLVMValueRef));
      PrintfArgs[0] = callstring;
      find_fentry(ft, prf, &PrintfFunction);
      LLVMBuildCall(builder, PrintfFunction, PrintfArgs, 1, "printslit");
      // Housekeeping
      free(printspp);
      free(PrintfArgs);
      break;
    case STMT_PRINT:
      print_exp = generate_expression(builder, n->node_simple_stmt.exp, ft, vt, level);
      print_exp_type = LLVMTypeOf(print_exp);
      PrintfArgs = calloc(2, sizeof(LLVMValueRef));
      if (print_exp_type == LLVMInt32Type()) {
        // case integer, use %d
        PrintInt = LLVMBuildGlobalStringPtr(builder, p_int, "print_int");
        PrintfArgs[0] = PrintInt;
        PrintfArgs[1] = print_exp;
        // call the function
        find_fentry(ft, prf, &PrintfFunction);
        LLVMBuildCall(builder, PrintfFunction, PrintfArgs, 2, "print");
      } else if (print_exp_type == LLVMFloatType()) {
        // case float, use %f
        PrintFloat = LLVMBuildGlobalStringPtr(builder, p_float, "print_float");
        PrintfArgs[0] = PrintFloat;
        // convert float type to double type
        print_exp = LLVMBuildFPExt(builder, print_exp, LLVMDoubleType(), "fpext");
        PrintfArgs[1] = print_exp;
        // call the function
        find_fentry(ft, prf, &PrintfFunction);
        LLVMBuildCall(builder, PrintfFunction, PrintfArgs, 2, "print");
      }
      // House keeping
      free(PrintfArgs);
      break;
    case STMT_EXPSTMT:
      generate_expression(builder, n->node_simple_stmt.exp, ft, vt, level);
      break;
    case STMT_VARDECL:
      declare_variable(builder, n, ft, vt, level);
      break;
    default:
      printf("still under construction.\n");
  }
}

/* Generate a variable vdecl_node */
// static void generate_vdecl(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {

/* Generate a variable */

/* Generate a simple statement
 *  args:
 *    LLVMBuilderRef: The builder
 *    Node n: The node pointer
 *  returns:
 *    void
 */
static void generate_block(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  assert(n->tag == T_BLK);
  generate_statement_T(builder, n->node_blk.stmts, ft, vt, level);
}

/* Generate and return a expression value
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The value.
 */
static LLVMValueRef generate_expression(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  assert(n->tag == T_EXP);

  LLVMValueRef ret_val = 0;

  switch (n->node_exp.exp_tag) {
    case EXP_EXP:
      ret_val = generate_expression(builder, n->node_exp.exp, ft, vt, level);
      break;
    case EXP_BINOP:
      ret_val = generate_binop(builder, n->node_exp.binop, ft, vt, level);
      break;
    case EXP_UOP:
      ret_val = generate_uop(builder, n->node_exp.binop, ft, vt, level);
      break;
    case EXP_LIT:
      ret_val = generate_lit(builder, n->node_exp.lit);
      break;
    case EXP_VAR:
      ret_val = generate_var(builder, n->node_exp.var, ft, vt, level);
      break;
    default:
      printf("still under construction (call).\n");
      break;
  }

  return ret_val;
}

/* Find the variable in the variable table by the given value */
static LLVMValueRef generate_var(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  assert(n->tag == T_VAR);

  LLVMValueRef ret_val = 0;
  char *varname = n->node_var.varname;
  // Find the function entry
  V_entry *vent = NULL;
  find_ventry(vt, varname, level, &vent);
  // load
  ret_val = LLVMBuildLoad(builder, vent->reference, "tmp");
  if (vent->is_ref) {
    ret_val = LLVMBuildLoad(builder, ret_val, "tmp");
  }

  return ret_val;
}

/* Generate and return a binop value
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The LLVMValueRef value.
 */
static LLVMValueRef generate_binop(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  assert(n->tag == T_BINOP);

  LLVMValueRef ret_val = 0;
  LLVMValueRef l_val = 0;
  LLVMValueRef r_val = 0;
  LLVMTypeRef l_type, r_type;
  LLVMValueRef flag1, flag2;

  // Get the value and type of the left hand
  switch (n->node_binop.op) {
    case BINOP_ASSIGN:
      // l_val = generate_var(builder, n->node_binop.var, ft, vt, level);
      l_val = LLVMConstInt(LLVMInt32Type(), 0, 0);
      break;
    default:
      l_val = generate_expression(builder, n->node_binop.exp_left, ft, vt, level);
  }

  l_type = LLVMTypeOf(l_val);
  // Get the value and type of the right hand
  r_val = generate_expression(builder, n->node_binop.exp_right, ft, vt, level);
  r_type = LLVMTypeOf(r_val);

  /* Do type promotions if needed
   *  Only cares about signed integer promotes to float
   */
  if (l_type != r_type && n->node_binop.op != BINOP_ASSIGN) {
    if (l_type == LLVMInt32Type()) {
      // left is Int32, convert to Float
      l_val = LLVMBuildSIToFP(builder, l_val, LLVMFloatType(), "conv");
      l_type = LLVMFloatType();
    } else {
      // right is Int32, convert to Float
      r_val = LLVMBuildSIToFP(builder, r_val, LLVMFloatType(), "conv");
      r_type = LLVMFloatType();
    }
  }

  // type checker
  if (l_type != r_type) {
    fprintf(stderr, "runtime time error (assign).\n");
    exit(-1);
  }

  // Compile LLVM IR
  if (l_type == LLVMInt32Type()) {
    // Case Int
    switch (n->node_binop.op) {
      case BINOP_ASSIGN:
        ret_val = assign_variable(builder, n, ft, vt, level);
        break;
      case BINOP_ADD:
        ret_val = LLVMBuildAdd(builder, l_val, r_val, "add");
        break;
      case BINOP_MUL:
        ret_val = LLVMBuildMul(builder, l_val, r_val, "mul");
        break;
      case BINOP_SUB:
        ret_val = LLVMBuildSub(builder, l_val, r_val, "sub");
        break;
      case BINOP_DIV:
        ret_val = LLVMBuildSDiv(builder, l_val, r_val, "div");
        break;
      case BINOP_EQ:
        ret_val = LLVMBuildICmp(builder, LLVMIntEQ, l_val, r_val, "icmp");
        break;
      case BINOP_LT:
        ret_val = LLVMBuildICmp(builder, LLVMIntSLT, l_val, r_val, "icmp");
        break;
      case BINOP_GT:
        ret_val = LLVMBuildICmp(builder, LLVMIntSGT, l_val, r_val, "icmp");
        break;
      case BINOP_AND:
        flag1 = LLVMBuildICmp(builder, LLVMIntNE, l_val, LLVMConstInt(LLVMInt32Type(), 0, 0), "and_flag");
        flag2 = LLVMBuildICmp(builder, LLVMIntNE, r_val, LLVMConstInt(LLVMInt32Type(), 0, 0), "and_flag");
        if (flag1 && flag2)
          ret_val = LLVMConstInt(LLVMInt32Type(), 1, 0);
        else
          ret_val = LLVMConstInt(LLVMInt32Type(), 0, 0);
        break;
      case BINOP_OR:
        break;
      default:
        printf("undefined.\n");
        break;
    }
  } else {
    // Case Float
    switch (n->node_binop.op) {
      case BINOP_ASSIGN:
        printf("undefined.\n");
        break;
      case BINOP_ADD:
        ret_val = LLVMBuildFAdd(builder, l_val, r_val, "fadd");
        break;
      case BINOP_MUL:
        ret_val = LLVMBuildFMul(builder, l_val, r_val, "fmul");
        break;
      case BINOP_SUB:
        ret_val = LLVMBuildFSub(builder, l_val, r_val, "fsub");
        break;
      case BINOP_DIV:
        ret_val = LLVMBuildFDiv(builder, l_val, r_val, "fdiv");
        break;
      case BINOP_EQ:
        ret_val = LLVMBuildFCmp(builder, LLVMRealOEQ, l_val, r_val, "cmp");
        break;
      case BINOP_LT:
        ret_val = LLVMBuildFCmp(builder, LLVMRealOLT, l_val, r_val, "cmp");
        break;
      case BINOP_GT:
        ret_val = LLVMBuildFCmp(builder, LLVMRealOGT, l_val, r_val, "cmp");
        break;
      default:
        printf("undefined.\n");
        break;
    }
  }

  return ret_val;
}

static LLVMValueRef assign_variable(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  LLVMValueRef ret_val = 0;
  LLVMValueRef exp_val = 0;
  // printf("activated\n");
  exp_val = generate_expression(builder, n->node_binop.exp_right, ft, vt, level);
  // LLVMTypeRef exp_type = LLVMTypeOf(exp_val);
  LLVMValueRef ptr = 0;

  char *varname = n->node_binop.var->node_var.varname;

  // printf("%s\n", varname);

  V_entry *vent = NULL;
  find_ventry(vt, varname, level, &vent);


  ptr = vent->reference;
  if (vent->is_ref) {
    ptr = LLVMBuildLoad(builder, ptr, "ptr");
  }

  ret_val = LLVMBuildStore(builder, exp_val, ptr);
  return ret_val;
}

/* Generate and return a uop value
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The LLVMValueRef value.
 */
static LLVMValueRef generate_uop(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level) {
  assert(n->tag == T_UOP);

  LLVMValueRef ret_val = 0, flag_bool, lnot, lnot_ext;
  LLVMValueRef val = generate_expression(builder, n->node_uop.exp, ft, vt, level);
  LLVMTypeRef vtype = LLVMTypeOf(val);

  if (vtype == LLVMInt32Type()) {
    switch (n->node_uop.op) {
      case UOP_MINUS:
        ret_val = LLVMBuildSub(builder, LLVMConstInt(LLVMInt32Type(), 0, 0), val, "sub");
        break;
      case UOP_NOT:
        flag_bool = LLVMBuildICmp(builder, LLVMIntNE, val, LLVMConstInt(LLVMInt32Type(), 0, 0), "tobool");
        lnot = LLVMBuildXor(builder, flag_bool, LLVMConstInt(LLVMInt1Type(), 1, 0), "lnot"); // true
        lnot_ext = LLVMBuildZExt(builder, lnot, LLVMInt32Type(), "lnot.ext");
        ret_val = lnot_ext;
        break;
    }
  } else {
    switch (n->node_uop.op) {
      case UOP_MINUS:
        ret_val = LLVMBuildFSub(builder, LLVMConstReal(LLVMFloatType(), 0.0), val, "fsub");
        break;
      case UOP_NOT:
        flag_bool = LLVMBuildFCmp(builder, LLVMRealONE, val, LLVMConstReal(LLVMFloatType(), 0.0), "tobool");
        lnot = LLVMBuildXor(builder, flag_bool, LLVMConstInt(LLVMInt1Type(), 1, 0), "lnot"); // true
        lnot_ext = LLVMBuildZExt(builder, lnot, LLVMInt32Type(), "lnot.ext");
        ret_val = lnot_ext;
      }
  }
  return ret_val;
}

/* Generate a LLVM constant
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The value, as a constant.
 */
static LLVMValueRef generate_lit(LLVMBuilderRef builder, Node *n) {
  assert(n->tag == T_LIT);

  LLVMValueRef ret_val = 0;

  if (n->node_lit.lit_type == LIT) {
    ret_val = generate_constant_int((int) n->node_lit.value);
  } else {
    ret_val = generate_constant_float(n->node_lit.value);
  }

  return ret_val;
}

static void generate_if_statement(LLVMBuilderRef builder, Node *n) {
  return;
}

static void generate_while_statement(LLVMBuilderRef builder, Node *n) {
  return;
}

/* Generate the llvm type of a type node
 *  args: Node <type>
 *  returns: LLVMTypeRef
 */
static LLVMTypeRef get_llvm_type(Node *n) {
  assert(n->tag == T_TYPE);

  enum llvm_type ret_type = type_to_llvmtype(n);

  switch (ret_type) {
    case LLVM_INT:
    case LLVM_CINT:
      return LLVMInt32Type();
    case LLVM_FLOAT:
    case LLVM_SFLOAT:
      return LLVMFloatType();
    case LLVM_VOID:
      return LLVMVoidType();
    case LLVM_REF_INT:
    case LLVM_REF_CINT:
    case LLVM_NOALIAS_REF_INT:
    case LLVM_NOALIAS_REF_CINT:
      return LLVMPointerType(LLVMInt32Type(), 0);
    case LLVM_REF_FLOAT:
    case LLVM_REF_SFLOAT:
    case LLVM_NOALIAS_REF_FLOAT:
    case LLVM_NOALIAS_REF_SFLOAT:
      return LLVMPointerType(LLVMFloatType(), 0);
    default:
      fprintf(stderr, "unknown function return type.\n");
      exit(UNKNOWN_RET_TYPE);
  }
}

/* Generate a constant integer value
 *  args: The integer value to be generated
 *  returns: LLVMValueRef
 */
static LLVMValueRef generate_constant_int(int value) {
  return LLVMConstInt(LLVMInt32Type(), value, 0);
}

/* Generate a constant float value
 *  args: The float-point value to be generated
 *  returns: LLVMValueRef
 */
static LLVMValueRef generate_constant_float(double value) {
  return LLVMConstReal(LLVMFloatType(), value);
}

/* Generate a LLVM string
 *  args: The string to be generated
 *  returns: LLVMValueRef
 */
LLVMValueRef generate_string(char *str) {
  unsigned int length = strlen(str);
  return LLVMConstString(str, length, 1);
}
