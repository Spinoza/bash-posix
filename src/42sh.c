#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <libgen.h>
#include "lexer.h"
#include "grammar_check.h"
#include "options.h"
#include "token.h"
#include "linked_list.h"
#include "ast.h"
#include "execution_ast.h"
#include "err.h"

static size_t mstrlen(char *str)
{
    return *str ? 1 + strlen(str + 1) : 0;
}

static int interactive_mode(struct option *options)
{
    while (isatty(STDIN_FILENO))
    {
        char *line;
        size_t n = 0;
        printf("%s$ ", basename("/build/42sh"));
        getline(&line, &n, stdin);
        size_t i = mstrlen(line);
        line[i - 1] = '\0';
        struct linked_list *tokens = lexer_c(line);
        int isgramm = grammar_check(tokens);
        if (!isgramm)
        {
            fprintf(stdout, "lexer error: Is your input conform to grammar ?\n");
        }
        else
        {
            struct node *ast = build_ast(tokens);
            if (options->ast_print == TRUE)
                print_ast(ast);
            execution_ast(ast);
        }
    }
    return 1;
}

int main(int argc, char *argv[])
{
    struct option *options = option_init();
    if (!options)
        errx(1, "Parsing error: memory full.");
    int index = options_parser(argc, argv, options);
    if (options->version == TRUE)
    {
        printf("Version 0.3\n");
        free(options);
        return 0;
    }
    if (argc == 1 || options->c == FALSE)
    {
        return interactive_mode(options);
    }
    struct linked_list *tokens;
    if (options->c == TRUE)
    {
        tokens = lexer_c(options->arg_c);
    }
    else
    {
        tokens = lexer(argv, argc, index);
    }
    int isgramm = grammar_check(tokens);
    if (!isgramm)
        errx(1, "Lexer error. Is your input conform to grammar ?");
    struct node *ast = build_ast(tokens);
    if (options->ast_print == TRUE)
        print_ast(ast);

    int res = execution_ast(ast);
    free(options);
    free_list(tokens);
    free_node(ast);
    return res;
}
