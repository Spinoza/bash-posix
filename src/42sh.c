#include <stdlib.h>
#include "lexer.h"
#include "grammar_check.h"
#include "option.h"
#include "token.h"
#include "linked_list."
#include "ast.h"
#include "execution_ast.h"
#include "err.H"

int main(int argc, char *argv[])
{
    struct linked_list *tokens = lexer(argv, argc);
    int isgramm = grammar_check(tokens);
    if (!isgramm)
        errx(1, "Lexer error. Is your input conform to grammar ?");
    struct node *ast = ast_build(tokens);
    return isgramm;
}
