#include "lexer.h"
#include "grammar_check.h"
#include "options.h"
#include "err.h"
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

    new->arg_pO = NULL;
    new->arg_mO = NULL;
    new->arg_c = NULL;
    return new;
}

void handle_option(struct option *options)
{
    if (options->minus_O == TRUE)
    {
        printf("Option -O detected. %s shall be set.\n", options->arg_mO);
    }

    if (options->plus_O == TRUE)
    {
        printf("Option +O detected. %s shall be unset.\n", options->arg_pO);
    }

    if (options->norc == TRUE)
    {
        printf("Option --norc detected. Deactivating the resource loader.\n");
    }

    if (options->ast_print == TRUE)
    {
        printf("--ast-print : a dot file containing the AST of your input shall be created in your current directory.\n");
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

    opts->arg_mO = arg[pos];
    return pos;
}

static int set_pO(struct option *opts, char *arg[], int pos, int argc)
{
    if (pos >= argc)
        errx(1, "Error: the +O option takes an argument.");

    opts->arg_pO = arg[pos];
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
            if ( (pos = set_commands(options,argv, pos, argc)) == 0)
            {
                errx(1, "Invalid option : %s.", argv[pos]);
            }
        }
        else
            break;
        pos++;
    }
    return pos;
}
