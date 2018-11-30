#ifndef AST_ASSIGNEMENT_H
#define AST_ASSIGNEMENT_H
#define HASH_TAB_SIZE 20
#define TETA 0.5987526325
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
    struct assignment *next;/**< The next assignement*/
};

void free_assignments(struct assignment **tab);

struct assignment **init_assignment(void);

void add_assignment(char *string, struct assignment **a_tab);

char *get_assign(char *name, struct assignment **a_tab);
#endif /* ! ASSIGNEMENT_AST_H */
