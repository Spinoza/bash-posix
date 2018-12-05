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
/*
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
        if (context == ENDOF && (iter->elem->type == FOR || iter->elem->type == CASE))
            context = FOR;
        if (context == ENDOF && iter->elem->type == WORD)
            context = WORD;
        if (type_oper(iter->elem))
            context = ENDOF;
    }
    if (context == WORD && new->type != EXPAND_W)
        new->type = WORD;
}*/

int special_character(char c)
{
    switch (c)
    {
        case ' ':
            return 1;
        case '(':
            return 1;
        case ')':
            return 1;
        case '{':
            return 1;
        case '}':
            return 1;
        case ';':
            return 1;
        case '=':
            return 1;
        case '&':
            return 1;
        case '|':
            return 1;
        default:
            return 0;
    }
}

struct token *set_token(struct token *new, char *res, int cur_index,
        enum type current_type)
{
    res = realloc(res, (cur_index + 1) * sizeof(char));
    new->name = res;
    new->type = current_type;
    return new;
}

struct token *set_specials(struct token *new, char *string,
        int *index, char **list)
{
    char *cmp = calloc(3, sizeof(char));
    for (; string[*index]; *index = *index + 1)
    {
        if (!cmp[0])
            cmp[0] = string[*index];
        else
        {
            if (cmp[0] == string[*index])
            {
                cmp[1] = string[*index];
                *index = *index + 1;
            }
            break;
        }
    }
    for (int i = 0; i < LIST_LENGTH; i++)
    {
        if (!strcmp(cmp, list[i]))
        {
            new->type = i;
            new->name = cmp;
            break;
        }
    }
    if (!strcmp(cmp, "\n"))
    {
        new->type = ENDOF;
        new->name = cmp;
    }
    return new;
}

int check_special_words(char *string, char **list)
{
    for (int i = 0; i < LIST_LENGTH; i++)
    {
        if (!strcmp(string, list[i]))
            return i;
    }
    if (!strcmp(string, "\n"))
    {
        return ENDOF;
    }
    return WORD;
}

struct token *read_characters(struct token *new, char *string,
        int *index, char **list)
{
    if (special_character(string[*index]))
    {
        if (string[*index] != ' ')
            return set_specials(new, string, index, list);
        *index = *index + 1;
    }
    int cur_index = 0;
    int len = strlen(string);
    char *res = calloc(len + 1, sizeof(char));
    enum type current_type = WORD;
    for (; *(string + *index); cur_index++, *index = *index + 1)
    {
        if (special_character(*(string + *index)))
        {
            if (string[*index] == '=')
                current_type = ASSIGNMENT_W;
            break;
        }
        res[cur_index] = string[*index];
    }
    current_type = check_special_words(res, list);
    return set_token(new, res, cur_index, current_type);
}

enum type set_context(struct token *new)
{
    switch (new->type)
    {
        case WORD:
            return WORD;
        case ASSIGNMENT_W:
            return WORD;
        case EXPAND_W:
            return WORD;
        case FOR:
            return FOR;
        case CASE:
            return FOR;
        default:
            return ENDOF;
    }
}

int is_special_word(struct token *new)
{
    switch (new->type)
    {
        case IF:
            return 1;
        case THEN:
            return 1;
        case ELSE:
            return 1;
        case FI:
            return 1;
        case WHILE:
            return 1;
        case FOR:
            return 1;
        case IN:
            return 1;
        case UNTIL:
            return 1;
        case CASE:
            return 1;
        case DO:
            return 1;
        case DONE:
            return 1;
        case ESAC:
            return 1;
        case ELIF:
            return 1;
        case ASSIGNMENT_W:
            return 1;
        default:
            return 0;
    }
}
void check_context(struct token *new, enum type *context)
{
    if (*context == FOR && new->type == IN)
    {
        *context = ENDOF;
        return;
    }
    if (*context == ENDOF || (new->type == SEMICOLON || new->type == ENDOF))
    {
        *context = set_context(new);
        return;
    }
    if (is_special_word(new))
        new->type = WORD;
}
struct linked_list *tmp_lexer_c(char *input)
{

    char **list = init_list();
    struct linked_list *l_list = init_link();
    int index = 0;
    enum type context = ENDOF;
    for (; input[index]; )
    {
        struct token *new = token_init();
        read_characters(new, input, &index, list);
        if (!new->name || !strcmp(new->name, ""))
        {
            free(new->name);
            free(new);
            new = NULL;
            continue;
        }
        check_context(new, &context);
        add(l_list, new);
    }
    struct token *eof = malloc(sizeof(struct token));
    eof->type = ENDOF;
    eof->name = calloc(6, sizeof(char));
    memcpy(eof->name, "ENDOF", 6);
    add(l_list, eof);
    free(list);
    return l_list;
}

struct linked_list *lexer_c(char *input)
{
    if (1)
        return tmp_lexer_c(input);
    /*char **list = init_list();
    struct linked_list *l_list = init_link();
    char *string = NULL;
    if (input)
        string = strtok(input, " ");
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
            if (!string)
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
    return l_list;*/
}

struct linked_list *lexer (char *input[], int argc, int begin)
{
    input = input;
    argc = argc;
    begin = begin;
    if (1)
        return NULL;
    /*
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
            if (i < argc)
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
*/}
