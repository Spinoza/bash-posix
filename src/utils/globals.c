#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "globals.h"
#include "builtins.h"
#include "helper_exec.h"

void out_of_memory(void)
{
    global.res = 127;
    fprintf(stderr,"42sh: ran out of memory.\n");
    global.data->builtins[0].builtin(NULL,127);
}

void init_globv(void)
{
    global.ast_root = NULL;
    global.res = 0;
    global.aliastab = vector_create();
    global.data = stored_data_init();
    global.options = option_init();
    global.oldPWD = NULL;
    char *pwd = getenv("PWD");
    if (!pwd)
    {
        global.PWD = NULL;
    }
    else
    {
        global.PWD = strcpy(calloc(strlen(pwd) + 1, sizeof(char))
    ,pwd);
    }
}

void free_glob(void)
{
    if (global.ast_root)
        free_node(global.ast_root);
    if (global.oldPWD)
        free(global.oldPWD);
    if (global.PWD)
        free(global.PWD);
    if (global.data)
        free_data(global.data);
    if (global.options)
        free(global.options);
    if (global.aliastab)
        vector_destroy(global.aliastab);
}

void change_pwd(char *newPID)
{
    if (!newPID)
        return;

    if (!global.PWD)
    {
        global.PWD = calloc(strlen(newPID) + 1, sizeof(char));
        strcpy(global.PWD, newPID);
    }
    else
    {
        if (global.oldPWD)
        {
            free(global.oldPWD);
            global.oldPWD = NULL;
        }
        global.oldPWD = calloc(strlen(global.PWD) + 1, sizeof(char));
        strcpy(global.oldPWD, global.PWD);
        free(global.PWD);
        global.PWD = calloc(strlen(newPID) + 1, sizeof(char));
        strcpy(global.PWD, newPID);
    }
}
