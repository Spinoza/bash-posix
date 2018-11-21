#define _GNU_SOURCE
#include "execution_ast.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

//TO IMPLEMENT LATER
/*
   struct assignment *to_assign(char *string)
   {
   struct assignment *new = malloc(sizeof(struct assignment));
   int i = 0;
   int len = strlen(string);
   for (; *(string + i) && *(string + i) != '='; i++);
   new->name = malloc(sizeof(char) * i);
   new->name = memcpy(new->name, string, i);
   new->name[i] = '\0';
   new->value = memcpy(new->value, string + i + 1, len - i);
   new->value[len - i] = '\0';
   return new;
   }
   int already_exists(struct assignment *to_assign, struct tab_a *tab)
   {
   char *string = to_assign->name;
   int name_len = strlen(string) + 1;
   int value_len = strlen(to_assign->name) +1;
   for (int i =0; i < nb; i++) //Value already present
   {
   if (!strcmp(string, tab->assignment->name))
   {
   free(tab->value);
   tab->value = malloc(sizeof(char) * value_len);
   tab->value = memcpy(tab->value, to_assign->value, value_len);
   free(to_assign);
   return 1;
   }
   }
   return 0;
   }
   void add_assignment(struct assignment *to_assign, struct tab_a *tab)
   {
   char *string = to_assign->name;
   int name_len = strlen(string) + 1;

   if (already_exists(to_assign, tab)) //just change the value field
   return;
   if (tab->nb == tab->capacity)
   {
   capacity *=2;
   void *tmp = realloc(tab->assignment,sizeof(struct assignment)
 * capacity);
 if (!tmp)
 {
 capacity /=2;
 fprintf(stderr,"42sh : Out of memory");
 return;
 }
 tab->assignment = tmp;
 }
 tab->assignment[tab->nb++] = to_assign;
 }
 char *get_assign(char *name, struct tab_a **tab)
 {
 for (int i = 0; i < la->nb; i++)
 {
 if (!strcmp(name, tab->assignment[i]->name))
 return tab->assignment[i]->value;
 }
 return name;
 }
*/

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
        free_command(command_call);
        if (oper_node == NULL
                || oper_node->tokentype == SEMICOLON
                || oper_node->tokentype == AND)
            break;
        char *oper = oper_node->instr;
        if ((!strcmp(oper,"logical_and") && !res) || (!strcmp(oper,"logical_or") && res))
            break;
        iter = oper_node->next;
    }
    return res;
}

int pipe_command(char **command1, struct node *n)
{
    struct node *oper_node = get_oper_node(n);
    char **command2 = to_execute(n, oper_node);

    int fd[2];
    pipe(fd);
    int saved_fd1 = dup(1);

    close(0);

    exec_command(command1);
    dup2(1, saved_fd1);
    close(saved_fd1);
    close(fd[0]);
    close(fd[1]);

    int status = exec_command(command2);
    free_command(command1);
    free(command1);
    free_command(command2);
    free(command2);
    return status;
}

struct node *instr_execution(struct node *n, int *res)
{
    /*check redirection*/
    struct node *oper_node = get_oper_node(n);
    char *oper = (oper_node ? oper_node->instr : "");
    char **command_call = to_execute(n, oper_node);
    if (oper_node && oper_node->tokentype == PIPE)
        *res = pipe_command(command_call, oper_node->next);
    else
        *res = exec_command(command_call);
    free_command(command_call);
    if ((!strcmp(oper,"logical_and") && !(*res))
            || (!strcmp(oper,"logical_or") && (*res)))
        return oper_node->next;
    if (!strcmp(oper, "semicolon") && oper_node->next)
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

struct node *for_execution(struct node *n, int *res)
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
                    || cond->tokentype != AND); cond = cond->next)
        {
            *res = traversal_ast(do_node, res);
        }
    }
    return (n->next);
}

struct node *case_execution(struct node *n)
{
    struct node *elt = n->children;
    struct node *cases = elt->next;
    for ( ; cases; cases = cases->next)
    {
        struct node *condition = cases->children->children;
        for ( ; condition && condition->next;
                condition = condition->next->next)
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

int traversal_ast(struct node *n, int *res)
{
    if (!n)
        return *res;
    if ((n->type != A_BODY && n->type != A_ROOT))
    {
        if (n->type == A_INSTRUCT)
        {
            if (strcmp(n->instr, ";;"))
                return traversal_ast(instr_execution(n, res), res);
        }
        if (n->type == A_IF)
            return traversal_ast(if_execution(n, res), res);
        if (n->type == A_CASE)
            return traversal_ast(case_execution(n), res);
        if (n->type == A_WHILE)
        {
            while (if_cond(n) == 0)
                *res = traversal_ast(n->children->next, res);
        }
        if (n->type == A_UNTIL)
        {
            while (if_cond(n))
                *res = traversal_ast(n->children->next, res);
        }
        if (n->type == A_FOR)
            return traversal_ast(for_execution(n, res), res);
        return traversal_ast(n->next,res);
    }
    return traversal_ast(n->children,res);
}

int execution_ast(struct node *n)
{
    /*struct tab_a *tab = calloc(1, sizeof(struct tab_a));
      tab->capacity = 10;
      struct assignment **assignment = malloc(sizeof(struct assignment *)
     * capacity);
     tab->assignment = assignment;*/
    int r = 1;
    return traversal_ast(n, &r);//call with tab;
}
