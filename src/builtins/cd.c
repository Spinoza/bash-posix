#include <unistd.h>
#include "builtins.h"
#include <stdio.h>
#include <string.h>
#include "globals.h"

int my_cd(int number, char *args[], ...)
{
    number = number;
    char *home = getenv("HOME");
    char *pwd = getenv("PWD");
    char *parent = getenv("PWD");
    int i = strlen(parent) - 1;
    if(parent[i] == '/')
    {
        parent[i] = '\0';
        i--;
    }
    while(parent[i] != '/')
    {
        parent[i] = '\0';
        i--;
    }
    if(args[1] != NULL && args[2] != NULL)
    {
        fprintf(stderr, "cd: Too many arguments");
        return 1;
    }
    if(args[1]== NULL || !strcmp(args[1], "~") ||
    !strcmp(args[1], "~/"))
    {
        change_pwd(home);
    }
    else if(!strcmp(args[1], ".") || !strcmp(args[1], "./"))
    {
        change_pwd(global.PWD);
    }
    else if(!strcmp(args[1], "..") || !strcmp(args[1], "../"))
    {
       change_pwd(parent);
    }
    else if(!strcmp(args[1], "-"))
    {
        if(global.oldPWD == NULL)
        {
            fprintf(stderr, "cd: old PWD not yet set");
            return 1;
        }
        change_pwd(global.oldPWD);
        return chdir(global.oldPWD);
    }
    else if(args[1][0] == '/')
    {
        change_pwd("/");
    }
    else
    {
        char *first = strtok(args[1], '/');
        if(!strcmp(first, "."))
        {
            char *chg = calloc((strlen(args[1])+strlen(pwd))+2,sizeof(char));
            strcat(chg, pwd);
            strcat(chg, "/");
            strcat(chg, args[1]);
            change_pwd(chg);
        }
        else if(!strcmp(first, ".."))
        {
            char *chg = calloc((strlen(args[1])+strlen(parent)+2,sizeof(char));
            strcat(chg, parent);
            strcat(chg, "/");
            strcat(chg, args[1]);
            change_pwd(chg);
        }
        if(!strcmp(first, "~"))
        {
            char *chg = calloc((strlen(args[1])+strlen(home)+2,sizeof(char));
            strcat(chg, home);
            strcat(chg, "/");
            strcat(chg, args[1]);
            change_pwd(chg);
        }
    }
    return chdir(args[1]);
}


