#include "lexer.h"
#include "grammar_check.h"
#include "options.h"
#include <string.h>
#include <stdio.h>

struct option *option_init(void)
{
    struct option *new = malloc(sizeof(struct option));
    new->minus_O = FALSE;
    new->plus_O = FALSE;
    new->c = FALSE;
    new->norc = FALSE;
    new->ast_print = FALSE;
    new->version = FALSE;
    return new;
}

int set_commands(struct option *options, char *string)
{
    if(!strcmp(string,"-c"))
    {
        options->c = TRUE;
        return 1;
    }
    if(!strcmp(string,"-O"))
    {
        options->minus_O = TRUE;
        return 1;
    }
    if(!strcmp(string,"norc"))
    {
        options->norc = TRUE;
        return 1;
    }
    if(!strcmp(string,"--ast-print"))
    {
        options->ast_print = TRUE;
        return 1;
    }
    if(!strcmp(string,"--version"))
    {
        options->version = TRUE;
        return 1;
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
            if(set_commands(options,argv[i]))
                continue;
            else // Invalid option
            {
                fprintf(stderr, "42sh : %s: invalid option.\n",argv[i]);
                return 2;
            }
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
    printf("c %d\n",options->c);
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
