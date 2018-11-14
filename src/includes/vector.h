#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

struct vector
{
    size_t size;
    size_t capacity;
    void **arr;
};

struct vector *vector_create(void);
void vector_destroy(struct vector *v);

struct vector *vector_append(struct vector *v, void *elm);

#endif /* VECTOR_H */
