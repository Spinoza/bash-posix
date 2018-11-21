#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include "linked_list.h"
#include "token.h"
#include "grammar_check.h"
#include "handlePS1.h"

struct nL *g_pipeline(struct nL *tok)
{
    if (!strcmp("!", tok->elem->name))
    {
        tok = tok->next;
    }

    if (!tok)
    {
        return NULL;
    }
    tok = g_command(tok);
    if (!tok)
    {
        return NULL;
    }
    struct nL *save;
    while (1)
    {
        save = tok;
        tok = tok->next;
        if (!tok)
            return NULL;
        if (tok->elem->type == PIPE)
        {
            struct nL *stock = tok;
            tok = tok->next;
            while (!tok || tok->elem->type == ENDOF)
            {
                if(!tok)
                {
                    stock->prev->next = handletty();
                    tok = stock->prev;
                }
                tok = stock;
                tok = tok->next;
            }
            tok = g_command(tok);
            if (!tok)
                return NULL;
        }
        else
        {
            return save;
        }
    }
}

struct nL *g_andor(struct nL *tok)
{
    tok = g_pipeline(tok);
    if (!tok)
    {
        return NULL;
    }
    struct  nL *save;
    while (1)
    {
        save = tok;
        tok = tok->next;
        if (!tok)
            return NULL;
        if (tok->elem->type == LOGICAL_AND || tok->elem->type == LOGICAL_OR)
        {
            struct nL *stock = tok;
            tok = tok->next;
            while (!tok || tok->elem->type == ENDOF)
            {
                if(!tok)
                {
                    stock->prev->next = handletty();
                    tok = stock->prev;
                }
                stock = tok;
                tok = tok->next;
            }
            tok = g_pipeline(tok);
            if (!tok)
                return NULL;
        }
        else
        {
            return save;
        }
    }
}


struct nL  *g_list(struct nL *tok)
{
    tok = g_andor(tok);
    if (!tok)
    {
        return NULL;
    }
    while (1)
    {
        tok = tok->next;
        if (!tok)
        {
            return NULL;
        }
        if (tok->elem->type == ENDOF)
        {
            return tok;
        }
        else if ((tok->elem->type == SEMICOLON)||
           (tok->elem->type == AND))
        {
            tok = tok->next;
            if (!tok)
            {
                return NULL;
            }
            if (tok->elem->type == ENDOF)
            {
                return tok;
            }

            tok = g_andor(tok);
            if (!tok)
            {
                return NULL;
            }
        }
        else
        {
            return NULL;
        }

    }
}

int grammar_check (struct linked_list *tokens)
{
    struct nL *tok = tokens->head;
    if (tok->elem->type == ENDOF)
        return 1;

     tok = g_list(tok);

    if (!tok)
        return 0;
    if (tok->elem->type == ENDOF)
        return 1;

    return 0;
}


