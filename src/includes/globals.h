#pragma once

#include "ast.h"
#include "options.h"
#include "execution_ast.h"

struct globv
{
    struct node *ast_root;
    struct stored_data *data;
    struct option *options;
    char *oldPWD;
    char *PWD;
};

void change_pid(char *newPID);
void init_globv(void);
void free_glob(void);

extern struct globv global;
