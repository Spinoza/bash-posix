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
#include <sys/types.h>
#include <sys/wait.h>

static int norc_opt(void)
{
    char *arg1[] = {"../etc/42shrc", NULL};
    char *arg2[] = {"../.42shrc", NULL};
    pid_t pid = fork();
    if (pid == -1)
    {
        errx(1, "oopsie: error in forking ! -_('_')_-");
    }

    if (pid == 0)
    {
        pid_t pid2 = fork();
        if (pid2 == -1)
        {
            errx(1, "oopsie: error in forking in child !");
        }

        if (pid2 == 0)
        {
            int a = execvp(arg1[0], arg1);
            exit (a);
        }

        else
        {
            int r = 0;
            waitpid(pid2, &r, 0);
            r = execvp(arg2[0], arg2);
            exit(r);
        }
    }

    else
    {
        int status = 0;
        waitpid(pid, &status, 0);
        return status;
    }
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
    char *listadd;
    while (isatty(STDIN_FILENO))
    {
        char *line = readline("42sh$ ");
        struct linked_list *tokens = lexer_c(line);
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
        tokens = lexer_c(options->arg_c);
    }
    else
    {
        tokens = lexer(argv, argc, index);
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
