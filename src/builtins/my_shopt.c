#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "options.h"
#include "builtins.h"


static int print_everything(struct option *opt)
{
    fprintf(stdout, "ast-print         %s\n", opt->ast_print == TRUE ? "on" : "off");
    fprintf(stdout, "dotglob           %s\n", opt->dotglob == TRUE ? "on" : "off");
    fprintf(stdout, "expand_aliases    %s\n", opt->expand_aliases == TRUE ? "on" : "off");
    fprintf(stdout, "extglob           %s\n", opt->extglob == TRUE ? "on" : "off");
    fprintf(stdout, "nocaseglob        %s\n", opt->nocaseglob == TRUE ? "on" : "off");
    fprintf(stdout, "nullglob          %s\n", opt->nullglob == TRUE ? "on" : "off");
    fprintf(stdout, "sourcepath        %s\n", opt->sourcepath == TRUE ? "on" : "off");
    fprintf(stdout, "xpg_echo          %s\n", opt->xpg_echo == TRUE ? "on" : "off");
    return 0;
}

static int set_opt(char *opt, int *q, int *s, int *u)
{
    if (opt[0] != '-')
        return 0;

    int retcode = 2;

    for (size_t i = 1; i < strlen(opt); i ++)
    {
        if (opt[i] == 's')
        {
            *s = 1;
            retcode = 0;
        }

        else if (opt[i] == 'q')
        {
            *q = 1;
            retcode = 0;
        }

        else if (opt[i] == 'u')
        {
            *u = 1;
            retcode = 0;
        }

        else
        {
            fprintf(stderr, "shopt: -%c: invalid option\n", opt[i]);
            fprintf(stderr, "shopt: usage: [-qsu] [optname...]\n");
            return 1;
        }
    }

    return retcode;
}

static int option_switch(struct option *ops, char *opt, int opcode)
{
    if (!strcmp("ast-print", opt))
    {
        if (opcode == 1)
        {
            ops->ast_print = TRUE;
        }
        else if (opcode == 2)
        {
            ops->ast_print = FALS;
        }
        else if (opcode == 4)
        {
            fprintf(stdout, "ast-print    %s", ops->ast_print == TRUE ? "on" : "off");
        }
        else
        {
            return ops->ast_print == TRUE ? 1 : 0;
        }
    }

    if (!strcmp("dotglob", opt))
    {
        if (opcode == 1)
        {
            ops->dotglob = TRUE;
        }
        else if (opcode == 2)
        {
            ops->dotglob = FALS;
        }
        else if (opcode == 4)
        {
            fprintf(stdout, "dotglob    %s", ops->dotglob == TRUE ? "on" : "off");
        }

        else
        {
            return ops->dotglob == TRUE ? 1 : 0;
        }
    }

    if (!strcmp("expand_aliases", opt))
    {
        if (opcode == 1)
        {
            ops->expand_aliases = TRUE;
        }
        else if (opcode == 2)
        {
            ops->expand_aliases = FALS;
        }
        else if (opcode == 4)
        {
            fprintf(stdout, "expand_aliases    %s", ops->expand_aliases == TRUE ? "on" : "off");
        }

        else
        {
            return ops->expand_aliases == TRUE ? 1 : 0;
        }
    }

    if (!strcmp("extglob", opt))
    {
        if (opcode == 1)
        {
            ops->extglob = TRUE;
        }
        else if (opcode == 2)
        {
            ops->extglob = FALS;
        }
        else if (opcode == 4)
        {
            fprintf(stdout, "extglob    %s", ops->extglob == TRUE ? "on" : "off");
        }
        else
        {
            return ops->extglob == TRUE ? 1 : 0;
        }
    }

    if (!strcmp("nocaseglob", opt))
    {
        if (opcode == 1)
        {
            ops->nocaseglob = TRUE;
        }
        else if (opcode == 2)
        {
            ops->nocaseglob = FALS;
        }
        else if (opcode == 4)
        {
            fprintf(stdout, "nocaseglob    %s", ops->nocaseglob == TRUE ? "on" : "off");
        }
        else
        {
            return ops->nocaseglob == TRUE ? 1 : 0;
        }
    }

    if (!strcmp("nullglob", opt))
    {
        if (opcode == 1)
        {
            ops->nullglob = TRUE;
        }
        else if (opcode == 2)
        {
            ops->nullglob = FALS;
        }
        else if (opcode == 4)
        {
            fprintf(stdout, "nullglob    %s", ops->nullglob == TRUE ? "on" : "off");
        }
        else
        {
            return ops->nullglob == TRUE ? 1 : 0;
        }
    }

    if (!strcmp("sourcepath", opt))
    {
        if (opcode == 1)
        {
            ops->sourcepath = TRUE;
        }
        else if (opcode == 2)
        {
            ops->sourcepath = FALS;
        }
        else if (opcode == 4)
        {
            fprintf(stdout, "sourcepath    %s", ops->sourcepath == TRUE ? "on" : "off");
        }
        else
        {
            return ops->sourcepath == TRUE ? 1 : 0;
        }
    }

    if (!strcmp("xpg_echo", opt))
    {
        if (opcode == 1)
        {
            ops->xpg_echo = TRUE;
        }
        else if (opcode == 2)
        {
            ops->xpg_echo = FALS;
        }
        else if (opcode == 4)
        {
            fprintf(stdout, "xpg_echo    %s", ops->xpg_echo == TRUE ? "on" : "off");
        }
        else
        {
            return ops->xpg_echo == TRUE ? 1 : 0;
        }
    }
    return 0;
}

static int handle_options(struct option *ops, char *opt, int q, int s,  int u)
{

    int retu = 0;

    if (!q && !s && !u)
    {
        option_switch (ops, opt, 4);
    }

    if (s)
    {
        option_switch(ops, opt, 1);
    }

    if (u)
    {
        option_switch(ops, opt, 2);
    }

    if (q)
    {
        retu = option_switch(ops, opt, 3);
    }

    return retu;

}

static int is_opt(char *optn)
{
    if (!strcmp("dotglob", optn))
        return 1;

    if (!strcmp("expand_aliases", optn))
        return 1;

    if (!strcmp("extglob", optn))
        return 1;

    if (!strcmp("nocaseglob", optn))
        return 1;

    if (!strcmp("nullglob", optn))
        return 1;

    if (!strcmp("sourcepath", optn))
        return 1;

    if (!strcmp("xpg_echo", optn))
        return 1;

    return 0;

}

int my_shopt(int number, char *args[], ...)
{
    number = number;

    va_list ap;
    va_start(ap, args);
    struct option *opt = global.options;

    int s = 0;
    int q = 0;
    int u = 0;

    if (!args[1])
    {
        va_end(ap);
        return print_everything(opt);
    }

    int i = 2;
    int retcode = 0;
    int qres = 0;

    while (args[i])
    {
        if (s && u)
        {
            fprintf(stderr, "shopt: cannot set and unset an option at the same time.\n");
            va_end(ap);
            return 1;
        }


        int vuO = set_opt(args[i], &q, &s, &u);

        if (vuO == 0)
            continue;
        else if (vuO == 1)
        {
            va_end(ap);
            return 1;
        }
        else
        {
            if (!is_opt(args[i]))
            {
                fprintf(stderr, "shopt:  %s: invalid option name\n", args[i]);
                retcode = 1;
            }
            else
            {
                int temp = handle_options(opt, args[i],q,s,u);
                if ( qres == 0 )
                    qres = temp;
            }
        }
        i++;
    }

    if (q)
        retcode = qres;

    va_end(ap);
    return retcode;
}
