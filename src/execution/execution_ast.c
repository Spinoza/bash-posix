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
#include <sys/stat.h>
#include <fcntl.h>
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
                || iter->tokentype == PIPE
                || iter->tokentype == REDIRECTION)
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

void changing_fd(struct node *oper_node)
{
    int fd;
    if (oper_node && oper_node->tokentype == REDIRECTION)
    {
        if (!strcmp(oper_node->instr,"<")
                ||!strcmp(oper_node->instr, ">"))
        {
            fd = open(oper_node->next->instr, O_RDWR ||O_CREAT);
            if (!strcmp(oper_node->instr,">"))
                dup2(fd, 1);
            else
                dup2(fd, 0);
        }
    }
}

int redirection_aux(char **command, struct node *n,
        struct stored_data *data)
{
    struct node *oper_node = get_oper_node(n);
    pid_t pid = fork();
    int status = 0;
    if (pid == -1)//error
    {
        fprintf(stderr,"42sh : fork : An error occured.\n");
        exit(1);
    }
    if (pid == 0)
    {
        int fd = open(oper_node->next->instr, O_RDWR ||O_CREAT);
        printf("%d\n", fd);
        dup2(fd, 1);
        int r = execvp(command[0], command);
        exit(r);
    }
    else
    {
        if (oper_node && oper_node->tokentype == REDIRECTION)
        {
            struct node *next_oper_node = get_oper_node(oper_node->next);
            char **command_next = to_execute(oper_node->next,
                    next_oper_node, data);
            status = redirection_aux(command_next, oper_node->next, data);
        }
        waitpid(pid,&status,0);
        if (status == 127)
            fprintf(stderr,"42sh : %s : \
                    command not found.\n", command[0]);
    }
    free_command(command);
    return status;
}

int redirection_handling(char **command1, struct node *n, struct stored_data *data)
{
    struct node *oper_node = get_oper_node(n);
    return redirection_aux(command1, oper_node, data);
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
        int r_builtin = is_builtin(command);
        if (r_builtin != -1)
        {
            exit(r_builtin);
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

void close_fds(void)
{
    for (int i = 3; close(i) != -1; i++)
        continue;
}

struct node *get_next_node(struct node *oper_node, int *res)
{
    if (!oper_node)
        return NULL;
    char *oper = oper_node->instr;
    if ((!strcmp(oper,"&&") && !(*res))
            || (!strcmp(oper,"||") && (*res)))
        return oper_node->next;
    if ((!strcmp(oper, ";") || !strcmp(oper, "&")) && oper_node->next)
        return oper_node->next;
    return NULL;
}
struct node *instr_execution(struct node *n, int *res,
        struct stored_data *data)
{
    struct node *oper_node = get_oper_node(n);
    char **command_call = to_execute(n, oper_node, data);
    struct function *f = is_a_function(command_call[0], data->f_tab);
    if (f)
    {
        free_command(command_call);
        struct node *next_node = func_execution(f, oper_node, res, n);
        return next_node;
    }
    if (!strcmp(command_call[0], "break"))
    {
        *res = 0;
        if (command_call[1])
        {
            break_execution(data, atoi(command_call[1]));
        }
        else
            break_execution(data, 1);
        free_command(command_call);
        return NULL;
    }
    else if (!strcmp(command_call[0], "continue"))
    {
        free_command(command_call);
        *res = 0;
        return NULL;
    }
    if (oper_node && oper_node->tokentype == PIPE)
    {
        *res = pipe_handling(command_call, n, data);
        close_fds();
        while (oper_node && oper_node->tokentype == PIPE)
            oper_node = get_oper_node(oper_node->next);
    }
    else if (oper_node && oper_node->tokentype == REDIRECTION)
    {
        *res = redirection_handling(command_call, n, data);
        close_fds();
        while (oper_node && oper_node->tokentype == REDIRECTION)
            oper_node = get_oper_node(oper_node->next);
    }
    else
        *res = exec_command(command_call);
    return get_next_node(oper_node, res);
}

int if_cond(struct node *cond, struct stored_data *data)
{
    struct node *condition = cond->children->children;
    int res = 0;
    while (condition)
        condition = instr_execution(condition, &res, data);
    return res;
}


struct node *if_execution(struct node *n, int *res, struct stored_data *data)
{
    *res = if_cond(n, data);
    if (*res == 0)
        return n->children->next;
    else //if res != 0, the exec returned an error
    {
        *res = 0;
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
        if (cond->instr[0] == '$'
                && cond->instr[1]
                && (cond->instr[1] == '@' || cond->instr[1] == '*'))
        {
            if (i + data->nbparam >= capacity)
            {
                capacity = (i + data->nbparam) * 2;
                instruction = realloc(instruction, capacity * sizeof(char *));
            }
            for (int j = 0 ; j < data->nbparam; j++, i++)
            {
                int len = strlen(data->param[j]);
                instruction[i] = calloc(len + 1, sizeof(char));
                instruction[i] = memcpy(instruction[i], data->param[j], len);
            }
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
        data->nbparent++;
        for (size_t i = 0; instruction[i] && data->brk > 0; i++)
        {
            int l = strlen(instruction[i]);
            char *value = calloc(l + 1, sizeof(char));
            value = memcpy(value, instruction[i], l);
            add_assignment_split(cpy_instr, value,
                    data->var_tab);
            *res = traversal_ast(do_node, res, data);
        }
        if (data->nbparent >= 0)
            data->nbparent--;
        else
            data->parent_list[0] = NULL;
        if (data->brk)
            data->brk--;
        free_command(instruction);
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
	    data->nbparent ++;
	    data->brk++;
	    while (if_cond(n, data) == 0 && data->brk > 0)
		*res = traversal_ast(n->children->next, res, data);
	    data->brk--;
	    if (data->nbparent >= 0)
		data->nbparent--;
	}
	if (n->type == A_UNTIL)
	{
	    data->nbparent++;
	    data->brk++;
	    while (if_cond(n, data) && data->brk > 0)
		*res = traversal_ast(n->children->next, res, data);
	    data->brk--;
	    if (data->nbparent >= 0)
		data->nbparent--;
	}
	if (n->type == A_FOR)
	{
	    for_execution(n, res, data);
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
