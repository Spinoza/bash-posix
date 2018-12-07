#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "ast.h"
#include "execution_ast.h"

struct function *is_a_function(char *string, struct f_tab *f_tab);

void function_stored(struct node *n, struct stored_data *data);

struct node *func_execution(struct function *f, struct node *oper_node,
        int *res, struct node *n);
#endif
