#include "builtins.h"
#include <string.h>
#include "vector.h"
#include "stdio.h"
#include "ast_assignement.h"

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
    while(args[i])
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
            if(file == 0)
            {
                file = 1;
            }
            if(args[i][0] == '-')
            {
                fprintf(stderr, "export: not a valid identifier");
                return 1;
            }
            if(n == 1)
            {
                int ind = hash_function(args[i]);
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
                fprintf(stdout, "When fct finish, it will add %s to export tab!", args[i]);
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
            fprintf(stdout, "declare -x %s=", assign->name);
            if(assign->value[0] != '\0')
            {
                fprintf(stdout, "\'%s\'\n", assign->value);
            }
        }
    }
    return 0;
}
