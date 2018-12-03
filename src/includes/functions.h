#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "ast.h"
#include "execution_ast.h"

struct function *is_a_function(struct node *n, struct f_tab *f_tab);

void function_stored(struct node *n, struct stored_data *data);

void get_function_param(struct node *child, struct node *oper_node,
        struct stored_data *data);
#endif
