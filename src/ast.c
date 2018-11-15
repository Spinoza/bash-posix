#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "linked_list.h"

static struct node* add_node(struct node* p, struct node *c)
{
    struct node *ch = p->children;
    if (!ch)
    {
        p->children = c;
        return p;
    }
    while(ch->next)
    {
        ch = ch->next;
    }

    ch->next = c;
    return p;
}

static struct node* setup_node(struct node *n, char *instr, enum ntype type)
{
    n->instr = instr;
    n->ntype = type;
    return n;
}

static void free_node(struct node *n)
{
    if (!n)
        return;

    if (n->instr)
        free(n->instr);

    free_node(node->children);
    free_node(node->next);
    free(n);
}

static struct node* init_node(void)
{
    struct node* node = malloc (sizeof(struct node));
    if (!node)
        return NULL;

    node->nbchild = 0;
    node->instr = NULL;
    node->type = ROOT;
    node->children = NULL;
    node->next = NULL;

    return node;
}

struct node* build_ast(struct linked_list *tokens);
