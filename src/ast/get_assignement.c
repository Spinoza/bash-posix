#define _GNU_SOURCE
#include "execution_ast.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


int isanumber(char *name)
{
    for (int i = 0; *(name + 1); i++)
    {
        if ( *(name + 1) <= '0' || *(name + 1) >= '9')
            return -1;
    }
    return atoi(name);
}

char *get_param(char *name, struct stored_data *data)
{
    int nb = isanumber(name);
    if (nb != -1)
    {
        if (nb < data->nbparam)
            return data->param[nb];
    }
    return "";
}

char *get_assign_var(char *name, struct assignment **a_tab)
{
    int pos = hash_function(name);
    struct assignment *a = a_tab[pos];
    for ( ; a ; a = a ->next)
    {
        if (!a->name)
            return "";
        if (!strcmp(name, a->name))
            return a->value;
    }
    return "";
}

char *get_assign(char *name, struct stored_data *data)
{
    if (isanumber(name) != -1)
        return get_param(name, data);
    return get_assign_var(name, data->var_tab);
}
