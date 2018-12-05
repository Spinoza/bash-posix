#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "helper_exec.h"


void init_globv(void)
{
    global.ast_root = NULL;
    global.res = 0;
    global.data = stored_data_init();
    global.options = option_init();
    global.oldPWD = NULL;
    global.PWD = strcpy(calloc(strlen(getenv("PWD")) + 1, sizeof(char))
    ,getenv("PWD"));
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
}

void change_pwd(char *newPID)
{
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
