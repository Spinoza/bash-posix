#include "lexer.h"
#include "token.h"

char **list init_list(void)
{
    char *list[11] = malloc(sizeof(char *));
    list =
    {
        "if",
        "then",
        "else",
        "fi",
        "&&",
        ";",
        "while",
        "for",
        "until",
        "do",
        "done"
    };
    return list;
}

struct queue *lexer (char *input[], int argc)
{
    for (int i = 0; i < argc; i++)
    {
        for (int j = 0; *(input[i] + j); j++)
        {
            struct token *new = malloc(sizeof(struct token));
            new->name = input[i];
            new->type = read_string(input[i]);
        }
    }
}

enum type read_string(char *string)
{

}
