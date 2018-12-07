#include "execution_ast.h"
#include "ast.h"
#include "builtins.h"
#include "globals.h"
#include <stdio.h>

struct node *continue_execution(struct stored_data *data)
{
    if (!data->nbparent)
    {
        fprintf(stderr, "continue: only meaningful in a loop");
        return NULL;
    }
    data->nbparent --;
    return data->parent_list[data->nbparent];
}


