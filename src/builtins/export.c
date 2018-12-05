#include "builtins.h"
#include <string.h>
#include "vector.h"
#include "stdio.h"
#include "ast_assignement.h"

static int has_equal(char *word)
{
    int i = 0;
    while(word[i])
    {
        if(word[i] == '=')
            return 1;
        i++;
    }
    return 0;
}

static void free_assign(struct assignment *assign)
{
    free(assign->name);
    free(assign->value);
    free(assign);
}

int my_export(int number, char *args[], ...)
{
    number = number;
    int i = 1;
    int n = 0;
    int file = 0;
    while (args[i])
    {
        if(!strcmp(args[i], "-p") && file == 0)
        {
            i++;
            continue;
        }
        else if(!strcmp(args[i], "-n") && file == 0)
        {
            n = 1;
        }
        else
        {
            if(args[i][0] == '-')
            {
                if (file)
                {
                    fprintf(stderr, "export: not a valid identifier");
                    return 1;
                }
                i++;
                continue;
            }
            if(file == 0)
            {
                file = 1;
            }
            if(n == 1)
            {
                char *split = args[1];
                if(has_equal(args[i]) == 1)
                {
                    add_assignment(args[1], global.data->var_tab);
                    split = strtok(split, "=");
                }
                int ind = hash_function(split);
                struct assignment *assign = global.data->export_tab[ind];
                struct assignment *prev = NULL;
                while(assign != NULL && assign->name != NULL)
                {
                    if(!strcmp(args[1], assign->name))
                    {
                        if(prev == NULL)
                        {
                            global.data->export_tab[ind] = assign->next;
                            free_assign(assign);
                            break;
                        }
                        else
                        {
                            prev->next = assign->next;
                            free_assign(assign);
                            break;
                        }
                    }
                    prev = assign;
                    assign = assign->next;
                }
            }
            else
            {
                if(has_equal(args[1]) == 1)
                {
                    add_assignment(args[1], global.data->var_tab);
                    add_assignment(args[1], global.data->export_tab);
                }
                else
                {
                    char *val = NULL;
                    int ind = hash_function(args[1]);
                    struct assignment *assign = global.data->var_tab[ind];
                    while(assign != NULL && assign->name != NULL)
                    {
                        if(!strcmp(args[1], assign->name))
                        {
                            val = assign->value;
                            break;
                        }
                        assign = assign->next;
                    }

                    add_assignment_split(args[1], val, global.data->export_tab);
                }
            }
        }
        i++;
    }
    if(file == 0)
    {
        struct vector *print = get_assigns_sorted(global.data->export_tab);
        for(ssize_t i = 0; i < print->size; i++)
        {
            struct assignment *assign = print->arr[i];
            fprintf(stdout, "declare -x %s", assign->name);
            if(assign->value)
            {
                fprintf(stdout, "=\"%s\"\n", assign->value);
            }
        }
    }
    return 0;
}
