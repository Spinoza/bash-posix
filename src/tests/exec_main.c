#include "execution_ast.h"
#include "ast.h"

int main(int argc, char *argv[])
{
    struct linked_list *tokens = lexer(argv,argc);
    if (!grammar_check(tokens))
        printf("nope\n");
    struct node *root = build_ast(tokens);
    }
