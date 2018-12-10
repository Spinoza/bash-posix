#pragma once

#include "ast.h"
#include "options.h"
#include "execution_ast.h"

struct globv
{
    int res;
    struct node *ast_root;
    struct vector *aliastab;
    struct stored_data *data;
    struct option *options;
    char *oldPWD;
    char *PWD;
};

void out_of_memory(void);
void change_pwd(char *newPID);
void init_globv(void);
void free_glob(void);

extern struct globv global;
