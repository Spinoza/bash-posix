#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "options.h"
#include "builtins.h"


static int print_everything(struct option *opt)
{
    fprintf(stdout, "ast-print      \t%s\n",
    opt->ast_print == TRUE ? "on" : "off");
    fprintf(stdout, "dotglob        \t%s\n",
    opt->dotglob == TRUE ? "on" : "off");
    fprintf(stdout, "expand_aliases \t%s\n",
    opt->expand_aliases == TRUE ? "on" : "off");
    fprintf(stdout, "extglob        \t%s\n",
    opt->extglob == TRUE ? "on" : "off");
    fprintf(stdout, "nocaseglob     \t%s\n",
    opt->nocaseglob == TRUE ? "on" : "off");
    fprintf(stdout, "nullglob       \t%s\n",
    opt->nullglob == TRUE ? "on" : "off");
    fprintf(stdout, "sourcepath     \t%s\n",
    opt->sourcepath == TRUE ? "on" : "off");
    fprintf(stdout, "xpg_echo       \t%s\n",
    opt->xpg_echo == TRUE ? "on" : "off");
    return 0;
}

static int set_opt(char *opt, int *q, int *s, int *u)
{
    if (opt[0] != '-')
        return 2;

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
            fprintf(stdout, "ast-print      \t%s\n",
            ops->ast_print == TRUE ? "on" : "off");
            return (!(ops->ast_print == TRUE));
        }
        else
        {
            return ops->ast_print == TRUE ? 0 : 1;
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
            fprintf(stdout, "dotglob        \t%s\n",
            ops->dotglob == TRUE ? "on" : "off");
            return (!(ops->dotglob == TRUE));

        }

        else
        {
            return ops->dotglob == TRUE ? 0 : 1;
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
            fprintf(stdout, "expand_aliases \t%s\n",
            ops->expand_aliases == TRUE ? "on" : "off");
            return (!(ops->expand_aliases == TRUE));

        }

        else
        {
            return ops->expand_aliases == TRUE ? 0 : 1;
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
            fprintf(stdout, "extglob        \t%s\n",
            ops->extglob == TRUE ? "on" : "off");
            return (!(ops->extglob == TRUE));

        }
        else
        {
            return ops->extglob == TRUE ? 0 : 1;
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
            fprintf(stdout, "nocaseglob     \t%s\n",
            ops->nocaseglob == TRUE ? "on" : "off");
            return (!(ops->nocaseglob == TRUE));

        }
        else
        {
            return ops->nocaseglob == TRUE ? 0 : 1;
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
            fprintf(stdout, "nullglob       \t%s\n",
            ops->nullglob == TRUE ? "on" : "off");
            return (!(ops->nullglob == TRUE));

        }
        else
        {
            return ops->nullglob == TRUE ? 0 : 1;
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
            fprintf(stdout, "sourcepath     \t%s\n",
            ops->sourcepath == TRUE ? "on" : "off");
            return (!(ops->sourcepath == TRUE));

        }
        else
        {
            return ops->sourcepath == TRUE ? 0 : 1;
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
            fprintf(stdout, "xpg_echo       \t%s\n",
            ops->xpg_echo == TRUE ? "on" : "off");
            return (!(ops->xpg_echo == TRUE));

        }
        else
        {
            return ops->xpg_echo == TRUE ? 0 : 1;
        }
    }
    return 0;
}

static int handle_options(struct option *ops, char *opt, int q, int s,  int u)
{

    int retu = 0;

    if (!q && !s && !u)
    {
        retu = option_switch (ops, opt, 4);
    }

    if (s)
    {
        option_switch(ops, opt, 1);
    }

    if (u)
    {
        option_switch(ops, opt, 2);
    }

    if (q && !(s) && (!u))
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

int my_shopt(char *args[], ...)
{


    va_list ap;
    va_start(ap, args);
    struct option *opt = global.options;

    int s = 0;
    int q = 0;
    int u = 0;

    int found = 0;
    if (!args[1])
    {
        va_end(ap);
        return print_everything(opt);
    }

    int i = 1;
    int retcode = 0;
    int qres = 0;

    while (args[i])
    {
        int vuO = set_opt(args[i], &q, &s, &u);

        if (s && u)
        {
            fprintf(stderr, "shopt: cannot set and unset an option at the same time.\n");
            va_end(ap);
            return 1;
        }



        if (vuO == 0 && !found)
        {
            i++;
            continue;
        }

        else if (vuO == 1)
        {
            va_end(ap);
            return 1;
        }
        else
        {
            found = 1;
            if (!is_opt(args[i]))
            {
                fprintf(stderr, "shopt:  %s: invalid option name\n", args[i]);
                retcode = 1;
            }
            else
            {
                retcode = handle_options(opt, args[i],q,s,u);
                if ( qres == 0 )
                    qres = retcode;
            }
        }
        i++;
    }

    if (q && (!s && !u))
        retcode = qres;

    va_end(ap);
    return retcode;
}
