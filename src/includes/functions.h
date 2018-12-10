#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "ast.h"
#include "execution_ast.h"

/**
  *\file functions.h
  *\brief
  *All information about the handling of functions.
  */
/**
  *\fn struct function *is_a_function(char *string, struct f_tab *f_tab);
  *\brief checks if the string corresponds to a function.
  *
  *Traverses the table f_tab to find a function with name == 'string'.
  *If found, return the node corresponding to this name. Else return null.
  */
struct function *is_a_function(char *string, struct f_tab *f_tab);

/**
  *\fn void function_store(struct node *n, struct stored_data *data);
  *\brief stores a function in memory.
  *
  *Adds the function corresponding to the node n, to the table f_tab inside data.
  */
void function_store(struct node *n, struct stored_data *data);

/**
  *\fn struct node *func_execution(struct function *f, struct node *oper_node,
                            int *res, struct node *n);
  *\brief executes a function.
  *
  *Executes the function f, called inside the node n. Takes everything from the node
  *n to the node oper_node as argument. Stores the result of the call to the function
  *inside res.
  *Returns the next node to be executed.
  */
struct node *func_execution(struct function *f, struct node *oper_node,
        int *res, struct node *n);
#endif
