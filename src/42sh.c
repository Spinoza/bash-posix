#include <stdlib.h>
#include "lexer.h"
#include "grammar_check.h"
#include "option.h"
#include "token.h"
#include "linked_list."
#include "ast.h"

int main(int argc, char *argv[])
{
    struct linked_list *tokens = lexer(argv, argc);
    int isgramm = grammar_check(tokens);
    return isgramm;
}
