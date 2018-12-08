#ifndef AST_H
#define AST_H

#include "linked_list.h"
#include "stddef.h"
#include "token.h"

/**
  *\file ast.h
  *\brief
  *All information about the structur of a node of the
  *ast and about the enum that gathers all the types of node.
  */

/**
  *\enum ntype
  *\brief An ast tree node type enum
  *
  *For each node in an ast, a type is associated to it.
  *This is done in order to be able to execute properly later.
  */
enum ntype
{
    A_IF, /**< Informs that what follows are if conditions and body*/
    A_BODY, /**< Every child will executed */
    A_EBODY, /**< Cf A_BODY */
    A_WHILE, /**< Informs that what follows are while conditions and body*/
    A_UNTIL, /**< Informs that what follows are until conditions and body*/
    A_CASE, /**< Informs that what follows are case conditions and body*/
    A_CONDITION, /**< Children will compared to each other*/
    A_INSTRUCT, /**< Instruction to be executed */
    A_ROOT, /**< Root af the tree*/
    A_FOR, /**< Informs that what follows are for conditions and body*/
    A_IN, /**< Node that separates comparing from comprared instructions */
    A_ELIF, /**< Cf A_IF*/
    A_PIPE, /**< Siblings must be executed with a pipe between them */
    A_FUNCTION, /**< Tree that will declare a function*/
    A_REDIR /**< Siblings must be executed with a redirection between them */
};

/**
  *\struct node
  *\brief Struct of the ast tree nodes
  *
  *This struct gathers information about the nodes created while building
  *the ast tree.
  */
struct node
{
    size_t nbchild; /**< The number of children of a node*/
    char *instr; /**< The name of the node (can be the name of the token contained)*/
    enum ntype type; /**< The type of the node*/
    enum type tokentype; /**< The type of token it contains*/
    struct node *parent; /**< The node that called the current one*/
    struct node *children; /**< The pointer to the first child*/
    struct node *next; /**< The pointer to the left sibling*/
};

/**
  *\fn struct node *build_ast(struct linked_list *tokens)
  *\brief Builds the ast tree
  *\param tokens is a linked list a tokens conform to the grammar
  *\return a struct node if everything works properly, NULL otherwise
  */
struct node* build_ast(struct linked_list *tokens);

/**
  *\fn void print_ast(struct node *ast)
  *\brief Prints the ast tree
  *\param ast is the root of the tree to print
  *\return void
  */
void print_ast(struct node *ast);

/**
  *\fn void free_node(struct node *n)
  *\brief Free the ast tree
  *\param n the root of the tree to free
  *\return void
  */
void free_node(struct node *n);

/**
  *\fn struct node *copy_node(struct node *n)
  *\brief copies a tree
  *\param struct node *n the root of the tree to copy
  *\return struct node if tree has been copied properly, NULL otherwise
  */
struct node *copy_node(struct node *n);

void free_node_copy(struct node *n);
#endif /* ! AST_H */
