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
            fprintf(stderr, "exit: too many arguments.");
            va_end(ap);
            return 1;
        }

        if (!isdig(args[i]))
        {
            fprintf(stderr, "exit: %s is a non-numeric arg.", args[i]);
            struct node *fre = va_arg(ap, struct node *);
            //FIXME ajouter les extractions d'args à free
            free_node(fre);
            va_end(ap);
            exit(2);
        }

        i++;
        found = 1;
        retcode = atoi(args[i]);
    }

    struct node *fre = va_arg(ap, struct node *);
    //FIXME ajouter les extractions d'args à free
    free_node(fre);
    va_end(ap);
    exit(retcode);
    return retcode;
}
