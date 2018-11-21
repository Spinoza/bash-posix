#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "linked_list.h"
#include "token.h"
#include "ast.h"

static struct node* add_node(struct node* p, struct node *c)
{
    struct node *ch = p->children;
    if (!ch)
    {
        p->children = c;
        return p;
    }
    while (ch->next)
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
            return A_IF;
        case 1:
            return A_BODY;
        case 2:
            return A_EBODY;
        case 6:
            return A_WHILE;
        case 7:
            return A_FOR;
        case 8:
            return A_UNTIL;
        case 9:
            return A_CASE;
        case 10:
            return A_BODY;
        case 15:
            return A_PIPE;
        case 18:
            return A_IN;
        case 20:
            return A_ELIF;
        case 23:
            return A_CONDITION;
        case 24:
            return A_ROOT;
        default:
            return A_INSTRUCT;
    }

}

void free_node(struct node *n)
{
    if (!n)
        return;

    free_node(n->children);
    free_node(n->next);
    free(n);
}

static struct node* init_node(char *instr, enum type type)
{
    struct node* node = malloc (sizeof(struct node));
    if (!node)
        return NULL;

    node->nbchild = 0;
    node->instr = instr;
    node->tokentype = type;
    node->type = node_getype(type);
    node->children = NULL;
    node->next = NULL;

    return node;
}

static struct nL *build_aux(struct node *r, struct nL *tok)
{
    if (r->type == A_ROOT)
    {
        while (tok->elem->type != ENDOF)
        {
            struct node *new = init_node(tok->elem->name, tok->elem->type);
            add_node(r, new);
            if(new->type != A_INSTRUCT && new->type != A_PIPE)
                tok = build_aux(new, tok->next);
            tok = tok->next;
        }
        return tok;
    }
    if (r->type == A_INSTRUCT || r->type == A_PIPE)
    {
        return tok;
    }
    if ((r->type == A_IF)||(r->type == A_ELIF))
    {
        struct node *new = init_node("condition", 23);
        add_node(r, new);
        tok = build_aux(new, tok);
        new = init_node("then", THEN);
        add_node(r, new);
        tok = build_aux(new, tok->next);
        if(tok->elem->type == ELIF)
        {
            new = init_node("elif", ELIF);
            add_node(r, new);
            tok = build_aux(new, tok->next);
        }
        else if(tok->elem->type == ELSE)
        {
            new = init_node("else", ELSE);
            add_node(r, new);
            tok = build_aux(new, tok->next);
        }
        return tok;
    }
    if (r->type == A_CONDITION)
    {
        while ((tok->elem->type != THEN)&&(tok->elem->type != DO)&&
               (strcmp(")", tok->elem->name)))
        {
            while (tok->elem->type == ENDOF && !(strcmp("\n", tok->elem->name)))
                tok = tok->next;
            struct node *new = init_node(tok->elem->name, tok->elem->type);
            add_node(r, new);
            tok = tok->next;
        }
        return tok;
    }
    if ((r->type == A_BODY)||(r->type == A_EBODY))
    {
        while ((tok->elem->type != ELIF)&&(tok->elem->type != ELSE)&&
              (tok->elem->type != DONE)&&(tok->elem->type != FI)&&
              (strcmp(";;", tok->elem->name)))
        {
            while (tok->elem->type == ENDOF && !(strcmp("\n", tok->elem->name)))
                tok = tok->next;
            struct node *new = init_node(tok->elem->name, tok->elem->type);
            add_node(r, new);
            tok = build_aux(new, tok->next);
        }
        return tok;
    }

    if (r->type == A_WHILE || r->type == A_UNTIL)
    {
        struct node *new = init_node("condition", 23);
        add_node(r, new);
        tok = build_aux(new, tok);
        new = init_node("do", DO);
        add_node(r,new);
        tok = build_aux(new, tok->next);
        return tok;
    }

    if (r->type == A_FOR)
    {
        struct node *new = init_node("condition", 23);
        add_node(r, new);
        tok = build_aux(new, tok);
        new = init_node("do", DO);
        add_node(r, new);
        tok = build_aux(new, tok->next);
        return tok;
    }
    if(r->type == A_CASE)
    {
        struct node *new = init_node(tok->elem->name, tok->elem->type);
        add_node(r, new);
        tok = build_aux(new, tok->next);
        while(tok->elem->type == ENDOF || tok->elem->type == IN ||
             (!strcmp("(", tok->elem->name)))
        {
            tok = tok->next;
        }
        while(tok->elem->type != ESAC)
        {
            new = init_node("case", 10);
            add_node(r, new);
            struct node *new2 = init_node("condition", 23);
            add_node(new, new2);
            tok = build_aux(new2, tok);
            while(tok->elem->type == ENDOF || (!strcmp(")", tok->elem->name)))
            {
                tok = tok->next;
            }
            new2 = init_node("then", 10);
            add_node(new, new2);
            tok = build_aux(new2, tok);
            tok = tok->next;
        }
        return tok;

    }
    return NULL;
}

struct node* build_ast(struct linked_list *tokens)
{
    struct node *tree = init_node("root", ROOT);
    struct nL *res = build_aux(tree, tokens->head);
    return res ? tree : NULL;
}