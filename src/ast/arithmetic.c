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
    if (n->instr[0] == '-')
    {
        n = isanumber(n-instr + 1);
        if (n != -1)
            return -n;
    }
    return 0;
}

void add_to_tree(struct bt_node *new, struct bt_node *root)
{

}

struct bt_node *arithmetic_expansion(struct node *n)
{
    int i = 0;
    struct bt_node *bt = calloc(1, sizeof(struct bt_node));
    struct node *iter = n;
    struct bt_node *prev = NULL;
    while (iter->tokentype != END_ARTHMETIC)
    {
        struct bt_node *new = calloc(1, sizeof(struct bt_node));
        char *number = iter->instr;

        if (prev && prev->operateur)
        {

        }
        bt->data = get_number(iter);
        prev = new;
        iter = iter->next;
    }
    return bt;
}

int binary_evaluation()
{
    int result = 0;

    return result;
}
