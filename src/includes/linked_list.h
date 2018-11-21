#ifndef LINK_H
#define LINK_H

#include <stdlib.h>
#include "token.h"

struct nL
{
    struct token *elem;
    struct nL *next;
    struct nL *prev;
};

struct linked_list
{
    size_t size;
    struct nL *head;
};

struct linked_list *init_link(void);
struct linked_list *add(struct linked_list *l, struct token *elem);
void free_list(struct linked_list *l);

#endif /* ! LINK_H */
