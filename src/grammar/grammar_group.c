#include <string.h>
#include <stdio.h>
#include "linked_list.h"
#include "grammar_check.h"
#include "handlePS1.h"

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

        struct nL *save= tok;
        tok = tok->next;
        if (!tok)
            return NULL;

        tok = g_elseclause(tok);
        return tok ? tok : save;

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

    tok= tok->next;
    if (!tok)
        return NULL;

    tok = g_compoundlist(tok);
    if (!tok)
        return NULL;

    tok = tok->next;
    if (!tok)
    {
        return NULL;
    }

    return tok->elem->type == DONE ? tok : NULL;
}

struct nL *g_caseclause(struct nL *tok)
{
    tok = g_caseitem(tok);
    if (!tok)
        return NULL;

    struct nL *save = tok;
    tok = tok->next;
    if (!tok)
        return NULL;
    while (tok->elem->type == TWO_SEMIC)
    {
        struct nL *stock = tok;
        tok = tok->next;
        while (!tok ||(tok->elem->type == ENDOF))
        {
            if (!tok)
            {
                struct nL *tok1 = handletty();
                if (!tok1)
                {
                    fprintf(stderr,
                            "42sh: syntax error: unexpected end of file.\n");
                    return NULL;
                }
                stock->prev->next = tok1;
                stock->prev->next->prev = stock->prev;
                tok = stock->prev;
                free(stock->elem->name);
                free(stock->elem);
                free(stock);
            }
            stock = tok;
            tok = tok->next;
        }

        tok = g_caseitem(tok);
        if (!tok)
        {
            tok = save;
            break;
        }

        save = tok;
        tok = tok->next;
    }
    tok = save;
    tok = tok->next;

    if (tok->elem->type != TWO_SEMIC)
    {
        return NULL;
    }
    save = tok;
    tok = tok->next;


    while (!tok || (tok->elem->type == ENDOF))
    {
        if (!tok)
        {
            struct nL *tok1 = handletty();
            if (!tok1)
            {
                fprintf(stderr,
                        "42sh: syntax error: unexpected end of file.\n");
                return NULL;
            }
            save->prev->next = tok1;
            save->prev->next->prev = save->prev;
            tok = save->prev;
            free(save->elem->name);
            free(save->elem);
            free(save);
        }
        save = tok;
        tok = tok->next;
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
    if (tok->elem->type == OPEN_PAR)
    {
        tok = tok->next;
        if (!tok)
            return NULL;
    }
    if (tok->elem->type != WORD || tok->elem->type == EXPAND_W)
    {
        return NULL;
    }

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
        if (tok->elem->type != WORD
                || tok->elem->type == EXPAND_W || conf != 0)
        {
            tok = save;
            break;
        }
        save = tok;
        tok = tok->next;
        if (!tok)
            return NULL;
    }
    tok = save;
    tok = tok->next;
    if (!tok)
        return NULL;

    if (tok->elem->type != CLOSE_PAR)
        return NULL;

    save = tok;
    tok = tok->next;
    while (!tok ||(tok->elem->type == ENDOF &&
                !strcmp(tok->elem->name, "\n")))
    {
        if (!tok)
        {
            struct nL *tok1 = handletty();
            if (!tok1)
            {
                fprintf(stderr,"42sh:syntax error: unexpected end of file.\n");
                return NULL;
            }
            save->prev->next = tok1;
            save->prev->next->prev = save->prev;
            tok=save->prev;
            free(save->elem->name);
            free(save->elem);
            free(save);
        }
        save = tok;
        tok = tok->next;
    }

    struct nL *save2 = g_compoundlist(tok);
    return (save2 ? save2 : save);
}
