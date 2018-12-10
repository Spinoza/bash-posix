#include "execution_ast.h"
#include "ast.h"
#include "helper_exec.h"
#include "builtins.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

extern char **environ;

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
    free(data->parent_list);
    free_assignments(data->var_tab);
    free_assignments(data->alias_tab);
    free_assignments(data->export_tab);
    free(data);
}

static void add_environ(struct stored_data *new)
{
    if (!environ)
        return;
    int i = 0;
    while (environ[i])
    {
        add_assignment(environ[i], new->var_tab);
        add_assignment(environ[i], new->export_tab);
        i++;
    }
}

struct stored_data *stored_data_init(void)
{
    struct stored_data *new = malloc(sizeof(struct stored_data));
    if (!new)
        out_of_memory();
    new->param = NULL;
    new->nbparam = 0;
    new->brk = 0;
    new->f_tab = NULL;
    new->parent_list = calloc(1, sizeof(struct node *));
    if (!new->parent_list)
        out_of_memory();
    new->nbparent = 0;
    new->var_tab = init_assignment();
    //FIXME: ne pas remove ces deux lignes !
    new->export_tab = init_assignment();
    new->alias_tab = init_assignment();
    //ne sont pas encore correctement free.
    new->builtins = init_builts();
    char *ifs = calloc(2, sizeof(char));
    //Shinyo: ajout des variables PS1 et PS2
    char *PS1 = calloc(7, sizeof(char));
    char *PS2 = calloc(3, sizeof(char));
    PS1 = memcpy(PS1, "42sh$ ", 6);
    PS2 = memcpy(PS2, "> ", 2);
    //ifs = memcpy(ifs, " ", 1);
    char *PS1_name = calloc(4,sizeof(char));
    PS1_name = memcpy(PS1_name, "PS1", 3);
    add_assignment_split(PS1_name, PS1, new->var_tab);
    char *PS2_name = calloc(4,sizeof(char));
    PS2_name = memcpy(PS2_name, "PS2", 3);
    add_assignment_split(PS2_name, PS2, new->var_tab);
    char *IFS_name = calloc(4,sizeof(char));
    IFS_name = memcpy(IFS_name, "IFS", 3);
    add_assignment_split(IFS_name, ifs, new->var_tab);
    add_environ(new);
    return new;
}

static int copy_string(char *source, char **dest, int start_index,
        int *capacity)
{
    if (!source)
        return start_index;
    int len = strlen(source);
    if (start_index + len >= *capacity)
    {
        *capacity += 2 * len;
        *dest = realloc(*dest, sizeof(char) * *capacity);
        if (!dest)
            out_of_memory();
    }
    memcpy(*dest + start_index, source, len);
    return start_index + len;
}

/*
 * index is the index of the character after the dollar sign in the input string
 * k is the current index where to write in the result string, capacity the
 * current capacity of result string.
 * */

char *set_string(char *instr, struct stored_data *data)
{
    int len = strlen(instr) + 1;
    int *capacity = malloc(sizeof(int));;
    *capacity = len;
    char *string = calloc(*capacity, sizeof(char));
    if (!string)
        out_of_memory();
    int after_dollar = 0;
    int k = 0; //index to copy characters
    for (int i = 0; *(instr + i); i++)
    {
        if (instr[i] == '$' && !after_dollar)
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
                name[0] = *(instr + i);
                i++;
                for (int j = 1; *(instr + i) && *(instr + i) != '$';
                        i++, j++)
                {
                    name[j] = instr[i];
                }
                i--;
                assigned_value = get_assign(name, data);
            }
            k = copy_string(assigned_value, &string, k, capacity);
            if (!strcmp(name, "UID") || !strcmp(name,"?")
                    || !strcmp(name, "$") || !strcmp(name, "#")
                    || (name[0] == '(' && name[1] == '('))
                free(assigned_value);
            free(name);
        }
        else
        {
            string[k++] = instr[i];
        }
        after_dollar = 0;
    }
    if (after_dollar)
        string[k] = '$';
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
        if (strcmp(iter->instr,"$@") && strcmp(iter->instr,"$*"))
        {
            instr = set_string(iter->instr, data);
            if (strcmp (instr, ""))
            {
                result = realloc(result, (i + 1) * sizeof(char *));
                result[i] = instr;
            }
            else
                i--;
        }
        else
        {
            result = realloc(result, (i + data->nbparam)* sizeof(char *));
            for (int j = 0; j < data->nbparam; j++, i++)
            {
                int len = strlen(data->param[j]);
                instr = calloc(len + 1, sizeof(char));
                instr = memcpy(instr, data->param[j], len);
                result[i] = instr;
            }
            i--;
        }
    }
    result = realloc(result, (i + 1) * sizeof(char *));
    result[i] = NULL;
    char *is_alias = get_assign_var(result[0], data->alias_tab);
    if (is_alias && strcmp(is_alias, ""))
    {
        int len = strlen(is_alias);
        free(result[0]);
        result[0] = calloc(len + 1, sizeof(char));
        result[0] = memcpy(result[0], is_alias, len);
    }
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

int is_builtin(char **string)
{
    for (int i = 0; i < NB_BUILTINS; i++)
    {
        if (!strcmp(global.data->builtins[i].name, string[0]))
        {
            int r = 0;
            if (!strcmp(string[0], "exit"))
                r = global.data->builtins[i].builtin(string, global.res);
            else
                r = global.data->builtins[i].builtin(string);
            return r;
        }
    }
    return -1;
}

int exec_command(char **string)
{
    int r_builtin = is_builtin(string);
    if (r_builtin != -1)
    {
        free_command(string);
        return r_builtin;
    }
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
        if (status == 65280)
            status = 127;
        else
            status = WEXITSTATUS(status);
        if (status == 127)
            fprintf(stderr,"42sh : %s : command not found.\n",string[0]);
        free_command(string);
        return status;
    }
}
