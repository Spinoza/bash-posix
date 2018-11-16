#include "grammar_check.h"
#include "stdio.h"
#include "lexer.h"
#include "ast.h"


int main(int argc, char *argv[])
{
    struct linked_list *l = lexer(argv, argc,1);
    struct nL *n = l->head;
    while (n)
    {
        printf("%s->", n->elem->name);
        n = n->next;
    }
    printf("\n");
    int a = grammar_check(l);
    printf("%i \n", a);

    struct node *ast = build_ast(l);
    print_ast(ast);

    return 1;
}
