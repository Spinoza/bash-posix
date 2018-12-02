#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <unistd.h>
/**
  *\file vector.h
  *\brief 
  *All information about vectores and all functions
  *to build, update and free them.
  */

/**
  *\struct vector
  *\brief Struct of the vectors
  */
struct vector 
{
    ssize_t size; /**< The number of elements in vector*/
    ssize_t capacity; /**< The total number of element that can be contained by the vector*/
    void **arr; /**< The array of element*/
};

/**
  *\fn struct vector *vector_create(void)
  *\brief Builds the vector
  *\return a struct vector if everything works properly, NULL otherwise
  */
struct vector *vector_create(void);

/**
  *\fn void vector *vector_destroy(struct vector *v)
  *\brief Destroys the vector
  *\param Struct vector v is the vector to destroy
  */
void vector_destroy(struct vector *v);

/**
  *\fn struct vector *vector_append(struct vector *v, void *elem)
  *\brief Adds elem to vector
  *\param void *elem is the element to add in Struct vector *v
  *\return a struct vector if everything works properly, NULL otherwise
  */
struct vector *vector_append(struct vector *v, void *elm);

#endif /* VECTOR_H */
