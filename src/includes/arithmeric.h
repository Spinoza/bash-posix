#ifndef ARITHMETIC_H
#define ARITHMETIC_H

enum operateur
{
    'n' = 0,
    '+' = 1,
    '-' = 2,
    '*' = 3,
    '/' = 4,
    '^' = 5,
    '~' = 6
}

struct number
{
    int sign;
    int number;
}

struct bt_node

{
    struct number *nb;
    enum operateur operateur;
    struct bt_node *left;
    struct bt_node *right;
}


#endif /* ! ARITHMETIC_H */
