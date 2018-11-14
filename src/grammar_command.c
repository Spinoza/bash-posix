#include "linked_list.h"
#include "grammar_check.h"

static struct nL *g_redirection(struct nL *tok)
{
/*
    struct nL *new = tok;
    if(new->elem->type == IONUMBER)
        new = new->next;
*/
    return tok;
}


static struct nL *g_element(struct nL *tok)
{
    return tok;
}


static struct nL *g_prefix(struct nL *tok)
{
    return tok;
}

struct nL *g_funcdec(struct nL *tok)
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

struct nL *g_simplecommand(struct nL *tok)
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
struct nL *g_compoundlist(struct nL *tok)
{
    return tok;
}

struct nL *g_shellcommand(struct nL *tok)
{
    struct nL *new = tok;
    if(!strcmp(new->elem->name, "{"))
    {
        new =new->next;
        if(!new)
            return NULL;
        new = g_compoundlist(new);
        if(new)
        {
            new = new->next;
            if(!new)
                return NULL;
            if(!strcmp("}", new->elem->name))
                return new;
        }
    }

    new = tok;
    if(!strcmp(new->elem->name, "("))
    {
        new =new->next;
        if(!new)
            return NULL;
        new = g_compoundlist(new);
        if(new)
        {
            new = new->next;
            if(!new)
                return NULL;
            if(!strcmp(")", new->elem->name))
                return new;
        }
    }

    new = g_rulefor(tok);
    if(new)
        return new;

    new = g_rulewhile(tok);
    if(new)
        return new;

    new = g_ruleuntil(tok);
    if(new)
        return new;

    new = g_rulecase(tok);
    if(new)
        return new;

    return g_ruleif(tok);

}

struct nL *g_command(struct nL *tok)
{
    struct nL *new = g_simplecommand(tok);
    if(new)
        return new;
    new = g_shellcommand(tok);
    if(new)
    {
        while(1)
        {
            tok = new;
            new = g_redirection(new->next);
            if(!new)
                return tok;
        }
    }
    new = g_funcdec(tok);
    if(!new)
        return NULL;
    while(1)
    {
        tok = new;
        new = g_redirection(new->next);
        if(!new)
            return tok;
    }
}

