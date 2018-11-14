#include "linked_list.h"
#include "grammar_check.h"

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

struct nL *g_ruleif(struct nL *tok)
{
    return tok;
}


struct nL *g_rulecase(struct nL *tok)
{
    if (tok->elem->type != CASE)
        return NULL;

    tok = tok->next;
    if (!tok)
        return NULL;

    int conf = is_conform(tok);
    if (tok->elem->type != WORD || conf == 1 || conf == 2)
        return NULL;

    tok = tok->next;
    if (!tok)
        return NULL;

    while (tok->elem->type == ENDOF)
    {
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    if (tok->elem->type != IN)
        return NULL;

    tok = tok->next;
    if (!tok)
        return NULL;

    while (tok->elem->type == ENDOF)
    {
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    struct token *save = g_caseclause(tok);
    tok = save ? save : tok;

    tok = tok->next;
    if (!tok)
        return NULL;

    return tok->type == ESAC ? tok : NULL;
}


struct nL *g_ruleuntil(struct nL *tok)
{
    if (tok->elem->type != UNTIL)
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

    tok = g_dogroup(tok);
    return tok;
}

struct nL *g_rulewhile(struct nL *tok)
{
    if (tok->elem->type != WHILE)
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

    tok = g_dogroup(tok);
    return tok;
}


struct nL *g_rulefor(struct nL *tok)
{
    return tok;
}


