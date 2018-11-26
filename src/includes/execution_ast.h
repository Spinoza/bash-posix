#include "ast.h"

struct assignment
{
    char *name;
    char *value;
};

enum operators
{
    FALSE = 0,
    INFERIOR = 2,
    SUPERIOR,
    INFERIOR_EQ,
    SUPERIOR_EQ,
    EQUAL,
    DIFFERENT,
    S_EQUAL,
    S_N_EQUAL
};
struct tab_a
{
    size_t nb;
    size_t capacity;
    struct assignment **assignment;
    //to implemen0t with hashmap
};

struct function
{
    char *name;
    struct node *function_start;
};

struct f_tab
{
    size_t nb;
    size_t capacity;//10
    struct function **f;
};

int execution_ast(struct node *n);
int traversal_ast(struct node *n, int *res, struct f_tab *f_tab);
