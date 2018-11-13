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
    q->tail = NULL;
    q->size = 0;
    return q;
}

struct queue *enqueue(struct queue *q, void *elem)
{
    if (!q)
        return q;
    struct nQ *node = malloc(sizeof(struct nQ));
    if(!node)
        return q;
    node->next = NULL;
    node->elem = elem;

    if (!q->head)
    {
        q->size ++;
        q->head = node;
        q->tail = node;
        return q;
    }

    q->size++;
    q->tail->next = node;
    q->tail = node;
    return q;
}

void *dequeue(struct queue *q)
{
    if (!q)
        return NULL;

    struct nQ *node = q->head;
    q->head = q->head->next;
    q->size--;
    void *elem = node->elem;
    /*if(node->elem->type == WORD || node->elem->type == ASSIGNMENT_W)
        free(node->elem->name);*/

    free(node);
    return elem;
}

void empty_queue(struct queue *q)
{
    while (q->size > 0)
    {
        struct nQ *node = q->head;
        q->head = q->head->next;
        if(node->elem->type == WORD || node->elem->type == ASSIGNMENT_W)
            free(node->elem->name);
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
