#ifndef LEXER_H
#define LEXER_H

#include "linked_list.h"

/**
  *\file lexer.h
  *\brief
  *Handles different way to transform arguments into
  *a list of tokens
  */


/**
  *\fn void print_list(struct linked_list *l_list)
  *\brief displays all the tokens of the list
  *\param struct linked_list *l_list list to print
  *\return void
  */
void print_list(struct linked_list *l_list);

/**
  *\fn struct linked_list *lexer(char *input[], int argc, int begin)
  *\brief creates a list of tokens from a list of arguments 
  *\param char *input[] list of args, int argc number opf args, int begin index 
  *where to start
  *\return struct linked_list if everything works, NULL otherwise
  */
struct linked_list *lexer(char *input[],int argc, int begin);

/**
  *\fn linked_list *lexer_c(char *input)
  *\brief creates a list of tokens from a string
  *\param char *input string to parser 
  *\return struct linked_list if everything works, NULL otherwise
  */
struct linked_list *lexer_c(char *input);

#endif /* ! LEXER_H */
