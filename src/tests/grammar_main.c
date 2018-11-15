#include "grammar_check.h"
#include "stdio.h"
#include "ast.h"

int main(void)
{
    struct linked_list *l= init_link();

    struct token *w1 = malloc(sizeof(struct token));
    w1->type = WHILE;
    w1->name = "while";
    struct token *lAN = malloc(sizeof(struct token));
    lAN->type = WORD;
    lAN->name = "bonsoir";
    struct token *w2 = malloc(sizeof(struct token));
    w2->type = LOGICAL_OR;
    w2->name = "or";
    struct token *semi = malloc(sizeof(struct token));
    semi->type = WORD;
    semi->name = "bonne nuit";
    struct token *w3 = malloc(sizeof(struct token));
    w3->type = SEMICOLON;
    w3->name = ";";
    struct token *lOR = malloc(sizeof(struct token));
    lOR->type = DO;
    lOR->name = "do";
    struct token *w4 = malloc(sizeof(struct token));
    w4->type = WORD;
    w4->name = "thx";
    struct token *WOR1 = malloc(sizeof(struct token));
    WOR1->type = SEMICOLON;
    WOR1->name = ";";
    struct token *WOR2 = malloc(sizeof(struct token));
    WOR2->type = IF;
    WOR2->name = "if";
    struct token *WOR3 = malloc(sizeof(struct token));
    WOR3->type = WORD;
    WOR3->name = "matin";
    struct token *WOR4 = malloc(sizeof(struct token));
    WOR4->type = SEMICOLON;
    WOR4->name = ";";
    struct token *eo = malloc(sizeof(struct token));
    eo->type = THEN;
    eo->name = "then";
    struct token *eo2 = malloc(sizeof(struct token));
    eo2->type = WORD;
    eo2->name = "but wtf";
   struct token *eo3 = malloc(sizeof(struct token));
    eo3->type = SEMICOLON;
    eo3->name = ";";
   struct token *eo4 = malloc(sizeof(struct token));
    eo4->type = ELIF;
    eo4->name = "elif";

   struct token *eo5 = malloc(sizeof(struct token));
    eo5->type = WORD;
    eo5->name = "jour";

   struct token *eo6 = malloc(sizeof(struct token));
    eo6->type = SEMICOLON;
    eo6->name = ";";

   struct token *eo7 = malloc(sizeof(struct token));
    eo7->type = THEN;
    eo7->name = "then";
   struct token *eo8 = malloc(sizeof(struct token));
    eo8->type = WORD;
    eo8->name = "but early";
   struct token *eo9 = malloc(sizeof(struct token));
    eo9->type = SEMICOLON;
    eo9->name = ";";
   struct token *eo10 = malloc(sizeof(struct token));
    eo10->type = ELSE;
    eo10->name = "else";
   struct token *eo11 = malloc(sizeof(struct token));
    eo11->type = WORD;
    eo11->name = "very much";
   struct token *eo12 = malloc(sizeof(struct token));
    eo12->type = SEMICOLON;
    eo12->name = ";";
   struct token *eo13 = malloc(sizeof(struct token));
    eo13->type = FI;
    eo13->name = "fi";
   struct token *eo15 = malloc(sizeof(struct token));
    eo15->type = DONE;
    eo15->name = "done";
   struct token *eo14 = malloc(sizeof(struct token));
    eo14->type = ENDOF;
    eo14->name = "eof";










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
 add(l, eo3);
 add(l, eo4);
 add(l, eo5);
 add(l, eo6);
 add(l, eo7);
 add(l, eo8);
 add(l, eo9);
 add(l, eo10);
 add(l, eo11);
 add(l, eo12);
 add(l, eo13);
 add(l, eo15);
 add(l, eo14);

    struct nL *head = l->head;
    while (head)
    {
        printf("%s\n", head->elem->name);
        head = head->next;
    }


    printf("%d\n", grammar_check(l));
    struct node *ast = build_ast(l);
    print_ast(ast);
    printf("AST has been printed ! well done everyone !\n");
    return 1;
}
