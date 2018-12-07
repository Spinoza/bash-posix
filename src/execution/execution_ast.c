#define _GNU_SOURCE
#include <err.h>
#include "execution_ast.h"
#include "ast.h"
#include "functions.h"
#include "helper_exec.h"
#include "builtins.h"
#include "globals.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


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

struct node *next_node(struct node *n)
{
    struct node *oper_node = get_oper_node(n);
    if (oper_node)
        return oper_node->next;
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
        free_command(command_call);
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

int pipe_aux(char **command, struct node *n, int fd[2],
        struct stored_data *data)
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
            char **command_next = to_execute(oper_node->next,
                    next_oper_node, data);
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

void break_execution(struct stored_data *data, int break_nb)
{

    if (!data->nbparent)
    {
        fprintf(stderr, "continue: only meaningful in a loop");
        return;
    }
    data->brk -= (break_nb);
    data->nbparent -= break_nb;
    if (data->nbparent < 0)
        data->nbparent = 0;
}

struct node *continue_execution(struct stored_data *data)
{
    if (!data->nbparent)
    {
        fprintf(stderr, "continue: only meaningful in a loop");
        return NULL;
    }
    data->nbparent --;
    return data->parent_list[data->nbparent];
}

struct node *instr_execution(struct node *n, int *res,
        struct stored_data *data)
{
    /*check redirection*/
    struct node *oper_node = get_oper_node(n);
    char *oper = (oper_node ? oper_node->instr : "");
    char **command_call = to_execute(n, oper_node, data);
    int r_builtin = is_builtin(command_call);
    int pipe = 0;
    if (r_builtin != -1)
    {
        *res = r_builtin;
    }
    else
    {
        if (!strcmp(command_call[0], "break"))
        {
            *res = 0;
            if (command_call[1])
                break_execution(data, atoi(command_call[1]));
            else
                break_execution(data, 1);
            free_command(command_call);
            return NULL;
        }
        else if (!strcmp(command_call[0], "continue"))
        {
            //struct node *continue_node = continue_execution(data);
            free_command(command_call);
            //if (continue_node)
            return NULL;
        }
        else
        {
            struct function *f = is_a_function(command_call[0], data->f_tab);
            struct node *func = NULL;
            if (f)
            {
                func = f->function_start;
                get_function_param(n, oper_node, data);
            }
            if (!func)
            {
                if (oper_node && oper_node->tokentype == PIPE)
                {
                    pipe = 1;
                    *res = pipe_handling(command_call, n, data);
                    while (oper_node && oper_node->tokentype == PIPE)
                        oper_node = get_oper_node(oper_node->next);
                    oper = (oper_node ? oper_node->instr : "");
                }
                else
                    *res = exec_command(command_call);
            }
            else
                *res = traversal_ast(func, res, data);
        }
    }
    if (!pipe)
        free_command(command_call);
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

char **get_instruction_for (struct node *cond, struct stored_data *data)
{
    int capacity = 2;
    char **instruction = calloc(capacity, sizeof(char *));
    int i = 0;
    for (; cond && cond->tokentype != SEMICOLON; cond = cond->next, i++)
    {
        if (cond->tokentype == EXPAND_W
                && (cond->instr[0] == '@' || cond->instr[0] == '*'))
        {
            if (i + data->nbparam >= capacity)
            {
                capacity = (i + data->nbparam) * 2;
                instruction = realloc(instruction, capacity * sizeof(char *));
            }
            for (; i < data->nbparam; i++)
                instruction[i] = data->param[i];
            i--;
        }
        else
        {
            if (i + 1 >= capacity)
            {
                capacity *= 2;
                instruction = realloc(instruction, capacity
                        * sizeof(char *));
            }
            instruction[i] = set_string(cond->instr, data);
        }
    }
    instruction[i] = NULL;
    return instruction;
}

void add_parent(struct node *n, struct stored_data *data)
{
    data->nbparent++;
    data->parent_list = realloc(data->parent_list,
            sizeof(struct node *) * (data->nbparent + 1));
    data->parent_list[data->nbparent - 1] = n;
}

int for_execution(struct node *n, int *res, struct stored_data *data)
{
    struct node *elem = n->children->children;
    struct node *cond = elem->next;
    if (cond)
    {
        cond = cond->next;
        struct node *do_node = n->children->next->children;
        char **instruction = get_instruction_for (cond, data);
        int len = strlen(elem->instr);
        char *cpy_instr = calloc(len + 1, sizeof(char));
        cpy_instr = memcpy(cpy_instr, elem->instr, len);
        if (!instruction[0])
            free(cpy_instr);
        data->brk++;
        for (size_t i = 0; instruction[i] && data->brk > 0; i++)
        {
            add_assignment_split(cpy_instr, instruction[i],
                    data->var_tab);
            add_parent(n, data);
            *res = traversal_ast(do_node, res, data);
            data->parent_list[data->nbparent--] = NULL;
        }
        if (data->brk)
            data->brk--;
        free(instruction);
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
            if (!strcmp(cond_instr, "*"))
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
    if (data->brk < 0)
        data->brk = 0;
    global.res = *res;
    if (!n)
        return *res;
    if ((n->type != A_BODY && n->type != A_ROOT && n->type != A_EBODY))
    {
        if (n->type == A_FUNCTION)
        {
            function_stored(n, data);
            int r = traversal_ast(next_node(n), res, data);
            if (data->param)
                free_command(data->param);
            return r;
        }
        if (n->type == A_INSTRUCT)
        {
            if (!strcmp(n->instr, ";"))
                return traversal_ast(n->next, res, data);
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
            add_parent(n, data);
            data->brk++;
            while (if_cond(n, data) == 0 && data->brk > 0)
                *res = traversal_ast(n->children->next, res, data);
            if (data->brk)
                data->brk--;
            data->parent_list[data->nbparent--] = NULL;
        }
        if (n->type == A_UNTIL)
        {
            add_parent(n, data);
            data->brk++;
            while (if_cond(n, data) && data->brk > 0)
                *res = traversal_ast(n->children->next, res, data);
            if (data->brk)
                data->brk--;
            data->parent_list[data->nbparent--] = NULL;
        }
        if (n->type == A_FOR)
        {
            add_parent(n, data);
            for_execution(n, res, data);
            data->parent_list[data->nbparent--] = NULL;
        }
        return traversal_ast(n->next,res, data);
    }
    return traversal_ast(n->children, res, data);
}

int execution_ast(struct node *n, struct stored_data *data)
{
    int r = 0;
    return traversal_ast(n, &r, data);
}
