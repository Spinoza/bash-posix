#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "ast_assignement.h"
#include "ast.h"
#include "options.h"
#include "builtins.h"

static int remove_given(struct assignment *assign)
{
    free(assign->name);
    assign->name = NULL;
    free(assign->value);
    assign->value = NULL;
    free(assign);
    assign = NULL;

    return 0;
}

static int remove_assignment(char *arg)
{
    int pos = hash_function(arg);
    struct assignment *a = global.data->alias_tab[pos];
    struct assignment *prev = NULL;
    for (; a; a= a->next)
    {
        if (a->name == NULL)
        {
            fprintf(stderr, "42sh: unalias: %s not found.\n", arg);
            return 1;
        }

        if (!strcmp(a->name, arg))
        {
            if (prev)
            {
                prev->next = a->next;
            }
            else
            {
                global.data->alias_tab[pos] = a->next;
            }
            remove_given(a);
            return 0;
        }

        prev = a;
        a = a->next;
    }

    fprintf(stderr, "42sh: unalias: %s not found.\n", arg);
    return 1;
}

//This function does 'transform' assigntab in a new one, empty.
static int remove_all(void)
{

    for (int i = 0; i < HASH_TAB_SIZE; i++)
    {
        struct assignment *a = global.data->alias_tab[i];
        struct assignment *next = a->next;
        while (next)
        {
            struct assignment *temp = next;
            next = next->next;
            free(temp->value);
            temp->value = NULL;
            free(temp->name);
            temp->name = NULL;
            free(temp);
        }
        free(a->name);
        a->name = NULL;
        free(a->value);
        a->value = NULL;
    }
    return 0;
}

int my_unalias(int number, char *args[], ...)
{
    number = number;
    if (!args[1])
    {
        fprintf(stderr, "unalias: usage: unalias [-a] name [name...]\n");
        return 2;
    }

    int retcode = 0;
    int found = 0;
    int i = 1;
    while (args[i])
    {
        if (!strcmp("-a", args[i]) && !found)
        {
            return remove_all();
        }
        else if (args[i][0] == '-' && !found)
        {
            fprintf(stderr, "42sh: unalias: %s: invalid option\n", args[i]);
            fprintf(stderr, "unalias: usage: [-a] name [name...]\n");
            return 2;
        }

        else
        {
            found = 1;
            if (retcode == 0)
                retcode = remove_assignment(args[i]);
            else
                remove_assignment(args[i]);
        }

        i++;
    }
    return retcode;
}
