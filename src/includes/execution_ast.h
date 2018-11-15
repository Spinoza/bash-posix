#include "ast.h"


struct assignement
{
    char *name;
    char *value;
}

struct tab_a
{
    int nb;
    struct assignement **assignement;
    //to implement with hashmap
}
