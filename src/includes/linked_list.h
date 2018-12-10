#ifndef LINK_H
#define LINK_H

#include <stdlib.h>
#include "token.h"

/**
  *\file linked_list.h
  *\brief
  *All information about the linked_list structure,
  *including all functions allowing to handle this structure.
  */

/**
  *\struct nL
  *\brief structure of a node in the linked_list
  *
  *This structure allows to travel through the list, and gives
  *information about a token in the list.
  */
struct nL
{
    struct token *elem;/**< The token contained in the node.*/
    struct nL *next;/**< The next node in the list.*/
    struct nL *prev;/**< The previous node in the list.*/
};

/**
  *\struct linked_list
  *\brief The starting point of the list.
  *
  *This structure acts as a sentinel, and offers a gateway to the first element
  *of the list.
  */
struct linked_list
{
    size_t size;/**< The number of nodes in the list.*/
    struct nL *head;/**< A pointer to the first node in the list.*/
};


/**
  *\fn struct linked_list *init_link(void)
  *\brief initialize a linked_list of size 0 and head NULL
  *\param void
  *\return a newly created linked_list or NULL if any error occured.
  */
struct linked_list *init_link(void);

/**
  *\fn struct linked_list *add(struct linked_list *l, struct token *elem)
  *\brief takes a linked_list and a token, creates a new node with the token
  *and adds it to the list
  *\param l a linked_list to which add the token and elem the token to add
  *\return the list l with the token elem added, or NULL if any error occured.
  */
struct linked_list *add(struct linked_list *l, struct token *elem);

/**
  *\fn void free_list(struct linked_list *l)
  *\brief Free the list l.
  *\param l the list to free
  *\return void
  */
void free_list(struct linked_list *l);

/**
  *\fn struct token *token_copy(struct token *tok)
  *\brief returns a copy of the token tok.
  *\param tok the token to copy
  *\return struct token *, a copy of tok
  */
struct token *token_copy(struct token *tok);
#endif /* ! LINK_H */
