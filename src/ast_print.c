#include <stdio.h>
#include "ast.h"
#include <err.h>


void print_aux(FILE *f, struct node *ast, int i)
{
    struct node *as = ast->children;
    while(as)
    {
        fprintf(f, "    %s_%i -- %s_%i\n", ast->instr, i-1, as->instr, i);
        print_aux(f, as, i+1);
        as = as->next;
    }
}

void print_ast(struct node *ast)
{
    FILE *file = fopen("ast.dot", "w+");
    if(!file)
        errx(-1, "42sh:print_ast: fail open file");
    fprintf(file,"graph ast\n{\n");
    print_aux(file,ast,1);
    fprintf(file,"}");
    fclose(file);
}
