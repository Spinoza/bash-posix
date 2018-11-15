#include "execution.h"

//if while until for

void add_assignement(char *string, struct tab_a *tab)
{

}

char *translate(char *name, struct list_a **la)
{
    for (int i = 0; i < la->nb; i++)
    {
        if (strcmp(name, la->assignement->name))
            return la->assignement->value;
    }
}

int if_execution(struct node *n, struct list_a **la)
{
    struct node *condition = n->children[0];
    int cond = 1;
    for (int i = 0; i < condition->nbchild; i++)
    {
        if (condition->children[i])

    }
    if (cond)
        execution_ast(n->children[1]);
    else
        execution_ast(n->children[2]);
}

int execution(struct node *n, struct list_a **la)
{
    if (n->type == IF)
        return if_execution(n);
    if (n->type == WHILE || n->type == FOR)
    {
        if (execution(n->children[0]))
            return execution(n->children[1]);
        else
            return 0;
    }
    else
    {
        if (n->instruction)
            return 1;
    }
}

int execution_ast(struct node *n, struct list_a **la)
{
    if (n->type != BODY && n->type != ROOT)
        return execution(n);
    for (int i = 0; i < n->nbchild; i++)
    {
        int r = execution_ast(n->children[i]);
        if (r != 0)
            return r;
    }
}


/*
int main (void)
{
    //call the ast creation
    struct list_a *la = calloc(1, sizeof(struct list_assignement));
    struct linked_list *l = init_link();
    la->list = l;
    execution_ast(ast, la);
    return 1;
}*/
