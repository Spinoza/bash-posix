#ifndef HELPER_EXEC_H
#define HELPER_EXEC_H

#include "execution_ast.h"
#include "ast.h"

/**
 *\file helper_exec.h
 *\brief
 *Functions used to manage the execution of the ast.
 */
/**
 *\fn void free_data(struct stored_data *data);
 *\brief frees a struct data.
 */
void free_data(struct stored_data *data);

/**
 *\fn struct stored_data *stored_data_init(void);
 *\brief initializes a struct stored_data and returns it.
 */
struct stored_data *stored_data_init(void);

/**
 *\fn char **to_execute(struct node *child, struct node *oper_node, struct stored_data *data);
 *\brief creates the array of strings to be executed.
 *
 *Creates an array of strings, with expansions handled.
 *child is the node to be executed, oper_node is the first operator following
 *the node child. data is where the assignments are to be stored and retrieved
 *from.
 */
char **to_execute(struct node *child, struct node *oper_node,
        struct stored_data *data);

/**
 *\fn void free_command(char **string);
 *\brief frees an array of string.
 *
 *This function is called after a node has been executed.
 */
void free_command(char **string);

/**
 *\fn int exec_command(char **string);
 *\brief executes a command.
 *
 *Executes the command given through string. String[0] is the builtin or executable
 *, the rest of the array are the parameters to be given.
 */
int exec_command(char **string);

/**
 *\fn char *set_string(char *instr, struct stored_data *data);
 *\brief returns a string after expanding it.
 *
 *Copies the string instr after expanding it to a new string and returns the
 *new string. Retrieves the assigned values from data.
 */
char *set_string(char *instr, struct stored_data *data);

/**
 *\fn int is_builtin(char **string)
 *\brief executes builtins.
 *
 *Checks if string[0] is a builtin, if it is then execute it and return the value
 *returned by the builtin. Else return -1.
 */
int is_builtin(char **string);

#endif /* ! HELPER_EXEC */
