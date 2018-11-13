#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "includes/queue.h"
#include "includes/token.h"
#include <string.h>

static struct queue *g_command(struct queue *tokens, struct queue *grammar)
{
    return enqueue(grammar, dequeue(tokens));
}

static struct queue *g_pipeline(struct queue *tokens, struct queue *grammar)
{
    struct token *tok = grammar->head->elem;
    if (!strcmp("!", tok->name))
    {
        grammar = enqueue(grammar, dequeue(tokens));
    }

    if (!g_command(tokens, grammar))
    {
        return NULL;
    }

    while (tokens->size > 0 && grammar)
    {
        struct token *tok = tokens->head->elem;
        if (tok->type == PIPE)
        {
            grammar = enqueue(grammar, dequeue(tokens));
            struct token *tok = tokens->head->elem;
            while (!strcmp(tok->name, '\n'))
            {
                grammar = enqueue(grammar, dequeue(tokens));
                if(!tokens->head)
                    return NULL;
                tok = tokens->head->elem;
            }
            grammar = g_command(tokens, grammar);
            if (!grammar)
                return NULL;
        }
        else
        {
            break;
        }
    }
    return grammar;
}

static struct queue *g_andor(struct queue *tokens, struct queue *grammar)
{
    if (!g_pipeline(tokens, grammar))
    {
        return NULL;
    }

    while (tokens->size > 0 && grammar)
    {
        struct token *tok = tokens->head->elem;
        if (tok->type == LOGICAL_AND || tok->type == LOGICAL_OR)
        {
            grammar = enqueue(grammar, dequeue(tokens));
            struct token *tok = tokens->head->elem;
            while (!strcmp(tok->name, '\n'))
            {
                grammar = enqueue(grammar, dequeue(tokens));
                if (!tokens->head)
                    return NULL;
                tok = tokens->head->elem;
            }
            grammar = g_pipeline(tokens, grammar);
            if (!grammar)
                return NULL;
        }
        else
        {
            break;
        }
    }

    return grammar;
}


static struct queue *g_list(struct queue *tokens, struct queue *grammar)
{
    if(g_andor(tokens, grammar) == NULL)
    {
        return NULL;
    }
    while(1)
    {
        if(tokens->head == NULL)
        {
            return NULL;
        }
        else if(tokens->head->type == ENDOF)
        {
            return grammar;
        }
        else if((tokens->head->elem->type == SEMICOLON)||
           (tokens->head->elem->type == AND))
        {
             grammar=enqueue(grammar, dequeue(tokens));
             if(tokens->head == NULL)
             {
                 return NULL;
             }
             else if(tokens->head->type == ENDOF)
             {
                 return grammar;
             }
             else if(g_andor(tokens, grammar))
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

struct queue *grammar_check (struct queue *tokens)
{
    struct queue *grammar = init_queue();
    int isEOF = 0;

    while(tokens->size > 0)
    {
        struct token *tok = tokens->head->elem;
        if(tok->type == ENDOF)
        {
            grammar = enqueue(grammar, dequeue(tokens);
            break;
        }

        else
        {
            grammar = list(tokens, grammar);
        }
    }

    free_queue(tokens);
    return grammar;
}


