#include "handlePS1.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "lexer.h"
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "grammar_check.h"

struct nL *handletty(void)
{
    while (isatty(STDIN_FILENO))
    {
        char *line;
        char *PS2 = get_assignment("PS2");
        if (!PS2)
            line = readline("> ");
        else
            line = readline(PS2);
        struct linked_list *tokens = lexer_c(line);
        struct nL *ret = tokens->head;
        free(tokens);
        free(line);
        return ret;
    }

    return NULL;
}
