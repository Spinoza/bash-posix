#define _GNU_SOURCE
#include "execution_ast.h"
#include "globals.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

size_t nbdigits(int r)
{
    int i = 0;
    for ( ; r > 0 ; r /= 10)
        i++;
    return i;
}

char *inttochar(int nb)
{
    size_t s = nbdigits(nb);
    char *number = malloc(sizeof(char) * s);
    for (size_t i = 0; i < s; i++)
    {
        number[i] = '0' + nb % 10;
        nb /= 10;
    }
    if (nb)
        s --;
    for (size_t i = 0; i < s / 2; i++)
    {
        char tmp = number[i];
        number[i] = number[s - i - 1];
        number[s - i - 1] = tmp;
    }
    number[s] = '\0';
    return number;
}

char *get_random(void)
{
    int r = rand();
    return inttochar(r);
}

int isanumber(char *name)
{
    for (int i = 0; *(name + i); i++)
    {
        if ( *(name + i) <= '0' || *(name + i) >= '9')
            return -1;
    }
    return atoi(name);
}

char *get_param(char *name, struct stored_data *data)
{
    int nb = isanumber(name);
    if (nb >= 0)
    {
        if (nb - 1 <= data->nbparam)
            return data->param[nb - 1];
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
    if (!strcmp(name, "$"))
        return inttochar(getpid());
    if (!strcmp(name, "OLDPWD"))
        return global.oldPWD;
    if (!strcmp(name, "?"))
        return inttochar(global.res);
    if (!strcmp(name, "RANDOM"))
        return get_random();
    if (!strcmp(name, "UID"))
    {
        int uid = getuid();
        return inttochar(uid);
    }
    if (isanumber(name) != -1)
        return get_param(name, data);
    return get_assign_var(name, data->var_tab);
}
