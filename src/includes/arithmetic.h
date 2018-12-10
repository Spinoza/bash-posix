#ifndef ARITHMETIC_H
#define ARITHMETIC_H
/**
 *\file arithmetic.h
 *\brief
 *\All information about the arithmetic expansions.
 */

/**
 *\enum type
 *\brief an enum for all kinds of operators.
 */

enum oper
{
    NOT_AN_OP = 0, /**< Current node is not an operator*/
    PLUS, /**< Current operator is a plus.*/
    MINUS, /**< Current operator is a minus.*/
    MULTIPLY, /**< Current operation is multiplication.*/
    DIVIDE, /**< Current operation is division.*/
    BIT_XOR, /**< Current operation is bitwise XOR.*/
    BIT_AND, /**< Current operation is bitwise AND.*/
    BIT_OR, /**< Current operation is bitwise OR.*/
    OPEN_PAR_OPER, /**< Current operator is an opening parenthesis.*/
    CLOSE_PAR_OPER, /**< Current operator is a closing parenthesis.*/
    POWER, /**< Current operator is exponent*/
    AND_OPER, /**< Current operator is logical and*/
    OR_OPER, /**< Current operator is logical or*/
    WHITE_SPACE /**< Not an operator, used in get_op function */
};

/**
 *\struct bt_node
 *\brief Struct of the nodes used in arithmetic expansions
 *
 *This struct keeps information about the current token, either a number or an
 *operator.
 */
struct bt_node
{
    double nb; /**< The integer part of the node */
    enum oper op;/**< The operator part of the node */
};

/**
 *\struct arith_list
 *\brief Struct used to list struct of the nodes used in arithmetic expansions
 *
 *All elements in the list are ordered by how they were inputted.
 */
struct arith_list
{
    int nb_nodes;/**< The number of nodes */
    int capacity;/**< The maximum capacity of the arith_list */
    struct bt_node **list; /**< The list of nodes */
};

/**
 *\fn char *arith_expansion(char *string);
 *\brief Expands the arithmetic expansion given as parameter.
 *\param char *string, input to expand.
 *\return char *, string of the result of the expansion.
 */
char *arith_expansion(char *string);
#endif /* ! ARITHMETIC_H */
