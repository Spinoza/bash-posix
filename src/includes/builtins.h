#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdarg.h>
#include "globals.h"

struct builtins
{
    char *name;
    //FIXME: potentiellement enlever l'int numb, peu utile ?
    int (*builtin)(int numb, char *args[], ...);
};

struct builtins *init_builts(void);
int my_exit(int number, char *args[], ...);
int my_shopt(int number, char *args[], ...);
#endif /* ! BUILTIN_H */
