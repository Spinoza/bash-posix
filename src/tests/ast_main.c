#include "grammar_check.h"
#include "stdio.h"
#include "lexer.h"
#include "ast.h"


int main(int argc, char *argv[])
{
    struct linked_list *l = lexer(argv, argc,1);
    int a = grammar_check(l);
    if (!a)
        return 0;
    struct node *ast = build_ast(l);
    print_ast(ast);

    return a;
}
