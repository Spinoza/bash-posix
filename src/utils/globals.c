#include <stdlib.h>
#include <string.h>
#include "globals.h"

static size_t mstrlen(char *string)
{
    return *string ? 1 + mstrlen(string + 1) : 0;
}

void init_globv(void)
{
    global.ast_root = NULL;
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
    free_data(global.data);
    free(global.options);
}

void change_pid(char *newPID)
{
    if (!global.PWD)
    {
        global.PWD = calloc(mstrlen(newPID) + 1, sizeof(char));
        strcpy(global.PWD, newPID);
    }

    else
    {
        if (global.oldPWD)
        {
            free(global.oldPWD);
            global.oldPWD = NULL;
        }

        global.oldPWD = calloc(mstrlen(global.PWD) + 1, sizeof(char));
        strcpy(global.oldPWD, global.PWD);
        free(global.PWD);
        global.PWD = calloc(mstrlen(newPID) + 1, sizeof(char));
        strcpy(global.PWD, newPID);
    }
}
