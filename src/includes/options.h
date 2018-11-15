#ifndef OPTIONS_H
#define OPTIONS_H

#define TRUE 1
#define FALSE 0
struct option
{
    int c;
    int minus_O;
    int plus_O;
    int norc;
    int ast_print;
    int version;
};
#endif /* ! OPTIONS_H */
