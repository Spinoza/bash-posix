#define _GNU_SOURCE
#include "execution_ast.h"
#include "arithmetic.h"
#include "globals.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

static size_t nbdigits(int r)
{
    int i = 0;
    for ( ; r > 0 ; r /= 10)
        i++;
    return i;
}

char *inttochar(int nb)
{
    size_t s = nbdigits(nb);
    char *number = malloc(sizeof(char) * (s + 2));
    if (nb == 0)
    {
        number[0] = '0';
        number[1] = '\0';
        return number;
    }
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

static char *get_random(void)
{
    int r = rand();
    return inttochar(r);
}

static int isanumber(char *name)
{
    for (int i = 0; *(name + i); i++)
    {
        if ( *(name + i) <= '0' || *(name + i) >= '9')
            return -1;
    }
    return atoi(name);
}

static char *get_param(char *name, struct stored_data *data)
{
    if (data->nbparam == 0)
        return "";
    int nb = isanumber(name);
    if (nb >= 0)
    {
        if (nb <= data->nbparam)
            return data->param[nb - 1];
    }
    return "";
}

int strcmp_expand(char *name, char *toexpand)
{
    int len = strlen(name);
    if (name[0] == '{' && name[len] == '}')
    {
        name++;
        return strncmp(name, toexpand, len - 1);
    }
    return strcmp(name, toexpand);
}

char *get_assign_var(char *name, struct assignment **a_tab)
{
    if (!name)
    {
        return NULL;
    }
    int pos = hash_function(name);
    struct assignment *a = a_tab[pos];
    for ( ; a ; a = a ->next)
    {
        if (!a->name)
            return NULL;
        if (!strcmp_expand(name, a->name))
        {
            char *recursive_value = get_assign_var(a->value, a_tab);
            if (!recursive_value)
                return a->value;
            return recursive_value;
        }
    }
    return NULL;
}

char *get_assign(char *name, struct stored_data *data)
{
    if (name[0] == '(' && name[1] == '(')
        return arith_expansion(name);
    if (!strcmp_expand(name, "#"))
        return inttochar(data->nbparam);
    if (!strcmp_expand(name, "$"))
        return inttochar(getpid());
    if (!strcmp_expand(name, "OLDPWD"))
        return global.oldPWD;
    if (!strcmp_expand(name, "?"))
        return inttochar(global.res);
    if (!strcmp_expand(name, "RANDOM"))
        return get_random();
    if (!strcmp_expand(name, "UID"))
        return inttochar(getuid());
    if (isanumber(name) != -1)
        return get_param(name, data);
    return get_assign_var(name, data->var_tab);
}
