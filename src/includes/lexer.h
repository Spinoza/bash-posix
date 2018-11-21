#ifndef LEXER_H
#define LEXER_H

#include "linked_list.h"

void print_list(struct linked_list *l_list);
struct linked_list *lexer(char *input[],int argc, int begin);
struct linked_list *lexer_c(char *input);

#endif /* ! LEXER_H */
