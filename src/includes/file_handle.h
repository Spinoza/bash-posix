#ifndef FILE_H
#define FILE_H

#include "linked_list.h"

int is_file(char *path);
struct linked_list *read_fil(char *path);
void fuse_lists(struct linked_list *ll, char *string);

#endif /* ! FILE_H */
