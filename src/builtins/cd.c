#include <unistd.h>
#include "builtins.h"
#include <stdio.h>
#include <string.h>
#include "globals.h"

int my_cd(int number, char *args[], ...)
{
    number = number;
    if(args[1] != NULL && args[2] != NULL)
    {
        fprintf(stderr, "cd: Too many arguments");
        return 1;
    }
    if(args[1]== NULL || !strcmp(args[1], "~") ||
    !strcmp(args[1], "~/"))
    {
        char *home = getenv("HOME");
        change_pwd(home);
        return chdir(home);
    }
    if(!strcmp(args[1], ".") || !strcmp(args[1], "./"))
    {
        change_pwd(global.PWD);
        return 0;
    }
    if(!strcmp(args[1], "..") || !strcmp(args[1], "../"))
    {
        char *pwd = getenv("PWD");
        int i = strlen(pwd) - 1;
        if(pwd[i] == '/')
        {
            pwd[i] '\0';
            i--;
        }
        while(pwd[i] != '/')
        {
            pwd[i] = '\0';
            i--;
        }
        change_pwd(pwd);
        return chdir(pwd);
    }
    if(!strcmp(args[1], "-"))
    {
        if(global.oldPWD == NULL)
        {
            fprintf(stderr, "cd: old PWD not yet set");
            return 1;
        }
        change_pwd(global.oldPWD);
        return chdir(global.oldPWD);
    }
    return chdir(args[1]);
}


