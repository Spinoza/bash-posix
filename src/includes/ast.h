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
    CONDITION,
    INSTRUCT,
    ROOT
}

struct node
{
    size_t nbchild;
    char *instr;
    enum ntype type;
    struct nT *children;
};

struct node* build_ast(struct linked_list *tokens);
#endif /* ! AST_H */
