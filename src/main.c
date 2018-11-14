#include "grammar.h"
#include "stdio.h"

int main(void)
{
    struct linked_list *l= init_link();

    struct token *w1 = malloc(sizeof(struct token));
    w1->type = WORD;
    w1->name = "word1";
    struct token *lAN = malloc(sizeof(struct token));
    lAN->type = LOGICAL_AND;
    lAN->name = "logical and";
    struct token *w2 = malloc(sizeof(struct token));
    w2->type = WORD;
    w2->name = "word2";
    struct token *semi = malloc(sizeof(struct token));
    semi->type = WORD;
    semi->name = "word5";
    struct token *w3 = malloc(sizeof(struct token));
    w3->type = WORD;
    w3->name = "word3";
    struct token *lOR = malloc(sizeof(struct token));
    lOR->type = LOGICAL_OR;
    lOR->name = "logical or";
    struct token *w4 = malloc(sizeof(struct token));
    w4->type = WORD;
    w4->name = "word4";
    struct token *eo = malloc(sizeof(struct token));
    eo->type = ENDOF;
    eo->name = "eof";

    add(l, w1);
    add(l, lAN);
    add(l, w2);
    add(l, semi);
    add(l, w3);
    add(l, lOR);
    add(l, w4);
    add(l, eo);

    printf("%d\n", grammar_check(l));
}
