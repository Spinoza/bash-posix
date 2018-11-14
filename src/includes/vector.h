#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <unistd.h>

struct vector
{
    ssize_t size;
    ssize_t capacity;
    void **arr;
};

struct vector *vector_create(void);
void vector_destroy(struct vector *v);

struct vector *vector_append(struct vector *v, void *elm);

#endif /* VECTOR_H */
