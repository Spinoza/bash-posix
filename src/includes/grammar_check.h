#ifndef GRAMMAR_H
#define GRAMMAR_H


#include "linked_list.h"

struct nL *g_ruleif(struct nL *tok);
struct nL *g_rulecase(struct nL *tok);
struct nL *g_ruleuntil(struct nL *tok);
struct nL *g_rulewhile(struct nL *tok);
struct nL *g_rulefor(struct nL *tok);

struct nL *g_funcdec(struct nL *tok);
struct nL *g_simplecommand(struct nL *tok);
struct nL *g_compoundlist(struct nL *tok);
struct nL *g_shellcommand(struct nL *tok);
struct nL *g_command(struct nL *tok);


struct nL *g_pipeline(struct nL *tok);
int grammar_check (struct linked_list *tokens);
struct nL  *g_list(struct nL *tok);
struct nL *g_andor(struct nL *tok);


#endif /* GRAMMAR_H */
