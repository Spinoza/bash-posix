#include <limits.h>
#include <stdlib.h>
#include "history.h"
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>

static size_t mstrlen(char *string)
{
    return *string ? 1 + mstrlen(string + 1) : 0;
}

struct passwd *get_path(void)
{
    uid_t uid = getuid();
    struct passwd *passw = getpwuid(uid);
    return passw;
}

void init_interact(void)
{
    struct passwd *user = get_path();
    char *path = calloc(mstrlen(user->pw_dir) + 14, sizeof(char));
    path = strcat(path, ".42sh_history");
    FILE *history = fopen(path, "r+");
    free(path);
    char *line;
    size_t i = 0;
    ssize_t read = 0;

    if (!history)
        return;

    while ((read = getline(&line, &i, history)) != -1)
    {
        line[mstrlen(line) - 1] = '\0';
        add_history(line);
        free(line);
        line = NULL;
    }

    fclose(history);
    free(line);
    return;
}
