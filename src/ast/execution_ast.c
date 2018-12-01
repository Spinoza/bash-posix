#define _GNU_SOURCE
#include <err.h>
#include "execution_ast.h"
#include "ast.h"
#include "builtins.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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
    free(data);
}

struct stored_data *stored_data_init(void)
{
    struct stored_data *new = malloc(sizeof(struct stored_data));
    new->f_tab = NULL;
    new->var_tab = init_assignment();
    new->builtins = init_builts();
    return new;
}

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

int copy_string(char *source, char **dest, int start_index, int *capacity)
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

char *set_string(char *instr, struct node *node, struct stored_data *data)
{
    int len = strlen(instr);
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
        if (k + 1 >= *capacity)
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

void get_function_param(struct node *child, struct node *oper_node
        , struct stored_data *data)
{
    struct node *iter = child;
    size_t len = 0;
    char **result = calloc(1, sizeof(char *));
    int i = 0;
    char *instr = NULL;
    for (; iter && iter != oper_node; i++, iter = iter->next)
    {
        if (iter->tokentype == EXPAND_W)
            instr = get_assign(iter->instr, data);
        else
            instr = iter->instr;
        len = strlen(instr) + 1;
        result = realloc(result, (i + 1) * sizeof(char *));
        result[i] = calloc(len, sizeof(char));
        memcpy(result[i], instr, len);
    }
    result = realloc(result, (i + 1) * sizeof(char *));
    result[i] = NULL;
    data->nbparam = i;
    data->param = result;
}

void free_command(char **string)
{
    int i = 0;
    for (; *(string + i); i++)
        free(*(string + i));
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

struct node *get_oper_node(struct node *start)
{
    struct node *iter = start;
    for (; iter; iter = iter->next)
    {
        if (iter->tokentype == LOGICAL_AND
                || iter->tokentype == LOGICAL_OR
                || iter->tokentype == SEMICOLON
                || iter->tokentype == AND
                || iter->tokentype == PIPE)
            return iter;
    }
    return NULL;
}
int if_cond(struct node *cond, struct stored_data *data)
{
    struct node *condition = cond->children;
    int res = 0;

    for (struct node *iter = condition->children; iter;)
    {
        struct node *oper_node = get_oper_node(iter);
        char **command_call = to_execute(iter, oper_node, data);
        res = exec_command(command_call);
        if (oper_node == NULL
                || oper_node->tokentype == SEMICOLON
                || oper_node->tokentype == AND)
            break;
        char *oper = oper_node->instr;
        if ((!strcmp(oper,"&&") && res) || (!strcmp(oper,"||") && !res))
            break;
        iter = oper_node->next;
    }
    return res;
}

int pipe_aux(char **command, struct node *n, int fd[2], struct stored_data *data)
{
    struct node *oper_node = get_oper_node(n);
    int fd_next[2];
    pipe(fd_next);
    pid_t pid = fork();
    int status = 0;
    if (pid == -1)//error
    {
        fprintf(stderr,"42sh : fork : An error occured.\n");
        exit(1);
    }
    if (pid == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[1]);
        if (oper_node && oper_node->tokentype == PIPE)
        {
            close(fd_next[0]);
            dup2(fd_next[1], STDOUT_FILENO);
            close(fd_next[1]);
        }
        int r = execvp(command[0], command);
        exit(r);
    }
    else
    {
        close(fd_next[1]);
        if (oper_node && oper_node->tokentype == PIPE)
        {
            struct node *next_oper_node = get_oper_node(oper_node->next);
            char **command_next = to_execute(oper_node->next, next_oper_node, data);
            status = pipe_aux(command_next, oper_node->next, fd_next, data);
        }
        waitpid(pid,&status,0);
        if (status == 127)
            fprintf(stderr,"42sh : %s : \
                    command not found.\n", command[0]);
    }
    free_command(command);
    return status;
}

int pipe_handling(char **command1, struct node *n, struct stored_data *data)
{
    struct node *oper_node = get_oper_node(n);
    int fd[2];
    fd[0] = 0;
    fd[1] = 1;
    return pipe_aux(command1, oper_node, fd, data);
}

struct node *instr_execution(struct node *n, int *res,
        struct stored_data *data)
{
    /*check redirection*/
    struct function *f = is_a_function(n, data->f_tab);
    struct node *func = NULL;

    struct node *oper_node = get_oper_node(n);
    if (f)
    {
        func = f->function_start;
        get_function_param(n, oper_node, data);
    }
    char *oper = (oper_node ? oper_node->instr : "");
    if (!func)
    {
        char **command_call = to_execute(n, oper_node, data);
        if (oper_node && oper_node->tokentype == PIPE)
        {
            *res = pipe_handling(command_call, n, data);
            while (oper_node && oper_node->tokentype == PIPE)
                oper_node = get_oper_node(oper_node->next);
            oper = (oper_node ? oper_node->instr : "");
        }
        else
            *res = exec_command(command_call);
    }
    else
    {
        //set the args table
        *res = traversal_ast(func, res, data);
    }
    if ((!strcmp(oper,"&&") && !(*res))
            || (!strcmp(oper,"||") && (*res)))
        return oper_node->next;
    if ((!strcmp(oper, ";") || !strcmp(oper, "&")) && oper_node->next)
        return oper_node->next;
    return NULL;
}

struct node *if_execution(struct node *n, int *res, struct stored_data *data)
{
    *res = if_cond(n, data);
    if (*res == 0)
        return n->children->next;
    else //if res != 0, the exec returned an error
    {
        if (n->children->next->next)
            return n->children->next->next;
        else
            return NULL;
    }
}

int for_execution(struct node *n, int *res, struct stored_data *data)
{
    struct node *elem = n->children->children;
    struct node *cond = elem->next;
    if (cond)
    {
        cond = cond->next;
        struct node *do_node = n->children->next->children;
        for ( ; cond && (cond->tokentype != SEMICOLON
                    && cond->tokentype != AND); cond = cond->next)
        {
            add_assignment_split(elem->instr, cond->instr, data->var_tab);
            *res = traversal_ast(do_node, res, data);
        }
    }
    return *res;
}

struct node *case_execution(struct node *n, struct stored_data *data)
{
    struct node *elt = n->children;
    struct node *cases = elt->next;
    char *elt_instr = NULL;
    if (elt->tokentype == EXPAND_W)
        elt_instr = get_assign(elt->instr, data);
    else
        elt_instr = elt->instr;
    for ( ; cases; cases = cases->next)
    {
        struct node *condition = cases->children->children;
        for ( ; condition; condition = condition->next)
        {
            char *cond_instr = NULL;
            if (condition->tokentype == EXPAND_W)
                cond_instr = get_assign(condition->instr, data);
            else
                cond_instr = condition->instr;
            if (!strcmp(elt_instr, cond_instr))
                break;
            if (!strcmp(elt_instr, "*"))
                break;
        }
        if (condition)
            break;
    }
    if (!cases)
        return NULL;
    return cases->children->next;
}

int traversal_ast(struct node *n, int *res, struct stored_data *data)
{
    if (!n || !strcmp(n->instr, ";"))
    {
        if (data->param)
            free_command(data->param);
    }
    if (!n)
        return *res;
    if (!strcmp(n->instr, ";"))
        return traversal_ast(n->next, res, data);
    if ((n->type != A_BODY && n->type != A_ROOT && n->type != A_EBODY))
    {
        if (n->type == A_FUNCTION)
        {
            function_stored(n, data);
        }
        if (n->type == A_INSTRUCT)
        {
            if (n->tokentype == ASSIGNMENT_W)
                add_assignment(n->instr, data->var_tab);
            else
                return traversal_ast(instr_execution(n, res, data), res, data);
        }
        if (n->type == A_IF || n->type == A_ELIF)
            traversal_ast(if_execution(n, res, data), res, data);
        if (n->type == A_CASE)
            *res = traversal_ast(case_execution(n, data), res, data);
        if (n->type == A_WHILE)
        {
            while (if_cond(n, data) == 0)
                traversal_ast(n->children->next, res, data);
        }
        if (n->type == A_UNTIL)
        {
            while (if_cond(n, data))
                traversal_ast(n->children->next, res, data);
        }
        if (n->type == A_FOR)
            for_execution(n, res, data);
        return traversal_ast(n->next,res, data);
    }
    return traversal_ast(n->children, res, data);
}

int execution_ast(struct node *n, struct stored_data *data)
{
    /*struct tab_a *tab = calloc(1, sizeof(struct tab_a));
      tab->capacity = 10;
      struct assignment **assignment = malloc(sizeof(struct assignment *)
     * capacity);
     tab->assignment = assignment;*/
    int r = 0;
    return traversal_ast(n, &r, data);//call with tab;
}
