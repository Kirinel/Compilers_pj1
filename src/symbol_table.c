#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

#define VARSPACE 16

hash_t hash(char *s)
{
  unsigned long hash_v = 5381;
  int c;

  while ((c = *s++)) {
    hash_v = ((hash_v << 5) + hash_v) + c; /* hash_v * 33 + c */
  }
  return hash_v;
}

/* Variable table APIs */
VAR_table *init_var_table(int buckets, char *funcname)
{
  // allocate table memory
  VAR_table *v = calloc(1, sizeof(VAR_table));
  v->num_bucs = buckets;
  // allocate buckets
  v->bucs = calloc(buckets, sizeof(VAR_entry *));
  v->func_name = funcname;

  return v;
}

VAR_entry *new_var_entry(char *name, char *type, hash_t hast_code, node *n, size_t id)
{
  VAR_entry *entry = calloc(1, sizeof(VAR_entry));
  entry->var_id = id;
  entry->name = strdup(name);
  entry->type = strdup(type);
  entry->hash_code = hast_code;
  entry->vdecl = n;
  // next defaults to NULL
  return entry;
}

int add_table_local_var(VAR_table *t, char *name, char *type, node *n)
{
  //NULL table pointer, return error
  if (t==NULL)  return -1;

  //globid is key
  hash_t hash_value = hash(name);
  //check which one to go
  size_t target_buc = hash_value % t->num_bucs;

  //start iterating to find the insertion place
  VAR_entry *curr = t->bucs[target_buc];

  if (curr == NULL) {
    t->bucs[target_buc] = new_var_entry(name, type, hash_value, n, ++t->num_vars);
    return 0;
  }

  while (curr->next != NULL) {
    //check duplicate varnames
    if (!strcmp(curr->name, name)) {
      return -1;
    }

    curr = curr->next;
  }

  //check duplicate varnames
  if (!strcmp(curr->name, name)) {
    //fprintf(stderr, "error: local variable (%s) has already been declared.\n", name);
    return -1;
  }
  //find the last case and insert;
  curr->next = new_var_entry(name, type, hash_value, n, ++t->num_vars);
  return 0;
}

void free_var_table(VAR_table *t)
{
  VAR_entry *curr, *next;

  for (size_t i=0; i<t->num_bucs; ++i) {
    curr = t->bucs[i];
    //if (curr==NULL) continue;

    while (curr != NULL) {
      next = curr->next;
      free(curr->name);
      free(curr->type);
      free(curr);
      curr = next;
    }
  }

  free(t->bucs);
  free(t);

  return;
}

int find_local_var(VAR_table *t, char *varname, VAR_entry **entry_p) {
  //find the bucket
  hash_t hash_value = hash(varname);
  size_t target_buc = hash_value % t->num_bucs;
  //start iterating to find the entry
  VAR_entry *curr = t->bucs[target_buc];

  if (curr == NULL) return -1;

  while (curr != NULL) {
    //check duplicate globids
    if (!strcmp(curr->name, varname)) {
      *entry_p = curr;
      return 0;
    }

    curr = curr->next;
  }

  return -1;
}

/* Function table APIs */
FUNC_table *init_func_table(int buckets)
{
  // allocate table memory
  FUNC_table *f = calloc(1, sizeof(FUNC_table));
  f->num_bucs = buckets;
  // allocate buckets
  f->bucs = calloc(buckets, sizeof(FUNC_entry *));

  return f;
}

FUNC_entry *new_func_entry(char *globid, char *ret_type, hash_t hash_code, node *n, size_t id)
{
  FUNC_entry *entry = calloc(1, sizeof(FUNC_entry));
  entry->func_id = id;
  entry->globid = strdup(globid);
  entry->ret_type = strdup(ret_type);
  entry->hash_code = hash_code;
  entry->func_node = n;
  entry->local_vars = init_var_table(VARSPACE, globid);
  //next defaults to NULL
  return entry;
}

int add_table_func(FUNC_table *t, char *globid, char *ret_type, node *n)
{
  //NULL table pointer, return error
  if (t==NULL)  return -1;

  //globid is key
  hash_t hash_value = hash(globid);
  //check which one to go
  size_t target_buc = hash_value % t->num_bucs;

  //start iterating to find the insertion place
  FUNC_entry *curr = t->bucs[target_buc];

  //printf("%s %zu %p\n", globid, target_buc, curr);
  if (curr == NULL) {
    t->bucs[target_buc] = new_func_entry(globid, ret_type, hash_value, n, ++t->num_funcs);
    return 0;
  }

  //printf("%s %zu %p\n", globid, target_buc, curr);
  while (curr->next != NULL) {
    //check duplicate globids
    //printf("%s %s\n", curr->globid, globid);
    if (!strcmp(curr->globid, globid)) {
      fprintf(stderr, "error: function (%s) has already been declared.\n", globid);
      return -1;
    }

    curr = curr->next;
  }


  //check duplicate globids
  if (!strcmp(curr->globid, globid)) {
    fprintf(stderr, "error: function (%s) has already been declared.\n", globid);
    return -1;
  }
  //find the last case and insert;
  curr->next = new_func_entry(globid, ret_type, hash_value, n, ++t->num_funcs);
  return 0;
}

void free_func_table(FUNC_table *t)
{
  FUNC_entry *curr, *next;

  for (size_t i=0; i<t->num_bucs; ++i) {
    curr = t->bucs[i];
    //if (curr==NULL) continue;

    while (curr != NULL) {
      next = curr->next;
      free(curr->globid);
      free(curr->ret_type);
      free_var_table(curr->local_vars);
      free(curr);
      curr = next;
    }

    free(t->bucs);
    free(t);

    return;

  }
}

int find_func(FUNC_table *t, char *globid, FUNC_entry **entry_p)
{
  //find the bucket
  hash_t hash_value = hash(globid);
  size_t target_buc = hash_value % t->num_bucs;
  //start iterating to find the entry
  FUNC_entry *curr = t->bucs[target_buc];

  if (curr == NULL) return -1;

  while (curr != NULL) {
    //check duplicate globids
    if (!strcmp(curr->globid, globid)) {
      *entry_p = curr;
      return 0;
    }

    curr = curr->next;
  }

  return -1;
}

//int add_var_to_func(FUNC_table *t, char *name, char *type);
