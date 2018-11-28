#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <libgen.h>
#include "lexer.h"
#include "history.h"
#include "grammar_check.h"
#include "options.h"
#include "token.h"
#include "linked_list.h"
#include "ast.h"
#include "execution_ast.h"
#include "err.h"
#include <sys/types.h>
#include <sys/wait.h>

static size_t mstrlen (char * string)
{
    return (*string ? 1 + mstrlen(string + 1) : 0);
}

static int norc_opt(void)
{
    char *arg1[] = {"/etc/42shrc", NULL};
    char *home = getenv("HOME");
    char *path = calloc(mstrlen(home) + 9, sizeof(char));
    strcpy(path, home);
    strcat(path, "/.42shrc");
    char *arg2[] = {path, NULL};
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
        free(path);
        return status;
    }
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
    char * final = calloc(fulllen(tokens) + 2, sizeof(char));
    cursor += mstrlen(start->elem->name);
    memcpy(final, start->elem->name, mstrlen(start->elem->name));
    while (start->next)
    {
        start = start->next;
        if (((start->elem->type == ENDOF && start->prev->elem->type != SEMICOLON)
            && !(strcmp(start->elem->name, "\n"))) || ((start->elem->type == SEMICOLON)))
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

static int interactive_mode(struct option *options)
{
    using_history();
    init_interact();
    int retcode = 0;
    char *listadd = NULL;
    struct f_tab *f_tab = NULL;
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
            retcode = 2;
        }
        else
        {
            free(line);
            listadd = from_tok_toS(tokens);
            if (listadd)
            {
                add_history(listadd);
                char *home = getenv("HOME");
                char *path = calloc(mstrlen(home) + 15, sizeof(char));
                strcpy(path, home);
                strcat(path, "/.42sh_history");
                FILE *history = fopen(path, "a");
                free(path);
                if (!history)
                    continue;
                fwrite(listadd, 1, mstrlen(listadd), history);
                fwrite("\n", 1, 1, history);
                fclose(history);
            }
            struct node *ast = build_ast(tokens);
            if (options->ast_print == TRUE)
                print_ast(ast);
            retcode = execution_ast(ast, &f_tab);
        }
    }
    return retcode;
}

int main(int argc, char *argv[])
{
    char *home = getenv("HOME");
    char *path = calloc(mstrlen(home) + 15, sizeof(char));
    strcpy(path, home);
    strcat(path, "/.42sh_history");
    FILE *history = fopen(path, "r+");
    if (!history)
    {
        history = fopen(path, "w+");
    }
    if (history)
        fclose(history);
    free(path);
    path = NULL;
    struct option *options = option_init();
    if (!options)
        errx(1, "Parsing error: memory full.");
    int index = options_parser(argc, argv, options);
    if (options->version == TRUE)
    {
        printf("Version 0.5\n");
        free(options);
        return 0;
    }
    if (options->norc == FALS)
    {
        norc_opt();
    }
    if (index >= argc && options->c == FALS)
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
    {
        errx(2, "Lexer error. Is your input conform to grammar ?");
    }
    char *home2 = getenv("HOME");
    char *path2 = calloc(mstrlen(home2) + 15, sizeof(char));
    strcpy(path2, home2);
    strcat(path2, "./42sh_history");
    history = fopen(path2, "a");
    char *toks = from_tok_toS(tokens);
    if (history)
    {
        fwrite(toks, 1, mstrlen(toks), history);
        fwrite("\n", 1, 1, history);
        fclose(history);
    }
    free(path2);
    path2 = NULL;
    free(toks);
    struct node *ast = build_ast(tokens);
    if (options->ast_print == TRUE)
        print_ast(ast);
    struct f_tab *f_tab = NULL;
    int res = execution_ast(ast, &f_tab);
    free(options);
    free_list(tokens);
    free_node(ast);
    return res;
}
