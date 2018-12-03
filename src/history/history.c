#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include "history.h"
#include <unistd.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>

void init_interact(void)
{
    char *home = getenv("HOME");
    if (!home)
        return;
    char *path = calloc(strlen(home) + 15, sizeof(char));
    strcpy(path, home);
    path = strcat(path, "/.42sh_history");
    FILE *history = fopen(path, "r+");
    free(path);
    char *line;
    size_t i = 0;
    ssize_t read = 0;

    if (!history)
        return;

    while ((read = getline(&line, &i, history)) != -1)
    {
        line[strlen(line) - 1] = '\0';
        add_history(line);
        free(line);
        line = NULL;
    }

    fclose(history);
    free(line);
    return;
}
