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
        fprintf(stdout, "%s='%s'\n", assign->name, assign->value);
    }
    vector_destroy(getter);
    return;
}

static int search_vector(char *arg)
{

    for (ssize_t i = 0; i < global.aliastab->size; i++)
    {
        char *actual = global.aliastab->arr[i];
        if (actual)
        {
            for (size_t j = 0; (j <= strlen(arg)) && (j < strlen(actual)); j++)
            {
                if (actual[j] == '=')
                {
                    char *cpy = calloc(strlen(actual) + 1, sizeof(char));
                    if (!cpy)
                        return 0;
                    strcpy(cpy, actual);
                    strtok(cpy, "=");
                    char *tp = strtok(NULL, "=");
                    if (!tp)
                    {
                        free(cpy);
                        return 0;
                    }
                    fprintf(stdout, "%s='%s'\n", arg, tp);
                    free(cpy);
                    return 1;
                }

                if (j == strlen(arg) || actual[j] != arg[j])
                {
                    break;
                }
            }
        }
    }

    return 0;
}

static int print_alias(char *arg)
{
    int pos = hash_function(arg);
    struct assignment *a = global.data->alias_tab[pos];
    for (; a; a = a->next)
    {
        if (a->name == NULL)
        {
            if (search_vector(arg))
            {
                return 0;
            }
            fprintf(stderr, "42sh: alias: %s not found.\n", arg);
            return 1;
        }

        if (!strcmp(a->name, arg))
        {
            fprintf(stdout, "%s='%s'\n", arg, a->value ? a->value : "\0");
            return 0;
        }
    }
    if (search_vector(arg))
    {
        return 0;
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

void add_aliases(void)
{
    for (ssize_t i = 0; i < global.aliastab->size; i++)
    {
        if (global.aliastab->arr[i])
        {
            char *toadd = global.aliastab->arr[i];
            add_assignment(toadd, global.data->alias_tab);
            free(global.aliastab->arr[i]);
            global.aliastab->arr[i] = NULL;
        }
    }

    global.aliastab->size = 0;
}

int my_alias(char *args[], ...)
{

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
            char *cpy = calloc(strlen(args[i]) + 1, sizeof(char));
            strcpy(cpy, args[i]);
            vector_append(global.aliastab, cpy);
        }
        i++;
    }
    return retcode;
}
