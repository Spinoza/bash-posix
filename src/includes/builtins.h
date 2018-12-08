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
    int (*builtin)(char *args[], ...);
};

struct vector *get_assigns_sorted(struct assignment **table);
struct builtins *init_builts(void);
void add_aliases(void);


int my_exit(char *args[], ...);
int my_shopt(char *args[], ...);
int my_cd(char *args[], ...);
int my_alias(char *args[], ...);
int my_unalias(char *args[], ...);
int my_echo(char *args[], ...);
int my_export(char *args[], ...);
#endif /* ! BUILTIN_H */
