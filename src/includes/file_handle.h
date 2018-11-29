#ifndef FILE_H
#define FILE_H

#include "linked_list.h"

/**
  *\file filehandle.h
  *\brief
  *Handle loading, opening and closing of files
  */


/**
  *\fn int is_file(char *path)
  *\brief CHecks wheather the path arguments is a file
  *\param Char *path is the path to check
  *\return an 1 if everything works properly, 0 otherwise
  */
int is_file(char *path);

/**
  *\fn struct linked_list *read_fil(char *path)
  *\brief handles the file passed in argument
  *\return a struct linked_list if everything works properly, NULL otherwise
  */
struct linked_list *read_fil(char *path);
void fuse_lists(struct linked_list *ll, char *string);

#endif /* ! FILE_H */
