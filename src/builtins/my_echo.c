#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "builtins.h"

static int is_echo_opt(char *arg, int *e, int *E, int *n)
{
    if (!strcmp("-e", arg))
    {
        *e = 1;
        return 1;
    }

    if (!strcmp("-n", arg))
    {
        *n = 1;
        return 1;
    }

    if (!strcmp("-E", arg))
    {
        *E = 1;
        return 1;
    }

    return 0;
}

static int match_bs(char *arg, char *printfin, int index)
{
    if (strlen(arg) == 1)
    {
        printfin[index] = 92;
        return index + 1;
    }

    if (!strcmp(arg, "\a"))
    {
        printfin[index] = 7;
        return index + 1;
    }

    if (!strcmp(arg, "\b"))
    {
        printfin[index] = 8;
        return index + 1;
    }

    if (!strcmp(arg, "\c"))
    {
        return -10;
    }


}

static size_t fullength(char *args[], int i)
{
    size_t length = 0;
    while (args[i])
    {
        length += strlen(args[i]);
        i++;
        if (args[i])
        {
            length += 1;
        }
    }

    return length;
}

int my_echo(int number, char *args[], ...)
{

    if (!args[1])
    {
        fprintf(stdout, "\n");
        return 0;
    }

    int e = 0;
    int E = 0;
    int n = 0;

    int i = 1;
    int found = 0;
    char *printfin = NULL;
    int index = -1;

    while (args[i])
    {
        if (is_echo_opt(args[i], &e, &E, &n) && !found)
            continue;

        else
        {
            found = 1;
            if (!printfin)
            {
                printfin = calloc(fullength(args, i) + 1, sizeof(char));
                index = 0;
            }
            if (e || E)
            {
                if (args[i][0] == '\\')
                {
                    index = match_bs(args[i], printfin, index);
                    if (index < 0)
                    {
                        n = 1;
                        break;
                    }
                }
                else
                {
                    strcat(printfin, args[i]);
                    index += strlen(args[i]);
                }
            }
            else
            {
                strcat(printfin, args[i]);
                index += strlen(args[i]);
            }
            i++;
            if (args[i])
            {
                printfin[index] = ' ';
                index++;
            }
        }
    }

    if (n)
    {
        fprintf(stdout, "%s", printfin);
    }

    else
    {
        frprintf(stdout, "%s\n", printfin);
    }
    return 0;
}
