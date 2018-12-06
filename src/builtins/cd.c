#include <unistd.h>
#include "builtins.h"
#include <stdio.h>
#include <string.h>
#include "globals.h"

static int find_path(char *arg)
{
    char *first = calloc (strlen(arg) + 1, sizeof(char));
    strcpy(first, arg);
    char *tok = strtok(first, "/ \0");
    while (tok)
    {
        if (!strcmp(first, "-"))
        {
            if (global.oldPWD == NULL)
            {
                fprintf(stderr, "cd: old PWD not yet set\n");
                free(first);
                return -1;
            }
            int res = chdir(global.oldPWD);
            if (res != -1)
            {
                char *cpy = calloc(strlen(global.oldPWD) + 1, sizeof(char));
                strcpy(cpy, global.oldPWD);
                change_pwd(cpy);
                free(cpy);
            }
            free(first);
            return res;
        }
        if (chdir(first) == -1)
            return -1;
        tok = strtok(NULL, "/ \0");
    }
    free(first);
    char * dir = getcwd(NULL, 0);
    change_pwd(dir);
    free(dir);
    return 0;
}

int my_cd(int number, char *args[], ...)
{
    number = number;
    if (args[1] != NULL && args[2] != NULL)
   {
        fprintf(stderr, "cd: Too many arguments\n");
        return 1;
    }
    if (args[1] == NULL)
    {
        int ret = chdir(getenv("HOME"));
        if (ret == -1)
           return 1;
        char *pwd = getenv("PWD");
        if (!pwd)
            return 1;
        change_pwd(pwd);
        return 0;
    }
    if (args[1][0] == '/')
    {
        change_pwd("/");
        return chdir("/");
    }
    int change = find_path(args[1]);
    if (change == -1)
        return 1;

    return 0;
}
