#include "functions.h"
#include <string.h>
#include "helper_exec.h"
#include <stdio.h>

struct function *is_a_function(struct node *n, struct f_tab *f_tab)
{
    if (!f_tab || !f_tab->nb)
        return NULL;
    for (size_t i = 0; i < f_tab->nb; i++)
    {
        if (!strcmp(n->instr, f_tab->f[i]->name))
            return f_tab->f[i];
    }
    return NULL;
}


void function_stored(struct node *n, struct stored_data *data)
{
    if (!data->f_tab)
    {
        data->f_tab = calloc(1, sizeof(struct f_tab));
        data->f_tab->capacity = 10;
        data->f_tab->f = calloc(10, sizeof(struct function*));
    }
    struct function *func = is_a_function(n, data->f_tab);
    if (func)
    {
        free_node_copy(func->function_start);
        func->function_start = copy_node(n->children->next);
    }
    else
    {
        struct function *new_f = calloc (1, sizeof(struct function));
        size_t len = strlen(n->children->instr) + 1;
        new_f->name = calloc (1, len);
        strcpy(new_f->name, n->children->instr);
        new_f->function_start = copy_node(n->children->next);
        data->f_tab->nb ++;
        if (data->f_tab->nb == data->f_tab->capacity)
        {
            data->f_tab->capacity *= 2;
            void *tmp = realloc(data->f_tab->f, data->f_tab->capacity
                    * sizeof(struct function *));
            if (!tmp)
            {
                data->f_tab->nb--;
                fprintf(stderr, "42sh: memory allocaton exausted");
                return;
            }
            data->f_tab->f = tmp;
        }
        data->f_tab->f[data->f_tab->nb - 1] = new_f;
    }
}


void get_function_param(struct node *child, struct node *oper_node
        , struct stored_data *data)
{
    struct node *iter = child->next;
    char **result = calloc(1, sizeof(char *));
    int i = 0;
    char *instr = NULL;
    for (; iter && iter != oper_node; i++, iter = iter->next)
    {
        instr = set_string(iter->instr, iter, data);
        result = realloc(result, (i + 1) * sizeof(char *));
        result[i] = instr;
    }
    result = realloc(result, (i + 1) * sizeof(char *));
    result[i] = NULL;
    data->nbparam = i;
    char *p = inttochar(i);
    add_assignment_split("#", p, data->var_tab);
    data->param = result;
}
