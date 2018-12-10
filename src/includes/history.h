#ifndef HISTORY_H
#define HISTORY_H

#include <stdio.h>

/**
 *\file history.h
 *\brief
 *Contains function to init the history
 */


/**
 *\fn void init_interact(void)
 *\brief fills dynamic history with content of file ~/.42sh_history
 *\return Nothing
 */
void init_interact(void);

void set_history(void);
#endif /* ! HISTORY_H */
