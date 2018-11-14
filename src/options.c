#include "lexer.h"
#include "grammar_check.h"
#include "options.h"
#include <string.h>
#include <stdio.h>

struct option *option_init(void)
{
    struct option *new = malloc(sizeof(struct option));
    new->i = FALSE;
    new->minus_O = FALSE;
    new->plus_O = FALSE;
    new->l = FALSE;
    new->r = FALSE;
    new->s = FALSE;
    new->D = FALSE;
    new->norc = FALSE;
    new->ast_print = FALSE;
    new->version = FALSE;
    return new;
}

int set_command(char *string, struct option *options)
{
    if(!strcmp(string, "i"))
    {
        options->i = TRUE;
        return 1;
    }
    if(!strcmp(string, "l"))
    {
        options->l = TRUE;
        return 1;
    }
    if(!strcmp(string, "r"))
    {
        options->r = TRUE;
        return 1;
    }
    if(!strcmp(string, "s"))
    {
        options->s = TRUE;
        return 1;
    }
    if(!strcmp(string, "D"))
    {
        options->D = TRUE;
        return 1;
    }
    return 0;
}

int option_command(char *argv[], struct option *options, int i)
{
    if(!argv[i + 1]) //because -c needs to be followed by a command
    {
        fprintf(stderr,"42sh : -c option must be followed by a valid command.\n"
                "Usage : -c [i,l,r,s,D].\n");
        return 2;
    }
    if(!set_command(argv[i + 1],options))
    {
        fprintf(stderr,"42sh : wrong command.\n\
                Usage : -c [i,l,r,s,D].\n");
        return 127;
    }
    return 0;
}

//Parses the options, return values are the ones from bash
int options_parser(int argc, char *argv[], struct option *options)
{
    for (int i = 1; i < argc; i++) //Skip the first arg
    {
        if(argv[i][0] == '-')
        {
            if(!strcmp(argv[i],"-c"))
            {
                int res = option_command(argv,options, i);
                i++;
                if(res != 0)
                    return res;
            }
            else if(!strcmp(argv[i],"-O"))
                options->minus_O = TRUE;
            else if(!strcmp(argv[i],"--norc"))
                options->norc = TRUE;
            else if(!strcmp(argv[i],"--ast-print"))
                options->ast_print = TRUE;
            else if(!strcmp(argv[i],"--version"))
                options->version = TRUE;

            else // Invalid option
            {
                fprintf(stderr, "42sh : %s: invalid option.\n",argv[i]);
                return 2;
            }
            continue;
        }
        if(!strcmp(argv[i],"+O"))
            options->plus_O = TRUE;
        else
            break;
    }
    return 0;
}
void print_options(struct option *options)
{
    printf("i %d\n",options->i);
    printf("l %d\n",options->l);
    printf("r %d\n",options->r);
    printf("s %d\n",options->s);
    printf("D %d\n",options->D);
    printf("minus_O %d\n",options->minus_O);
    printf("plus_O %d\n",options->plus_O);
    printf("norc %d\n",options->norc);
    printf("ast_print %d\n",options->ast_print);
    printf("version %d\n",options->version);
}
int main(int argc, char *argv[])
{
    struct option *options = option_init();
    int res = options_parser(argc, argv, options);
    print_options(options);
    return res;
}
