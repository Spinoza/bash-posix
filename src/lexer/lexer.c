#include "lexer.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

#define LIST_LENGTH 42
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
                printf("IONUMBER");
                break;
        case 23:
                printf("CONDITION");
                break;
        case 24:
                printf("ROOT");
                break;
        case 25:
                printf("REDIRECTION");
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
    *(list + 31) = "";
    *(list + 32) = "";
    *(list + 33) = ">";
    *(list + 34) = "<";
    *(list + 35) = "<<";
    *(list + 36) = ">>";
    *(list + 37) = "<<-";
    *(list + 38) = ">&";
    *(list + 39) = "<&";
    *(list + 40) = ">|";
    *(list + 41) = "<>";
    return list;
}

int special_character(char c, int quoting, enum type context)
{
    switch (c)
    {
        case '#':
            return 1;
        case '\n':
            return !quoting;
        case ' ':
            return !quoting;
        case '\t': // tab character
            return !quoting;
        case '(':
            return !quoting;
        case ')':
            return !quoting;
        case '{':
            return 1;
        case '}':
            return 1;
        case ';':
            return 1;
        case '=':
            return context != WORD;
        case '&':
            return 1;
        case '|':
            return 1;
        case '>':
            return 1;
        case '<':
            return 1;
        default:
            return 0;
    }
}

static struct token *set_token(struct token *new, char *res, int cur_index,
        enum type current_type)
{
    res = realloc(res, (cur_index + 1) * sizeof(char));
    new->name = res;
    new->type = current_type;
    return new;
}

static int redirection_char(char c, char pre, int i)
{
    if (i == 1)
    {
        switch (c)
        {
            case '>':
                return 1;
            case '<':
                if(pre == '<')
                    return 1;
                break;
            case '&':
                return 1;
            case '|':
                if(pre == '>')
                    return 1;
                break;
        }
    }
    else
    {
        if(c == '-' && pre == '<' && i == 2)
        {
            return 1;
        }
    }
    return 0;
}

static struct token *set_specials(struct token *new, char *string,
        int *index, char **list)
{
    char *cmp = calloc(4, sizeof(char));
    int i = 0;
    for (; string[*index]; *index = *index + 1)
    {
        if (!cmp[0])
            cmp[0] = string[*index];
        else
        {
            if(cmp[0] == '<' || cmp[0] == '>')
            {
                if(redirection_char(string[*index], string[*index - 1], i))
                {
                    cmp[i] = string[*index];
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (cmp[0] == string[*index] && i == 1)
                {
                    cmp[1] = string[*index];
                    *index = *index + 1;
                }
                break;
            }
        }
        i++;
    }
    for (int i = 0; i < LIST_LENGTH; i++)
    {
        if (!strcmp(cmp, list[i]))
        {
            if(i >= 33)
                new->type = 25;
            else
            {
                new->type = i;
            }
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

static int check_special_words(char *string, char **list)
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

void remove_comment(char *string, int *index)
{
    if (string[*index] != '#')
        return;
    for (; string[*index] && string[*index] != '\n'; *index = *index + 1);
}
static struct token *read_characters(struct token *new, char *string,
        int *index, char **list, enum type context)
{
    if (special_character(string[*index], 0, context))
    {
        if (string[*index] != ' ')
            return set_specials(new, string, index, list);
        *index = *index + 1;
    }
    int cur_index = 0;
    int len = strlen(string);
    char *res = calloc(len + 1, sizeof(char));
    int quoting = 0;
    enum type current_type = WORD;
    for (; *(string + *index);*index = *index + 1)
    {
        if (*(string + *index) == '\'' || *(string + *index) == '\"')
        {
            quoting = !quoting;
            continue;
        }
        if (special_character(*(string + *index), quoting, context))
        {
            if (string[*index] == '#')
                remove_comment(string, index);
            if ((string[*index] == ')' && string[*index + 1] == ')')
                    || (string[*index] == '(' && string[*index + 1] == '('))
            {
                quoting = !quoting;
                res[cur_index++] = string[*index];
                *index = *index + 1;
                res[cur_index++] = string[*index];
                continue;
            }
            if (string[*index] != '=')
                break;
            current_type = ASSIGNMENT_W;
        }
        res[cur_index++] = string[*index];
    }
    if (current_type == WORD)
        current_type = check_special_words(res, list);
    return set_token(new, res, cur_index, current_type);
}

static enum type set_context(struct token *new)
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

static int is_special_word(struct token *new)
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
static void check_context(struct token *new, enum type *context)
{
    if (*context == FOR && new->type == IN)
    {
        *context = ENDOF;
        return;
    }
    if (*context == ENDOF || new->type == ENDOF)
    {
        *context = set_context(new);
        return;
    }
    if (is_special_word(new) || new->type == WORD)
        new->type = WORD;
    else
    {
        *context = ENDOF;
    }
}
struct linked_list *lexer_c(char *input)
{

    char **list = init_list();
    struct linked_list *l_list = init_link();
    int index = 0;
    enum type context = ENDOF;
    for (; input[index]; )
    {
        struct token *new = token_init();
        read_characters(new, input, &index, list, context);
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

