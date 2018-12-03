#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "linked_list.h"

struct linked_list *init_link(void)
{
    struct linked_list *l = malloc(sizeof(struct linked_list));
    if (!l)
    {
        return NULL;
    }
    l->head = NULL;
    l->size = 0;
    return l;
}

struct linked_list *add(struct linked_list *l, struct token *elem)
{
    if (!l)
        return l;
    struct nL *node = malloc(sizeof(struct nL));
    if (!node)
        return l;

    node->next = NULL;
    node->prev = NULL;
    node->elem = elem;

    if (!l->head)
    {
        l->size++;
        l->head = node;
        return l;
    }

    l->size++;
    struct nL *head = l->head;

    while (head->next)
    {
        head = head->next;
    }

    node->prev = head;
    head->next = node;
    return l;
}

void free_list(struct linked_list *l)
{
    if (!l)
        return;

    struct nL *node = l->head;
    while (node)
    {
        struct nL *copy = node;
        node = node->next;
        free(copy->elem->name);
        free(copy->elem);
        free(copy);
    }

    free(l);
}

struct token *token_copy(struct token *tok)
{
    struct token *copy = calloc(1, sizeof(struct token));
    copy->type = tok->type;
    copy->name = calloc(strlen(tok->name) + 1, sizeof(char));
    strcpy(copy->name, tok->name);

    return copy;


}
