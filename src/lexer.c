#include "lexer.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

#define LIST_LENGTH 11


struct token *token_init(void)
{
    struct token *new = malloc(sizeof(struct token));
    new->type = -1;
    new->name = NULL;
    return new;
}
void read_string(struct token *new, char *string, char **list,
        struct linked_list *l_list);

char **init_list(void)
{
    char **list = malloc(sizeof(char *) * 12);
    *list = "if";
    *(list + 1) ="then";
    *(list + 2) = "else";
    *(list + 3) = "fi";
    *(list + 4) = "&&";
    *(list + 5) = ";";
    *(list + 6) = "while";
    *(list + 7) = "for";
    *(list + 8) = "until";
    *(list + 9) = "case";
    *(list + 10) = "do";
    *(list + 11) = "done";
    return list;
}
int check_semicolon(char *string)
{
    for (int i = 0; *(string + i); i++)
    {
        if(*(string + i) == ';')
            return i;
    }
    return 0;
}
enum type check_word(char *string)
{
    for (int i = 1; *(string + i); i++)
    {
        if(*(string + i) == '=')
            return ASSIGNMENT_W;
    }
    return WORD;
}

int check_list(struct token *new, char *string, char **list)
{
    for (int i = 0; i < LIST_LENGTH; i++)
    {
        if (!strcmp(string, list[i]))
        {
            new->type = i;
            new->name = string;
            return 1;
        }
    }
    if(!strcmp(string, "\n"))
    {
        new->type = ENDOF;
        return 1;
    }
    return 0;
}

void split_semicolon(struct token *new, char *string,
        struct linked_list *l_list, int index_sc)
{
    new->name = malloc(sizeof(char) * index_sc);
    new->name = memcpy(new->name, string, index_sc);
    new->name[index_sc-1] = '\0';
    struct token *semicolon = token_init();
    semicolon->type = SEMICOLON;
    add(l_list,new);
    return;
}

void read_string(struct token *new, char *string, char **list,
        struct linked_list *l_list)
{
    if(!string || !strcmp(string,""))
    {
        free(new);
        return;
    }
    add(l_list,new);
    if(check_list(new,string,list))
        return;
    new->type = check_word(string);
    int index_sc = check_semicolon(string);
    if(index_sc)
    {
        split_semicolon(new,string,l_list,index_sc);
        struct token *next = token_init();
        read_string(next, string + index_sc + 1,list,l_list);
    }
    int string_len = strlen(string) + 1;
    new->name = malloc(sizeof(char) * string_len);
    memcpy(new->name, string, string_len);
}

struct linked_list *lexer (char *input[], int argc)
{
    char **list = init_list();
    struct linked_list *l_list = init_link();
    for (int i = 0; i < argc; i++)
    {
        struct token *new = token_init();
        read_string(new, input[i], list, l_list);
    }
    struct token *eof = malloc(sizeof(struct token));
    eof->type = ENDOF;
    add(l_list,eof);
    free(list);
    return l_list;
}
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
                printf("PIPE");
                break;
        case 16:
                printf("LOGICAL_OR");
                break;
        case 17:
                printf("AND");
                break;
    }
}
void print_list(struct linked_list *l_list)
{
    printf("printing new list");
    struct nL *head = l_list->head;
    for (; head; head = head->next)
    {
        print_enum(head->elem->type);
        printf("\n");
    }
}

int main(void)
{/*
    char *if_s = "if";
    char *else_s = "else";
    char *then_s = "then";
    char *semi = ";";
    char *word = "word";
    //char *assign_w = "pad=2";
    char *command = "cd";
    char *cond = "1";
    char *while_s = "while";
    char *do_s = "do";
    char *done_s = "done";
    //char *until = "until";
    //char *for_s = "for";
    char **input = malloc(sizeof(char *) * 11);
    int i = 0;
    input[i++] = word;
    input[i++] = if_s;
    input[i++] = word;
    input[i++] = then_s;
    input[i++] = word;
    input[i++] = else_s;
    input[i++] = command;
    input[i++] = semi;
    input[i++] = while_s;
    input[i++] = cond;
    input[i++] = do_s;
    input[i++] = word;
    input[i++] = done_s;
    struct linked_list *l_list = lexer(input, 11);
    print_list(l_list,12);
*/
    char **input = malloc(sizeof(char *) *11);
    char *assign_w = "PAD=2;";
    char *new_line = "\n";
    int i = 0;
    input[i++] = assign_w;
    input[i++] = assign_w;
    input[i++] = assign_w;
    input[i++] = new_line;
    input[i++] = assign_w;
    input[i++] = new_line;
    struct linked_list *l_list = lexer(input, i);
    print_list(l_list);
    free(input);
}
