#include "arithmetic.h"
#include "stack.h"
#include "ast.h"
#include "globals.h"
#include "builtins.h"
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
    if (!number)
        out_of_memory();
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
        if (!arith_list->list)
            out_of_memory();

    }
    arith_list->list[arith_list->nb_nodes] = new;
    arith_list->nb_nodes++;
}

static enum oper get_op(char *string, int *index)
{
    if (!string)
        return 0;
    //z is here to match with the enum
    char *list_op = "z+-*/^zz() ";
    for (int i = 0; *(list_op + i); i++)
    {
        if (string[*index] == list_op[i])
        {
            *index = *index + 1;
            return i;
        }
    }
    if (string[*index] == '*' && string[*index + 1] == '*')
    {
        *index = *index + 2;
        return POWER;
    }
    if (string[*index] == '&')
    {
        *index = *index + 1;
        if (string[*index] == '&')
        {
            *index = *index + 1;
            return AND_OPER;
        }
        return BIT_AND;
    }
    if (string[*index] == '|')
    {
        *index = *index + 1;
        if (string[*index] == '|')
        {
            *index = *index + 1;
            return OR_OPER;
        }
        return BIT_OR;
    }
    return 0;
}

static int next_token(char *string, int index)
{
    for (; string[index] && string[index] == ' '; index++);
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
    if (!number)
        out_of_memory();
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
                    "42sh: error in arithmetic expression"
                    " unexpected operator %c.\n", string[*index-1]);
            free_arith_list(arith_list);
            free(number);
            global.res = 1;
            global.data->builtins[0].builtin(NULL, global.res);
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

static struct arith_list *build_list(char *string)
{
    struct arith_list *arith_list = calloc(1, sizeof(struct arith_list));
    if (!arith_list)
        out_of_memory();
    arith_list->list = calloc(10, sizeof(struct bt_node*));
    if (!arith_list->list)
        out_of_memory();
    arith_list->capacity = 10;
    arith_list->nb_nodes = 0;
    struct bt_node *new;
    int index = 0;
    enum oper prev_op = -1;
    int parenthesis = 0;
    while (string[index])
    {
        new = calloc(1, sizeof(struct bt_node));
        if (string[index] == ' ')
            index = next_token(string, index);
        enum oper op = get_op(string, &index);
        if (!(prev_op == NOT_AN_OP || prev_op == CLOSE_PAR_OPER)
                && (op == PLUS || op == MINUS))
        {
            op = 0; //Operator was actually a sign for the next number
            index--;
        }
        if (op == OPEN_PAR_OPER || op == CLOSE_PAR_OPER)
        {
            if (op == CLOSE_PAR_OPER)
                parenthesis--;
            if (op == OPEN_PAR_OPER)
                parenthesis++;
        }
        if (prev_op == 0)
        {
            if (!op || op == OPEN_PAR_OPER)
            {
                fprintf(stderr,
                        "42sh: error in arithmetic expression bad syntax.\n");
                free_arith_list(arith_list);
                global.res = 1;
                global.data->builtins[0].builtin(NULL, global.res);
            }
            index = next_token(string, index);
        }
        if (op == NOT_AN_OP)
        {
            if (!set_number(new,string, &index, arith_list))
                return NULL;
        }
        new->op = op;
        add_to_list(arith_list, new);
        prev_op = new->op;
    }
    if (parenthesis != 0)
    {
        fprintf(stderr, "42sh: arithmetic expansion missing a parenthesis\n");
        free_arith_list(arith_list);
        global.res = 1;
        global.data->builtins[0].builtin(NULL, global.res);
    }
    return arith_list;
}

static int power(double nb, long int exp, double res)
{
    if (!exp)
        return 1;
    if (exp == 1)
        return res;
    return power(nb, exp - 1, res * nb);
}

// this function is here to convert double values to int for bitwise operations
static int bit_operations(int left, int right, enum oper op)
{
    if (op == BIT_XOR)
        return left ^ right;
    if (op == BIT_AND)
        return left & right;
    return left | right;
}
static double compute(struct bt_node *left, struct bt_node *oper,
        struct bt_node *right)
{
    switch (oper->op)
    {
        case BIT_XOR:
            return bit_operations(left->nb, right->nb, oper->op);
        case BIT_AND:
            return bit_operations(left->nb, right->nb, oper->op);
        case BIT_OR:
            return bit_operations(left->nb, right->nb, oper->op);
        case PLUS:
            return left->nb + right->nb;
        case MINUS:
            return left->nb - right->nb;
        case MULTIPLY:
            return left->nb * right->nb;
        case DIVIDE:
            if (!right->nb)
                return 0;
            return left->nb / right->nb;
        case POWER:
            return power(left->nb, right->nb, 1);
        case AND_OPER:
            return left->nb && right->nb;
        case OR_OPER:
            return left->nb || right->nb;
        default:
            global.res = 127;
            fprintf(stderr,"42sh: arithmetic expansion, bad operator.\n");
            global.data->builtins[0].builtin(NULL, global.res);
            return -1;
    }
    return -1;
}

static struct stack *eval_nodes(struct stack *stack,
        struct stack *operators_stack)
{
    struct bt_node *right = pop(stack);
    struct bt_node *oper = (operators_stack->head ? pop(operators_stack) : NULL);
    if (oper->op == OPEN_PAR_OPER)
    {
        free(oper);
        push(stack, right);
        return stack;
    }
    struct bt_node *left = (stack->head ? pop(stack) : NULL);
    if (!left)
    {
        global.res = 127;
        fprintf(stderr,"42sh: arithmetic expansion, operators"
                " take two operands.\n");
        global.data->builtins[0].builtin(NULL, global.res);
    }
    if (oper->op == DIVIDE && right->nb == 0)
    {
        global.res = 127;
        fprintf(stderr,"42sh: division by 0.\n");
        global.data->builtins[0].builtin(NULL, global.res);
        free(right);
        free(oper);
        free(left);
        return NULL;
    }
    long int res = compute(left, oper, right);
    free(right);
    free(oper);
    free(left);
    struct bt_node *res_node = calloc(1, sizeof(struct bt_node));
    res_node->nb = res;
    return push(stack, res_node);
}

static int get_precedence(struct bt_node *node)
{
    if (!node)
        return 0;
    switch (node->op)
    {
        case OPEN_PAR_OPER:
            return 0;
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
        case POWER:
            return 3;
        case CLOSE_PAR_OPER:
            return 4;
        case BIT_XOR:
            return 1;
        case BIT_AND:
            return 1;
        case BIT_OR:
            return 1;
        default:
            return 1;
    }
}

struct bt_node *copy_bt_node(struct bt_node *to_copy)
{
    struct bt_node *new = calloc(1, sizeof(struct bt_node));
    if (!arith_list->list)
        out_of_memory();
    new->nb = to_copy->nb;
    new->op = to_copy->op;
    return new;
}

struct stack *pop_stack(struct stack *stack, struct stack *operators_stack,
        int to_parenthesis, int next_precedence)
{
    struct bt_node *top = peek(operators_stack);
    if (!stack || stack->size == 0)
    {
        global.res = 127;
        fprintf(stderr,"42sh: arithmetic expansion, bad expression.\n");
        global.data->builtins[0].builtin(NULL, global.res);
    }
    if (to_parenthesis)
    {
        if (top->op == OPEN_PAR_OPER)
        {
            stack = eval_nodes(stack, operators_stack);
            return stack;
        }
        while (stack && top && top->op != OPEN_PAR_OPER)
        {
            stack = eval_nodes(stack, operators_stack);
            top = peek(operators_stack);
        }
        if (top->op == OPEN_PAR_OPER)
            free(pop(operators_stack));
    }
    else
    {
        int precedence = get_precedence(top);
        while (stack && top && precedence >= next_precedence)
        {
            stack = eval_nodes(stack, operators_stack);
            top = peek(operators_stack);
            precedence = get_precedence(top);
        }
    }
    return stack;
}
/*
 * function to know if the next operator is a parenthesis
 */
long int eval_list(struct stack *stack, struct stack *operators_stack,
        struct arith_list *list, int *index)
{
    int precedence = 0;
    while (stack && *index < list->nb_nodes)
    {
        struct bt_node *current = list->list[*index];
        if (current->op == CLOSE_PAR_OPER)
        {
            stack = pop_stack(stack, operators_stack, 1, 0);
            precedence = get_precedence(peek(operators_stack));
            *index = *index + 1;
            continue;
        }
        if (stack && current->op && current->op != OPEN_PAR_OPER)
        {
            int next_precedence = get_precedence(current);
            if (next_precedence <= precedence)
                stack = pop_stack(stack, operators_stack, 0, next_precedence);
            precedence = next_precedence;
            operators_stack = push(operators_stack, copy_bt_node(current));
        }
        if (stack && current->op == OPEN_PAR_OPER)
        {
            precedence = 0;
            operators_stack = push(operators_stack, copy_bt_node(current));
        }
        if (!current->op)
            stack = push(stack, copy_bt_node(current));
        *index = *index + 1;
    }
    if (!stack)
        return 0;
    while (stack->size != 1 && operators_stack->head)
    {
        stack = eval_nodes(stack, operators_stack);
    }
    long int r = stack->head->elem->nb;
    return r;
}

char *arith_expansion(char *string)
{
    struct arith_list *list = build_list(string);
    if (!list)
        return NULL;
    int index = 0;
    struct stack *stack = init_stack();
    struct stack *operators_stack = init_stack();
    long int r = eval_list(stack, operators_stack, list, &index);
    for (; stack->size;)
    {
        struct bt_node *to_free= pop(stack);
        free(to_free);
    }
    free_arith_list(list);
    free_stack(stack);
    free_stack(operators_stack);
    return itoa(r);;
}
