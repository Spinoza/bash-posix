#include "binary.h"
#include "arithmetic.h"
#include <stdio.h>
#include <stdlib.h>


int get_number(struct node *n)
{
    if (n->tokentype == EXPAND_WORD)
        return atoi(get_assign(n->instr));
    int n = isnumber(n->instr);
    if (n != -1)
        return n;
    return 0;
}

struct binary_tree *arithmetic_expansion(struct node *n)
{
    int i = 0;
    struct binary_tree *bt = calloc(1, sizeof(struct binary_tree));
    struct node *iter = n;
    while (iter->tokentype != END_ARTHMETIC)
    {
        bt->data = get_number(iter);
        iter = iter->next;
    }
    return bt;
}

int binary_evaluation()
{
    int result = 0;

    return result;
}
