#include "arithmetic.h"
#include "stack.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX(A, B) ((A < B ? A : B))

static int nbdigits(int r)
{
    int i = 0;
    for (; r > 0; r/=10, i++);
    return i;
}
static char *itoa(int nb)
{
    int s = nbdigits(nb);
    char *number = malloc(sizeof(char) * (s + 1));
    if (nb == 0)
    {
        number[0] = '0';
        number[1] = '\0';
        return number;
    }
    for (int i = s-1; i >= 0; i--)
    {
        number[i] = '0' + nb % 10;
        nb /= 10;
    }
    number[s] = '\0';
    return number;
}

static void add_to_list(struct arith_list *arith_list, struct bt_node *new)
{
    if (arith_list->nb_nodes == arith_list->capacity)
    {
        arith_list->capacity *= 2;
        arith_list->list = realloc(arith_list->list,
                sizeof(struct bt_node) * arith_list->capacity);
    }
    arith_list->list[arith_list->nb_nodes] = new;
    arith_list->nb_nodes++;
}

static enum oper get_op(char *string)
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

static char *next_string(char *string)
{
    while (string && string[0] == ' ')
        string ++;
    return string;
}

void free_arith_list(struct arith_list *arith_list)
{
    for (int i = 0; i < arith_list->nb_nodes; i++)
        free(arith_list->list[i]);
    free(arith_list->list);
    free(arith_list);
}

struct arith_list *list_arithmetic(char *string)
{
    int parenthesis = 2;
    struct arith_list *arith_list = calloc(1, sizeof(struct arith_list));
    arith_list->list = calloc(10, sizeof(struct bt_node*));
    arith_list->capacity = 10;
    arith_list->nb_nodes = 0;
    struct bt_node *new = calloc(1, sizeof(struct bt_node));
    char *number = calloc(15, sizeof(char));
    int nbdigits = 0;
    int nb = 1;
    enum oper prev_op = 0;
    while (parenthesis && string)
    {
        if (new->op || new->nb)
        {
            number = calloc(15, sizeof(char));
            add_to_list(arith_list, new);
            prev_op = new->op;
            nb = 1;
            nbdigits = 0;
            new = calloc(1, sizeof(struct bt_node));
        }
        enum oper op = get_op(string);
        if (prev_op == 0)
        {
            if (!op || op == OPEN_PAR_OPER || op == CLOSE_PAR_OPER)
            {
                //nb --> nb
                fprintf(stderr, "42sh: error in arithmetic expression");
                free_arith_list(arith_list);
                return NULL;
            }
            new->op = op;
            string = next_string(string);
        }
        else
        {
            //prev is null or an operator : hence we can have
            //a parenthesis
            if (op == OPEN_PAR_OPER || op == CLOSE_PAR_OPER)
            {
                if (op == CLOSE_PAR_OPER)
                    parenthesis--;
                if (op == OPEN_PAR_OPER)
                    parenthesis++;
                new->op = op;
                string = next_string(string);
            }//or a number
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
                        free_arith_list(arith_list);
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
    }
    return arith_list;
}

long int power(long int nb, long int exp, int res)
{
    if (!exp)
        return 1;
    if (exp == 1)
        return res;
    return power(nb, exp - 1, res * nb);
}
long int compute(struct bt_node *left, struct bt_node *oper,
        struct bt_node *right)
{
    int res = 0;
    switch (oper->op)
    {
        case PLUS:
            res =left->nb + right->nb;
            break;
        case MINUS:
            res = left->nb - right->nb;
            break;
        case MULTIPLY:
            res =left->nb * right->nb;
            break;
        case DIVIDE:
            res = left->nb / right->nb;
            break;
        case POWER:
            res = power(left->nb, right->nb, 1);
            break;
        default:
            fprintf(stderr,"42sh: arithmetic expansion, bad operator");
            return 0;
    }
    return res;
}

struct stack *eval_nodes(struct stack *stack)
{
    struct bt_node *right = pop(stack);
    if (right->op == CLOSE_PAR_OPER)
    {
        free(right);
        right = pop(stack);
    }
    struct bt_node *oper = pop(stack);
    struct bt_node *left = pop(stack);
    long int res = compute(left, oper, right);
    struct bt_node *res_node = calloc(1, sizeof(struct bt_node));
    res_node->nb = res;
    return push(stack, res_node);
}

int get_precedence(struct bt_node *node)
{
    if (!node->op)
        return -1;
    if (node->op == OPEN_PAR_OPER)
        return 0;
    return node->op / 2;
}

long int eval_tree(struct stack *stack, struct arith_list *list, int *index)
{
    int precedence = 0;
    while (*index < list->nb_nodes)
    {
        struct bt_node *current = list->list[*index];
        if (current->nb)
            stack = push(stack, current);
        if (current->op == CLOSE_PAR_OPER)
        {
            struct bt_node *top = peek(stack);
            while (top->op != OPEN_PAR_OPER)
            {
                stack = eval_nodes(stack);
                top = peek(stack);
            }
            free(pop(stack));
        }
        if (current->op)
        {
            int next_precedence = get_precedence(current);
            if (next_precedence <= precedence)
                stack = eval_nodes(stack);
            else
                precedence = next_precedence;
        }
        *index = *index + 1;
    }
    while (stack->size != 1)
    {
        stack = eval_nodes(stack);
    }
    long int r = stack->head->elem->nb;
    free(stack->head->elem);
    return r;
}

char *arith_expansion(char *string)
{
    struct arith_list *list = list_arithmetic(string);
    if (list->nb_nodes == 1)
    {
        return itoa(list->list[0]->nb);
    }
    int index = 0;
    struct stack *stack = stack_init();
    long int r = eval_tree(stack, list, &index);
    free_arith_list(list);
    free_stack(stack);
    return itoa(r);;
}
