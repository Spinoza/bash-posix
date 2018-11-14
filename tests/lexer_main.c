#include "lexer.h"
#include "token.h"

#include <stdio.h>
#include <string.h>

#define LIST_LENGTH 11

int main(void)
{
    /*
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
