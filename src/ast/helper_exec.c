#include "execution_ast.h"
#include "ast.h"
#include "helper_exec.h"
#include "builtins.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void free_data(struct stored_data *data)
{
    if (!data)
        return;
    struct f_tab *f_tab = data->f_tab;
    if (f_tab)
    {
        for (size_t i = 0;i < f_tab->nb; i++)
        {
            free_node_copy(f_tab->f[i]->function_start);
            free(f_tab->f[i]->name);
            free(f_tab->f[i]);
        }
        free(f_tab->f);
        free(f_tab);
    }
    if (data->builtins)
    {
        free(data->builtins);
    }
    free_assignments(data->var_tab);
    free_assignments(data->alias_tab);
    free_assignments(data->export_tab);
    free(data);
}

struct stored_data *stored_data_init(void)
{
    struct stored_data *new = malloc(sizeof(struct stored_data));
    new->param = NULL;
    new->nbparam = 0;
    new->f_tab = NULL;
    new->var_tab = init_assignment();
    //FIXME: ne pas remove ces deux lignes !
    new->export_tab = init_assignment();
    new->alias_tab = init_assignment();
    //ne sont pas encore correctement free.
    new->builtins = init_builts();
    char *ifs = calloc(2, sizeof(char));
    char *hashtag = calloc(2, sizeof(char));
    //Shinyo: ajout des variables PS1 et PS2
    char *PS1 = calloc(7, sizeof(char));
    char *PS2 = calloc(3, sizeof(char));
    strcpy(PS1, "42sh$ ");
    strcpy(PS2, "> ");
    ifs[0] = ' ';
    hashtag[0] = '0';
    add_assignment_split("IFS", ifs, new->var_tab);
    add_assignment_split("#", hashtag, new->var_tab);
    add_assignment_split("PS1", PS1, new->var_tab);
    add_assignment_split("PS2", PS2, new->var_tab);
    return new;
}

static int copy_string(char *source, char **dest, int start_index,
        int *capacity)
{
    int len = strlen(source);
    if (start_index + len >= *capacity)
    {
        *capacity += 2 * len;
        void *tmp = realloc(*dest, sizeof(char) * *capacity);
        if (tmp)
            *dest = tmp;
        else
        {
            free(dest);
            return 0;
        }
    }
    memcpy(*dest + start_index, source, len);
    return start_index + len;
}

/*
 * index is the index of the character after the dollar sign in the input string
 * k is the current index where to write in the result string, capacity the
 * current capacity of result string.
 * */

char *set_string(char *instr, struct node *node,
        struct stored_data *data)
{
    int len = strlen(instr) + 1;
    int *capacity = malloc(sizeof(int));;
    *capacity = len;
    char *string = calloc(*capacity, sizeof(char));
    int after_dollar = 0;
    if (node->tokentype == EXPAND_W)
        after_dollar = 1;
    int k = 0; //index to copy characters
    for (int i = 0; *(instr + i); i++)
    {
        if (instr[i] == '$')
        {
            after_dollar = 1;
            continue;
        }
        if (k + 2 >= *capacity)
        {
            *capacity *=2;
            string = realloc(string, sizeof(char) * *capacity);
        }
        if (after_dollar)
        {
            char *assigned_value = NULL;
            char *name = NULL;
            if (instr[i] >= '0' && instr[i] <= '9')
            {
                name = calloc(2, sizeof(char));
                name[0] = instr[i];
                assigned_value = get_assign(name, data);
            }
            else
            {
                name = calloc(len, sizeof(char));
                for (int j = 0; *(instr + i) && *(instr + i) != '$';
                        i++, j++)
                    name[j] = instr[i];
                i--;
                assigned_value = get_assign(name, data);
            }
            k = copy_string(assigned_value, &string, k, capacity);
            free(name);
        }
        else
        {
            string[k++] = instr[i];
        }
        after_dollar = 0;
    }
    string[k + 1] = '\0';
    free(capacity);
    return string;
}

char **to_execute(struct node *child, struct node *oper_node
        , struct stored_data *data)
{
    struct node *iter = child;
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
    return result;
}


void free_command(char **string)
{
    int i = 0;
    for (; *(string + i); i++)
    {
        free(*(string + i));
        *(string + i) = NULL;
    }
    free(*(string + i));
    free(string);
}

int exec_command(char **string)
{
    pid_t pid = fork();
    if (pid == -1)//error
    {
        fprintf(stderr,"42sh : fork : An error occured.\n");
        exit(1);
    }
    if (pid == 0)//child
    {
        int r = execvp(string[0], string);
        exit(r);
    }
    else//father
    {
        int status = 0;
        waitpid(pid, &status, 0);
        if (status == 127)
            fprintf(stderr,"42sh : %s : command not found.\n",string[0]);
        free_command(string);
        return status;
    }
}
