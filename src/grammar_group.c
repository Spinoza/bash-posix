#include <string.h>
#include "linked_list.h"
#include "grammar_check.h"

struct nL *g_elseclause(struct nL *tok)
{
    if (tok->elem->type == ELSE)
    {
        tok = tok->next;
        if (!tok)
            return NULL;
        return g_compoundlist(tok);
    }

    else if (tok->elem->type == ELIF)
    {
        tok = tok->next;
        if (!tok)
            return NULL;
        tok = g_compoundlist(tok);
        if (!tok)
            return NULL;

        tok = tok->next;
        if (!tok)
            return NULL;

        if (tok->elem->type != THEN)
            return NULL;

        tok = tok->next;
        if (!tok)
            return NULL;

        tok = g_compoundlist(tok);
        if (!tok)
            return NULL;

        tok = tok->next;
        if (!tok)
            return NULL;

        struct token *save = g_elseclause(tok);
        return save ? save : tok;

    }

    else
    {
        return NULL;
    }
}

struct nL *g_dogroup(struct nL *tok)
{
    if (tok->elem->type != DO)
        return NULL;

    tok = tok->next;
    if (!tok)
        return NULL;

    tok = g_compoundlist(tok);
    if (!tok)
        return NULL;

    tok = tok->next;
    if (!tok)
        return NULL;

    return tok->elem->type == DONE ? tok : NULL;
}

struct nL *g_caseclause(struct nL *tok)
{
    tok = g_caseitem(tok);
    if (!tok)
        return NULL;

    struct token *save = tok;
    tok = tok->next;
    if(!tok)
        return NULL;
    int count = 0;
    while (tok->elem->type == WORD && (!strcmp(";;", tok->elem->name)))
    {
        if (count != 0)
            save = tok;
        tok = tok->next;
        if (!tok)
            return NULL;

        while (tok->elem->type == ENDOF && !(strcmp("\n", tok->elem->name)))
        {
            tok = tok->next;
            if (!tok)
                return NULL;
        }

        tok = g_caseitem(tok);
        if (!tok)
        {
            tok = save;
            break;
        }

        count++;
    }

    save = tok;
    tok = tok->next;
    if (!tok)
        return NULL;

    if (tok->elem->type == WORD && !(strcmp(";;", tok->elem->name)))
    {
        save = tok;
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    while (tok->elem->type == ENDOF && (!strcmp("\n", tok->elem->name)))
    {
        save = tok;
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    return save;
}

struct nL *g_caseitem(struct nL *tok);
