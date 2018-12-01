#include <unistd.h>
#include "builtins.h"
#include <stdio.h>
#include <string.h>

int my_cd(int number, char *args[], ...)
{
    number = number;
//FIXME: change_PID
    if(args[1] == NULL)
    {
        char *home = getenv("HOME");
        return chdir(home);
    }
    if(args[2] != NULL)
    {
        fprintf(stderr, "cd: Too many arguments");
    }
   return chdir(args[2]);
}


