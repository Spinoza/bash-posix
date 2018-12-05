#ifndef ARITHMETIC_H
#define ARITHMETIC_H

enum oper
{
    NOT_AN_OP = 0,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    POWER,
    TILDE,
    OPEN_PAR_OPER,
    CLOSE_PAR_OPER
};

struct bt_node
{
    long int nb;
    enum oper op;
    struct bt_node *left;
    struct bt_node *right;
};

#endif /* ! ARITHMETIC_H */
