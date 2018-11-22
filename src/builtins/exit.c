#include <err.h>
#include <stdlib.h>
#include <string.h>
#include "builtins.h"

static size_t mstrlen(char *string)
{
    return *string ? 1 + mstrlen(string + 1) : 0;
}

static int isdig(char *string)
{
    for (size_t i = 0; i < mstrlen(string); i++)
    {
        if (string[i] < 48 || string[i] > 57)
            return 0;
    }

    return 1;
}

static void freespace(char *cpy, char *tocomp)
{
    free(cpy);
    free(tocomp);
}

int handle_exit(char *tocomp, int toret)
{
    size_t length = mstrlen(tocomp);
    char *cpy = calloc(length + 1, sizeof(char));
    strcpy(cpy, tocomp);
    char *stock = NULL

    char *string = strtok_r(cpy, " \n", &stock);
    if (!strcmp(string, "exit;"))
    {
        freespace(cpy, tocomp);
        exit(toret);
    }

    if (!strcmp(string, "exit"))
    {
        string = strtok(NULL, " \n", &stock);
        if (!string)
        {
            freespace(cpy, tocomp);
            exit(toret);
        }

        if (!isdig(string))
        {
            fprintf(stderr, "exit: numeric argument required.\n");
            freespace(cpy, tocomp);
            exit(toret);
        }

        if (stock && strcmp(stock, ""))
        {
            fprintf(stderr, "exit: too many arguments.");
            freespace(cpy, tocomp);
            return -1;
        }

        freespace(cpy, tocomp);
        exit(atoi(string));
    }

    return -2;
}
