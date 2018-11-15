#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lexer.h"
#include "linked_list.h"


struct node* build_ast(struct linked_list *tokens);
