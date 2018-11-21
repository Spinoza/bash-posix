#include "handlePS1.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "lexer.h"
#include "grammar_check.h"

static size_t mstrlen(char *str)
{
    return *str ? 1 + mstrlen(str + 1) : 0;
}


struct nL *handletty(void)
{
    while (isatty(STDIN_FILENO))
    {
        char *line;
        size_t n = 0;
        printf("> ");
        getline(&line, &n, stdin);
        size_t i = mstrlen(line);
        line[i - 1] = '\0';
        struct linked_list *tokens = lexer_c(line);
        struct nL *ret = tokens->head;
        free(tokens);
        return ret;
    }
}
