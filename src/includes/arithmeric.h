#ifndef ARITHMETIC_H
#define ARITHMETIC_H

enum sign
{
    '+',
    '-',
}

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
    enum sign;
    int number;
}

struct bt_node

{
    struct number *nb;
    enum operateur op;
    struct bt_node *left;
    struct bt_node *right;
}

#endif /* ! ARITHMETIC_H */
