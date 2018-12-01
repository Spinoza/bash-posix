#include <unistd.h>
#include "builtins.h"
#include <stdio.h>
#include <string.h>

int my_cd(int number, struct node *ast, ...)
{
    number = number;

    if(ast->next == NULL || ast->next->tokentype == SEMICOLON)
    {
        char *home = getenv("HOME");
        return chdir(home);
    }
    ast = ast->next;
    if(ast->next != NULL && ast->next->tokentype != SEMICOLON)
    {
        fprintf(stderr, "cd: Too many arguments");
    }
   /* if(!strcmp(ast->instr, ".") || !strcmp(ast->instr, "./"))
    {
        return 0;
    }*/
    return chdir(ast->instr);

}
