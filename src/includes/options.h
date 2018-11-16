#ifndef OPTIONS_H
#define OPTIONS_H

#define TRUE 1
#define FALS 0

#include <stdio.h>

struct option
{
    int c;
    int minus_O;
    int plus_O;
    int norc;
    int ast_print;
    int version;

    char *arg_c;
    char *arg_pO;
    char *arg_mO;
};

struct option *option_init(void);
void handle_option(struct option *options);
int options_parser(int argc, char *argv[], struct option *options);
#endif /* ! OPTIONS_H */
