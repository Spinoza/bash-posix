#include "ast.h"



int execution_ast(struct node *n)
{

    for (int i = 0; i < n->nbchild; i++)
    {
        execution_ast(n->children[i])
    }
}
