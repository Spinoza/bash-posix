#ifndef ARITHMETIC_H
#define ARITHMETIC_H

enum oper
{
    NOT_AN_OP = 0,
    PLUS, // precendence 1
    MINUS, // precedence 1
    MULTIPLY, // precedence 2
    DIVIDE, // precedence 2
    TILDE, // precedence 3
    BIT_XOR, // precedence 3
    BIT_AND, // precedence 3
    BIT_OR, // precedence 3
    OPEN_PAR_OPER, // precedence 0
    CLOSE_PAR_OPER, //precedence 4
    POWER, // precedence 3
    AND_OPER,
    OR_OPER,
    WHITE_SPACE //Used in get_op function
};

struct bt_node
{
    double nb;
    enum oper op;
};

struct arith_list
{
    int nb_nodes;
    int capacity;
    struct bt_node **list;
};

char *arith_expansion(char *string);
#endif /* ! ARITHMETIC_H */
