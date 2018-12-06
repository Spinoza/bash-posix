#ifndef ARITHMETIC_H
#define ARITHMETIC_H

enum oper
{
    NOT_AN_OP = 0,
    PLUS, // precendence 1
    MINUS, // precedence 1
    MULTIPLY, // precedence 2
    DIVIDE, // precedence 2
    POWER, // precedence 3
    TILDE, // precedence 3
    OPEN_PAR_OPER, // precedence 0
    CLOSE_PAR_OPER //precedence 4
};

struct bt_node
{
    long int nb;
    enum oper op;
};

struct arith_list
{
    int nb_nodes;
    int capacity;
    struct bt_node **list;
};
#endif /* ! ARITHMETIC_H */
