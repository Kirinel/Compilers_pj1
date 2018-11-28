#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

/* hash: The main hash function used in this assignment
 *  args:     char pointer (string)
 *  returns:  hash value (unsigned long int)
 */
hash_t hash(char *s)
{
  unsigned long hash_v = 5381;
  int c;

  while ((c = *s++)) {
    hash_v = ((hash_v << 5) + hash_v) + c; /* hash_v * 33 + c */
  }
  return hash_v;
}

/* Variable table APIs
 */

/* init_var_table: Initialize a variable table.
 *  args:
 *    buckets: number of initial buckets
 *    funcname: the function name (where all variables are defined)
 *  returns:
 *    a pointer to the newly created variable table
 */
VAR_table *init_var_table(int buckets, char *funcname)
{
 // allocate table memory
 VAR_table *v = calloc(1, sizeof(VAR_table));
 v->num_bucs = buckets;
 // vt->num_vars = 0;
 v->func_name = strdup(funcname);

 // allocate buckets
 v->bucs = calloc(buckets, sizeof(VAR_entry *));

 return v;
}

/* free_var_table: Free the space occupied by the variable table
 *  Free each variable entry
 *  Free the function name string
 *  Free the array of pointers
 *  Free the table
 */
void free_var_table(VAR_table *t)
{
 VAR_entry *curr=NULL, *next=NULL;

 for (size_t i=0; i<t->num_bucs; ++i) {
   curr = t->bucs[i];
   //if (curr==NULL) continue;

   while (curr != NULL) {
     next = curr->next;
     free(curr->varname);
     free(curr);
     curr = next;
   }
 }

 free(t->bucs);
 free(t->func_name);
 free(t);

 return;
}

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
                          enum type_tag vartype, double varval, VAR_entry *ref, Node *vdecl, unsigned int level)
{
  VAR_entry *entry = calloc(1, sizeof(VAR_entry));
  entry->hash_code = hast_code;
  entry->var_id = var_id;
  entry->varname = strdup(varname);
  entry->vartype = vartype;
  entry->valtype = type_to_exptype(vdecl->node_vdecl.type);
  entry->varval = varval;
  entry->ref = ref;
  // entry->noalias_ref = NULL;
  // No need to assign this at initiation
  entry->vdecl = vdecl;
  entry->level = level;
  // next defaults to NULL
  return entry;
}

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
int add_table_local_var(VAR_table *t, char *varname, Node *vdecl, char *refname, double varval, unsigned int level)
{
  // NULL table pointer, return error
  if (t==NULL)  return P_NULL;

  // Parse the vdecl node to get enough information
  assert(vdecl->tag == T_VDECL);
  enum type_tag vartype = vdecl->node_vdecl.type->node_type.type_tag;
  // search the referenc node
  VAR_entry *ref_node = NULL;
  if (refname != NULL) {
    assert( find_local_var(t, varname, &ref_node, level) == SUCCESS );
  }

  // Hash the varname and find the target bucket, assign the head pointer
  hash_t hash_value = hash(varname);
  size_t target_buc = hash_value % t->num_bucs;
  VAR_entry *curr = t->bucs[target_buc];

  // If head pointer is NULL, insert at head pointer
  if (curr == NULL) {
    t->bucs[target_buc] = new_var_entry(hash_value, ++t->num_vars, varname, vartype, varval, ref_node, vdecl, level);
    return SUCCESS;
  }

  while (curr->next != NULL) {
    //check duplicate varnames, return an error code if the variable has already been declared in the hash table.
    if (!strcmp(curr->varname, varname) && curr->level==level) {
      return VAR_DUP;
    }
    curr = curr->next;
  }

  //check duplicate varnames
  if (!strcmp(curr->varname, varname) && curr->level==level) {
    // fprintf(stderr, "error: local variable (%s) has already been declared.\n", name);
    return VAR_DUP;
  }
  //find the last case and insert;
  curr->next = new_var_entry(hash_value, ++t->num_vars, varname, vartype, varval, ref_node, vdecl, level);
  return SUCCESS;
}

/* find_local_var: Find the local variable with the given variable name
 *  args:
 *    t: The variable table
 *    varname: The name of the variable
 *    entry_p: out paramter (variable entry)
 *  returns:
 *    error code: nonzero values signify failure
 */
int find_local_var(VAR_table *t, char *varname, VAR_entry **entry_p, unsigned int level)
{
 //find the bucket
 hash_t hash_value = hash(varname);
 size_t target_buc = hash_value % t->num_bucs;

 for (int l=level; l>=0; --l) {
   //start iterating to find the entry
   VAR_entry *curr = t->bucs[target_buc];

   if (curr == NULL) return NOT_FOUND;

   while (curr != NULL) {
     //check duplicate globids
     // printf("%s %d %s %d\n", varname, l, `curr->varname, curr->level);
     if (!strcmp(curr->varname, varname) && l==curr->level) {
       *entry_p = curr;
       return SUCCESS;
     }

     curr = curr->next;
   }
 }

 return NOT_FOUND;
}

/* Function table APIs
 */

/* init_func_table: Create a function table
 *  One source file can only have one function table
 *    args:
 *      buckets: The number of buckets
 *    returns:
 *      The pointer to the newly created function table
 */
FUNC_table *init_func_table(int buckets)
{
 // allocate table memory
 FUNC_table *f = calloc(1, sizeof(FUNC_table));
 f->num_bucs = buckets;
 // allocate buckets
 f->bucs = calloc(buckets, sizeof(FUNC_entry *));

 return f;
}

/* free_func_table: Free the space occupied by the function table
 *  args:
 *    t: The pointer to the function table
 *  returns:
 *    void
 */
void free_func_table(FUNC_table *t)
{
  FUNC_entry *curr=NULL, *next=NULL;

  for (size_t i=0; i<t->num_bucs; ++i) {
    curr = t->bucs[i];
    //if (curr==NULL) continue;

    while (curr != NULL) {
      next = curr->next;
      free(curr->funcname);
      free_var_table(curr->local_vars);
      free(curr);
      curr = next;
    }

    free(t->bucs);
    free(t);

    return;

  }
}

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
FUNC_entry *new_func_entry(hash_t hash_code, size_t id, char *funcname, enum exp_type ret_type, Node *func_node)
{
  FUNC_entry *entry = calloc(1, sizeof(FUNC_entry));
  entry->hash_code = hash_code;
  entry->func_id = id;
  entry->funcname = strdup(funcname);
  entry->ret_type = ret_type;
  entry->func_node = func_node;
  entry->local_vars = init_var_table(INIT_SIZE, funcname);
  //next defaults to NULL
  return entry;
}

/* add_table_func: Add a function to the table
 *  args:
 *    t: The pointer to the function table
 *    funcname: The name of the function
 *    n: The node of the function in the AST
 */
int add_table_func(FUNC_table *t, char *funcname, Node *n)
{
  // NULL table pointer, return error
  if (t==NULL)  return P_NULL;

  // Generate the necessary information
  assert(n->tag == T_FUNC || n->tag == T_EXTERN);
  enum exp_type ret_type = type_to_exptype(n->node_func.type);

  // Determine the buc and assign the head pointer
  hash_t hash_value = hash(funcname);
  size_t target_buc = hash_value % t->num_bucs;

  //start iterating to find the insertion place
  FUNC_entry *curr = t->bucs[target_buc];

  if (curr == NULL) {
    t->bucs[target_buc] = new_func_entry(hash_value, ++t->num_funcs, funcname, ret_type, n);
    return SUCCESS;
  }

  while (curr->next != NULL) {
    //check duplicate funcnames
    if (!strcmp(curr->funcname, funcname)) {
      return FUNC_DUP;
    }

    curr = curr->next;
  }


  //check duplicate globids
  if (!strcmp(curr->funcname, funcname)) {
    // fprintf(stderr, "error: function (%s) has already been declared (code: 007).\n", globid);
    return FUNC_DUP;
  }
  //find the last case and insert;
  curr->next = new_func_entry(hash_value, ++t->num_funcs, funcname, ret_type, n);
  return SUCCESS;
}

/* find_func: Find the local variable with the given variable name
 *  args:
 *    t: The pointer to the func table
 *    funcname: The name of the function
 *    entry_p: out paramter (function entry)
 *  returns:
 *    error code: nonzero values signify failure
 */
int find_func(FUNC_table *t, char *funcname, FUNC_entry **entry_p)
{
  //find the bucket
  hash_t hash_value = hash(funcname);
  size_t target_buc = hash_value % t->num_bucs;
  //start iterating to find the entry
  FUNC_entry *curr = t->bucs[target_buc];

  if (curr == NULL) return NOT_FOUND;

  while (curr != NULL) {
    //check duplicate globids
    if (!strcmp(curr->funcname, funcname)) {
      *entry_p = curr;
      return SUCCESS;
    }

    curr = curr->next;
  }

  return NOT_FOUND;
}

/* Free the variables by each level
 *  t: the pointer to the variable table
 *  level: the level of the variables
 */
void delete_var_bylevel(VAR_table *t, unsigned int level) {
  // printf("free %d\n", level);
  VAR_entry *curr=NULL, *tmp=NULL;

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
