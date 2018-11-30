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
                printf("LOGICAL_AND");
                break;
        case 5:
                printf("SEMICOLON");
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
                printf("AMPERSAND");
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
                printf("REDIRECTION");
                break;
        case 23:
                printf("IONUMBER");
                break;
        case 24:
                printf("CONDITION");
                break;
        case 25:
                printf("ROOT");
                break;
        case 26:
                printf("OPEN_PAR");
                break;
        case 27:
                printf("CLOSE_PAR");
                break;
        case 28:
                printf("OPEN_BRA");
                break;
        case 29:
                printf("CLOSE_BRA");
                break;
        case 30:
                printf("TWO_SEMIC");
                break;
        case 31:
                printf("FUNCTION");
                break;
        case 32:
                printf("EXPAND_W");
                break;
    }
}
void print_list(struct linked_list *l_list)
{
    struct nL *head = l_list->head;
    for (; head; head = head->next)
    {
        print_enum(head->elem->type);
        printf("%s ",head->elem->name);
        printf(" -> ");
    }
    printf("\n");
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
                || *(string + i) == '='
                || *(string + i) == '('
                || *(string + i) == ')'
                || *(string + i) == '{'
                || *(string + i) == '}')
            return i;
        if (!*(string + i + 1) && *(string + i) == '&')
            return i;
        if (*(string + i) == '&' && *(string + i + 1) != '&')
            return i;
    }
    return -1;
}

void check_word(struct token *new, char *string)
{
    if (string[0] == '$')
    {
        new->type = EXPAND_W;
        return;
    }
    for (int i = 1; *(string + i); i++)
    {
        if (*(string + i) == '=')
            new->type = ASSIGNMENT_W;
        if (*(string + i) == '<' || *(string + i) == '>')
            new->type = IONUMBER;
    }
    new->type = WORD;
}

int check_list(struct token *new, char *string, char **list)
{
    for (int i = 0; i < LIST_LENGTH; i++)
    {
        if (!strcmp(string, list[i]))
        {
            new->type = i;
            int len = strlen(list[i]);
            if (!strcmp(list[i], "fi"))
                new->name = calloc(10, sizeof(char));
            else
                new->name = calloc(20, sizeof(char));
            new->name = memcpy(new->name, list[i], len);
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

void splitted_tokens(struct token *new, char c, char **list)
{
    char *string = calloc(2, sizeof(char));
    string[0] = c;
    check_list(new, string, list);
    free(string);
}

/*
 * returns 1 if the name has already been set i.e the string is a special token
*/

int split_tokens(struct token *new, char *string, struct linked_list *l_list,
        int index_sc, char **list)
{
    if(index_sc == 0)
    {
        splitted_tokens(new, string[index_sc], list);
        return 0;
    }
    else
    {
        struct token *to_add = token_init();
        splitted_tokens(to_add, string[index_sc], list);
        add(l_list, to_add);
    }
    string[index_sc] = '\0';
    if (check_list(new,string,list))
        return 0;
    new->name = calloc(index_sc + 1, sizeof(char));
    new->name = memcpy(new->name, string, index_sc);
    return 1;
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
        if (split_tokens(new,string,l_list,index_sc, list))
            check_word(new, string);
        read_string(next, string + index_sc + 1,list,l_list);
        return;
    }
    check_word(new, string); //Set the type of the token
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
        if (context == ENDOF
                && (iter->elem->type == FOR || iter->elem->type == CASE))
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
        if (string[0] == '#') //Currently in a comment
        {
            while (string)
            {
                if (!strcmp(string, "\n"))
                    break;
                string = strtok(NULL," ");
            }
            if(!string)
                break;
        }
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
        if (input[i][0] == '#') //Currently in a comment
        {
            while (i < argc)
            {
                if (!strcmp(input[i], "\n"))
                    break;
                i++;
            }
            if(i < argc)
                break;
        }
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
