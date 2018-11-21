#include "lexer.h"
#include "token.h"


#include <stdio.h>
#include <string.h>

#define LIST_LENGTH 11


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: lexer : takes at least one argument\n");
        return 1;
    }
    struct linked_list *l_list = lexer(argv,argc, 1);
    print_list(l_list);
    free_list(l_list);
}
