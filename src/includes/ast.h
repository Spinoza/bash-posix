#ifndef AST_H
#define AST_H

#include "linked_list.h"

enum ntype
{
    IF,
    BODY,
    EBODY,
    WHILE,
    UNTIL,
    CASE,
    INSTRUCT
}

struct node
{
    size_t nbchild;
    char *instr;
    enum ntype type;
    struct nL *children;
};

struct node* build_ast(struct linked_list *tokens);
#endif /* ! AST_H */
