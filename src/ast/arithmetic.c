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

void add_to_list(struct bt_node **list, struct bt_node *root)
{

}

int get_op(char *string)
{
    if (!string)
        return 0;
    char *list_op = "+-*/^~";
    for (int i = 0; *(list_op + i); i++)
    {
        if (string[0] == list_op[i])
            return i - 1;
    }
    return 0;
}

struct bt_node *arithmetic_expansion(struct node *n)
{
    int i = 0;
    struct bt_node *bt = calloc(1, sizeof(struct bt_node));
    struct node *iter = n;
    struct bt_node *prev = NULL;
    struct bt_node *new = calloc(1, sizeof(struct bt_node));
    new->number = NULL;
    while (iter)
    {
        if (new->operateur || new->number)
        {
            add_to_list(new, bt);
            prev = new;
            iter = iter->next;
            struct bt_node *new = calloc(1, sizeof(struct bt_node));
        }
        while (!bt_node->op && !bt_node->nb)
        {
            if (!string)
            {
                if (iter->next)
                    iter = iter-next;
                else
                    iter = NULL;
                if (iter)
                    string = iter->instr;
            }
            int op = get_op(string);
            if (prev && !prev->operateur)
            {
                if (!op)
                    fprintf(stderr, "error in arithmetic expression");
                //need to  return
                new->op = op - 1;
            }
            else
            {
                while (op)
                {
                    op --;
                    if (op == '+' || op == '-')
                    {
                        if (op == '-')
                            int sign = new->nb->sign + 1;
                        if (sign == 2)
                            new->nb->sign = '+';
                        else
                            new->nb->sign = '-';
                    }
                    else
                        fprintf(stderr, "error in arithmetic expression");
                    //need to return
                    string ++;
                    op = get_op(string);
                }

                //number
            }
        }
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
