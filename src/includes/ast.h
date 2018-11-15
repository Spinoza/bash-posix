#ifndef AST_H
#define AST_H

#include "linked_list.h"
#include "stddef.h"
#include "token.h"

enum ntype
{
    A_IF,
    A_BODY,
    A_EBODY,
    A_WHILE,
    A_UNTIL,
    A_CASE,
    A_CONDITION,
    A_INSTRUCT,
    A_ROOT,
    A_FOR,
    A_IN,
    A_ELIF
};

struct node
{
    size_t nbchild;
    char *instr;
    enum ntype type;
    enum type tokentype;
    struct node *children;
    struct node *next;
};

struct node* build_ast(struct linked_list *tokens);
void print_ast(struct node *ast);
#endif /* ! AST_H */
