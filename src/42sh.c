#include <stdlib.h>
#include "lexer.h"
#include "grammar_check.h"
#include "options.h"
#include "token.h"
#include "linked_list.h"
#include "ast.h"
#include "execution_ast.h"
#include "err.h"

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
    handle_option(options);
    struct linked_list *tokens = lexer(argv, argc, index);
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
