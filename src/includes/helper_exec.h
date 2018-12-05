#ifndef HELPER_EXEC_H
#define HELPER_EXEC_H

#include "execution_ast.h"
#include "ast.h"

void free_data(struct stored_data *data);

struct stored_data *stored_data_init(void);

char **to_execute(struct node *child, struct node *oper_node,
        struct stored_data *data);

void free_command(char **string);

int exec_command(char **string);

char *set_string(char *instr, struct node *node, struct stored_data *data);

int is_builtin(char **string);

#endif /* ! HELPER_EXEC */
