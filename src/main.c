#include "grammar.h"
#include "stdio.h"

int main(void)
{
    struct queue *q= init_queue();

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

    enqueue(q, w1);
    enqueue(q, lAN);
    enqueue(q, w2);
    enqueue(q, semi);
    enqueue(q, w3);
    enqueue(q, lOR);
    enqueue(q, w4);
    enqueue(q, eo);

    struct queue *grammar = grammar_check(q);

    struct nQ *node = grammar->head;
    while(node)
    {
        printf("%s \n", node->elem->name);
        node = node->next;
    }
}
