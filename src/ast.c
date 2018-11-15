#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "linked_list.h"

static struct node* init_node(void)
{
    struct node* node = malloc (sizeof(struct node));
    if (!node)
        return NULL;

    node->nbchild = 0;
    node->instr = NULL;
    node->type = ROOT;
    node->children = NULL;

    return node;
}

struct node* build_ast(struct linked_list *tokens);
