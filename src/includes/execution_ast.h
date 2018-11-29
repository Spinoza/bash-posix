#include "ast.h"

/**
  *\file execution_ast.h
  *\brief
  *All information needed in order to execute the AST created by ast.h,
  *including the particular structures and enumerations.
  */


/**
  *\struct assignment
  *\brief structure aiming to help handling assignment word.
  *
  *This struct has to represent an assignment word, 
  *by storing its name and value.
  */
struct assignment
{
    char *name; /**< The name of the variable*/
    char *value;/**< The value of the variable*/
};

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
struct tab_a
{
    size_t nb;/**< Is the size of the table.*/
    size_t capacity;/**< Is the maximum capacity of the table.*/
    struct assignment **assignment; /*< A table containing every assignment structure.*/
    //to implemen0t with hashmap
};

/**
  *\struct function
  *\brief structure aiming to represent a function.
  *
  *This structure is storing a function as a name and an AST for its execution.
  */
struct function
{
    char *name;/**< The name by witch the function si called.*/
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
  *\fn int execution_ast(struct node *n, struct f_tab **f_tab)
  *\brief starts the execution of the tree, from node *n.
  *\param n is the root of the tree to execute, 
  *f_tab is the matrix of functions encountered.
  *\return an int representing the return value of the exeuction.
  */
int execution_ast(struct node *n, struct f_tab **f_tab);

/**
  *\fn int traversal_ast(struct node *n, int *res, struct f_tab **f_tab)
  *\brief does a traversal of the tree starting at node n, keeping track
  *of the executions's return value.
  *\param n is the root of the tree to go through,
  *res is the return value, f_tab is the matrix of functions encountered.
  *\return an int representing the return value of the exeuction.
  */
int traversal_ast(struct node *n, int *res, struct f_tab **f_tab);
