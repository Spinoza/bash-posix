#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
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

//FIXME: temporary place for this function

struct builtins *init_builts(void)
{
    struct builtins *tab = malloc( 11 * sizeof(struct builtins));
    tab[0].name = "exit";
    tab[0].builtin = my_exit;
    tab[1].name = "shopt";
    tab[1].builtin = my_shopt;
    tab[2].name = "cd";
    tab[2].builtin = my_cd;
    tab[3].name = "alias";
    tab[3].builtin = my_alias;

    return tab;
}

int my_exit(int number, char *args[],  ...)
{
    number = number;
    va_list ap;
    va_start(ap, args);

    int found = 0;
    int retcode = va_arg(ap, int);
    int i = 1;

    while (args[i] && strcmp(args[i], ";"))
    {
        if (found)
        {
            fprintf(stderr, "42sh: exit: too many arguments.\n");
            va_end(ap);
            return 1;
        }

        if (!isdig(args[i]))
        {
            fprintf(stderr, "42sh: exit: %s is a non-numeric arg.\n", args[i]);
            va_end(ap);
            exit(2);
        }

        retcode = atoi(args[i]);
        found = 1;
        i++;
    }

    va_end(ap);
    exit(retcode);
    return retcode;
}
