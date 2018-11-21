#include <string.h>
#include "linked_list.h"
#include "grammar_check.h"


static int is_conform(struct nL *tok)
{
    char *args[14] = { ">", "<", ">>", ">&", "<&", ">|", "<>"
                , "!", "{", "}", ";;", "(", ")", "function"};

    for (int i = 0; i < 7; i++)
    {
        if (strcmp(args[i], tok->elem->name) == 0)
            return 1;
    }

    for (int j = 7; j < 14; j++)
    {
        if (strcmp(args[j], tok->elem->name) == 0)
            return 2;
    }

    return 0;
}

struct nL *g_ruleif(struct nL *tok)
{
    if(tok->elem->type != IF)
        return NULL;
    tok = tok->next;
    if(!tok)
        return NULL;
    tok = g_compoundlist(tok);
    if(!tok)
        return NULL;
    tok = tok->next;
    if(!tok)
        return NULL;
    if(tok->elem->type != THEN)
        return NULL;
    tok = tok->next;
    if(!tok)
        return NULL;
    tok = g_compoundlist(tok);
    if(!tok)
        return NULL;
    tok = tok->next;
    if(!tok)
        return NULL;
    struct nL *save = g_elseclause(tok);
    if(save)
    {
        tok = save->next;
        if(!tok)
            return NULL;
    }
    if(tok->elem->type == FI)
        return tok;
    return NULL;
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
    struct nL *save = tok;
    tok = tok->next;
    while (!tok || tok->elem->type == ENDOF)
    {
        if (!tok)
        {
            save->next = handletty();
            tok = save;
        }
        save = tok;
        tok = tok->next;
    }

    if (tok->elem->type != IN)
        return NULL;

    save = tok;
    tok = tok->next;
    while (!tok || tok->elem->type == ENDOF)
    {
        if (!tok)
        {
            save->next = handletty();
            tok =save;
        }
        save =tok;
        tok = tok->next;
    }

    struct nL *new = g_caseclause(tok);
    tok = (new ? new : tok);

    tok = tok->next;
    if(!tok)
        return NULL;

    return tok->elem->type == ESAC ? tok : NULL;
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
    if(tok->elem->type != FOR)
        return NULL;
    tok = tok->next;
    if(!tok)
        return NULL;
    if((tok->elem->type != WORD)||(is_conform(tok)!= 0))
        return NULL;
    tok = tok->next;
    if(!tok)
        return NULL;

    if(tok->elem->type == SEMICOLON)
    {
        tok = tok->next;
        if(!tok)
            return NULL;
    }
    else
    {
        struct nL *save = tok;
        while(tok->elem->type == ENDOF)
        {
            tok = tok->next;
            if(!tok)
                return NULL;
        }
        if(tok->elem->type == IN)
        {
            tok = tok->next;
            if(!tok)
                return NULL;
            while((tok->elem->type == WORD)&&(is_conform(tok) == 0))
            {
                tok = tok->next;
                if(!tok)
                    return NULL;
            }
            if((tok->elem->type == SEMICOLON)||(tok->elem->type == ENDOF))
            {
                tok = tok->next;
                if (!tok)
                    return NULL;
            }
            else
            {
                tok = save;
            }
        }
        else
        {
            tok = save;
        }
    }

    while(tok->elem->type == ENDOF)
    {
        tok = tok->next;
        if(!tok)
            return NULL;
    }
    return g_dogroup(tok);
}


