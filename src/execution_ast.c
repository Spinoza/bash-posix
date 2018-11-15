#include "ast.h"

int if_execution(struct node *n)
{
    if (execution(n->children[0]))
        execution_ast(n->children[1]);
    else
        execution_ast(n->children[2]);
}

int execution(struct node *n)
{
    if (n->type == IF)
        return if_execution(n);
    if (n->type == WHILE)
    {
        if (execution(n->children[0]))
            return execution(n->children[1]);
        else
            return 0;
    }
    if (n->type == )
}


int execution_ast(struct node *n)
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
