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
#include "file_handle.h"
#include <sys/types.h>
#include <sys/wait.h>

static int norc_opt(void)
{
    char *arg1 = "../etc/42shrc";
    char *arg2 =  "../.42shrc";

    struct linked_list *tokens1 = read_fil(arg1);
    if(!grammar_check(tokens1))
    {
        printf("Error in loading 42shrc. Check your grammar\n");
        free_list(tokens1);
    }

    struct linked_list *tokens2 = read_fil(arg2);
    if(!grammar_check(tokens2))
    {
        printf("Error in loading .42shrc. Check your grammar\n");
        free_list(tokens2);
    }

    struct node *ast1 = build_ast(tokens1);
    struct node *ast2 = build_ast(tokens2);

    execution_ast(ast1);
    execution_ast(ast2);

    free_list(tokens1);
    free_node(ast1);
    free_list(tokens2);
    free_node(ast2);

    return 0;
}
static size_t mstrlen (char * string)
{
    return *string ? 1 + mstrlen(string + 1) : 0;
}

static size_t fulllen(struct linked_list *tokens)
{
    size_t len = 0;
    struct nL *start = tokens->head;
    while (start)
    {
        len += mstrlen(start->elem->name) + 2;
        start = start->next;
    }

    return len;
}

static char * from_tok_toS(struct linked_list *tokens)
{
    int cursor = 0;
    struct nL *start = tokens->head;
    char * final = malloc(fulllen(tokens) * sizeof(char) + 2);
    cursor += mstrlen(start->elem->name);
    memcpy(final, start->elem->name, mstrlen(start->elem->name));
    while (start->next)
    {
        start = start->next;
        if (((start->elem->type == ENDOF && start->prev->elem->type != SEMICOLON) && !(strcmp(start->elem->name, "\n"))) || ((start->elem->type == SEMICOLON)))
        {
            final[cursor] = ';';
            cursor++;
        }

        else if (start->elem->type != SEMICOLON && start->elem->type != ENDOF)
        {
            final[cursor] = ' ';
            cursor++;
            memcpy(final + cursor, start->elem->name, mstrlen(start->elem->name));
            cursor += mstrlen(start->elem->name);
        }
    }

    return final;
}

static int interactive_mode(struct option *options, FILE *history)
{
    using_history();
    char *listadd = NULL;
    struct linked_list *tokens = NULL;
    while (isatty(STDIN_FILENO))
    {
        char *line = readline("42sh$ ");
        if (is_file(line))
        {
            tokens = read_fil(line);
        }
        else
        {
            tokens = lexer_c(line);
        }
        int isgramm = grammar_check(tokens);
        if (!isgramm)
        {
            free(line);
            listadd = from_tok_toS(tokens);
            if (listadd)
                 add_history(listadd);
            fprintf(stdout, "lexer error: Is your input conform to grammar ?\n");
        }
        else
        {
            free(line);
            listadd = from_tok_toS(tokens);
            if (listadd)
                add_history(listadd);
            if (listadd && history)
            {
                fwrite(listadd, 1, mstrlen(listadd), history);
                fclose(history);
                history = fopen("../.42sh_history", "a");
            }
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
    FILE *history = fopen("../.42sh_history", "a");
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
    if (options->norc == FALS)
    {
        norc_opt();
    }
    if (index >= argc && options->c == FALS)
    {
        return interactive_mode(options, history);
    }
    struct linked_list *tokens;
    if (options->c == TRUE)
    {
        if (is_file(options->arg_c))
        {
            tokens = read_fil(options->arg_c);
        }
        else
        {
            tokens = lexer_c(options->arg_c);
        }
    }
    else
    {
        if (is_file(argv[1]))
        {
            tokens = read_fil(argv[1]);
        }
        else
        {
            tokens = lexer(argv, argc, index);
        }
    }
    int isgramm = grammar_check(tokens);
    if (!isgramm)
        errx(1, "Lexer error. Is your input conform to grammar ?");
    char *toks = from_tok_toS(tokens);
    fwrite(toks, 1, mstrlen(toks), history);
    free(toks);
    struct node *ast = build_ast(tokens);
    if (options->ast_print == TRUE)
        print_ast(ast);

    int res = execution_ast(ast);
    fclose(history);
    free(options);
    free_list(tokens);
    free_node(ast);
    return res;
}
