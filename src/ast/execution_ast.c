#define _GNU_SOURCE
#include <err.h>
#include "execution_ast.h"
#include "ast.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

struct f_tab *function_store(struct node *n, struct f_tab *f_tab)
{
    if (!f_tab)
    {
        f_tab = calloc(1, sizeof(struct f_tab));
        f_tab->capacity = 10;
        f_tab->f = calloc(10, sizeof(struct function*));
    }
    struct function *func = is_a_function(n, f_tab);
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
        f_tab->nb ++;
        if (f_tab->nb == f_tab->capacity)
        {
            f_tab->capacity *= 2;
            void *tmp = realloc(f_tab->f, f_tab->capacity
                    * sizeof(struct function *));
            if (!tmp)
            {
                f_tab->nb--;
                fprintf(stderr, "42sh: memory allocaton exausted");
                return f_tab;
            }
            f_tab->f = tmp;
        }
        f_tab->f[f_tab->nb - 1] = new_f;
    }
    return f_tab;
}


char **to_execute(struct node *child, struct node *oper_node)
{
    struct node *iter = child;
    size_t len = strlen(iter->instr) + 1;
    char **result = calloc(1, sizeof(char *));
    int i = 0;
    result[i] = calloc(len, sizeof(char));
    result[i++] = memcpy(result[0], iter->instr, len);
    iter = iter->next;
    for (; iter && iter != oper_node; i++, iter = iter->next)
    {
        len = strlen(iter->instr) + 1;
        result = realloc(result, (i + 1) * sizeof(char *));
        result[i] = calloc(len, sizeof(char));
        memcpy(result[i], iter->instr, len);
    }
    result = realloc(result, (i + 1) * sizeof(char *));
    result[i] = NULL;
    return result;
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
int if_cond(struct node *cond)
{
    struct node *condition = cond->children;
    int res = 0;

    for (struct node *iter = condition->children; iter;)
    {
        struct node *oper_node = get_oper_node(iter);
        char **command_call = to_execute(iter, oper_node);
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

int pipe_aux(char **command, struct node *n, int fd[2])
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
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        if (oper_node && oper_node->tokentype == PIPE)
        {
            close(fd_next[0]);
            dup2(fd_next[1], 1);
            close(fd_next[1]);
        }
        int r = execvp(command[0], command);
        exit(r);
    }
    else
    {
        waitpid(pid,&status,0);
        close(fd_next[1]);
        if (status == 127)
            fprintf(stderr,"42sh : %s : \
                    command not found.\n", command[0]);
    }

    if (oper_node && oper_node->tokentype == PIPE)
    {
        struct node *next_oper_node = get_oper_node(oper_node->next);
        char **command_next = to_execute(oper_node->next, next_oper_node);
        pipe_aux(command_next, oper_node->next, fd_next);
    }

    free_command(command);
    return status;
}

int pipe_handling(char **command1, struct node *n)
{
    struct node *oper_node = get_oper_node(n);
    int fd[2];
    pipe(fd);
    return pipe_aux(command1, oper_node, fd);
}


struct node *instr_execution(struct node *n, int *res,
        struct f_tab *f_tab)
{
    /*check redirection*/
    struct function *f = is_a_function(n, f_tab);
    struct node *func = NULL;
    if (f)
        func = f->function_start;
    struct node *oper_node = get_oper_node(n);
    char *oper = (oper_node ? oper_node->instr : "");
    if (!func)
    {
        char **command_call = to_execute(n, oper_node);
        if (oper_node && oper_node->tokentype == PIPE)
        {
            *res = pipe_handling(command_call, n);
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
        *res = traversal_ast(func, res, &f_tab);
    }
    if ((!strcmp(oper,"&&") && !(*res))
            || (!strcmp(oper,"||") && (*res)))
        return oper_node->next;
    if ((!strcmp(oper, ";") || !strcmp(oper, "&")) && oper_node->next)
        return oper_node->next;
    return NULL;
}

struct node *if_execution(struct node *n, int *res)
{
    *res = if_cond(n);
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

int for_execution(struct node *n, int *res, struct f_tab *f_tab)
{
    struct node *cond = n->children->children;
    for ( ; cond; cond = cond->next)
    {
        if (cond->type == A_IN)
            break;
    }
    if (cond)
    {
        cond = cond->next;
        struct node *do_node = n->children->next->children;
        for ( ; cond && (cond->tokentype != SEMICOLON
                    && cond->tokentype != AND); cond = cond->next)
        {
            *res = traversal_ast(do_node, res, &f_tab);
        }
    }
    return *res;
}

struct node *case_execution(struct node *n)
{
    struct node *elt = n->children;
    struct node *cases = elt->next;
    for ( ; cases; cases = cases->next)
    {
        struct node *condition = cases->children->children;
        for ( ; condition; condition = condition->next)
        {
            if (!strcmp(condition->instr, elt->instr))
                break;
            if (!strcmp(condition->instr, "*"))
                break;
        }
        if (condition)
            break;
    }
    if (!cases)
        return NULL;
    return cases->children->next;
}

int traversal_ast(struct node *n, int *res, struct f_tab **f_tab)
{
    if (!n)
        return *res;
    if ((n->type != A_BODY && n->type != A_ROOT && n->type != A_EBODY))
    {
        if (n->type == A_FUNCTION)
        {
            *f_tab = function_store(n, *f_tab);
        }
        if (n->type == A_INSTRUCT)
        {
            return traversal_ast(instr_execution(n, res, *f_tab), res, f_tab);
        }
        if (n->type == A_IF || n->type == A_ELIF)
            *res = traversal_ast(if_execution(n, res), res, f_tab);
        if (n->type == A_CASE)
            *res = traversal_ast(case_execution(n), res, f_tab);
        if (n->type == A_WHILE)
        {
            while (if_cond(n) == 0)
                *res = traversal_ast(n->children->next, res, f_tab);
        }
        if (n->type == A_UNTIL)
        {
            while (if_cond(n))
                *res = traversal_ast(n->children->next, res, f_tab);
        }
        if (n->type == A_FOR)
            *res = for_execution(n, res, *f_tab);
        return traversal_ast(n->next,res, f_tab);
    }
    return traversal_ast(n->children, res, f_tab);
}

int execution_ast(struct node *n, struct f_tab **f_tab)
{
    /*struct tab_a *tab = calloc(1, sizeof(struct tab_a));
      tab->capacity = 10;
      struct assignment **assignment = malloc(sizeof(struct assignment *)
     * capacity);
     tab->assignment = assignment;*/
    int r = 0;
    return traversal_ast(n, &r, f_tab);//call with tab;
}
