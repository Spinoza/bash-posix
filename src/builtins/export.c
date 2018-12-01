#include "builtins.h"

int my_export(int number, char *args[], ...)
{
    int i = 1;
    int p = 0;
    int n = 0;
    int file = 0;
    while(args[i])
    {
        if(!strcmp(args[i], "-p") && file == 0)
        {
            p = 1;
        }
        else if(!strcmp(args[i], "-n") && file == 0)
        {
            n = 1;
        }
        else
        {
            if(file == 0)
            {
                file = 1;
            }
            if(args[i][0] == '-')
            {
                fprintf(stderr, "export: not a valid identifier");
                return 1;
            }
            //FIXME: faire des trucs
        }
        i++;
    }
    if(file == 0)
    {
        //FIXME: PRINT EXPORT TABLE
        return 0;
    }
}
