#include "lexer.h"
#include "token.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define LIST_LENGTH 21
#define REDIR_LENGTH 9

static struct token *token_init(void)
{
    struct token *new = malloc(sizeof(struct token));
    new->type = -1;
    new->name = NULL;
    return new;
}
static void read_string(struct token *new, char *string, char **list,
        struct linked_list *l_list);
/*
static char **init_redir_list(void)
{
    char **list = malloc(sizeof(char *) * REDIR_LENGTH);
    *list = "<";
    *(list + 1) =">";
    *(list + 2) = "<<";
    *(list + 3) = ">>";
    *(list + 4) = "<<-";
    *(list + 5) = ">&";
    *(list + 6) = "<&";
    *(list + 7) = ">|";
    *(list + 8) = "<>";

    return list;
}*/
static char **init_list(void)
{
    char **list = malloc(sizeof(char *) * LIST_LENGTH);
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
    *(list + 12) = "";
    *(list + 13) = "";
    *(list + 14) = "ENDOF";
    *(list + 15) = "|";
    *(list + 16) = "||";
    *(list + 17) = "&";
    *(list + 18) = "in";
    *(list + 19) = "esac";
    *(list + 20) = "elif";

    return list;
}
int check_specials(char *string) //checks for & and ;
{
    for (int i = 0; *(string + i); i++)
    {
        if (*(string + i) == ';' || *(string + i) == '&')
            return i;
    }
    return 0;
}
enum type check_word(char *string)
{
    for (int i = 1; *(string + i); i++)
    {
        if (*(string + i) == '=')
            return ASSIGNMENT_W;
        if (*(string + i) == '<' || *(string + i) == '>')
            return IONUMBER;
    }
    return WORD;
}
/*
enum type check_ionumber(char *string)
{
    int b = 0;
    int i = 0;
    while (*(string + i) >= '0' && *(string +i) <= '9')
        i++;
    if (*(string + i) == '<' || *(string + i) == '>')
    {

    }
    for (int i = 0; *(string + i); i++)
    {
        if (isanumber(*(string + i)))
            b++;
        else
        {
            if (b)
            {
                if (isaredirection(*(string + i)))
                    return IONUMBER;
            }
        }
    }
    return WORD;
}

int pos_redirection(char *string)
{
    for (int i = 0; *(string + i); i++)
    {
        if (*(string + i) == '<' || *(string + i) == '>')
            return i;
    }
    return 0;
}*/
/*
void split_redirection(struct token *new, char *string,
        struct linked_list *l_list, int index)
{
    int len = strlen(string);
    new->name = malloc(sizeof(char)
    return;
}*/

void set_name(struct token *new, char **list, int index)
{
    switch (index)
    {
        case 4:
            new->name = "logical_and";
            break;
        case 5:
            new->name = "semicolon";
            break;
        case 15:
            new->name = "pipe";
            break;
        case 16:
            new->name = "logical_or";
            break;
        case 17:
            new->name = "and";
            break;
        default:
            new->name = list[index];
            break;
    }
}

int check_list(struct token *new, char *string, char **list)
{
    for (int i = 0; i < LIST_LENGTH; i++)
    {
        if (!strcmp(string, list[i]))
        {
            new->type = i;
            set_name(new,list,i);
            return 1;
        }
    }
    if (!strcmp(string, "\n"))
    {
        new->type = ENDOF;
        new->name = "\n";
        return 1;
    }
    return 0;
}
void split_ampersand(struct token *new, char *string,
        struct linked_list *l_list, int index_sc)
{
    new->name = malloc(sizeof(char) * index_sc);
    new->name = memcpy(new->name, string, index_sc);
    new->name[index_sc] = '\0';
    struct token *ampersand = token_init();
    ampersand->type = AND;
    ampersand->name = "&";
    add(l_list,ampersand);
    return;
}

void split_semicolon(struct token *new, char *string,
        struct linked_list *l_list, int index_sc)
{
    new->name = malloc(sizeof(char) * index_sc);
    new->name = memcpy(new->name, string, index_sc);
    new->name[index_sc] = '\0';
    struct token *semicolon = token_init();
    semicolon->type = SEMICOLON;
    semicolon->name = "semicolon";
    add(l_list,semicolon);
    return;
}

void split_tokens(struct token *new, char *string, struct linked_list *l_list,
        int index_sc)
{
    switch (string[index_sc])
    {
        case ';':
            split_semicolon(new,string,l_list,index_sc);
            break;
        case '&':
            split_ampersand(new,string,l_list,index_sc);
            break;
    }
}
void read_string(struct token *new, char *string, char **list,
        struct linked_list *l_list)
{
    if (!string || !strcmp(string,""))
    {
        free(new);
        return;
    }
    add(l_list,new);
    if (check_list(new,string,list))
        return;
    /*if (check_ionumber(string))
    {}*/
    new->type = check_word(string);
    /*if (new->type == IONUMBER)
    {
        int index = pos_redirection(string);
        split_redirection(new,string,l_list,index);
        struct token *n = token_init();
        read_string(n, string + index, list, l_list);
    }*/
    int index_sc = check_specials(string);
    if (index_sc)
    {
        split_tokens(new,string,l_list,index_sc);
        struct token *next = token_init();
        read_string(next, string + index_sc + 1,list,l_list);
        return;
    }
    int string_len = strlen(string) + 1;
    new->name = malloc(sizeof(char) * string_len);
    memcpy(new->name, string, string_len);
}

struct linked_list *lexer_c(char *input)
{
    char **list = init_list();
    struct linked_list *l_list = init_link();
    char *string = strtok(input, " ");
    while (string)
    {
        struct token *new = token_init();
        read_string(new, string, list, l_list);
        string = strtok(NULL, " ");
    }

    struct token *eof = malloc(sizeof(struct token));
    eof->type = ENDOF;
    eof->name = "ENDOF";
    add(l_list, eof);
    free(list);
    return l_list;
}

struct linked_list *lexer (char *input[], int argc, int begin)
{
    char **list = init_list();
    //char **redir_list = init_redir_list();
    struct linked_list *l_list = init_link();
    for (int i = begin; i < argc; i++)
    {
        struct token *new = token_init();
        read_string(new, input[i], list, l_list);//, redir_list);
    }
    struct token *eof = malloc(sizeof(struct token));
    eof->type = ENDOF;
    eof->name = "ENDOF";
    add(l_list,eof);
    free(list);
    return l_list;
}
