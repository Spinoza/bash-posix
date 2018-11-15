#include <stdio.h>
#include "ast.h"
#include <err.h>

void print_aux(FILE *f, struct node *ast)
{
    struct node *as = ast->children;
    while(as)
    {
        fprintf(f, "    %s -- %s\n", ast->instr, as->instr);
        print_aux(f, as);
        as = as->next;
    }
}

void print_ast(struct node *ast)
{
    FILE *file = fopen("ast.dot", "w+");
    if(!file)
        errx(-1, "42sh:print_ast: fail open file");
    fprintf(file,"graph ast\n{\n    ");
    print_aux(file,ast);
    fprintf(file,"}");
    fclose(file);
}
