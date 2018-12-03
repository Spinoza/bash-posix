#include <unistd.h>
#include "builtins.h"
#include <stdio.h>
#include <string.h>
#include "globals.h"

static char *find_path(char *arg)
{
    char *first = calloc (strlen(arg + 1, sizeof(char)));
    strcpy(first, arg);
    char *remain;
    first strtok_r(first, "/ \0", &remain);
    while(first)
    {
        if(!strcmp(frist, "-"))
        {
            if(global.oldPWD == NULL)
            {
                fprintf(stderr, "cd: old PWD not yet set");
                return -1;
            }
            return chdir(global.oldPWD);
        }
        if(chdir(first) == -1)
            return -1;
    }
    change_pwd(getenv("PWD"));
    return 0;
}

int my_cd(int number, char *args[], ...)
{
    number = number;
    if(args[1] != NULL && args[2] != NULL)
    {
        fprintf(stderr, "cd: Too many arguments");
        return 1;
    }
    if(args[1] == NULL)
    {
        int ret = chdir(getenv("HOME"));
        if(ret == -1)
            return 1;
        change_pwd(getenv("PWD"));
        return 0;
    }
    if(args[1][0] == '/')
    {
        change_pwd("/");
        return chdir("/");
    }
    int change = find_path(args[1]);
    if(change == -1)
        return 1;

    return 0;
}


