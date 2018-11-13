#include <stddef.h>
#include <stdlib.h>
#include "queue.h"

struct queue *init_queue(void)
{
    struct queue *q = malloc(sizeof (struct queue));
    if (!q)
    {
        return NULL;
    }

    q->head = NULL;
    q->size = 0;
}

struct queue *enqueue(struct queue *q, void *elem)
{
    struct nQ *node = malloc(sizeof(struct nQ));
    if (!q || !node)
        return q;

    node->next = NULL;
    node->elem = elem;

    if (!q->head)
    {
        q->size ++;
        q->head = node;
        return q;
    }

    q->size++;
    q->head->next = node;
    return q;
}

void *dequeue(struct queue *q)
{
    if (!q)
        return NULL;

    struct nQ *node = q->head;
    q->head = q->head->next;

    void *elem = node->elem;
    free(node);

    return elem;
}

void empty_queue(struct queue *q)
{
    while (q->size > 0)
    {
        struct nQ *node = q->head;
        q->head = q->head->next;
        free(node->elem);
        free(node);
        q->size--;
    }

    q->head = NULL;
    return;
}

void free_queue(struct queue *q)
{
    empty_queue(q);
    free(q);
}
