#include <stddef.h>
#include <stdlib.h>
#include "stack.h"

struct stack *init_stack(void)
{
    struct stack *s = malloc(sizeof(struct stack));
    if (!s)
        return NULL;

    s->size = 0;
    s->head = NULL;
    return s;
}

struct stack *push(struct stack *s, void *elem)
{
    struct nS *data = malloc(sizeof(struct nS));
    if (!s || !data)
        return s;

    data->elem = elem;
    data->next = NULL;

    if (!s->head)
    {
        s->size++;
        s->head = data;
        return s;
    }

    data->next = s->head;
    s->head = data;
    s->size++;
    return s;
}

void *pop(struct stack *s)
{
    if (!s)
        return NULL;

    s->size--;
    struct nS *head = s->head;
    s->head = s->head->next;

    void *elem = head->elem;
    free(head);

    return elem;
}

void *peek(struct stack *s)
{
    return s->head->elem;
}

void empty_stack(struct stack *s)
{
    while (s->size > 0)
    {
        struct nS *head = s->head;
        s->size--;
        s->head = head->next;
        free(head->elem);
        free(head);
    }

    s->head = NULL;
}

void free_stack(struct stack *s)
{
    empty_stack(s);
    free(s);
}
