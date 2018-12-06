#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "builtins.h"
#include "hardbs.h"

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

static int handle_hardbs(char *arg, char *printfin, int index, size_t *i)
{
    if (arg[*i+1] && arg[*i] == '\\')
    {
        switch (arg[*i+1])
        {
            case 'x':
                (*i)++;
                return back_x(arg, printfin, index, i);
                break;
            case 'u':
                (*i)++;
                return back_u(arg, printfin, index, i);
                break;
            case 'U':
                (*i)++;
                return back_U(arg, printfin, index, i);
                break;
            default:
            {
                for (; *i < strlen(arg) && !isforbidden(arg[*i]); (*i)++)
                {
                    printfin[index] = arg[*i];
                    index++;
                }
                return index;
            }
            break;
        }
    }

    for (; *i < strlen(arg) && !isforbidden(arg[*i]); (*i)++)
    {
        printfin[index] = arg[*i];
        index++;
    }
    return index;
}

static int match_bs(char *arg, char *printfin, int index)
{

    size_t i = 0;
    while (i < strlen(arg))
    {
        if (arg[i] == '\a')
        {
            printfin[index] = 7;
            index++;
            i++;
        }

        else if (arg[i] == '\b')
        {
            printfin[index] = 8;
            index++;
            i++;
        }

        else if (arg[i+1] && arg[i] == '\\'&& arg[i+1] == 'c')
        {
            return -10;
        }

        else if (arg[i] == '\f')
        {
            printfin[index] = 12;
            index++;
            i++;
        }

        else if (arg[i] == '\n')
        {
            printfin[index] = 10;
            index++;
            i++;
        }

        else if (arg[i] == '\r')
        {
            printfin[index] = 13;
            index++;
            i++;
        }

        else if (arg[i] == '\t')
        {
            printfin[index] = 9;
            index++;
            i++;
        }

        else if (arg[i] == '\v')
        {
            printfin[index] = 11;
            index ++;
            i++;
        }

        else if (arg[i] == '\0')
        {
            index = back_zero(arg, printfin, index, &i);
        }

        else
        {
            index = handle_hardbs(arg, printfin, index, &i);
        }
    }
    return index;
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
    number = number;

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
        {
            i++;
            continue;
        }

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
        fprintf(stdout, "%s", printfin ? printfin : "");
    }

    else
    {
        fprintf(stdout, "%s\n", printfin ? printfin : "");
    }
    if (printfin)
    {
        free(printfin);
    }
    return 0;
}
