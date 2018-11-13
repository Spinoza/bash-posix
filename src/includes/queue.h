#ifndef QUEUE_H
#define QUEUE_H

struct nQ
{
    void *elem;
    struct nQ *next;
};

struct queue
{
    size_t size;
    struct nQ *head;
};

struct queue *init_queue(void);
struct queue *enqueue(struct queue *q, void *elem);
void *dequeue(struct queue *q);
void empty_queue(struct queue *q);
void free_queue(struct queue *q);

#endif /* ! QUEUE_H */
