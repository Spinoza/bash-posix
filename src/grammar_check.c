#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include "linked_list.h"
#include "token.h"
#include "grammar_check.h"

static struct nL *g_funcdec(struct nL *tok)
{
    if (tok->type != WORD)
        return NULL;

    if (!strcmp(tok->name, "function"))
    {
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    if (tok->type != WORD)
        return NULL;

    tok = tok->next;
    if (!tok)
        return NULL;
    if (strcmp(tok->name, "("))
        return NULL;
    tok = tok->next;
    if (!tok)
        return NULL;
    if (strcmp(tok->name, ")"))
        return NULL;
    tok = tok->next;
    if (!tok)
        return NULL;

    while (tok->type == ENDOF)
    {
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    tok = shell_command(tok);
    return tok;
}

static struct nL *g_simplecommand(struct nL *tok)
{
    struct nL *save = g_prefix(tok);

    if (!save)
    {
        save = g_element(tok);
        if (!save)
        {
            return NULL;
        }

        while (save)
        {
            tok = save;
            save = save->next;
            if (!save)
                return NULL;
            save = g_element(save);
        }
        return tok;
    }

    else
    {
        while (save)
        {
            tok = save;
            save = save->next;
            if (!save)
                return NULL;
            save = g_prefix(save);
        }

        save = tok->next;
        if (!save)
            return NULL;
        save = g_element(save);
        if (!save)
        {
            return tok;
        }

        while (save)
        {
            tok = save;
            save = save->next;
            if (!save)
                return NULL;
            save = g_element(save);
        }
        return tok;
    }

}

static struct nL *g_command(struct nL *tok)
{
    return tok;
}

static struct nL *g_pipeline(struct nL *tok)
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
    if(!tok)
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
            tok = tok->next;
            if (!tok)
                return NULL;
            while (tok->elem->type == ENDOF)
            {
                tok = tok->next;
                if(!tok)
                    return NULL;
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

static struct nL *g_andor(struct nL *tok)
{
    tok = g_pipeline(tok);
    if(!tok)
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
            tok = tok->next;
            if (!tok)
                return NULL;
            while (tok->elem->type == ENDOF)
            {
                tok = tok->next;
                if (!tok)
                    return NULL;
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


static struct nL  *g_list(struct nL *tok)
{
    tok = g_andor(tok);
    if(!tok)
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
            if(!tok)
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


