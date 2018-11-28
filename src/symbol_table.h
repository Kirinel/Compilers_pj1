#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "def.h"
#include <stddef.h>

#define INIT_SIZE 16
#define P_NULL    (-1)
#define SUCCESS   (0)
#define VAR_DUP   (11)
#define FUNC_DUP  (12)
#define NOT_FOUND (13)

/* hash_t (hash value) is unsigned long int
 */
typedef unsigned long int hash_t;

/* hash: The main hash function used in this assignment
 *  args:     char pointer (string)
 *  returns:  hash value (unsigned long int)
 */
hash_t hash(char *);

/* The entry of VAR_table
 */
typedef struct var_entry {
  hash_t hash_code; // The hash_code
  size_t var_id; // The id of the variable
  char *varname; // (key) The name of the variable
  enum type_tag vartype; // The type of the variable (include the ref type)
  enum exp_type valtype;
  double varval; // The value of the variable
  struct var_entry *ref; // The target variable being referenced
  struct var_entry *noalias_ref; // The noalias_ref variable to this variable
  Node *vdecl; // The node when the variable get declared
  unsigned int level; // The scope of the variable
  struct var_entry *next; // The next variable
} VAR_entry;

/* The symbol table to store the local variables for a function
 */
typedef struct var_table {
  size_t num_bucs; // num of buckets in the hash table
  size_t num_vars; // num of variables in the hash table
  char *func_name; // The function name of the hash table
  VAR_entry **bucs; // The table
} VAR_table;

/* Variable table APIs
 */

/* init_var_table: Initialize a variable table.
 *  args:
 *    buckets: number of initial buckets
 *    funcname: the function name (where all variables are defined)
 *  returns:
 *    a pointer to the newly created variable table
 */
VAR_table *init_var_table(int buckets, char *funcname);
/* free_var_table: Free the space occupied by the variable table
 *  Free each variable entry
 *  Free the function name string
 *  Free the array of pointers
 *  Free the table
 */
void free_var_table(VAR_table *t);
/* new_var_entry: Create a new variable entry
 *  args:
 *    hash_code: The hash code of the function
 *    var_id: The variable id
 *    varname:  The variable name (as a string)
 *    vartype: The declared type of the varialbe
 *    varval: The value of the variable, if the variable is not ref type
 *    ref:  The variable to be referenced (pointed to the function entry)
 *    noalias_ref: The variable's noalias reference (can only have one)
 *    vdecl: The variable's declared node
 *    next: The next local variable entry.
 */
 VAR_entry *new_var_entry(hash_t hast_code, size_t var_id, char *varname,
                           enum type_tag vartype, double varval, VAR_entry *ref, Node *vdecl, unsigned int level);
/* add_table_local_var: Add the local variable to the variable table
*  args:
*    t: the pointer to the variable table
*    varname: The name of the variable (string)
*    vdecl: The <vdecl> node
*    refname: The name of the variable being referenced
*    varval: The value of the variable
*  returns:
*    an error code (values other than 0 are abnormal executions).
*/
int add_table_local_var(VAR_table *t, char *varname, Node *vdecl, char *refname, double varval, unsigned int level);
/* find_local_var: Find the local variable with the given variable name
 *  args:
 *    t: The variable table
 *    varname: The name of the variable
 *    entry_p: out paramter (variable entry)
 *  returns:
 *    error code: nonzero values signify failure
 */
int find_local_var(VAR_table *, char *, VAR_entry **, unsigned int);

/* Free the variables when exiting a block
 *  t: the pointer to the variable table
 *  level: the level of the variables to be deleted.
 */
void delete_var_bylevel(VAR_table *t, unsigned int level);


/* The entry of FUNC_table
 */
typedef struct function_entry {
  hash_t hash_code; // The hash_code
  size_t func_id; // The id of the function
  char *funcname; // (key) The name of the function
  enum exp_type ret_type; // The return type of the function, must not be a <ref> type
  Node *func_node; // The node of the function (when it gets declared)
  VAR_table *local_vars; // The local variables of this function
  struct function_entry *next; // The next function entry
} FUNC_entry;

/* The symbol table to store all the function names are relevant information
 */
typedef struct function_table {
  size_t num_bucs; // num of buckets in the hash table
  size_t num_funcs; // num of functions in the hash table
  FUNC_entry **bucs; // The table
} FUNC_table;

/* Function table APIs
 */

/* init_func_table: Create a function table
 *  One source file can only have one function table
 *    args:
 *      buckets: The number of buckets
 *    returns:
 *      The pointer to the newly created function table
 */
FUNC_table *init_func_table(int);
/* free_func_table: Free the space occupied by the function table
 *  args:
 *    t: The pointer to the function table
 *  returns:
 *    void
 */
void free_func_table(FUNC_table *);
/* new_func_entry: Add a new entry to the function table
 *  args:
 *    hash_code: Hash value of the function
 *    id: The id of the function
 *    funcname: The name of the function
 *    ret_type: The return type of the function
 *    func_node:  The node of the function at the AST
 *  returns:
 *    a pointer to the function entry (in the table)
 */
FUNC_entry *new_func_entry(hash_t hash_code, size_t id, char *funcname, enum exp_type ret_type, Node *func_node);
/* add_table_func: Add a function to the table
 *  args:
 *    t: The pointer to the function table
 *    funcname: The name of the function
 *    n: The node of the function in the AST
 */
int add_table_func(FUNC_table *t, char *funcname, Node *n);
/* find_func: Find the local variable with the given variable name
 *  args:
 *    t: The pointer to the func table
 *    funcname: The name of the function
 *    entry_p: out paramter (function entry)
 *  returns:
 *    error code: nonzero values signify failure
 */
int find_func(FUNC_table *, char *, FUNC_entry **);

#endif
