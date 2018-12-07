#include "execution_ast.h"
#include "ast.h"
#include "globals.h"
#include <stdio.h>

void break_execution(struct stored_data *data, int break_nb)
{

    if (!data->nbparent)
    {
        fprintf(stderr, "continue: only meaningful in a loop");
        return;
    }
    if (break_nb < data->nbparent)
    {
        data->brk += break_nb + 1;
        /*for (int i = 0; i < break_nb; i++)
        {
            data->parent_list[data->nbparent - i] = NULL;
        }*/
        data->nbparent-= break_nb;
    }
    else
    {
        data->brk += data->nbparent;
        data->nbparent = 0;
    }
}

