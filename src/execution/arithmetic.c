#include "arithmetic.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

void add_to_list(struct bt_node **list, struct bt_node *new,
        int *capacity, int *nb_node)
{
    if (*nb_node == *capacity)
    {
        *capacity *= 2;
        list = realloc(list, sizeof(struct bt_node) * *capacity);
    }
    list[*nb_node] = new;
    *nb_node = *nb_node + 1;
}

enum oper get_op(char *string)
{
    if (!string)
        return 0;
    //z is here to match with the enum
    char *list_op = "z+-*/^~()";
    for (int i = 0; *(list_op + i); i++)
    {
        if (string[0] == list_op[i])
            return i;
    }
    return 0;
}

char *next_string(char *string)
{
    while (string && string[0] == ' ')
        string ++;
    return string;
}

void free_list_bt_node(struct bt_node **list, int nb_node)
{
    for (int i = 0; i < nb_node; i++)
        free(list[i]);
    free(list);
}

struct bt_node **list_arithmetic(char **input)
{
    char *string = *input;
    int parenthesis = 2;
    int capacity = 10;
    int nb_node = 0;
    struct bt_node **list = calloc(10, sizeof(struct bt_node*));
    struct bt_node *prev = NULL;
    struct bt_node *new = calloc(1, sizeof(struct bt_node));
    char *number = calloc(15, sizeof(char));
    int nbdigits = 0;
    int nb = 1;
    while (parenthesis && string)
    {
        if (new->op || new->nb)
        {
            number = calloc(15, sizeof(char));
            add_to_list(list, new, &capacity, &nb_node);
            prev = new;
            nb = 1;
            nbdigits = 0;
            new = calloc(1, sizeof(struct bt_node));
        }
        enum oper op = get_op(string);
        if (op == CLOSE_PAR_OPER)
            parenthesis--;
        if (op == OPEN_PAR_OPER)
            parenthesis++;
        if (prev && prev->nb) //précédent et précédent == chiffre
        {
            if (!op) //courant = chiffre
            {
                //chiffre suivi de chiffre
                fprintf(stderr, "42sh: error in arithmetic expression");
                free_list_bt_node(list, nb_node);
                return NULL;
            }
            new->op = op;
            string = next_string(string);
        }
        else
        {
            while (op)
            {
                if (op == PLUS || op == MINUS)
                {
                    if (op == MINUS)
                        nb = -nb;
                }
                else
                {
                    fprintf(stderr, "error in arithmetic expression");
                    free_list_bt_node(list, nb_node);
                    free(number);
                    return NULL;
                }
                string = next_string(string);
                op = get_op(string);
            }
            while (string && !op)
            {
                number[nbdigits++] = string[0];
                op = get_op(string);
                string++;
            }
            new->nb = nb * atoi(number);
            free(number);
        }
    }
    *input = string;
    return list;
}
struct bt_node *build_tree(struct bt_node **list_node)
{
    <
}
int eval_tree(struct bt_node *bt_node);
int binary_evaluation(char **string)
{
    int result = 0;
    struct bt_node **list_node = list_arithmetic(string);
    struct bt_node *root = build_tree(list_node);
    return eval_tree(root);
}
