#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "ast_assignement.h"
#include "ast.h"
#include "options.h"
#include "builtins.h"

static void sort_assigns(struct vector *getter)
{
    for (ssize_t i = 0; i < getter->size; i++)
    {
        struct assignment *actual = getter->arr[i];
        size_t indswp = 0;
        for (ssize_t j = i; j < getter->size; j++)
        {
            struct assignment *tocomp = getter->arr[j];
            struct assignment *comp2 = getter->arr[indswp];
            if ((indswp == 0) && (strcmp(actual->name, tocomp->name) > 0))
            {
                indswp = j;
            }

            else if ((indswp != 0) && (strcmp(comp2->name, tocomp->name) > 0))
            {
                indswp = j;
            }
        }

        if (indswp != 0)
        {
            void *temp = getter->arr[i];
            getter->arr[i] = getter->arr[indswp];
            getter->arr[indswp] = temp;
        }
    }

}

struct vector *get_assigns_sorted(struct assignment **table)
{
    struct vector *getter = vector_create();
    for (int i = 0; i < HASH_TAB_SIZE; i++)
    {
        struct assignment *a = table[i];
        while (a)
        {
            if (a->name)
                getter = vector_append(getter, a);
            a = a->next;
        }
    }

    sort_assigns(getter);
    return getter;

}

static void print_alphabet(struct assignment **alias_tb)
{
    struct vector *getter = get_assigns_sorted(alias_tb);
    for (ssize_t i = 0; i < getter->size; i++)
    {
        struct assignment *assign = getter->arr[i];
        fprintf(stdout, "alias %s='%s'\n", assign->name, assign->value);
    }
    vector_destroy(getter);
    return;
}

static int print_alias(char *arg)
{
    int pos = hash_function(arg);
    struct assignment *a = global.data->alias_tab[pos];
    for (; a; a = a->next)
    {
        if (a->name == NULL)
        {
            fprintf(stderr, "42sh: alias: %s not found.\n", arg);
            return 1;
        }

        if (!strcmp(a->name, arg))
        {
            fprintf(stdout, "alias %s='%s'\n", arg, a->value);
            return 0;
        }
    }
    fprintf(stderr, "42sh: alias: %s not found.\n", arg);
    return 1;
}

static int contain_equal(char *string)
{
    size_t length = strlen(string);
    for (size_t i = 0; i < length; i++)
    {
        if (string[i] == '=')
        {
            return 1;
        }
    }
    return 0;
}

int my_alias(int number, char *args[], ...)
{
    number = number;
    if (!args[1])
    {
        print_alphabet(global.data->alias_tab);
        return 0;
    }

    int retcode = 0;
    int found = 0;
    int i = 1;
    while (args[i])
    {
        if (!strcmp("-p", args[i]) && !found)
        {
            found = 1;
            print_alphabet(global.data->alias_tab);
        }
        else if (args[i][0] == '-' && !found)
        {
            fprintf(stderr, "42sh: alias: %s: invalid option.\n", args[i]);
            fprintf(stderr, "alias: usage: alias [-p] [name[=value] ... ]\n");
            return 2;
        }
        else if (!contain_equal(args[i]))
        {
            if (retcode == 0)
                retcode = print_alias(args[i]);
            else
                print_alias(args[i]);
        }

        else
        {
            add_assignment(args[i], global.data->alias_tab);
        }

        i++;
    }
    return retcode;
}
