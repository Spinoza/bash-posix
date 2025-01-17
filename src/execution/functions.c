#include "functions.h"
#include "globals.h"
#include <string.h>
#include "helper_exec.h"
#include <stdio.h>

struct function *is_a_function(char *name, struct f_tab *f_tab)
{
    if (!f_tab || !f_tab->nb)
        return NULL;
    for (size_t i = 0; i < f_tab->nb; i++)
    {
        if (!strcmp(name, f_tab->f[i]->name))
            return f_tab->f[i];
    }
    return NULL;
}


void function_store(struct node *n, struct stored_data *data)
{
    if (!data->f_tab)
    {
        data->f_tab = calloc(1, sizeof(struct f_tab));
        data->f_tab->capacity = 10;
        data->f_tab->f = calloc(10, sizeof(struct function*));
    }
    struct function *func = is_a_function(n->children->instr, data->f_tab);
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
        , struct stored_data **data)
{
    struct node *iter = child->next;
    char **result = calloc(1, sizeof(char *));
    int i = 0;
    char *instr = NULL;
    for (; iter && iter != oper_node; i++, iter = iter->next)
    {
        instr = set_string(iter->instr, *data);
        result = realloc(result, (i + 1) * sizeof(char *));
        result[i] = instr;
    }
    result = realloc(result, (i + 1) * sizeof(char *));
    result[i] = NULL;
    (*data)->nbparam = i;
    (*data)->param = result;
}

struct node *func_execution(struct function *f, struct node *oper_node,
        int *res, struct node *n)
{
    struct node *func = f->function_start;
    get_function_param(n, oper_node, &global.data);
    *res = traversal_ast(func, res, global.data);
    free_command(global.data->param);
    if (!oper_node)
        return NULL;
    if ((!strcmp(oper_node->instr, "&&") && !(*res))
            || (!strcmp(oper_node->instr, "||") && *(res))
            || ((!strcmp(oper_node->instr, ";")
            || !strcmp(oper_node->instr, "&")) && oper_node->next))
        return oper_node->next;
    return NULL;
}

