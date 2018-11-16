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

        struct nL *save = g_elseclause(tok);
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

    struct nL *save = tok;
    tok = tok->next;
    if(!tok)
        return NULL;
    while (tok->elem->type == WORD && (!strcmp(";;", tok->elem->name)))
    {
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

        save = tok;
    }
    tok = save;
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

static int is_conform(struct nL *tok)
{
    char *args[13] = { ">", "<", ">>", ">&", "<&", ">|", "<>"
                , "!", "{", "}", ";;", "(", ")"};

    for (int i = 0; i < 7; i++)
    {
        if (strcmp(args[i], tok->elem->name) == 0)
            return 1;
    }

    for (int j = 7; j < 13; j++)
    {
        if (strcmp(args[j], tok->elem->name) == 0)
            return 2;
    }

    return 0;
}

struct nL *g_caseitem(struct nL *tok)
{
    if (tok->elem->type == WORD || !(strcmp(tok->elem->name, "(")))
    {
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    if (tok->elem->type == WORD)
        return NULL;

    struct nL *save = tok;
    tok = tok->next;
    if (!tok)
        return NULL;

    while (tok->elem->type == PIPE)
    {
        tok = tok->next;
        if (!tok)
            return tok;

        int conf = is_conform(tok);
        if (tok->elem->type != WORD || conf != 0)
        {
            tok = save;
            break;
        }
        save = tok;
    }
    tok = save;
    tok = tok->next;
    if (!tok)
        return NULL;

    if (strcmp(tok->elem->name, ")"))
        return NULL;

    save = tok;
    tok = tok->next;
    if (!tok)
        return NULL;

    while (tok->elem->type == ENDOF && !strcmp(tok->elem->name, "\n"))
    {
        save = tok;
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    struct nL *save2 = g_compoundlist(tok);
    return (save2 ? save2 : save);
}
