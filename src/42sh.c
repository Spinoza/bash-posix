#include <stdlib.h>
#include <string.h>
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


static size_t mstrlen (char * string)
{
    return *string ? 1 + mstrlen(string + 1) : 0;
}

static char * from_tok_toS(struct linked_list *tokens)
{
    int cursor = 0;
    struct nL *start = tokens->head;
    char * final = malloc(mstrlen(start->elem->name) * sizeof(char) + 2);
    cursor += mstrlen(start->elem->name);
    final = strcat(final, start->elem->name);
    while (start->next)
    {
        start = start->next;
        final = realloc(final, cursor + 1 + mstrlen(start->elem->name) * sizeof(char));
        if (((start->elem->type == ENDOF && start->prev->elem->type != SEMICOLON) && !(strcmp(start->elem->name, "\n"))) || ((start->elem->type == SEMICOLON)))
        {
            final[cursor] = ';';
            cursor++;
        }

        else if (start->elem->type != SEMICOLON && start->elem->type != ENDOF)
        {
            final[cursor] = ' ';
            cursor++;
            final = strcat(final, start->elem->name);
            cursor += mstrlen(start->elem->name);
        }
    }

    return final;
}

static int interactive_mode(struct option *options)
{
    using_history();
    while (isatty(STDIN_FILENO))
    {
        char * line = readline("42sh$ ");
        struct linked_list *tokens = lexer_c(line);
        int isgramm = grammar_check(tokens);
        if (!isgramm)
        {
            fprintf(stdout, "lexer error: Is your input conform to grammar ?\n");
        }
        else
        {
            char * hist_add = from_tok_toS(tokens);
            add_history(hist_add);
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
    if (argc == 1)
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
