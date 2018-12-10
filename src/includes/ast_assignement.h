#ifndef AST_ASSIGNEMENT_H
#define AST_ASSIGNEMENT_H
#define HASH_TAB_SIZE 20
#define TETA 0.5987526325

/**
 *\file ast_assignment.h
 *\brief
 *All information needed to assign variables and store them.
 */

/*
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
    struct assignment *next;/**< The next assignment*/
};
/**
 *\fn void free_assignments(struct assignment **tab);
 *\brief frees all assignments in the table tab.
 */
void free_assignments(struct assignment **tab);

/**
 *\fn struct assignment init_assignment(void);
 *\brief initializes a struct assignment and returns it.
 */
struct assignment **init_assignment(void);

/**
 *\fn void add_assignment_split(char *name, char *value, struct assignment **tab);
 *\brief function that adds a variable to an assignment table.
 *
 *Adds variable 'name' to assignment 'a_tab' with value 'value'.
 */
void add_assignment_split(char *name, char *value, struct assignment **a_tab);

/**
 *\fn void add_assignment(char *name, struct assignment **tab);
 *\brief function that adds a variable to an assignment table.
 *
 *This function is called with a string that contains both the name and value
 *to assign, separated by a '='. It splits this string and then adds the splitted
 *strings to the table a_tab.
 */

void add_assignment(char *string, struct assignment **a_tab);

/**
 *\fn char *inttochar(int nb);
 *\brief Basic itoa function.
 *
 *nb is the number to turn into a string. Returns nb converted to string.
 */
char *inttochar(int nb);

/**
 *\fn int *hash_function(char *string);
 *\brief Basic hash function.
 *
 *string is the string to hash, returns an int which will be the position of
 *the string in the hash_map.
 */
int hash_function(char *string);
#endif /* ! AST_ASSIGNEMENT_H */
