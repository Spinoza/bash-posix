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
    global.oldPID = NULL;
    global.PID = NULL;
}

void free_glob(void)
{
    if (global.ast_root)
        free_node(global.ast_root);
    if (global.oldPID)
        free(global.oldPID);
    if (global.PID)
        free(global.PID);
    free_data(global.data);
    free(global.options);
}

void change_pid(char *newPID)
{
    if (!global.PID)
    {
        global.PID = calloc(mstrlen(newPID) + 1, sizeof(char));
        strcpy(global.PID, newPID);
    }

    else
    {
        if (global.oldPID)
        {
            free(global.oldPID);
        }

        global.oldPID = calloc(mstrlen(global.PID) + 1, sizeof(char));
        strcpy(global.oldPID, global.PID);
        free(global.PID);
        global.PID = calloc(mstrlen(newPID) + 1, sizeof(char));
        strcpy(global.PID, newPID);
    }
}
