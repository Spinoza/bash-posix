#ifndef ATTY_H
#define ATTY_H

#include "linked_list.h"

/**
 *\file handlePS1.h
 *\brief
 *Contains function to handle everything that happens
 *while in tty
 */

char *get_assignment(char *name);
/**
 *\fn struct nL *handletty(void)
 *\brief handles everything that happens in the prompt
 *\return a Struct nL if everything works properly, NULL otherwise
 */
struct nL *handletty(void);
#endif /* ! ATTY_H */
