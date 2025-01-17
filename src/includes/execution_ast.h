#ifndef EXECUTION_AST_H
#define EXECUTION_AST_H

#include "ast.h"
#include "ast_assignement.h"

/**
 *\file execution_ast.h
 *\brief
 *All information needed in order to execute the AST created by ast.h,
 *including the particular structures and enumerations.
 */

/**
 *\enum operators
 *\brief An enumeration storing different type of operators.
 *
 *For each possible operator, a field in this enumeration is given, allowing
 *to keep track of which condition we are computing.
 */
enum operators
{
    FALSE = 0, /**< The FALSE statement, saying a condition isn't matched.*/
    INFERIOR = 2, /**< Informs that the symbol is an inferior operator. */
    SUPERIOR, /**< Informs that the symbol is a superior operator.*/
    INFERIOR_EQ,/**< Informs that the symbol is an inferior or equal operator.*/
    SUPERIOR_EQ,/**< Informs that the symbol is a superior or equel operator.*/
    EQUAL,/**< Informs that the symbol is an equality operator.*/
    DIFFERENT,/**< Informs that the symbol is a different operator.*/
    S_EQUAL,/**< Informs that the condition is matched.*/
    S_N_EQUAL/**< Informs that the condition is not matched.*/
};

/**
 *\struct tab_a
 *\brief structure aiming to store every assignment word encountered.
 *
 *This struct is basically a matrix containing each assignment word encountered,
 *stored as an assignment structure.
 */

struct function
{
    char *name;/**< The name by wich the function is called.*/
    struct node *function_start;/**< The first node of the AST representing the func.*/
};

/**
 *\struct f_tab
 *\brief structure aiming to keep track of every function encountered.
 *
 *This structure is basically a matrix containing every function structure created in
 *execution.
 */

struct f_tab
{
    size_t nb; /**< The number of functions in the matrix.*/
    size_t capacity;/**< The maximum capacity of the matrix, initialized at 10*/
    struct function **f;/**< The matrix containing every function structure.*/
};

/**
 *\struct stored_data
 *\brief structure aiming to keep track of every type of data that need to be
 *stored.
 *
 *This structure contains all the assignments, the aliases and the exports,
 *the builtins as well as the functions and the current parameters if the
 *context is a function. brk is a variable used for the break builtin.
 */
struct stored_data
{
    char **param;/**< The list of parameters in the current function*/
    int nbparam;/**< The number of parameters in the current function*/
    int nbparent;/**< The number of nested loop we are in*/
    int brk;/**< Number used to know if we have to break*/
    struct node **parent_list;/**< List of parents, above the current node*/
    struct f_tab *f_tab;/**< List of functions*/
    struct assignment **var_tab;/**< List of variable assignment*/
    struct assignment **export_tab;/**< List of exported assignment*/
    struct assignment **alias_tab;/**< List of aliases in a tab*/
    struct builtins *builtins;/**< List of builtins*/
};

/**
 *\fn int execution_ast(struct node *n, struct f_tab **f_tab)
 *\brief starts the execution of the tree, from node *n.
 *\param n is the root of the tree to execute,
 *f_tab is the matrix of functions encountered.
 *\return an int representing the return value of the exeuction.
 */
char *get_assign_var(char *name, struct assignment **alias_tab);
char *get_assign(char *name, struct stored_data *data);
int execution_ast(struct node *n, struct stored_data *data);
struct node *continue_execution(struct stored_data *data);
void break_execution(struct stored_data *data, int break_nb);

/**
 *\fn int traversal_ast(struct node *n, int *res, struct f_tab **f_tab)
 *\brief does a traversal of the tree starting at node n, keeping track
 *of the executions's return value.
 *\param n is the root of the tree to go through,
 *res is the return value, f_tab is the matrix of functions encountered.
 *\return an int representing the return value of the exeuction.
 */
int traversal_ast(struct node *n, int *res, struct stored_data *data);
#endif /* ! EXECUTION_AST_H */
