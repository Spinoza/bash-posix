#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdarg.h>
#include "globals.h"
#include "vector.h"

#define NB_BUILTINS 7

struct builtins
{
    char *name;
    //FIXME: potentiellement enlever l'int numb, peu utile ?
    int (*builtin)(int numb, char *args[], ...);
};

struct vector *get_assigns_sorted(struct assignment **table);
struct builtins *init_builts(void);

int my_exit(int number, char *args[], ...);
int my_shopt(int number, char *args[], ...);
int my_cd(int number, char *args[], ...);
int my_alias(int number, char *args[], ...);
int my_unalias(int number, char *args[], ...);
int my_echo(int number, char *args[], ...);
int my_export(int number, char *args[], ...);
#endif /* ! BUILTIN_H */
