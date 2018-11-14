#include <stddef.h>
#include <stdlib.h>
#include "linked_list.h"

struct linked_list *init_link(void)
{
    struct linked_list *l = malloc(sizeof (struct queue));
    if (!l)
    {
        return NULL;
    }

    l->head = NULL;
    l->size = 0;

    return q;
}

struct linked_list *add(struct linked_list *l, struct token *elem)
{
    if (!q)
        return q;
    struct nL *node = malloc(sizeof(struct nL));
    if(!node)
        return q;

    node->next = NULL;
    node->elem = elem;

    if (!q->head)
    {
        q->size++;
        q->head = node;
        return q;
    }

    q->size++;
    struct nL *head = q->head;

    while(head->next)
    {
        head = head->next;
    }

    head->next = node;
    return q;
}

void free_list(struct linked_list *l)
{
    //FIXME : find a way to handle tokens freeing.
    if (!l)
        return;

    struct nL *node = l->head;
    while (node)
    {
        struct nL *copy = node;
        node = node->next;
        free(copy);
    }

    free(l);
}
