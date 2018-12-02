#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "options.h"
#include "builtins.h"

//FIXME: to implement.
static void print_alphabet(struct assignment **alias_tb)
{
    alias_tb = alias_tb;
    return;
}

//FIXME: to implement.
static int print_alias(char *args[i])
{
    return 0;
}

static int contain_equal(char *string)
{
    size_t length = strlen(string);
    for (int i = 0; i < length; i++)
    {
        if (string[i] == '=')
        {
            return 1;
        }
    }
    return 0;
}

int my_alias(int number, char *args[], ...)
{
    if (!args[1])
    {
        print_alphabet(global.alias_tab);
        return 1;
    }

    int found = 0;
    int i = 1;
    while (args[i])
    {
        if (!strcmp("-p", args[i]) && !found)
        {
            found = 1;
            print_alphabet(global.alias_tab);
        }
        else if (!contain_equal(args[i]))
        {
            return print_alias(args[i]);
        }

        else
        {
            //FIXME: every char authorized ? => yes it seems.
            //FIXME: need to handle expansions though. (to see with Neganta and Leo)
            add_assignment(args[i]);
        }

        i++;
    }

}
