#include "execution_ast.h"
#include "ast.h"
#include "globals.h"
#include <stdio.h>

void break_execution(struct stored_data *data, int break_nb)
{

    if (!data->nbparent)
    {
        fprintf(stderr, "break: only meaningful in a loop");
        return;
    }
    if (break_nb < data->nbparent)
    {
        data->brk -= break_nb;
        data->nbparent -= break_nb;
    }
    else
    {
        data->brk = 0;
        data->nbparent = 0;
    }
}
