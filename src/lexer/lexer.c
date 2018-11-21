#include "lexer.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

#define LIST_LENGTH 31
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
    *(list + 12) = ""; // WORD
    *(list + 13) = ""; // ASSIGNMENT_W
    *(list + 14) = "ENDOF";
    *(list + 15) = "|";
    *(list + 16) = "||";
    *(list + 17) = "&";
    *(list + 18) = "in";
    *(list + 19) = "esac";
    *(list + 20) = "elif";
    *(list + 21) = ""; // HEREDOC
    *(list + 22) = "";
    *(list + 23) = "";
    *(list + 24) = "";
    *(list + 25) = "";
    *(list + 26) = "(";
    *(list + 27) = ")";
    *(list + 28) = "{";
    *(list + 29) = "}";
    *(list + 30) = ";;";

    return list;
}
int check_specials(char *string) //checks for & and ;
{
    for (int i = 0; *(string + i); i++)
    {
        if (*(string + i) == ';'
                || *(string + i) == '('
                || *(string + i) == ')'
                || *(string + i) == '{'
                || *(string + i) == '}')
            return i;
        if(!*(string + i + 1) && *(string + i) == '&')
            return i;
        if(*(string + i) == '&' && *(string + i + 1) != '&')
            return i;
    }
    return -1;
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
    char *string = list[index];
    switch (index)
    {
        case 4:
            string = "logical_and";
            break;
        case 5:
            string = "semicolon";
            break;
        case 15:
            string = "pipe";
            break;
        case 16:
            string = "logical_or";
            break;
        case 17:
            string = "and";
            break;
        case 26:
            string = "open_par";
            break;
        case 27:
            string = "close_par";
            break;
        case 28:
            string = "open_bra";
            break;
        case 29:
            string = "close_bra";
            break;
        case 30:
            string = "two_semic";
            break;
        default:
            string = list[index];
            break;
    }
    int len = strlen(string);
    new->name = calloc(20, sizeof(char));
    new->name = memcpy(new->name, string, len);
}

int check_list(struct token *new, char *string, char **list)
{
    for (int i = 0; i < LIST_LENGTH; i++)
    {
        if (!strcmp(string, list[i]))
        {
            new->type = i;
            set_name(new, list, i);
            return 1;
        }
    }
    if (!strcmp(string, "\n"))
    {
        new->type = ENDOF;
        new->name = calloc(20, sizeof(char));
        memcpy(new->name, "\n", 2);
        return 1;
    }
    return 0;
}
void split_ampersand(struct token *new, char *string,
        struct linked_list *l_list, int index_sc)
{
    new->name = calloc(index_sc + 1, sizeof(char));
    new->name = memcpy(new->name, string, index_sc);
    struct token *ampersand = token_init();
    ampersand->type = AND;
    ampersand->name = calloc(5, sizeof(char));
    memcpy(ampersand->name, "and", 4);
    add(l_list,ampersand);
    return;
}

void split_semicolon(struct token *new, char *string,
        struct linked_list *l_list, int index_sc)
{
    new->name = calloc(index_sc + 1, sizeof(char));
    new->name = memcpy(new->name, string, index_sc);
    struct token *semicolon = token_init();
    semicolon->type = SEMICOLON;
    semicolon->name = calloc(20, sizeof(char));
    memcpy(semicolon->name, "semicolon", 10);
    add(l_list,semicolon);
    return;
}

void split_parenthesis(struct token *new, char *string,
        struct linked_list *l_list, int index_sc)
{
    new->name = calloc(index_sc + 1, sizeof(char));
    new->name = memcpy(new->name, string, index_sc);
    struct token *parenthesis = token_init();
    parenthesis->name = calloc(2, sizeof(char));
    parenthesis->name[0] = string[index_sc];
    if (parenthesis->name[0] == '(')
        parenthesis->type = OPEN_PAR;
    else
        parenthesis->type = CLOSE_PAR;
    add(l_list,parenthesis);
    return;
}

void split_bracket(struct token *new, char *string,
        struct linked_list *l_list, int index_sc)
{
    new->name = calloc(index_sc + 1, sizeof(char));
    new->name = memcpy(new->name, string, index_sc);
    struct token *bracket = token_init();
    bracket->name = calloc(2, sizeof(char));
    bracket->name[0] = string[index_sc];
    if (bracket->name[0] == '{')
        bracket->type = OPEN_BRA;
    else
        bracket->type = CLOSE_BRA;
    add(l_list, bracket);
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
        case '(':
            split_parenthesis(new,string,l_list, index_sc);
            break;
        case ')':
            split_parenthesis(new,string,l_list, index_sc);
            break;
        case '{':
            split_bracket(new,string,l_list, index_sc);
            break;
        case '}':
            split_bracket(new,string,l_list, index_sc);
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
    int index_sc = check_specials(string);
    if (index_sc != -1)
    {
        struct token *next = token_init();
        if (index_sc == 0)
        {
            new->name = calloc(2, sizeof(char));
            new->name[0] = string[index_sc];
        }
        else
            split_tokens(new,string,l_list,index_sc);
        read_string(next, string + index_sc + 1,list,l_list);
        if (check_list(new,new->name,list))
            return;
        new->type = check_word(string);
        return;
    }
    new->type = check_word(string);
    int string_len = strlen(string) + 1;
    new->name = calloc(sizeof(char), string_len);
    memcpy(new->name, string, string_len);
}

int type_oper(struct token *new)
{
    switch (new->type)
    {
        case LOGICAL_AND:
            return 1;
        case SEMICOLON:
            return 1;
        case PIPE:
            return 1;
        case LOGICAL_OR:
            return 1;
        case AND:
            return 1;
        case OPEN_PAR:
            return 1;
        case CLOSE_PAR:
            return 1;
        case OPEN_BRA:
            return 1;
        case CLOSE_BRA:
            return 1;
        case TWO_SEMIC:
            return 1;
        default :
            return 0;
    }
}
void check_context(struct linked_list *l_list, struct token *new)
{
    if (type_oper(new))
        return;
    struct nL *iter = l_list->head;
    enum type context = ENDOF;
    for (; iter && iter->elem != new; iter = iter->next)
    {
        if (context == FOR && iter->elem->type == DO)
            context = ENDOF;
        if (context == ENDOF && iter->elem->type == FOR)
            context = FOR;
        if (context == ENDOF && iter->elem->type == WORD)
            context = WORD;
        if (type_oper(iter->elem))
            context = ENDOF;
    }
    if (context == WORD)
        new->type = WORD;
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
        check_context(l_list,new);
        string = strtok(NULL, " ");
    }

    struct token *eof = malloc(sizeof(struct token));
    eof->type = ENDOF;
    eof->name = calloc(6, sizeof(char));
    memcpy(eof->name, "ENDOF", 6);
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
        read_string(new, input[i], list, l_list);
        check_context(l_list,new);//, redir_list);
    }
    struct token *eof = malloc(sizeof(struct token));
    eof->type = ENDOF;
    eof->name = calloc(6, sizeof(char));
    memcpy(eof->name, "ENDOF", 6);
    add(l_list,eof);
    free(list);
    return l_list;
}
