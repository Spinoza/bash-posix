#ifndef STACK_H
#define STACK_H

#include "arithmetic.h"

struct nS
{
    struct bt_node *elem;
    struct nS *next;
};

struct stack
{
    int size;
    struct nS *head;
};

struct stack *init_stack(void);

void *peek(struct stack *s);

void free_stack(struct stack *s);

struct stack *push(struct stack *s, void *elem);

void *pop(struct stack *s);
#endif /* ! STACK_H */
