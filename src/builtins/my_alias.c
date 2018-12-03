#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "ast_assignment.h"
#include "ast.h"
#include "options.h"
#include "builtins.h"

static void sort_assigns(struct vector *getter)
{
    for (size_t i = 0; i < getter->size; i++)
    {
        struct assignment *actual = getter[i];
        size_t indswp = 0;
        for (size_t j = i; j < getter->size; j++)
        {
            if ((indswp == 0) && (strcmp(actual, getter[j]) > 0))
            {
                indswp = j;
            }

            else if ((indswp != 0) && (strcmp(getter[indswp], getter[j]) > 0))
            {
                indswp = j;
            }
        }

        if (indswp != 0)
        {
            struct assignment *temp = getter[i];
            getter[i] = getter[indswp];
            getter[indswp] = temp;
        }
    }

}

static struct vector *get_assigns_sorted(struct assignment **table)
{
    struct vector *getter = vector_create();
    for (int i = 0; i < HASH_TAB_SIZE; i++)
    {
        struct assignment *a = table[i];
        while (a)
        {
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
    for (size_t i = 0; i < getter->size; i++)
    {
        struct assignment *assign = getter[i];
        fprintf(stdout, "%s='%s'", assign->name, assign->value);
    }
    return;
}

static int print_alias(char *arg)
{
    int pos = hash_function(arg[i]);
    struct assignment *a = global.alias_tab[pos];
    for (; a; a = a->next)
    {
        if (a->name == NULL)
        {
            fprintf(stderr, "42sh: alias: %s not found.\n", arg);
            return 1;
        }

        if (!strcmp(a->name, arg))
        {
            fprintf(stdout, "alias %s='%s'\n", arg, a->name);
            return 0;
        }
    }
    return 1;
}

static int contain_equal(char *string)
{
    size_t length = strlen(string);
    for (int i = 0; i < length; i++)
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
    if (!args[1])
    {
        print_alphabet(global.alias_tab);
        return 0;
    }

    int found = 0;
    int i = 1;
    while (args[i])
    {
        if (!strcmp("-p", args[i]) && !found)
        {
            found = 1;
            print_alphabet(global.alias_tab);
        }
        else if (!contain_equal(args[i]))
        {
            return print_alias(args[i]);
        }

        else
        {
            //FIXME: need to handle expansions though. (to see with Neganta and Leo)
            add_assignment(args[i]);
        }

        i++;
    }

}
