#ifndef STACK_H
#define STACK_H

#include "arithmetic.h"

/**
  *\file stack.h
  *\brief
  *All information about the stack structure, including the functions to
  *manage the structure.
  */
/**
  *\struct nS
  *\brief structure of a node in the stack.
  *
  *This structure is used to traverse through the stack and contains the
  *information about a bt_node, used in arithmetic expansions.
  */
struct nS
{
    struct bt_node *elem;
    struct nS *next;
};

/**
  *\struct stack
  *\brief structure of a stack.
  *
  *This structure contains all bt_nodes, used in arithmetic expansions.
  */
struct stack
{
    int size;
    struct nS *head;
};

/**
  *\fn struct stack *init_stack(void);
  *\brief initializes a stack structure.
  */
struct stack *init_stack(void);

/**
  *\fn void *peek(struct stack *s);
  *\brief looks at the top element of a stack.
  *
  *Returns the top element of stack s.
  */
void *peek(struct stack *s);

/**
  *\fn void free_stack (struct stack *s);
  *\brief function that frees a stack.
  */
void free_stack(struct stack *s);

/**
  *\fn struct stack *push(struct stack *s, void *elem);
  *\brief adds an element to the top of a stack.
  *
  *Adds element elem to the top of the stack s, and returns the new stack.
  */
struct stack *push(struct stack *s, void *elem);

/**
  *\fn void *pop(struct stack *s)
  *\brief removes the top element of stack.
  *
  *Removes the top element of the stack s and returns it.
  */
void *pop(struct stack *s);
#endif /* ! STACK_H */
