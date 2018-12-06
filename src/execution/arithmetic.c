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
    int negative = 0;
    if (nb < 0)
    {
        negative = 1;
        nb *= -1;
    }
    int s = nbdigits(nb) + negative;
    char *number = malloc(sizeof(char) * (s + 1));
    if (nb == 0)
    {
        number[0] = '0';
        number[1] = '\0';
        return number;
    }
    for (int i = s-1; i >= negative; i--)
    {
        number[i] = '0' + nb % 10;
        nb /= 10;
    }
    if (negative)
        number[0] = '-';
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

static enum oper get_op(char *string, int *index)
{
    if (!string)
        return 0;
    //z is here to match with the enum
    char *list_op = "z+-*/^~() ";
    for (int i = 0; *(list_op + i); i++)
    {
        if (string[*index] == list_op[i])
        {
            *index = *index + 1;
            return i;
        }
    }
    return 0;
}

static int next_token(char *string, int index)
{
    for (; string[index] && string[index] == ' '; index++);
    if (!string[index])
        index++;
    return index;
}

void free_arith_list(struct arith_list *arith_list)
{
    for (int i = 0; i < arith_list->nb_nodes; i++)
        free(arith_list->list[i]);
    free(arith_list->list);
    free(arith_list);
}

int set_number(struct bt_node *new, char *string, int *index,
        struct arith_list *arith_list)
{
    char *number = calloc(15, sizeof(char));
    int sign = 1;
    enum oper op = get_op(string, index);
    int nbdigits = 0;
    while (op)
    {
        if (op == PLUS || op == MINUS)
        {
            if (op == MINUS)
                sign = -sign;
        }
        else
        {
            fprintf(stderr,
                    "42sh: error in arithmetic expression.\n");
            free_arith_list(arith_list);
            free(number);
            return 0;
        }
        *index = next_token(string, *index);
        op = get_op(string, index);
    }
    while (string[*index] &&
            string[*index] >= '0' && string[*index] <= '9')
    {
        number[nbdigits++] = string[*index];
        *index = *index + 1;
    }
    new->nb = sign * atoi(number);
    free(number);
    return 1;
}

static struct arith_list *list_arithmetic(char *string)
{
    struct arith_list *arith_list = calloc(1, sizeof(struct arith_list));
    arith_list->list = calloc(10, sizeof(struct bt_node*));
    arith_list->capacity = 10;
    arith_list->nb_nodes = 0;
    struct bt_node *new = calloc(1, sizeof(struct bt_node));
    int index = 0;
    enum oper prev_op = -1;
    int parenthesis = 0;
    while (string[index])
    {
        if (string[index] == ' ')
            index = next_token(string, index);
        enum oper op = get_op(string, &index);
        if (prev_op != 0 && (op == PLUS || op == MINUS))
        {
            op = 0; //Operator was actually a sign for the next number
            index--;
        }
        if (prev_op == 0)
        {
            if (!op || op == OPEN_PAR_OPER || op == CLOSE_PAR_OPER)
            {
                fprintf(stderr, "42sh: error in arithmetic expression.\n");
                free_arith_list(arith_list);
                return NULL;
            }
            new->op = op;
            index = next_token(string, index);
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
                index = next_token(string, index);
            }//or a number
            else
            {
                if (!set_number(new,string, &index, arith_list))
                    return NULL;
            }
        }
        add_to_list(arith_list, new);
        prev_op = new->op;
        new = calloc(1, sizeof(struct bt_node));
    }
    if (parenthesis != 0)
    {
        fprintf(stderr, "42sh: arithmetic expansion missing a parenthesis");
        free_arith_list(arith_list);
        return NULL;
    }
    return arith_list;
}

static long int power(long int nb, long int exp, int res)
{
    if (!exp)
        return 1;
    if (exp == 1)
        return res;
    return power(nb, exp - 1, res * nb);
}

static long int compute(struct bt_node *left, struct bt_node *oper,
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
            fprintf(stderr,"42sh: arithmetic expansion, bad operator.\n");
            return 0;
    }
    return res;
}

static struct stack *eval_nodes(struct stack *stack)
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

static int get_precedence(struct bt_node *node)
{
    switch (node->op)
    {
        case NOT_AN_OP:
            return -1;
        case WHITE_SPACE:
            return -1;
        case PLUS:
            return 1;
        case MINUS:
            return 1;
        case MULTIPLY:
            return 2;
        case DIVIDE:
            return 2;
        case TILDE:
            return 3;
        case POWER:
            return 3;
        case OPEN_PAR_OPER:
            return 0;
        case CLOSE_PAR_OPER:
            return 4;
    }
    fprintf(stderr,"42sh: bad operator in arithmetic expression");
    return -1;
}

long int eval_list(struct stack *stack, struct arith_list *list, int *index)
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
            {
                precedence = next_precedence;
                stack = push(stack, current);
            }
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
    if (!list)
        return NULL;
    if (list->nb_nodes == 1)
    {
        return itoa(list->list[0]->nb);
    }
    int index = 0;
    struct stack *stack = init_stack();
    long int r = eval_list(stack, list, &index);
    free_arith_list(list);
    free_stack(stack);
    return itoa(r);;
}
