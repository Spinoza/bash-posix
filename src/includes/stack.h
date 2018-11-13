#ifndef STACK_H
#define STACK_H

struct nS
{
    void *elem;
    struct nS *next;
};

struct stack
{
    size_t size;
    struct nS *head;
};

struct stack *init_stack(void);
struct stack *push(struct stack *s, void *elem);
void *pop(struct stack *s);
void empty_stack(struct stack *s);
void free_stack(struct stack *s);

#endif /* STACK_H */
