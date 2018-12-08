#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
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

static int handle_hardbs(char *arg, wchar_t *printfin, int index, int *cursor)
{
    switch (arg[*cursor])
    {
        case 'x':
            (*cursor) += 1;
            return back_x(arg, printfin, index, cursor);
            break;
        case 'u':
            (*cursor) += 1;
            return back_u(arg, printfin, index, cursor);
            break;
        case 'U':
            (*cursor) += 1;
            return back_U(arg, printfin, index, cursor);
            break;
        default:
        {
            printfin[index] = arg[*(cursor)-1];
            index++;
            int j = strlen(arg);
            for (; *cursor < j && !isforbidden(arg[*cursor]); (*cursor)++)
            {
                printfin[index] = arg[*cursor];
                index++;
            }
            return index;
        }
    }
}

static int match_bs(char *arg, wchar_t *printfin, int index, int *cursor)
{

     if (arg[*cursor] == 'a')
     {
         printfin[index] = 7;
         *cursor += 1;
         index++;
     }

     else if (arg[*cursor] == 'b')
     {
         printfin[index] = 8;
         *cursor += 1;
         index++;
     }

     else if (arg[*cursor] == 'c')
     {
        return -10;
     }

     else if (arg[*cursor] == 'f')
     {
        printfin[index] = 12;
        index++;
        *cursor += 1;
     }

     else if (arg[*cursor] == 'n')
     {
        printfin[index] = 10;
        index++;
        *cursor += 1;
     }

    else if (arg[*cursor] == 'r')
    {
        printfin[index] = 13;
        index++;
        *cursor += 1;
    }

    else if (arg[*cursor] == 'e' || arg[*cursor] == 'E')
    {
        if (arg[*(cursor) + 1] == '\\')
            *cursor += 3;
        else
            *cursor += 2;
    }

    else if (arg[*cursor] == 't')
    {
        printfin[index] = 9;
        index++;
        *cursor += 1;
    }

    else if (arg[*cursor] == 'v')
    {
        printfin[index] = 11;
        index ++;
        *cursor += 1;
    }

    else if (arg[*cursor] == '0')
    {
        *cursor += 1;
        index = back_zero(arg, printfin, index, cursor);
    }

    else
    {
        index = handle_hardbs(arg, printfin, index, cursor);
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

int my_echo(char *args[], ...)
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
    wchar_t *printfin = NULL;
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
                printfin = calloc(fullength(args, i) + 1, sizeof(wchar_t));
                index = 0;
            }
            int tomatch = strlen(args[i]);
            int cursor = 0;
            while (cursor < tomatch)
            {
                if (args[i][cursor] == '\\' && (e || E) && args[i][cursor+1])
                {
                    cursor++;
                    index = match_bs(args[i], printfin, index, &cursor);
                    if (index < 0)
                    {
                        n = 1;
                        break;
                    }
                }
                else
                {
                    printfin[index] = args[i][cursor];
                    index++;
                    cursor++;
                }
            }
            if (index < 0)
            {
                break;
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
        if (printfin)
            printf("%ls", printfin);
        else
            printf("%s", "");
    }

    else
    {
        if (printfin)
            printf("%ls\n", printfin);
        else
            printf("%s", "");
    }
    fflush(stdout);
    if (printfin)
    {
        free(printfin);
    }
    return 0;
}
