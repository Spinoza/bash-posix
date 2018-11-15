#include "grammar_check.h"
#include "stdio.h"

int main(void)
{
    struct linked_list *l= init_link();

    struct token *w1 = malloc(sizeof(struct token));
    w1->type = IF;
    w1->name = "if";
    struct token *lAN = malloc(sizeof(struct token));
    lAN->type = WORD;
    lAN->name = "PAD";
    struct token *w2 = malloc(sizeof(struct token));
    w2->type = LOGICAL_AND;
    w2->name = "&&";
    struct token *semi = malloc(sizeof(struct token));
    semi->type = WORD;
    semi->name = "LS";
    struct token *w3 = malloc(sizeof(struct token));
    w3->type = SEMICOLON;
    w3->name = ";";
    struct token *lOR = malloc(sizeof(struct token));
    lOR->type = THEN;
    lOR->name = "then";
    struct token *w4 = malloc(sizeof(struct token));
    w4->type = WORD;
    w4->name = "cd";
    struct token *WOR1 = malloc(sizeof(struct token));
    WOR1->type = SEMICOLON;
    WOR1->name = ";";
    struct token *WOR2 = malloc(sizeof(struct token));
    WOR2->type = ELSE;
    WOR2->name = "else";
    struct token *WOR3 = malloc(sizeof(struct token));
    WOR3->type = WORD;
    WOR3->name = "alias";
    struct token *WOR4 = malloc(sizeof(struct token));
    WOR4->type = SEMICOLON;
    WOR4->name = ";";
    struct token *eo = malloc(sizeof(struct token));
    eo->type = FI;
    eo->name = "fi";
    struct token *eo2 = malloc(sizeof(struct token));
    eo2->type = ENDOF;
    eo2->name = "eof";

    add(l, w1);
    add(l, lAN);
    add(l, w2);
    add(l, semi);
    add(l, w3);
    add(l, lOR);
    add(l, w4);
    add(l, WOR1);
    add(l, WOR2);
    add(l, WOR3);
    add(l, WOR4);
    add(l, eo);
    add(l, eo2);

    struct nL *head = l->head;
    while(head)
    {
        printf("%s\n", head->elem->name);
        head = head->next;
    }


    printf("%d\n", grammar_check(l));
}
