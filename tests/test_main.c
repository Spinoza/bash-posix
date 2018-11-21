#include "grammar_check.h"
#include "lexer.h"
#include "token.h"
#include "lexer.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

struct test_options
{
    int sanity;
    int lexer;
    int grammar;
    int ast-print;
    int 42sh;
}

void print_list(void)
{
    fprintf(stdout,"List of test categories to call with -c <category> : \n,
            lexer\n
            grammar\n
            ast-print\n
            42sh");
}

void parse_options(int argc, char *argv[], struct test_options *options)
{
    for (int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i],"-c") || !strcmp(argv[i],"--category"))
        {
            while ( i < argc && argv[i][0] != '-')
            {
                if(!strcmp(argv[i], "lexer"))
                    options->lexer = TRUE;
                if(!strcmp(argv[i], "42sh"))
                    options->42sh = TRUE;
                if(!strcmp(argv[i], "grammar"))
                    options->grammar = TRUE;
                if(!strcmp(argv[i], "ast-print"))
                    options->ast-print = TRUE;
            }
        }
        if(!strcmp(argv[i],"-l") || !strcmp(argv[i],"--list"))
        {
            print_list();
        }
        if(!strcmp(argv[i],"-s") || !strcmp(argv[i],"--sanity"))
        {
            options->sanity = TRUE;
        }
    }
}
int main(int argc, char *argv[])
{
    struct test_options *options = calloc(1,sizeof(struct test_options));
    parse_options(argv, argv, options);
    if(
}
