#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "ast.h"

typedef unsigned long int hash_t;
/* the main hash function */
hash_t hash(char *);

/* The entry of VAR_table */
typedef struct var_entry {
  size_t var_id;
  char *name;
  char *type;
  hash_t hash_code;
  node *vdecl;
  struct var_entry *next;
} VAR_entry;
/* The symbol table to store the local variables for a function */
typedef struct var_table {
  size_t num_bucs;
  size_t num_vars;
  char *func_name;
  VAR_entry **bucs;
} VAR_table;

VAR_table *init_var_table(int, char *);
VAR_entry *new_var_entry(char *, char *, hash_t, node *, size_t);
int add_table_local_var(VAR_table *, char *, char *, node *);
void free_var_table(VAR_table *);
int find_local_var(VAR_table *, char *, VAR_entry **);

/* The entry of FUNC_table */
typedef struct function_entry {
  size_t func_id;
  char *globid;
  char *ret_type;
  hash_t hash_code;
  node *func_node;
  VAR_table *local_vars;
  struct function_entry *next;
} FUNC_entry;
/* The symbol table to store all the function names are relevant information */
typedef struct function_table {
  size_t num_bucs;
  size_t num_funcs;
  FUNC_entry **bucs;
} FUNC_table;

FUNC_table *init_func_table(int);
FUNC_entry *new_func_entry(char *, char *, hash_t, node *, size_t);
int add_table_func(FUNC_table *, char *, char *, node *);
void free_func_table(FUNC_table *);
int find_func(FUNC_table *, char *, FUNC_entry **);


#endif
