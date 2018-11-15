#define _GNU_SOURCE
#include "execution.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include  <sys/types.h>
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

char *to_execute(struct node *parent, size_t i, size_t index)
{
    size_t len = strlen(parent->children[i]->instr) + 2;
    char *result = calloc(len);
    i++;
    result = memcpy(result, parent->children[i]->instr, len);
    for (; i < parent->nbchild && i < index; i++)
    {
        result[len] = ' ';
        len += strlen(parent->children[i]->instr) + 2;
        result = realloc(result, len);
        result = strcat(result, parent->children[i]->instr);
    }
    return result;
}

int exec_command(char *string)
{
    pid_t pid = fork();
    if (pid == -1)//error
    {
        fprintf(stderr,"42sh : fork : An error occured.\n");
        exit(1);
    }
    if (pid == 0)//child
    {
        execvp(string);
        exit(1);
    }
    else//father
    {
        int status = 0;
        waitpid(pid, &status, 0);
        return status;
    }
}

int get_index(struct node *cond, size_t index)
{
    for (size_t i = index; i < cond->nbchild; i++)
    {
        if (cond->children[i]->token == LOGICAL_AND ||
                cond->children[i]->token == LOGICAL_OR)
            return i;
    }
    return -1;
}
int if_cond(struct node *n)
{
    struct node *condition = n->children[0];
    int res = 0;
    for (size_t i = 0; i < condition->nbchild;)
    {
        int index = get_index(condition, i + 1);
        size_t ind;
        if (index == -1)
            ind = parent->nbchild;
        else
            ind = index;
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
    return res; //? what is the return value, void fct ?
}
/*
int while_cond(struct node *n)
{
    struct node *condition = n->children[0];
    int res = 0;
    for (size_t i = 0; i < condition->nbchild;)
    {
        int index = get_index(condition,i + 1);
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

    if (n->type != BODY && n->type != ROOT)
    {
        int res = 0;
        if (n->type == IF)
        {
            res = if_cond(n);
            if (res)
                traversal_ast(n->children[1]);
            else
                traversal_ast(n->children[2]);
        }
        if (n->type == WHILE)
        {
            res = while_cond(n);
        }
        if (n->type == FOR)
            return for_cond(n);
    }
    int rvalue = exec_command(n->instruction);
    if (!rvalue)
    {
        for (int i = 0; i < n->nbchild; i++)
        {
            int r = traversal_ast(n->children[i]);
            if (r != 0)
                return r;
        }
    }
    else
        return rvalue;
}

int execution_ast(struct node *n)
{
    /*struct tab_a *tab = calloc(1, sizeof(struct tab_a));
      tab->capacity = 10;
      struct assignment **assignment = malloc(sizeof(struct assignment *)
     * capacity);
     tab->assignment = assignment;*/
    return traversal_ast(n)//call with tab;
}
