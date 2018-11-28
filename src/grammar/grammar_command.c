#include <string.h>
#include "linked_list.h"
#include "grammar_check.h"
#include "handlePS1.h"

static int is_conform(struct nL *tok)
{
    char *args[9] = { ">", "<", ">>", ">&", "<&", ">|", "<>", "!",
                     "function"};

    for (int i = 0; i < 7; i++)
    {
        if (strcmp(args[i], tok->elem->name) == 0)
            return 1;
    }

    for (int j = 7; j < 9; j++)
    {
        if (strcmp(args[j], tok->elem->name) == 0)
            return 2;
    }

    return 0;
}

static struct nL *g_redirection(struct nL *tok)
{
    if (tok->elem->type == IONUMBER)
        tok = tok->next ? tok->next : NULL;

    if (tok && tok->elem->type == REDIRECTION && is_conform(tok) == 1)
    {
        tok = tok->next;
        if (!tok)
            return NULL;
        if (tok->elem->type == WORD)
            return tok;
        return NULL;
    }

    if (tok && !strcmp(tok->elem->name, "<<-"))
    {
        tok = tok->next;
        if (!tok)
            return NULL;
        if (tok->elem->type == HEREDOC)
            return tok;
        return NULL;
    }

    if (tok && !strcmp(tok->elem->name, "<<"))
    {
        tok = tok->next;
        if (!tok)
            return NULL;
        if (tok->elem->type == HEREDOC)
            return tok;
        return NULL;
    }

    return NULL;
}


static struct nL *g_element(struct nL *tok)
{
    int conf = is_conform(tok);
    if (conf == 1 || conf == 2)
        return NULL;

    if (tok->elem->type == WORD)
        return tok;

    return g_redirection(tok);
}


static struct nL *g_prefix(struct nL *tok)
{
    if (tok->elem->type == ASSIGNMENT_W)
        return tok;

    return g_redirection(tok);
}

struct nL *g_funcdec(struct nL *tok)
{
    if (tok->elem->type != WORD)
        return NULL;

    if (!strcmp(tok->elem->name, "function"))
    {
        tok = tok->next;
        if (!tok)
            return NULL;
    }

    if (tok->elem->type != WORD)
        return NULL;

    tok = tok->next;
    if (!tok)
        return NULL;
    if (tok->elem->type != OPEN_PAR)
        return NULL;
    tok = tok->next;
    if (!tok)
        return NULL;
    if (tok->elem->type != CLOSE_PAR)
        return NULL;
    tok = tok->next;
    if (!tok)
        return NULL;

    struct nL *save = tok;
    while (!tok || tok->elem->type == ENDOF)
    {
        if(!tok)
        {
            save->prev->next = handletty();
            save->prev->next->prev = save->prev;
            tok = save->prev;
            free(save->elem->name);
            free(save->elem);
            free(save);
        }
        save = tok;
        tok = tok->next;
    }

    tok = g_shellcommand(tok);
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
    struct nL *save = tok;
    while (!tok || tok->elem->type == ENDOF)
    {
        if(!tok)
        {
            save->prev->next = handletty();
            save->prev->next->prev = save->prev;
            tok = save->prev;
            free(save->elem->name);
            free(save->elem);
            free(save);
        }
        save = tok;
        tok = tok->next;
    }

    tok = g_andor(tok);
    if (!tok)
        return NULL;

    save = tok;
    tok = tok->next;
    if (!tok->next)
        return NULL;

    enum type t = tok->elem->type;
    while (t == SEMICOLON || t == AND || t == ENDOF)
    {
        struct nL *stock = tok;
        tok = tok->next;
        while (!tok || tok->elem->type == ENDOF)
        {
            if(!tok)
            {
                stock->prev->next = handletty();
                stock->prev->next->prev = stock->prev;
                tok = stock->prev;
                free(stock->elem->name);
                free(stock->elem);
                free(stock);
            }
            stock = tok;
            tok = tok->next;
        }

        tok = g_andor(tok);
        if (!tok)
        {
            tok = save;
            break;
        }
        save = tok;
        if (!tok->next)
            return NULL;
        t = tok->next->elem->type;
        tok = tok->next;
    }
    tok = save;
    tok = tok->next;
    if (!tok)
        return NULL;

    t = tok->elem->type;
    if (t == SEMICOLON || t == AND || t == ENDOF)
    {
        save = tok;
        tok = tok->next;
        while (!tok || tok->elem->type == ENDOF)
        {
            if(!tok)
            {
                save->prev->next = handletty();
                save->prev->next->prev = save->prev;
                tok = save->prev;
                free(save->elem->name);
                free(save->elem);
                free(save);
            }
            save = tok;
            tok = tok->next;
        }
    }

    return save;
}

struct nL *g_shellcommand(struct nL *tok)
{
    struct nL *new = tok;
    if(tok->elem->type == OPEN_BRA)
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
            if(new->elem->type == CLOSE_BRA)
                return new;
        }
    }

    new = tok;
    if(tok->elem->type == OPEN_PAR)
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
            if(new->elem->type == CLOSE_PAR)
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
    struct nL *new = g_funcdec(tok);
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
    new = g_simplecommand(tok);
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
    return NULL;
}

