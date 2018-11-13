#ifndef QUEUE_H
#define QUEUE_H
#include <stdlib.h>

#include "token.h"

struct nQ
{
    struct token *elem;
    struct nQ *next;
};

struct queue
{
    size_t size;
    struct nQ *head;
    struct nQ *tail;
};

struct queue *init_queue(void);
struct queue *enqueue(struct queue *q, void *elem);
void *dequeue(struct queue *q);
void empty_queue(struct queue *q);
void free_queue(struct queue *q);

#endif /* ! QUEUE_H */
