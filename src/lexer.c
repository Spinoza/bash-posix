#include "lexer.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

#define LIST_LENGTH 11
char **init_list(void)
{
    char **list = malloc(sizeof(char *) * 11);
    *list = "if";
    *(list + 1) ="then";
    *(list + 2) = "else";
    *(list + 3) = "fi";
    *(list + 4) = "&&";
    *(list + 5) = ";";
    *(list + 6) = "while";
    *(list + 7) = "for";
    *(list + 8) = "until";
    *(list + 9) = "do";
    *(list + 10) = "done";
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

void read_string(struct token *new, char *string, char **list,
        struct queue *queue)
{
    if(!string)
        return;
    for (int i = 0; i < LIST_LENGTH; i++)
    {
        if (!strcmp(string, list[i]))
        {
            new->type = i;
            return;
        }
    }
    new->type = check_word(string);
    int index_sc = check_semicolon(string);
    if(index_sc)
    {
        new->name = malloc(sizeof(char) * index_sc);
        new->name = memcpy(new->name, string, index_sc);
        new->name[index_sc-1] = '\0';
        struct token *semicolon = malloc(sizeof(struct token));
        semicolon->type = SEMICOLON;
        enqueue(queue,semicolon);
        struct token *next = malloc(sizeof(struct token));
        read_string(next, string + index_sc + 1,list,queue);
        return;
    }
    int string_len = strlen(string) + 1;
    new->name = malloc(sizeof(char) * string_len);
    memcpy(new->name, string, string_len);
}

struct queue *lexer (char *input[], int argc)
{
    char **list = init_list();
    struct queue *queue = init_queue();
    for (int i = 0; i < argc; i++)
    {
        struct token *new = malloc(sizeof(struct token));
        read_string(new, input[i], list, queue);
        enqueue(queue, new);
    }
    struct token *eof = malloc(sizeof(struct token));
    eof->type = ENDOF;
    enqueue(queue, eof);
    return queue;
}
void print_queue(struct queue *queue, size_t size)
{
    printf("printing new queue\n");
    for (size_t i = 0; i < size; i++)
    {
        struct token *tok = dequeue(queue);
        printf("%d\n",tok->type);
    }
}
int main(void)
{
    char *if_s = "if";
    //char *else_s = "else";
    char *then_s = "then";
    char *semi = ";";
    char *word = "word";
    //char *assign_w = "pad=2";
    //char *command = "cd";
    char *cond = "1";
    char *while_s = "while";
    char *do_s = "do";
    char *done_s = "done";
    //char *until = "until";
    //char *for_s = "for";
    char **input = malloc(sizeof(char *) * 8);
    input[0] = word;
    //struct queue *queue1 = lexer(input,1);
    //print_queue(queue1,2);
    input[1] = if_s;
    input[2] = word;
    input[3] = then_s;
    input[4] = word;
    input[5] = semi;
    struct queue *queue2 =lexer(input,6);
    print_queue(queue2,7);
    input[6] = while_s;
    input[7] = cond;
    input[8] = do_s;
    input[9] = word;
    input[10] = done_s;
    struct queue *queue = lexer(input, 11);
    print_queue(queue,12);

}
