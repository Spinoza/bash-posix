#include <stdlib.h>
#include "history.h"
#include <readline/readline.h>
#include <readline/history.h>

static size_t mstrlen(char *string)
{
    return *string ? 1 + mstrlen(string + 1) : 0;
}

void init_interact(FILE *history)
{
    if (!history)
        return;

    char *line;
    size_t i = 0;
    ssize_t read = 0;

    while ((read = getline(&line, &i, history)) != -1)
    {
        line[mstrlen(line) - 1] = '\0';
        add_history(line);
        free(line);
        line = NULL;
    }

    free(line);
    return;
}
