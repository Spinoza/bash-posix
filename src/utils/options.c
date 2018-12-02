#include "lexer.h"
#include "grammar_check.h"
#include "options.h"
#include "err.h"
#include "globals.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

struct option *option_init(void)
{
    struct option *new = malloc(sizeof(struct option));
    new->minus_O = FALS;
    new->plus_O = FALS;
    new->c = FALS;
    new->norc = FALS;
    new->ast_print = FALS;
    new->version = FALS;
    new->dotglob = FALS;
    new->expand_aliases = FALS;
    new->extglob = FALS;
    new->nocaseglob = FALS;
    new->nullglob = FALS;
    new->sourcepath = FALS;
    new->xpg_echo = FALS;

    new->arg_c = NULL;
    return new;
}

static void set_opt(struct option *opts, char *ops, int retcode)
{
    if (!(strcmp("ast-print", ops)))
    {
        opts->ast_print = retcode ? FALS : TRUE;
    }

    else if (!(strcmp("dotglob", ops)))
    {
        opts->ast_print = retcode ? FALS : TRUE;
    }

    else if (!(strcmp("expand_aliases", ops)))
    {
        opts->ast_print = retcode ? FALS : TRUE;
    }

    else if (!(strcmp("extglob", ops)))
    {
        opts->ast_print = retcode ? FALS : TRUE;
    }

    else if (!(strcmp("nocaseglob", ops)))
    {
        opts->ast_print = retcode ? FALS : TRUE;
    }

    else if (!(strcmp("nullglob", ops)))
    {
        opts->ast_print = retcode ? FALS : TRUE;
    }

    else if (!(strcmp("sourcepath", ops)))
    {
        opts->ast_print = retcode ? FALS : TRUE;
    }

    else if (!(strcmp("xpg_echo", ops)))
    {
        opts->ast_print = retcode ? FALS : TRUE;
    }

    else
    {
        //FIXME: correctly free global* free_glob();
        errx(127, "-O: %s: invalid option name.\n", ops);
    }
}

static int set_c(struct option *opts, char *arg[], int pos, int argc)
{
    if (pos >= argc)
        errx(1, "Error: the c option takes an argument.");

    opts->arg_c = arg[pos];
    return pos;
}

static int set_mO(struct option *opts, char *arg[], int pos, int argc)
{
    if (pos >= argc)
        errx(1, "Error: the -O option takes an argument.");

    //opts->arg_mO = arg[pos];
    set_opt(opts, arg[pos], 0);
    return pos;
}

static int set_pO(struct option *opts, char *arg[], int pos, int argc)
{
    if (pos >= argc)
        errx(1, "Error: the +O option takes an argument.");

    //opts->arg_pO = arg[pos];
    set_opt(opts, arg[pos], 1);
    return pos;
}

static int set_commands(struct option *options, char *argv[], int i, int argc)
{
    if (!strcmp(argv[i],"-c"))
    {
        options->c = TRUE;
        return set_c(options, argv, i + 1, argc);
    }
    if (!strcmp(argv[i],"-O"))
    {
        options->minus_O = TRUE;
        return set_mO(options, argv, i + 1, argc);
    }
    if (!strcmp(argv[i],"--norc"))
    {
        options->norc = TRUE;
        return i;
    }
    if (!strcmp(argv[i],"--ast-print"))
    {
        options->ast_print = TRUE;
        return i;
    }
    if (!strcmp(argv[i],"--version"))
    {
        options->version = TRUE;
        return i;
    }

    if (!strcmp(argv[i],"+O"))
    {
        options->plus_O = TRUE;
        return set_pO(options, argv, i+1, argc);
    }
    return 0;
}
//Parses the options, return values are the ones from bash
int options_parser(int argc, char *argv[], struct option *options)
{
    int pos = 1;
    while (pos < argc) //Skip the first arg
    {
        if (argv[pos][0] == '-' || argv[pos][0] == '+')
        {
            int temp = pos;
            if ( (pos = set_commands(options,argv, pos, argc)) == 0)
            {
                errx(2, "Invalid option : %s.", argv[temp]);
            }
        }
        else
            break;
        pos++;
    }
    return pos;
}
