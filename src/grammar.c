#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include "includes/queue.h"
#include "includes/token.h"


struct queue *grammar_check (struct queue *tokens)
{
    struct queue *grammar = init_queue();
    int isEOF = 0;

    while(tokens->size > 0)
    {
        struct token *tok = tokens->head->elem;
        if(tok->type == ENDOF)
        {
            grammar = push(grammar, pop(tokens);
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

struct queue *g_andor(struct queue *tokens, struct queue *grammar)
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
            grammar = push(grammar, pop(tokens));
            struct token *tok = tokens->head->elem;
            while (!strcmp(tok->name, '\n'))
            {
                grammar = push(grammar, pop(tokens));
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

struct queue *g_pipeline(struct queue *tokens, struct queue *grammar)
{
    struct token *tok = grammar->head->elem;
    if (!strcmp("!", tok->name))
    {
        grammar = push(grammar, pop(tokens));
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
            grammar = push(grammar, pop(tokens));
            struct token *tok = tokens->head->elem;
            while (!strcmp(tok->name, '\n'))
            {
                free(pop(tokens));
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
