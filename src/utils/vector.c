#include <stdlib.h>
#include "vector.h"

struct vector *vector_create(void)
{
    struct vector *v = malloc(sizeof(struct vector));

    v->size = 0;
    v->capacity = 8;
    v->arr = malloc(v->capacity * sizeof(void *));
    return v;
}

void vector_destroy(struct vector *v)
{
    if (!v)
        return;

    //FIXME : need to implement a free fct for the tokens
    free(v->arr);
    free(v);
}

static void vector_add(struct vector *v, void *elem, size_t index)
{
    if (v->size >= v->capacity)
    {
        v->capacity = 2 * v->capacity;
        v->arr = realloc(v->arr, v->capacity * sizeof(void *));
    }

    v->arr[index] = elem;
    v->size++;
}

struct vector *vector_append(struct vector *v, void *elm)
{
    vector_add(v, elm, v->size);
    return v;
}
