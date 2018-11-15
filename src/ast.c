#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "linked_list.h"
#include "token.h"

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
    p->nbchild++;
    return p;
}

static enum ntype node_getype(enum type type)
{

    switch (type)
    {
        case 0:
            return IF;
        case 1:
            return BODY;
        case 2:
            return EBODY;
        case 6:
            return WHILE;
        case 7:
            return FOR;
        case 8:
            return UNTIL;
        case 9:
            return CASE;
        case 10:
            return BODY;
        case 18:
            return IN;
        case 20:
            return ELIF;
        default:
            return INSTRUCT;
    }

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

static struct node* init_node(char *instr, enum type type)
{
    struct node* node = malloc (sizeof(struct node));
    if (!node)
        return NULL;

    node->nbchild = 0;
    node->instr = instr;
    node->type = node_getype(type);
    node->children = NULL;
    node->next = NULL;

    return node;
}

static nL *build_aux(struct node *r, struct nL *tok)
{
    if(r->type == ROOT)
    {
        struct node *new = init_node(tok->elem->name, tok->elem->type);
        add_node(r, new);
        tok = build_aux(new, tok->next);
    }
    if(r->type == IF)
    {
        struct node *new = init_node("condition", 23);
        add_node(r, new);
        tok = build_aux(new, tok);
        new = init_node("then", THEN);
        add_node(r, new);
        tok = build_aux(then, tok->next);
        while(tok->elem->type == ELIF)
        {
            new = init_node("elif", ELIF);
        }
    }
}

struct node* build_ast(struct linked_list *tokens)
{
    struct node *tree = init_node();
    build_aux(tree, tokens->head);
    return tree;
}
