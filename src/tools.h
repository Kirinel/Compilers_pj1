#ifndef TOOLS_H
#define TOOLS_H

#include "ast.h"
#include "symbol_table.h"


void parse_tree(char *, node **);

void process_tree(node *);
int load_funcs(FUNC_table *, node *);
int check_run(FUNC_table *);

void emit_ast(char *, char *, node *);



#endif
