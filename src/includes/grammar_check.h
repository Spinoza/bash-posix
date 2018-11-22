#ifndef GRAMMAR_H
#define GRAMMAR_H


#include "linked_list.h"


/**
  *\file grammar_check.h
  *\brief 
  *All functions that are used to check if the list of token parsed
  *is conform to LL grammar
  */

/**
  *\fn struct nL *g_ruleif(struct nL *tok)
  *\brief Check if the list is conform to the rule if syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_ruleif(struct nL *tok);

/**
  *\fn struct nL *g_rulecase(struct nL *tok)
  *\brief Check if the list is conform to the rule case syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_rulecase(struct nL *tok);

/**
  *\fn struct nL *g_ruleuntil(struct nL *tok)
  *\brief Check if the list is conform to the rule until syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_ruleuntil(struct nL *tok);

/**
  *\fn struct nL *g_rulewhile(struct nL *tok)
  *\brief Check if the list is conform to the rule while syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_rulewhile(struct nL *tok);

/**
  *\fn struct nL *g_rulefor(struct nL *tok)
  *\brief Check if the list is conform to the rule for syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_rulefor(struct nL *tok);


/**
  *\fn struct nL *g_funcdec(struct nL *tok)
  *\brief Check if the list is conform to a function declaration syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_funcdec(struct nL *tok);

/**
  *\fn struct nL *g_simplecommand(struct nL *tok)
  *\brief Check if the list is conform to a simple command syntax (word or assignment word)
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_simplecommand(struct nL *tok);

/**
  *\fn struct nL *g_compoundlist(struct nL *tok)
  *\brief Check if the list is conform to the syntax of the body of a rule or a function
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_compoundlist(struct nL *tok);

/**
  *\fn struct nL *g_shellcommand(struct nL *tok)
  *\brief Check if the list is conform to a shell command syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_shellcommand(struct nL *tok);

/**
  *\fn struct nL *g_command(struct nL *tok)
  *\brief Check if the list is conform to a command syntax and which one
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_command(struct nL *tok);



/**
  *\fn struct nL *g_pipeline(struct nL *tok)
  *\brief Checks for pipes between commands
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_pipeline(struct nL *tok);

/**
  *\fn int grammar_check(struct linked_list *tokens)
  *\brief Checks if a whole list of tokens is conform to the LL grammar
  *\param token is the list of token to be checked
  *\return 1 if the list is conform to the grammar, 0 otherwise
  */
int grammar_check (struct linked_list *tokens);

/**
  *\fn struct nL *g_list(struct nL *tok)
  *\brief Checks if a list is conform to grammar
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL  *g_list(struct nL *tok);

/**
  *\fn struct nL *g_andor(struct nL *tok)
  *\brief Checks for logical and or logical or between commands
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_andor(struct nL *tok);



/**
  *\fn struct nL *g_elseclause(struct nL *tok)
  *\brief Checks if a list is conform to an else clause syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_elseclause(struct nL *tok);

/**
  *\fn struct nL *g_caseclause(struct nL *tok)
  *\brief Checks if a list is conform to a case clause syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_caseclause(struct nL *tok);

/**
  *\fn struct nL *g_dogroup(struct nL *tok)
  *\brief Checks if a list is conform to a do group syntax
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_dogroup(struct nL *tok);

/**
  *\fn struct nL *g_caseitem(struct nL *tok)
  *\brief Checks if a list is conform to the body of a case item 
  *\param tok is the head of the rest of the list to be checked
  *\return the last token conform to the grammar, NULL otherwise
  */
struct nL *g_caseitem(struct nL *tok);

#endif /* GRAMMAR_H */
