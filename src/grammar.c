#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include "linked_list.h"
#include "token.h"

static int g_command(struct vector *tokens, ssize_t index)
{
    tokens = tokens;
    return index;
}

static int g_pipeline(struct vector *tokens, ssize_t index)
{
    struct token *tok = tokens->arr[index];
    if (!strcmp("!", tok->name))
    {
        index++;
    }

    if (index > tokens->size)
    {
        return -1;
    }
    tok = tokens->arr[index];

    if ((index = g_command(tokens, index)) == -1)
    {
        return -1;
    }

    while (index < tokens->size)
    {
        index++;
        if (index >= tokens->size)
            return -1;

        tok = tokens->arr[index];
        if (tok->type == PIPE)
        {
            index++;
            if (index >= tokens->size)
                return -1;
            tok = tokens->arr[index];
            while (tok->type == ENDOF)
            {
                index++;
                if(index >= tokens->size)
                    return -1;
                tok = tokens->arr[index];
            }
            index = g_command(tokens, index);
            if (index == -1)
                return -1;
        }
        else
        {
            index--;
            break;
        }
    }
    return index;
}

static int g_andor(struct vector *tokens, ssize_t index)
{
    if ((index = g_pipeline(tokens, index)) == -1)
    {
        return -1;
    }

    while (index < tokens->size)
    {
        index++;
        if (index >= tokens->size)
            return -1;

        struct token *tok = tokens->arr[index];
        if (tok->type == LOGICAL_AND || tok->type == LOGICAL_OR)
        {
            index++;
            if (index >= tokens->size)
                return -1;
            tok = tokens->arr[index];
            while (tok->type == ENDOF)
            {
                index++;
                if (index >= tokens->size)
                    return -1;
                tok = tokens->arr[index];
            }
            index = g_pipeline(tokens, index);
            if (index == -1)
                return -1;
        }
        else
        {
            index--;
            break;
        }
    }

    return index;
}


static int g_list(struct vector *tokens, ssize_t index)
{
    index = g_andor(tokens, index);
    if(index == -1)
    {
        return -1;
    }
    while (1)
    {
        index++;
        if (index >= tokens->size)
        {
            return -1;
        }
        struct token *tok = tokens->arr[index];
        if (tok->type == ENDOF)
        {
            return index;
        }
        else if ((tok->type == SEMICOLON)||
           (tok->type == AND))
        {
            index++;
            if (index >= tokens->size)
            {
                return -1;
            }

            tok = tokens->arr[index];
            if (tok->type == ENDOF)
            {
                return index;
            }

            if ((index = g_andor(tokens, index)) == -1)
            {
                return -1;
            }
        }
        else
        {
            return -1;
        }

    }
}

int grammar_check (struct vector *tokens)
{
    ssize_t ind = 0;

    struct token *tok = tokens->arr[ind];
    if (tok->type == ENDOF)
        return 1;

    ind = g_list(tokens, ind);

    if (ind == -1)
        return -1;
    tok = tokens->arr[ind];
    if (tok->type == ENDOF)
        return 1;

    return 0;
}


