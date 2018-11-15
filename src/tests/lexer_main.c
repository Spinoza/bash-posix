#include "lexer.h"
#include "token.h"


#include <stdio.h>
#include <string.h>

#define LIST_LENGTH 11

void print_enum(enum type type)
{
    switch (type)
    {
        case 0:
                printf("IF");
                break;
        case 1:
                printf("THEN");
                break;
        case 2:
                printf("ELSE");
                break;
        case 3:
                printf("FI");
                break;
        case 4:
                printf("&&");
                break;
        case 5:
                printf(";");
                break;
        case 6:
                printf("WHILE");
                break;
        case 7:
                printf("FOR");
                break;
        case 8:
                printf("UNTIL");
                break;
        case 9:
                printf("CASE");
                break;
        case 10:
                printf("DO");
                break;
        case 11:
                printf("DONE");
                break;
        case 12:
                printf("WORD");
                break;
        case 13:
                printf("ASSIGNMENT_W");
                break;
        case 14:
                printf("ENDOF");
                break;
        case 15:
                printf("|");
                break;
        case 16:
                printf("||");
                break;
        case 17:
                printf("&");
                break;
        case 18:
                printf("IN");
                break;
        case 19:
                printf("ESAC");
                break;
        case 20:
                printf("ELIF");
                break;
        case 21:
                printf("HEREDOC");
                break;
        case 22:
                printf("IONUMBER");
                break;
        case 23:
                printf("CONDITION");
                break;
    }
}
void print_list(struct linked_list *l_list)
{
    printf("\nPrinting new token list\n");
    struct nL *head = l_list->head;
    for (; head; head = head->next)
    {
        print_enum(head->elem->type);
        printf(" -> ");
    }
    printf("\n");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: lexer : takes at least one argument\n");
        return 1;
    }
    struct linked_list *l_list = lexer(argv,argc);
    print_list(l_list);
    free_list(l_list);
}
