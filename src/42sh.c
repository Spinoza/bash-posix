#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <libgen.h>
#include <locale.h>
#include <signal.h>
#include "lexer.h"
#include "builtins.h"
#include "history.h"
#include "grammar_check.h"
#include "options.h"
#include "token.h"
#include "linked_list.h"
#include "ast.h"
#include "file_handle.h"
#include "execution_ast.h"
#include "ast_assignement.h"
#include "err.h"
#include "globals.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

struct globv global;

static int norc_opt(void)
{
    char *arg1[] = {"/etc/42shrc", NULL};
    char *home = getenv("HOME");
    if (!home)
        return 1;
    char *path = calloc(strlen(home) + 9, sizeof(char));
    strcpy(path, home);
    strcat(path, "/.42shrc");
    char *arg2[] = {path, NULL};
    pid_t pid = fork();
    if (pid == -1)
    {
        errx(1, "error in forking\n");
    }
    if (pid == 0)
    {
        pid_t pid2 = fork();
        if (pid2 == -1)
        {
            errx(1, "error in forking\n");
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
        len += strlen(start->elem->name) + 2;
        start = start->next;
    }
    return len;
}

static char * from_tok_toS(struct linked_list *tokens)
{
    int cursor = 0;
    struct nL *start = tokens->head;
    char * final = calloc(fulllen(tokens) + 2, sizeof(char));
    cursor += strlen(start->elem->name);
    memcpy(final, start->elem->name, strlen(start->elem->name));
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
            memcpy(final + cursor, start->elem->name, strlen(start->elem->name));
            cursor += strlen(start->elem->name);
        }
    }

    return final;
}

static void sig_handling(__attribute__((unused))int signum)
{
    return;
}

static void handle_signals()
{
    signal(SIGINT, sig_handling);
    signal(SIGTSTP, sig_handling);
}

static int interactive_mode(struct globv global)
{
    using_history();
    init_interact();
    int retcode = 0;
    char *listadd = NULL;
    while (1)
    {
        handle_signals();
        char *line;
        char *PS1 = get_assign("PS1", global.data);
        if (!PS1)
            line = readline("42sh$ ");
        else
            line = readline(PS1);
        if (!line)
        {
            printf("exit\n");
            char *ex[] =
            {
                "exit", NULL
            }
            ;
            my_exit(ex, 0);
        }
        struct linked_list *tokens = lexer_c(line);
        int isgramm = grammar_check(tokens);
        if (!isgramm)
        {
            free(line);
            listadd = from_tok_toS(tokens);
            if (listadd)
            {
                add_history(listadd);
                free(listadd);
            }
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
                if (!home)
                {
                    free(listadd);
                    continue;
                }
                char *path = calloc(strlen(home) + 15, sizeof(char));
                strcpy(path, home);
                strcat(path, "/.42sh_history");
                FILE *history = fopen(path, "a");
                free(path);
                if (!history)
                    continue;
                fwrite(listadd, 1, strlen(listadd), history);
                fwrite("\n", 1, 1, history);
                fclose(history);
                free(listadd);
            }
            struct node *ast = build_ast(tokens);
            global.ast_root = ast;
            if (global.options->ast_print == TRUE)
                print_ast(ast);
            retcode = execution_ast(ast, global.data);
            add_aliases();
            free_node(global.ast_root);
            global.ast_root = NULL;
            ast = NULL;
        }
        free_list(tokens);
    }
    return retcode;
}
int main(int argc, char *argv[])
{
    setlocale(LC_CTYPE, "");
    init_globv();
    srand(getpid());
    set_history();
    if (!global.options)
        errx(1, "Parsing error: memory full.");
    int index = options_parser(argc, argv, global.options);
    if (global.options->version == TRUE)
    {
        printf("Version 0.9\n");
        free_glob();
        return 0;
    }
    if (global.options->norc == FALS)
    {
        norc_opt();
    }
    if (index >= argc && global.options->c == FALS && isatty(STDIN_FILENO))
    {
        return interactive_mode(global);
    }
    struct linked_list *tokens;
    if (!isatty(STDIN_FILENO))
    {
        tokens = reading(stdin);
    }
    else if (global.options->c == TRUE)
    {
        tokens = lexer_c(global.options->arg_c);
    }
    else if (argc == index + 1)
    {
        FILE *toread = fopen(argv[index], "r");
        if (!toread)
        {
            errx(126, "%s: permission denied.", argv[1]);
        }
        tokens = reading(toread);
        fclose(toread);
    }

    int isgramm = grammar_check(tokens);
    if (!isgramm)
    {
        free_list(tokens);
        free_glob();
        errx(2, "lexer: unexpected token.");
    }
    struct node *ast = build_ast(tokens);
    global.ast_root = ast;
    if (global.options->ast_print == TRUE)
        print_ast(ast);
    int res = execution_ast(ast, global.data);
    add_aliases();
    free_list(tokens);
    free_glob();
    return res;
}
