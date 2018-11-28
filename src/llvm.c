#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Analysis.h>
// Optimizer
#include <llvm-c/Transforms/PassManagerBuilder.h>
#include <llvm-c/Transforms/Scalar.h>
// User defined header files
#include "llvm.h"
#include "symbol_table.h"
#include "compile.h"
#include "cint.h"

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
F_entry *new_fentry(char *fname, LLVMValueRef ref, LLVMTypeRef functype) {
  F_entry *e = calloc(1, sizeof(F_entry));
  e->funcname = strdup(fname);
  e->reference = ref;
  e->function_type = functype;
  e->vtable = init_vtable(fname);
  return e;
}
/* Add a new function to the table */
int add_fentry(F_table *t, char *funcname, LLVMValueRef ref, LLVMTypeRef functype) {
  // Search bucket
  hash_t hash_value = hash(funcname);
  size_t target_buc = hash_value % t->num_bucs;
  F_entry *curr = t->bucs[target_buc];
  // Insert if the head pointer is NULL
  if (curr == NULL) {
    t->bucs[target_buc] = new_fentry(funcname, ref, functype);
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
  curr->next = new_fentry(funcname, ref, functype);
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
/* Find a function type */
int find_ftype(F_table *t, char *funcname, LLVMTypeRef *type) {
  // find the bucket
  hash_t hash_value = hash(funcname);
  size_t target_buc = hash_value % t->num_bucs;
  // start iterating to find the entry
  F_entry *curr = t->bucs[target_buc];
  if (curr == NULL) return NOT_FOUND;
  while (curr != NULL) {
    //check duplicate globids
    if (!strcmp(curr->funcname, funcname)) {
      *type = curr->function_type;
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
/*   Free the variable table */
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
  // printf("free %d\n", level);
  V_entry *curr=NULL, *tmp=NULL;

  for (size_t i=0; i<t->num_bucs; ++i) {
    curr = t->bucs[i];
    // skip empty bucket, continue
    if (curr == NULL) continue;

    // free the first entry iteratively
    while (curr != NULL && curr->level == level) {
      // destroy
      t->bucs[i] = curr->next;
      free(curr->varname);
      free(curr);
      curr = t->bucs[i];
    }

    // if nothing is left, continue
    if (curr == NULL || curr->next == NULL) continue;

    // proceed with one unit
    curr = curr->next;

    while (curr->next != NULL) {
      // check the variable to get destroyed
      if (curr->level == level) {
        // Destroy
        tmp = curr->next;
        curr->next = curr->next->next;
        free(tmp->varname);
        free(tmp);
      } else {
        curr = curr->next;
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
void compile_llvm(Node *n, char *out,int write_to_file, int optimizer, double *cl_args, int cl_len) {
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
  add_fentry(ft, ppf, PrintfFunction, PrintfTy);

  init_checked_integers(module, ft);

  /* Recurse into the function blocks
   */
  generate_extern_T(module, n->node_prog.externs, ft, cl_args, cl_len);
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

  // Optimizer
  if (optimizer) {
    // Create passBuilder
    LLVMPassManagerBuilderRef passBuilder;

    passBuilder = LLVMPassManagerBuilderCreate();
    LLVMPassManagerBuilderSetOptLevel(passBuilder, 3);
    LLVMPassManagerBuilderSetSizeLevel(passBuilder, 0);

    LLVMPassManagerRef functionPasses = LLVMCreateFunctionPassManagerForModule(module);
    LLVMPassManagerRef modulePasses = LLVMCreatePassManager();


    LLVMPassManagerBuilderPopulateFunctionPassManager(passBuilder, functionPasses);
    LLVMPassManagerBuilderPopulateModulePassManager(passBuilder, modulePasses);

    // Dispose passBuilder
    LLVMPassManagerBuilderDispose(passBuilder);

    // optimize every function block
    LLVMInitializeFunctionPassManager(functionPasses);
    for (LLVMValueRef value = LLVMGetFirstFunction(module); value; value = LLVMGetNextFunction(value)) {
      LLVMRunFunctionPassManager(functionPasses, value);
    }
    LLVMFinalizeFunctionPassManager(functionPasses);

    LLVMRunPassManager(modulePasses, module);

    LLVMDisposePassManager(functionPasses);
    LLVMDisposePassManager(modulePasses);
  }


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
  run();

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
  // LLVMPrintModuleToFile(module, "test.ll", &error);
  // LLVMDisposeMessage(error);

  /* The end of function - Housekeeping */
  LLVMRemoveModule(engine, module, &module, &error);
  LLVMDisposeExecutionEngine(engine);

  LLVMDisposeModule(module);
}

/* Initialize the functions for checked integers and add them to the module */
static void init_checked_integers(LLVMModuleRef module, F_table *ft) {
  // printf("cint declared\n");

  // Declare test information
  // char warn[] = "warn";
  // LLVMTypeRef warn_args[] = {LLVMInt32Type()};
  // LLVMTypeRef warn_type = LLVMFunctionType(LLVMInt32Type(), warn_args, 1, 0);
  // LLVMValueRef warn_func = LLVMAddFunction(module, warn, warn_type);
  // add_fentry(ft, warn, warn_func, warn_type);

  // Declare negation
  char cint_negation[] = "cint_negation";
  LLVMTypeRef negation_args[] = {LLVMInt32Type(), LLVMPointerType(LLVMInt32Type(), 0)};
  LLVMTypeRef negation_type = LLVMFunctionType(LLVMVoidType(), negation_args, 2, 0);
  LLVMValueRef negation = LLVMAddFunction(module, cint_negation, negation_type);
  add_fentry(ft, cint_negation, negation, negation_type);
  // Declare addition
  char cint_addition[] = "cint_addition";
  LLVMTypeRef addition_args[] = {LLVMInt32Type(), LLVMInt32Type(), LLVMPointerType(LLVMInt32Type(), 0)};
  LLVMTypeRef addition_type = LLVMFunctionType(LLVMVoidType(), addition_args, 3, 0);
  LLVMValueRef addition = LLVMAddFunction(module, cint_addition, addition_type);
  add_fentry(ft, cint_addition, addition, addition_type);
  // Declare subtraction
  char cint_subtraction[] = "cint_subtraction";
  LLVMTypeRef subtraction_args[] = {LLVMInt32Type(), LLVMInt32Type(), LLVMPointerType(LLVMInt32Type(), 0)};
  LLVMTypeRef subtraction_type = LLVMFunctionType(LLVMVoidType(), subtraction_args, 3, 0);
  LLVMValueRef subtraction = LLVMAddFunction(module, cint_subtraction, subtraction_type);
  add_fentry(ft, cint_subtraction, subtraction, subtraction_type);
  // Declare multiplication
  char cint_multiplication[] = "cint_multiplication";
  LLVMTypeRef multiplication_args[] = {LLVMInt32Type(), LLVMInt32Type(), LLVMPointerType(LLVMInt32Type(), 0)};
  LLVMTypeRef multiplication_type = LLVMFunctionType(LLVMVoidType(), multiplication_args, 3, 0);
  LLVMValueRef multiplication = LLVMAddFunction(module, cint_multiplication, multiplication_type);
  add_fentry(ft, cint_multiplication, multiplication, multiplication_type);
  // Declare division
  char cint_division[] = "cint_division";
  LLVMTypeRef division_args[] = {LLVMInt32Type(), LLVMInt32Type(), LLVMPointerType(LLVMInt32Type(), 0)};
  LLVMTypeRef division_type = LLVMFunctionType(LLVMVoidType(), division_args, 3, 0);
  LLVMValueRef division = LLVMAddFunction(module, cint_division, division_type);
  add_fentry(ft, cint_division, division, division_type);
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
  // LLVMPrintModuleToFile(mod, out, &err);
  // if (err) fprintf(stderr, "Failure: LLVM IR cannot be written to the given file.\n");

  /* House cleaning
   */
  LLVMDisposeModule(mod);

  return 0;
}

/* Traverse through the externs node to find extern nodes
 *  args: Node, LLVMModuleRef
 *  returns: void
 */
static void generate_extern_T(LLVMModuleRef module, Node *n, F_table *ft, double *cl_args, int cl_len) {
  if (n==NULL)  return;

  assert(n->tag == T_EXTERNS || n->tag == T_EXTERN);

  if (n->tag == T_EXTERNS) {
    generate_extern_T(module, n->node_externs.left, ft, cl_args, cl_len);
    generate_extern_T(module, n->node_externs.right, ft, cl_args, cl_len);
  } else if (n->tag == T_EXTERN) {
    generate_extern(module, n, ft, cl_args, cl_len);
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
static void generate_extern(LLVMModuleRef module, Node *n, F_table *ft, double *cl_args, int cl_len) {
  assert(n->tag == T_EXTERN);

  // Declare the needed parameters
  unsigned int ParamCount = 0;
  LLVMTypeRef *param_types = NULL;
  LLVMBool IsVarArg = 0;
  // Generate the return type
  LLVMTypeRef ReturnType = get_llvm_type(n->node_extern.type);
  // Get the function name
  char *funcname = n->node_extern.globid->node_globid.funcname;
  // Declare the LLVM variables to be filled
  LLVMTypeRef ret_type;
  LLVMValueRef function;

  // noalias qualifier constants
  const char *noalias_str = "noalias";
  unsigned noalias_id = LLVMGetEnumAttributeKindForName(noalias_str, strlen(noalias_str));
  LLVMContextRef ctx = LLVMGetModuleContext(module);
  LLVMAttributeRef llvm_attr = LLVMCreateEnumAttribute(ctx, noalias_id, 0);
  // Noalias string
  int *noalias = NULL;

  /* Step1: Generate args type */
  if (n->node_extern.tdecls != NULL)
    generate_args_T(&param_types, &ParamCount, n->node_extern.tdecls, &noalias);

  /* Step2: Generate the Return type (LLVM object) */
  ret_type = LLVMFunctionType(ReturnType, param_types, ParamCount, IsVarArg);

  /* Step3: Generate the function (as LLVMValueRef) and add it to hash table */
  function = LLVMAddFunction(module, funcname, ret_type);

  // Generate the noalias qualifier
  for (unsigned int i=0; i<ParamCount; ++i) {
    if (noalias[i])
      // arguments start at 1
      LLVMAddAttributeAtIndex(function, i+1, llvm_attr);
  }

  add_fentry(ft, funcname, function, ret_type);

  // Check if the extern function is getarg, or getargf
  if (strcmp(funcname, "getarg")==0) {
    // Treat it as a normal function
    LLVMSetFunctionCallConv(function, LLVMCCallConv);
    generate_getarg(module, ft, function, cl_args, cl_len, 0);
  } else if (strcmp(funcname, "getargf")==0) {
    // Treat it as a normal function
    LLVMSetFunctionCallConv(function, LLVMCCallConv);
    generate_getarg(module, ft, function, cl_args, cl_len, 1);
  }

  // Free intermediate variables
  free(param_types);
  free(noalias);

  return;
}

/* Get the function parameters after declaring each function */
static void generate_getarg(LLVMModuleRef module, F_table *ft, LLVMValueRef function, double *cl_args, int cl_len, int getfloat) {
  if (cl_len <= 0)  return;

  // Add commandline arguments as global variables
  LLVMValueRef *arr = NULL;
  for (int i=0; i<cl_len; ++i) {
    // printf("activated (%.2f)!\n", cl_args[i]);
    if (arr == NULL) {
      arr = malloc(sizeof(LLVMValueRef));
      arr[0] = LLVMConstReal(LLVMFloatType(), cl_args[i]);
    } else {
      arr = realloc(arr, (i+1)*sizeof(LLVMValueRef));
      arr[i] = LLVMConstReal(LLVMFloatType(), cl_args[i]);
    }
  // LLVMValueRef c = LLVMAddGlobal(module, LLVMFloatType(), "glob_const");
  // LLVMSetInitializer(c, LLVMConstReal(LLVMFloatType(), cl_args[i]));
  }
  // Create an array
  LLVMValueRef vector = LLVMConstVector(arr, cl_len);
  // LLVMValueRef c = LLVMAddGlobal(module, LLVMTypeOf(array), "commandline");
  // LLVMSetInitializer(c, array);
  free(arr);


  // create builder
  LLVMBuilderRef builder = LLVMCreateBuilder();
  LLVMBasicBlockRef entry = LLVMAppendBasicBlock(function, "entry");
  LLVMPositionBuilderAtEnd(builder, entry);
  // LLVMValueRef glob = LLVMGetFirstGlobal(module);
  // LLVMValueRef array = LLVMBuildLoad(builder, glob, "globalarray");
  LLVMValueRef ret_val = LLVMBuildExtractElement(builder, vector, LLVMGetParam(function, 0), "load_from_array");
  // ret_val = LLVMBuildLoad(builder, ret_val, "globalarray");
  if (!getfloat)
    ret_val = LLVMBuildFPToSI(builder, ret_val, LLVMInt32Type(), "conv");
  LLVMBuildRet(builder, ret_val);


  // // create entry block
  // LLVMBasicBlockRef entry, cond, loop, end;
  // LLVMValueRef bool1;
  //
  // entry = LLVMAppendBasicBlock(function, "entry");
  // cond = LLVMAppendBasicBlock(function, "cond");
  // loop = LLVMAppendBasicBlock(function, "loop");
  // end = LLVMAppendBasicBlock(function, "end");
  //
  // // The entry block
  // LLVMPositionBuilderAtEnd(builder, entry);
  // LLVMValueRef ind = LLVMGetParam(function, 0);
  // LLVMValueRef glob = LLVMGetFirstGlobal(module);
  // LLVMValueRef ptr = LLVMBuildAlloca(builder, LLVMPointerType(LLVMFloatType(), 0), "glob");
  // LLVMBuildStore(builder, glob, ptr);
  // LLVMBuildBr(builder, cond);
  //
  // // Get the argument and build cond
  // LLVMPositionBuilderAtEnd(builder, cond);
  // bool1 = LLVMBuildICmp(builder, LLVMIntSGT, ind, LLVMConstInt(LLVMInt32Type(), 0, 0), "tobool");
  // LLVMBuildCondBr(builder, bool1, loop, end);
  //
  // // Build loop
  // LLVMPositionBuilderAtEnd(builder, loop);
  // LLVMBuildSub(builder, ind, LLVMConstInt(LLVMInt32Type(), 1, 0), "sub");
  // glob = LLVMGetNextGlobal(glob);
  // LLVMBuildStore(builder, glob, ptr);
  // LLVMBuildBr(builder, cond);

  // // Build end
  // LLVMPositionBuilderAtEnd(builder, end);
  // LLVMValueRef ret = LLVMBuildLoad(builder, ptr, "load");
  // ret = LLVMBuildLoad(builder, ret, "load");
  // // printf("activated\n");
  // ret = LLVMBuildFPToSI(builder, ret, LLVMInt32Type(), "conv");
  // LLVMBuildRet(builder, ret);

  // // Allocate memory and store, input must be i32
  // LLVMValueRef ptr = LLVMBuildAlloca(builder, LLVMInt32Type(), "ptr");
  // // Store the value to the pointer
  // LLVMBuildStore(builder, ind, ptr);
  //
  //
  // // Load the value
  // LLVMValueRef ret_val = LLVMBuildLoad(builder, ptr, "ret_val");
  // // Return the value
  // LLVMBuildRet(builder, ret_val);

  // Dispose the builder at the end of the function
  LLVMDisposeBuilder(builder);
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

  // noalias qualifier constants
  const char *noalias_str = "noalias";
  unsigned noalias_id = LLVMGetEnumAttributeKindForName(noalias_str, strlen(noalias_str));
  LLVMContextRef ctx = LLVMGetModuleContext(module);
  LLVMAttributeRef llvm_attr = LLVMCreateEnumAttribute(ctx, noalias_id, 0);
  // Noalias array of indices
  int *noalias = NULL;

  /* Step1: Generate args type
   *  if vdecls != NULL, use recursive function to generate
   *  if vdecls == NULL, nothing needs to do
   */
  if (n->node_func.vdecls != NULL)
    generate_args_T(&param_types, &ParamCount, n->node_func.vdecls, &noalias);

  // Confirm the types are written in correctly
  // printf("funcname: %s, args %d\n", funcname, ParamCount);
  // if (ParamCount == 2)
  //   printf("%d %d\n", param_types[0], param_types[1]);
  // else if (ParamCount == 1)
  //   printf("%d\n", param_types[0]);

  /* Step2: Generate the Return type (LLVM object) */
  ret_type = LLVMFunctionType(ReturnType, param_types, ParamCount, IsVarArg);

  /* Step3: Generate the function (as LLVMValueRef) and add it to hash table */
  function = LLVMAddFunction(module, funcname, ret_type);
  add_fentry(ft, funcname, function, ret_type);

  // Generate the noalias qualifier
  for (unsigned int i=0; i<ParamCount; ++i) {
    if (noalias[i])
      // arguments start at 1
      LLVMAddAttributeAtIndex(function, i+1, llvm_attr);
      // printf("noalias at index %u\n", i);
  }


  // Set calling convention
  LLVMSetFunctionCallConv(function, LLVMCCallConv);

  // Free intermediate variables
  free(param_types);
  free(noalias);

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

  // Get the function arguments and push them onto the stack
  int argi = 0;
  generate_funcargs_T(builder, n->node_func.vdecls, ft, vt, level, function, &argi);

  // Building the function block
  enter_function_block(builder, function, n->node_func.blk, ft, vt, level);

  // enforce a check against basic blocks without no terminators.
  LLVMBasicBlockRef end_block = LLVMGetInsertBlock(builder);
  LLVMValueRef terminator = LLVMGetBasicBlockTerminator(end_block);

  if (terminator == NULL) {
    if (ReturnType == LLVMVoidType()) {
      // printf("no return found in a void function!\n");
      LLVMBuildRetVoid(builder);
    } else {
      fprintf(stderr, "error: no terminator found for a non-void function.\n");
      exit(-1);
    }
  }

  // Dispose the builder at the end of the function
  LLVMDisposeBuilder(builder);
}

/* Generate a vdecl node
 */
static void declare_variable(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  if (n==NULL)  return;

  assert(n->tag == T_SIMPLE_STMT);

  Node *vdecl = n->node_simple_stmt.vdecl;
  Node *exp = n->node_simple_stmt.exp;
  Node *var = vdecl->node_vdecl.var;
  char *varname = var->node_var.varname;

  char is_ref = 0;

  LLVMValueRef exp_ptr, exp_val = generate_expression(builder, exp, ft, vt, level, function);
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
    // Not a ref type
    if (exp_type != type) {
      if (type == LLVMFloatType()) exp_val = LLVMBuildSIToFP(builder, exp_val, LLVMFloatType(), "conv");
      if (type == LLVMInt32Type()) exp_val = LLVMBuildFPToSI(builder, exp_val, LLVMInt32Type(), "conv");
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
static void generate_args_T(LLVMTypeRef **param_types, unsigned int *nargs, Node *n, int **noalias) {
  if (n==NULL)  return;

  assert(n->tag == T_VDECLS || n->tag == T_VDECL ||
         n->tag == T_TDECLS || n->tag == T_TYPE);

  if (n->tag == T_VDECLS) {
    generate_args_T(param_types, nargs, n->node_vdecls.left, noalias);
    generate_args_T(param_types, nargs, n->node_vdecls.right, noalias);
  } else if (n->tag == T_TDECLS) {
    generate_args_T(param_types, nargs, n->node_tdecls.left, noalias);
    generate_args_T(param_types, nargs, n->node_tdecls.right, noalias);
  } else if (n->tag == T_VDECL || n->tag == T_TYPE) {
    generate_args(param_types, nargs, n, noalias);
  }
  // End of function
}

/* Generate the LLVM types of the arguments (in order to generate the Function type) */
static void generate_args(LLVMTypeRef **param_types, unsigned int *nargs, Node *n, int **noalias) {
  /* Generate (extern) function params types
 *  args:
 *    LLVMTypeRef **param_types
 *    int *, which records to number of arguments
 *    Node <vdecls> <vdecl>
 *  returns:
 *    void
 */
  assert(n->tag == T_VDECL || n->tag == T_TYPE);

  // Needed types
  LLVMTypeRef arg_type = 0;
  enum llvm_type arg_type_llvm = 0;
  // Noalias ref type declarations
  int i_noalias = 0;

  if (n->tag == T_VDECL) {
    arg_type = get_llvm_type(n->node_vdecl.type);
    arg_type_llvm = type_to_llvmtype(n->node_vdecl.type);
  } else if (n->tag == T_TYPE) {
    arg_type = get_llvm_type(n);
    arg_type_llvm = type_to_llvmtype(n);
  }

  // start accumulating the informations
  if (*nargs == 0) {
    // if there was no previous arguments, add one argument
    *param_types = calloc(1, sizeof(LLVMTypeRef));
    (*param_types)[0] = arg_type;
  } else {
    // Put data to new memory location
    LLVMTypeRef *new_param_types = calloc((*nargs)+1, sizeof(LLVMTypeRef));
    for (unsigned int i=0; i<(*nargs); ++i) {
      new_param_types[i] = (*param_types)[i];
    }
    free(*param_types);
    new_param_types[*nargs] = arg_type;
    *param_types = new_param_types;
  }
  // increment
  ++(*nargs);

  // increase the noalias integer array size
  if (*noalias == NULL)
    *noalias = malloc(sizeof(int));
  else
    *noalias = realloc(*noalias, (*nargs)*sizeof(int));

  // check noalias qualifier
  switch (arg_type_llvm) {
    case LLVM_NOALIAS_REF_INT:
    case LLVM_NOALIAS_REF_CINT:
    case LLVM_NOALIAS_REF_FLOAT:
    case LLVM_NOALIAS_REF_SFLOAT:
      i_noalias = 1;
      break;
    default:
      break;
  }

  // Assigning the values
  (*noalias)[*nargs-1] = i_noalias;
}

/* Generate function args and push them onto the stack - Traversal */
static void generate_funcargs_T(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function, int *argi) {
  if (n==NULL)  return;

  assert(n->tag == T_VDECLS || n->tag == T_VDECL);

  if (n->tag == T_VDECLS) {
    generate_funcargs_T(builder, n->node_vdecls.left, ft, vt, level, function, argi);
    generate_funcargs_T(builder, n->node_vdecls.right, ft, vt, level, function, argi);
  } else if (n->tag == T_VDECL) {
    generate_funcargs(builder, n, ft, vt, level, function, argi);
  }
  // End of function
}

/* Generate function args and push them onto the stack */
static void generate_funcargs(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function, int *argi) {
  assert(n->tag == T_VDECL);

  Node *type = n->node_vdecl.type;
  Node *var = n->node_vdecl.var;
  char *varname = var->node_var.varname;
  char is_ref = 0;

  enum llvm_type arg_llvmtype = type_to_llvmtype(type);
  LLVMTypeRef arg_type = get_llvm_type(type);

  // Testing if the given argument is ref type
  switch (arg_llvmtype) {
    case LLVM_REF_INT:
    case LLVM_REF_CINT:
    case LLVM_NOALIAS_REF_INT:
    case LLVM_NOALIAS_REF_CINT:
    case LLVM_REF_FLOAT:
    case LLVM_REF_SFLOAT:
    case LLVM_NOALIAS_REF_FLOAT:
    case LLVM_NOALIAS_REF_SFLOAT:
      is_ref = 1;
      break;
    default:
      break;
  }

  // Create a pointer for this variable
  LLVMValueRef ptr = LLVMBuildAlloca(builder, arg_type, varname);

  // printf("%s, seq: %d\n", varname, *argi);
  // printf("get value\n");

  LLVMValueRef arg_val = LLVMGetParam(function, (*argi)++);
  // Store the argument value in the address (*argi)++
  // printf("store value\n");
  LLVMBuildStore(builder, arg_val, ptr);

  // printf("store complete\n");

  // Add the variable to the table
  add_ventry(vt, varname, ptr, is_ref, level);

  return;
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

  generate_statement_T(builder, n->node_blk.stmts, ft, vt, level, function);
}

/* Search for the statement - Traversal
 *  args:
 *    LLVMBuilderRef: The builder
 *    Node n: The node pointer
 *  returns:
 *    void
 */
static void generate_statement_T(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  if (n==NULL)  return;

  assert(n->tag == T_STMTS || n->tag == T_STMT || n->tag == T_OPEN_STMT ||
         n->tag == T_CLOSED_STMT || n->tag == T_SIMPLE_STMT);

  switch (n->tag) {
    case T_STMTS:
      generate_statement_T(builder, n->node_stmts.left, ft, vt, level, function);
      generate_statement_T(builder, n->node_stmts.right, ft, vt, level, function);
      break;
    case T_STMT:
      generate_statement_T(builder, n->node_stmt.oc_stmt, ft, vt, level, function);
      break;
    case T_OPEN_STMT:
      if (n->node_open_stmt.ocstmt_tag == STMT_IF) {
        // case if
        generate_if_statement(builder, n, ft, vt, level, function);
      } else {
        // case while
        generate_while_statement(builder, n, ft, vt, level, function);
      }
      break;
    case T_CLOSED_STMT:
      if (n->node_closed_stmt.ocstmt_tag == STMT_IF) {
        // case if
        generate_if_statement(builder, n, ft, vt, level, function);
      } else if (n->node_closed_stmt.ocstmt_tag == STMT_WHILE) {
        // case while
        generate_while_statement(builder, n, ft, vt, level, function);
      } else {
        generate_statement_T(builder, n->node_closed_stmt.stmt1, ft, vt, level, function);
      }
      break;
    case T_SIMPLE_STMT:
      generate_simple_statement(builder, n, ft, vt, level, function);
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
static void generate_simple_statement(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
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
      generate_block(builder, n->node_simple_stmt.blk, ft, vt, level+1, function);
      destroy_ventry_bylevel(vt, level+1);
      break;
    case STMT_RETURN:
      // Return expression value
      LLVMBuildRet(builder, generate_expression(builder, n->node_simple_stmt.exp, ft, vt, level, function));
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
      print_exp = generate_expression(builder, n->node_simple_stmt.exp, ft, vt, level, function);
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
      generate_expression(builder, n->node_simple_stmt.exp, ft, vt, level, function);
      break;
    case STMT_VARDECL:
      declare_variable(builder, n, ft, vt, level, function);
      break;
    default:
      printf("Unknown statement.\n");
  }
}

/* Generate a simple statement
 *  args:
 *    LLVMBuilderRef: The builder
 *    Node n: The node pointer
 *  returns:
 *    void
 */
static void generate_block(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  // Level has already been incremented when the function is called
  assert(n->tag == T_BLK);
  generate_statement_T(builder, n->node_blk.stmts, ft, vt, level, function);
}

/* Generate and return a expression value
 *  args:
 *    (LLVMBuilderRef) The builder object.
 *    (Node) node pointer
 *  returns:
 *    (LLVMValueRef) The value.
 */
static LLVMValueRef generate_expression(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  assert(n->tag == T_EXP);

  LLVMValueRef ret_val = 0;

  switch (n->node_exp.exp_tag) {
    case EXP_EXP:
      ret_val = generate_expression(builder, n->node_exp.exp, ft, vt, level, function);
      break;
    case EXP_BINOP:
      ret_val = generate_binop(builder, n->node_exp.binop, ft, vt, level, function);
      break;
    case EXP_UOP:
      ret_val = generate_uop(builder, n->node_exp.uop, ft, vt, level, function);
      break;
    case EXP_LIT:
      ret_val = generate_lit(builder, n->node_exp.lit);
      break;
    case EXP_VAR:
      ret_val = generate_var(builder, n->node_exp.var, ft, vt, level, function);
      break;
    case EXP_GLOBID:
      ret_val = generate_call(builder, n, ft, vt, level, function);
      break;
    default:
      printf("Unknown expression.\n");
      break;
  }

  return ret_val;
}

/* Generate a function call */
static LLVMValueRef generate_call(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  assert(n->tag == T_EXP);
  LLVMValueRef *Args=NULL, ret_val=0, calledf;
  LLVMTypeRef calledf_type;
  int nargs = 0;
  Node *globid = n->node_exp.globid;
  Node *exps = n->node_exp.exps;
  char *funcname = globid->node_globid.funcname;

  // Find the called function and type
  find_fentry(ft, funcname, &calledf);
  find_ftype(ft, funcname, &calledf_type);

  // Check the return type of the function
  LLVMTypeRef ret_type = LLVMGetReturnType(calledf_type);
  // Get the types of the argument
  LLVMTypeRef *args_types = calloc(LLVMCountParamTypes(calledf_type), sizeof(LLVMTypeRef));
  // Get the parameters of the function
  LLVMGetParamTypes(calledf_type, args_types);

  // Generate the arguments
  generate_callargs(builder, exps, ft, vt, level, function, &Args, &nargs, args_types);

  // Check the number of arguments is correct
  // printf("%d\n", nargs);


  // Call the function
  if (ret_type == LLVMVoidType()) {
    ret_val = LLVMBuildCall(builder, calledf, Args, nargs, "");
  } else {
    ret_val = LLVMBuildCall(builder, calledf, Args, nargs, funcname);
  }

  // Housekeeping
  free(Args);
  free(args_types);

  return ret_val;
}

/* Generate a return value */
static void generate_callargs(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function, LLVMValueRef **Args, int *nargs, LLVMTypeRef *args_types) {
  if (n==NULL)  return;

  assert(n->tag == T_EXPS || n->tag == T_EXP);

  switch (n->tag) {
    case T_EXPS:
      generate_callargs(builder, n->node_exps.left, ft, vt, level, function, Args, nargs, args_types);
      generate_callargs(builder, n->node_exps.right, ft, vt, level, function, Args, nargs, args_types);
      break;
    case T_EXP:
      if (*Args == NULL) {
        *Args = calloc(++(*nargs), sizeof(LLVMValueRef));
      } else {
        LLVMValueRef *new_Args = calloc(++(*nargs), sizeof(LLVMValueRef));
        for (int i=0; i<(*nargs); ++i) {
          new_Args[i] = (*Args)[i];
        }
        free(*Args);
        *Args = new_Args;
        // *Args = realloc(*Args, ++(*nargs) * sizeof(LLVMValueRef));
      }
      // printf("get argument %d\n", *nargs);

      // Check if the argument is a ref type
      // Begin pulling information
      if (LLVMGetTypeKind(args_types[*nargs-1]) == LLVMPointerTypeKind) {
        Node *var = n->node_exp.var;
        (*Args)[*nargs-1] = generate_varptr(builder, var, ft, vt, level);
      } else {
        (*Args)[*nargs-1] = generate_expression(builder, n, ft, vt, level, function);
      }
      break;
    default:
      fprintf(stderr, "Unknown node encountered at function call.\n");
  }
  // end of function
}

/* Find the variable in the variable table by the given value */
static LLVMValueRef generate_var(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
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
static LLVMValueRef generate_binop(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  assert(n->tag == T_BINOP);

  LLVMValueRef ret_val = 0;
  LLVMValueRef l_val = 0;
  LLVMValueRef r_val = 0;
  LLVMTypeRef l_type, r_type;
  LLVMValueRef bool1, bool2;
  LLVMBasicBlockRef br1, br2, end;
  LLVMValueRef cmp_result;
  // cint flag
  int cint_flag = 0;


  // Get the value and type of the left hand
  switch (n->node_binop.op) {
    case BINOP_ASSIGN:
      // l_val = generate_var(builder, n->node_binop.var, ft, vt, level);
      l_val = LLVMConstInt(LLVMInt32Type(), 0, 0);
      break;
    default:
      l_val = generate_expression(builder, n->node_binop.exp_left, ft, vt, level, function);
  }

  l_type = LLVMTypeOf(l_val);
  // Get the value and type of the right hand
  r_val = generate_expression(builder, n->node_binop.exp_right, ft, vt, level, function);
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
  if (l_type != r_type && n->node_binop.op != BINOP_ASSIGN) {
    fprintf(stderr, "runtime time error (assign).\n");
    exit(-1);
  }

  // Check cint type (flag)
  if (n->node_binop.op != BINOP_ASSIGN) {
    Node *node_l = n->node_binop.exp_left;
    Node *node_r = n->node_binop.exp_right;
    if (node_l->node_exp.exp_type == CINT &&
        node_r->node_exp.exp_type == CINT)
        cint_flag = 1;
  }

  // Compile LLVM IR
  if (l_type == LLVMInt32Type()) {
    // Case Int
    switch (n->node_binop.op) {
      case BINOP_ASSIGN:
        ret_val = assign_variable(builder, n, ft, vt, level, function);
        break;
      case BINOP_ADD:
        ret_val = LLVMBuildNSWAdd(builder, l_val, r_val, "add");
        /* Add routine to check binary additions */
        if (cint_flag) {
          // Store the return value in a pointer
          LLVMValueRef ptr = LLVMBuildAlloca(builder, LLVMInt32Type(), "ptr");
          LLVMBuildStore(builder, ret_val, ptr);
          // Call the function cint_negation
          LLVMValueRef calledf;
          char function[] = "cint_addition";
          // Find the called function and type
          find_fentry(ft, function, &calledf);
          // Set call arguments
          LLVMValueRef args[] = {l_val, r_val, ptr};
          // LLVMValueRef args[] = {val};
          LLVMBuildCall(builder, calledf, args, 3, "");
        }
        break;
      case BINOP_MUL:
        ret_val = LLVMBuildMul(builder, l_val, r_val, "mul");
        /* Add routine to check binary multiplications */
        if (cint_flag) {
          // Store the return value in a pointer
          LLVMValueRef ptr = LLVMBuildAlloca(builder, LLVMInt32Type(), "ptr");
          LLVMBuildStore(builder, ret_val, ptr);
          // Call the function cint_negation
          LLVMValueRef calledf;
          char function[] = "cint_multiplication";
          // Find the called function and type
          find_fentry(ft, function, &calledf);
          // Set call arguments
          LLVMValueRef args[] = {l_val, r_val, ptr};
          // LLVMValueRef args[] = {val};
          LLVMBuildCall(builder, calledf, args, 3, "");
        }
        break;
      case BINOP_SUB:
        ret_val = LLVMBuildSub(builder, l_val, r_val, "sub");
        /* Add routine to check binary subtractions */
        if (cint_flag) {
          // Store the return value in a pointer
          LLVMValueRef ptr = LLVMBuildAlloca(builder, LLVMInt32Type(), "ptr");
          LLVMBuildStore(builder, ret_val, ptr);
          // Call the function cint_negation
          LLVMValueRef calledf;
          char function[] = "cint_subtraction";
          // Find the called function and type
          find_fentry(ft, function, &calledf);
          // Set call arguments
          LLVMValueRef args[] = {l_val, r_val, ptr};
          // LLVMValueRef args[] = {val};
          LLVMBuildCall(builder, calledf, args, 3, "");
        }
        break;
      case BINOP_DIV:
        ret_val = LLVMBuildSDiv(builder, l_val, r_val, "div");
        /* Add routine to check binary division */
        if (cint_flag) {
          // Store the return value in a pointer
          LLVMValueRef ptr = LLVMBuildAlloca(builder, LLVMInt32Type(), "ptr");
          LLVMBuildStore(builder, ret_val, ptr);
          // Call the function cint_negation
          LLVMValueRef calledf;
          char function[] = "cint_division";
          // Find the called function and type
          find_fentry(ft, function, &calledf);
          // Set call arguments
          LLVMValueRef args[] = {l_val, r_val, ptr};
          // LLVMValueRef args[] = {val};
          LLVMBuildCall(builder, calledf, args, 3, "");
        }
        break;
      case BINOP_EQ:
        ret_val = LLVMBuildICmp(builder, LLVMIntEQ, l_val, r_val, "icmp");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      case BINOP_LT:
        ret_val = LLVMBuildICmp(builder, LLVMIntSLT, l_val, r_val, "icmp");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      case BINOP_GT:
        ret_val = LLVMBuildICmp(builder, LLVMIntSGT, l_val, r_val, "icmp");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      case BINOP_AND:
        // Create a variable to store result (cmp_result: Int1)
        cmp_result = LLVMBuildAlloca(builder, LLVMInt1Type(), "cmp_ptr");
        LLVMBuildStore(builder, LLVMConstInt(LLVMInt1Type(), 0, 0), cmp_result);
        // Master branch
        bool1 = LLVMBuildICmp(builder, LLVMIntNE, l_val, LLVMConstInt(LLVMInt32Type(), 0, 0), "tobool");
        br1 = LLVMAppendBasicBlock(function, "and(br1)");
        br2 = LLVMAppendBasicBlock(function, "and(br2)");
        end = LLVMAppendBasicBlock(function, "and(end)");
        LLVMBuildCondBr(builder, bool1, br1, end);
        // Branch br1
        LLVMPositionBuilderAtEnd(builder, br1);
        bool2 = LLVMBuildICmp(builder, LLVMIntNE, r_val, LLVMConstInt(LLVMInt32Type(), 0, 0), "tobool");
        LLVMBuildCondBr(builder, bool2, br2, end);
        // Branch br2
        LLVMPositionBuilderAtEnd(builder, br2);
        LLVMBuildStore(builder, LLVMConstInt(LLVMInt1Type(), 1, 0), cmp_result);
        LLVMBuildBr(builder, end);
        // Branch end
        LLVMPositionBuilderAtEnd(builder, end);
        ret_val = LLVMBuildLoad(builder, cmp_result, "cmp_value");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      case BINOP_OR:
        // Create a variable to store result (cmp_result: Int1)
        cmp_result = LLVMBuildAlloca(builder, LLVMInt1Type(), "cmp_ptr");
        LLVMBuildStore(builder, LLVMConstInt(LLVMInt1Type(), 1, 0), cmp_result);
        // Master branch
        bool1 = LLVMBuildICmp(builder, LLVMIntNE, l_val, LLVMConstInt(LLVMInt32Type(), 0, 0), "tobool");
        br1 = LLVMAppendBasicBlock(function, "or(br1)");
        br2 = LLVMAppendBasicBlock(function, "or(br2)");
        end = LLVMAppendBasicBlock(function, "or(end)");
        LLVMBuildCondBr(builder, bool1, end, br1);
        // Branch br1
        LLVMPositionBuilderAtEnd(builder, br1);
        bool2 = LLVMBuildICmp(builder, LLVMIntNE, r_val, LLVMConstInt(LLVMInt32Type(), 0, 0), "tobool");
        LLVMBuildCondBr(builder, bool2, end, br2);
        // Branch br2
        LLVMPositionBuilderAtEnd(builder, br2);
        LLVMBuildStore(builder, LLVMConstInt(LLVMInt1Type(), 0, 0), cmp_result);
        LLVMBuildBr(builder, end);
        // Branch end
        LLVMPositionBuilderAtEnd(builder, end);
        ret_val = LLVMBuildLoad(builder, cmp_result, "cmp_value");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
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
        ret_val = LLVMBuildFCmp(builder, LLVMRealUEQ, l_val, r_val, "cmp");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      case BINOP_LT:
        ret_val = LLVMBuildFCmp(builder, LLVMRealULT, l_val, r_val, "cmp");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      case BINOP_GT:
        ret_val = LLVMBuildFCmp(builder, LLVMRealUGT, l_val, r_val, "cmp");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      case BINOP_AND:
        // Create a variable to store result (cmp_result: Int1)
        cmp_result = LLVMBuildAlloca(builder, LLVMInt1Type(), "cmp_ptr");
        LLVMBuildStore(builder, LLVMConstInt(LLVMInt1Type(), 0, 0), cmp_result);
        // Master branch
        bool1 = LLVMBuildFCmp(builder, LLVMRealUNE, l_val, LLVMConstReal(LLVMFloatType(), 0), "tobool");
        br1 = LLVMAppendBasicBlock(function, "fand(br1)");
        br2 = LLVMAppendBasicBlock(function, "fand(br2)");
        end = LLVMAppendBasicBlock(function, "fand(end)");
        LLVMBuildCondBr(builder, bool1, br1, end);
        // Branch br1
        LLVMPositionBuilderAtEnd(builder, br1);
        bool2 = LLVMBuildFCmp(builder, LLVMRealUNE, r_val, LLVMConstReal(LLVMFloatType(), 0), "tobool");
        LLVMBuildCondBr(builder, bool2, br2, end);
        // Branch br2
        LLVMPositionBuilderAtEnd(builder, br2);
        LLVMBuildStore(builder, LLVMConstInt(LLVMInt1Type(), 1, 0), cmp_result);
        LLVMBuildBr(builder, end);
        // Branch end
        LLVMPositionBuilderAtEnd(builder, end);
        ret_val = LLVMBuildLoad(builder, cmp_result, "cmp_value");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      case BINOP_OR:
        // Create a variable to store result (cmp_result: Int1)
        cmp_result = LLVMBuildAlloca(builder, LLVMInt1Type(), "cmp_ptr");
        LLVMBuildStore(builder, LLVMConstInt(LLVMInt1Type(), 1, 0), cmp_result);
        // Master branch
        bool1 = LLVMBuildFCmp(builder, LLVMRealUNE, l_val, LLVMConstReal(LLVMFloatType(), 0), "tobool");
        br1 = LLVMAppendBasicBlock(function, "for(br1)");
        br2 = LLVMAppendBasicBlock(function, "for(br2)");
        end = LLVMAppendBasicBlock(function, "for(end)");
        LLVMBuildCondBr(builder, bool1, end, br1);
        // Branch br1
        LLVMPositionBuilderAtEnd(builder, br1);
        bool2 = LLVMBuildFCmp(builder, LLVMRealUNE, r_val, LLVMConstReal(LLVMFloatType(), 0), "tobool");
        LLVMBuildCondBr(builder, bool2, end, br2);
        // Branch br2
        LLVMPositionBuilderAtEnd(builder, br2);
        LLVMBuildStore(builder, LLVMConstInt(LLVMInt1Type(), 0, 0), cmp_result);
        LLVMBuildBr(builder, end);
        // Branch end
        LLVMPositionBuilderAtEnd(builder, end);
        ret_val = LLVMBuildLoad(builder, cmp_result, "cmp_value");
        ret_val = LLVMBuildZExt(builder, ret_val, LLVMInt32Type(), "cmp_value");
        break;
      default:
        printf("undefined.\n");
        break;
    }
  }

  return ret_val;
}

/* Assign the value to a variable */
static LLVMValueRef assign_variable(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  LLVMValueRef ret_val = 0;
  LLVMValueRef exp_val = 0;
  // printf("activated\n");
  exp_val = generate_expression(builder, n->node_binop.exp_right, ft, vt, level, function);
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

  // Compare the types
  LLVMTypeRef exp_type = LLVMTypeOf(exp_val);
  LLVMValueRef var_v = LLVMBuildLoad(builder, ptr, "varval");
  LLVMTypeRef var_type = LLVMTypeOf(var_v);

  if (var_type != exp_type) {
    if (exp_type == LLVMFloatType()) {
      // Convert float to int
      exp_val = LLVMBuildFPToSI(builder, exp_val, LLVMInt32Type(), "conv");
    } else if (exp_type == LLVMInt32Type()) {
      exp_val = LLVMBuildSIToFP(builder, exp_val, LLVMFloatType(), "conv");
    }
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
static LLVMValueRef generate_uop(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  assert(n->tag == T_UOP);

  LLVMValueRef ret_val = 0, flag_bool, lnot, lnot_ext;
  LLVMValueRef val = generate_expression(builder, n->node_uop.exp, ft, vt, level, function);
  LLVMTypeRef vtype = LLVMTypeOf(val);

  if (vtype == LLVMInt32Type()) {
    // cint flag (check cint overflow)
    int cint_flag = 0;
    // Check the type of the expression
    Node *u_exp = n->node_uop.exp;
    if (u_exp->node_exp.exp_type == CINT)
      cint_flag = 1;

    switch (n->node_uop.op) {
      case UOP_MINUS:
        ret_val = LLVMBuildSub(builder, LLVMConstInt(LLVMInt32Type(), 0, 0), val, "sub");
        /* Add routine for checking unary negations */
        if (cint_flag) {
          // Store the return value in a pointer
          LLVMValueRef ptr = LLVMBuildAlloca(builder, LLVMInt32Type(), "ptr");
          LLVMBuildStore(builder, ret_val, ptr);
          // Call the function cint_negation
          LLVMValueRef calledf;
          char function[] = "cint_negation";
          // Find the called function and type
          find_fentry(ft, function, &calledf);
          // Set call arguments
          LLVMValueRef args[] = {val, ptr};
          // LLVMValueRef args[] = {val};
          LLVMBuildCall(builder, calledf, args, 2, "");
        }
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

/* Generate a if Statement */
static void generate_if_statement(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  // Nodes
  Node *exp=NULL, *stmt1=NULL, *stmt2=NULL;
  // Comparison Flag
  LLVMValueRef bool1;
  LLVMBasicBlockRef br1, br2=NULL, end;
  LLVMValueRef terminator = NULL;
  int br1_end=0, br2_end=0;

  // printf("activated\n");
  // Judge if it is a open or closed statement, and pull out the children
  if (n->tag == T_OPEN_STMT) {
    exp = n->node_open_stmt.exp;
    stmt1 = n->node_open_stmt.stmt1;
    stmt2 = n->node_open_stmt.stmt2;
  } else if (n->tag == T_CLOSED_STMT) {
    exp = n->node_closed_stmt.exp;
    stmt1 = n->node_closed_stmt.stmt1;
    stmt2 = n->node_closed_stmt.stmt2;
  } else {
    fprintf(stderr, "error: Invalid call of <if>.\n");
  }

  // printf("yes!\n");

  // Create the basic blocks
  // The branches depend on whether there is a second (else) statement
  br1 = LLVMAppendBasicBlock(function, "if_yes");
  if (stmt2 != NULL)  br2 = LLVMAppendBasicBlock(function, "if_no");
  end = LLVMAppendBasicBlock(function, "if_end");

  // Generate the condition value: bool1
  LLVMValueRef exp_val = generate_expression(builder, exp, ft, vt, level, function);
  LLVMTypeRef exp_type = LLVMTypeOf(exp_val);
  if (exp_type == LLVMInt32Type()) {
    bool1 = LLVMBuildICmp(builder, LLVMIntNE, exp_val, LLVMConstInt(LLVMInt32Type(), 0, 0), "tobool");
  } else {
    bool1 = LLVMBuildFCmp(builder, LLVMRealUNE, exp_val, LLVMConstReal(LLVMFloatType(), 0), "tobool");
  }

  // Create branching
  if (stmt2 != NULL) {
    LLVMBuildCondBr(builder, bool1, br1, br2);
  } else {
    LLVMBuildCondBr(builder, bool1, br1, end);
  }

  // Generate each statement
  LLVMPositionBuilderAtEnd(builder, br1);
  generate_statement_T(builder, stmt1, ft, vt, level, function);
  // Check the terminator
  terminator = LLVMGetBasicBlockTerminator(br1);
  if (terminator == NULL) {
    LLVMBuildBr(builder, end);
  } else {
    br1_end = 1;
  }


  if (stmt2 != NULL) {
    LLVMPositionBuilderAtEnd(builder, br2);
    generate_statement_T(builder, stmt2, ft, vt, level, function);
    // Check the terminator
    terminator = LLVMGetBasicBlockTerminator(br2);
    if (terminator == NULL){
      LLVMBuildBr(builder, end);
    } else {
      br2_end = 1;
    }
  }

  if (br1_end && br2_end) {
    LLVMDeleteBasicBlock(end);
  } else {
    // Point builder to the end
    LLVMPositionBuilderAtEnd(builder, end);
  }

  return;
}

/* Generate a while statement */
static void generate_while_statement(LLVMBuilderRef builder, Node *n, F_table *ft, V_table *vt, unsigned int level, LLVMValueRef function) {
  // Nodes
  Node *exp=NULL, *stmt=NULL;
  // Comparison Flag
  LLVMValueRef bool1;
  LLVMBasicBlockRef cond, loop, end;

  // Judge if it is a open or closed statement, and pull out the children
  if (n->tag == T_OPEN_STMT) {
    exp = n->node_open_stmt.exp;
    stmt = n->node_open_stmt.stmt1;
  } else if (n->tag == T_CLOSED_STMT) {
    exp = n->node_closed_stmt.exp;
    stmt = n->node_closed_stmt.stmt1;
  } else {
    fprintf(stderr, "error: Invalid call of <while>.\n");
  }

  // Create the basic blocks
  cond = LLVMAppendBasicBlock(function, "while_cond");
  loop = LLVMAppendBasicBlock(function, "while_loop");
  end = LLVMAppendBasicBlock(function, "while_end");

  // Go to cond branch first
  LLVMBuildBr(builder, cond);

  // Build cond
  LLVMPositionBuilderAtEnd(builder, cond);
  LLVMValueRef exp_val = generate_expression(builder, exp, ft, vt, level, function);
  LLVMTypeRef exp_type = LLVMTypeOf(exp_val);
  if (exp_type == LLVMInt32Type()) {
    bool1 = LLVMBuildICmp(builder, LLVMIntNE, exp_val, LLVMConstInt(LLVMInt32Type(), 0, 0), "tobool");
  } else {
    bool1 = LLVMBuildFCmp(builder, LLVMRealUNE, exp_val, LLVMConstReal(LLVMFloatType(), 0), "tobool");
  }
  // Branching
  LLVMBuildCondBr(builder, bool1, loop, end);

  // Build loop
  LLVMPositionBuilderAtEnd(builder, loop);
  generate_statement_T(builder, stmt, ft, vt, level, function);
  LLVMBuildBr(builder, cond);

  // Build end
  LLVMPositionBuilderAtEnd(builder, end);

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
// LLVMValueRef generate_string(char *str) {
//   unsigned int length = strlen(str);
//   return LLVMConstString(str, length, 1);
// }
