#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdarg.h>

struct builtin
{
    char *name;
    int (*builtin)(int numb, struct node *ast, ...);
}

#endif /* ! BUILTIN_H */
