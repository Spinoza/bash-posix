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
    char **result = calloc(len, sizeof(char *));
    result[0] = malloc(len * sizeof(char));
    result[0] = memcpy(result[0], iter->instr, len);
    iter = iter->next;
    for (int i = 1; iter != oper_node; i++, iter = iter->next)
    {
        len = strlen(iter->instr) + 1;
        result = realloc(result, i * sizeof(char *));
        result[i] = malloc(len * sizeof(char));
        memcpy(result[i], iter->instr, len);
    }
    return result;
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
        if (r)
            exit(r);
        else
            exit(0);
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
        if (iter->tokentype == LOGICAL_AND || iter->tokentype == LOGICAL_OR)
            return iter;
    }
    return NULL;
}
int if_cond(struct node *n)
{
    struct node *condition = n->children;
    int res = 0;

    for (struct node *iter = condition->children; iter;)
    {
        struct node *oper_node = get_oper_node(iter);
        char **command_call = to_execute(iter, oper_node);
        res = exec_command(command_call);
        free(command_call);
        if (oper_node == NULL)
            break;
        char *oper = oper_node->instr;
        if ((!strcmp(oper,"&&") && !res) || (!strcmp(oper,"||") && res))
            break;
        iter = oper_node->next;
    }
    return res;
}
/*
int while_cond(struct node *n)
{
    struct node *condition = n->children[0];
    int res = 0;
    for (size_t i = 0; i < condition->nbchild;)
    {
        int index = get_oper_node(condition,i + 1);
        if (index == - 1)
            index = parent->nbchild;
        size_t ind = index;
        char *string = to_execute(parent, i, ind);
        res = exec_command(string);
        if (index == -1)
            break;
        char *oper = condition->children[index]->instr;
        if ((!strcmp(oper,"&&") && res) || (!strcmp(oper,"||") && !res))
        {
            res = exec_command(condition->children[index + 1]);
        }
        i = index + 2;
    }
    return res;
}
*/
int traversal_ast(struct node *n)
{

    if ((n->type != A_BODY && n->type != A_ROOT))
    {
        int res = 0;
        if (n->type == A_INSTRUCT)
        {
            struct node *oper_node = get_oper_node(n);
            char *oper = "";
            if (oper_node)
            {
                oper = oper_node->instr;
            }
            char **command_call = to_execute(n, oper_node);
            int rvalue = exec_command(command_call);
            if ((!strcmp(oper,"&&") && !rvalue)
                || (!strcmp(oper,"||") && rvalue))
                return traversal_ast(oper_node->next);

            free(command_call);
        }
        if (n->type == A_IF)
        {
            res = if_cond(n);
            if (res == 0)
                traversal_ast(n->children->next);
            else if (res == 1) //if res != 1, the exec returned an error
                traversal_ast(n->children->next->next);
        }
        return res;
        /*
        if (n->type == A_WHILE)
        {
            res = while_cond(n);
        }
        if (n->type == A_FOR)
            return for_cond(n);*/
    }
    struct node *iter = n->children;
    /*for (; iter; iter = iter->next)
    {
        int r = traversal_ast(iter);
        if (r != 0)
            return r;
    }*/
    return traversal_ast(iter); //FIX RETURN VALUE
}

int execution_ast(struct node *n)
{
    /*struct tab_a *tab = calloc(1, sizeof(struct tab_a));
      tab->capacity = 10;
      struct assignment **assignment = malloc(sizeof(struct assignment *)
     * capacity);
     tab->assignment = assignment;*/
    return traversal_ast(n);//call with tab;
}
