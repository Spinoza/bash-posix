#include <stdio.h>
#include "ast.h"
#include <err.h>


static void update_encount(enum type tokentype, int *encountered)
{
    encountered[tokentype] += 1;
}

static void print_aux(FILE *f, struct node *ast, int* encountered, int act)
{
    char *args[33] = { "IF", "THEN", "ELSE", "FI", "LOGICAND", "SEMI", "WHILE", "FOR",
    "UNTIL", "CASE", "DO", "DONE", "WORD", "ASSIGNW", "ENDOF", "PIPE", "LOGOR", "AND",
    "IN", "ESAC", "ELIF", "HERED", "IONUM", "COND", "ROOT", "REDIR", "OPENPAR", "CLOSEPAR", "OPENBRA", "CLOSEBRA", "DOUBLESEM", "FUNC", "EXPANDW" };
    struct node *as = ast->children;
    while (as)
    {
        fprintf(f, "    %s%i -- %s%i\n", args[ast->tokentype], act, args[as->tokentype],
                        encountered[as->tokentype]);
        int j = encountered[as->tokentype];
        update_encount(as->tokentype, encountered);
        print_aux(f, as, encountered, j);
        as = as->next;
    }
}

void print_ast(struct node *ast)
{
    FILE *file = fopen("ast.dot", "w+");
    if(!file)
        errx(-1, "42sh:print_ast: fail open file");
    fprintf(file,"graph ast\n{\n");
    int *encount = malloc(32 * sizeof(int));
    print_aux(file,ast,encount, 0);
    free(encount);
    fprintf(file,"}");
    fclose(file);
}
