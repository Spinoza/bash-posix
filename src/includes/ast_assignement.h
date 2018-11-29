#ifndef AST_ASSIGNEMENT_H
#define AST_ASSIGNEMENT_H

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
}

#endif /* ! ASSIGNEMENT_AST_H */
