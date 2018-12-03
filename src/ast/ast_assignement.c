#define _GNU_SOURCE
#include "ast_assignement.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct assignment **init_assignment(void)
{
    struct assignment **a_tab = calloc(HASH_TAB_SIZE,
            sizeof(struct assignment *));
    for (int i = 0; i < HASH_TAB_SIZE; i++)
    {
        a_tab[i] = calloc(1, sizeof(struct assignment));
    }
    return a_tab;
}

void free_assignments(struct assignment **tab)
{
    for (size_t i = 0; i < HASH_TAB_SIZE; i++)
    {
        struct assignment *to_free = tab[i];
        struct assignment *next = tab[i];
        for (; next;)
        {
            to_free = next;
            free(to_free->value);
            free(to_free->name);
            next = next->next;
            free(to_free);
        }
    }
    free(tab);
    tab = NULL;
}
//return the position in the hash table
int hash_function(char *string)
{

    float r = 0;
    int i = 0;
    for ( ; *(string + i); i++)
        r+= ((double)string[i] * TETA );
    return (int)(r / i) % HASH_TAB_SIZE;
}

static struct assignment *to_assign(char *name, char *value)
{
    struct assignment *new = calloc(1, sizeof(struct assignment));
    new->name = name;
    new->value = value;
    return new;
}

static int already_exists(char *name, char *value,
        struct assignment **a_tab, int pos)
{
    struct assignment *a = a_tab[pos];
    for (; a; a = a->next)
    {
        if (!a->name)
            return 0;
        if (!strcmp(name, a->name))
        {
            free(a->value);
            a->value = value;
            return 1;
        }
    }
    return 0;
}

void split_string(char *name, char *value, char *string)
{
    int len = strlen(string);
    int i = 0;
    for (; *(string + i) && *(string + i) != '='; i++);
    memcpy(name, string, i);
    memcpy(value, string + i + 1, len - i);
}
void add_assignment_split(char *name, char *value, struct assignment **a_tab)
{
    int pos = hash_function(name);
    if (already_exists(name, value, a_tab, pos))
        return;
    struct assignment *new = to_assign(name, value);
    struct assignment *a = a_tab[pos];
    if (a->name == NULL)
    {
        free(a);
        a_tab[pos] = new;
        return;
    }
    for (; a && a->next; a = a ->next)
        continue;
    a->next = new;
    return;
}

void add_assignment(char *string, struct assignment **a_tab)
{
    int len = strlen(string);
    char *name = calloc(len, sizeof(char));
    char *value = calloc(len, sizeof(char));
    split_string(name, value, string);
    int pos = hash_function(name);
    if (already_exists(name, value, a_tab, pos))
        return;
    struct assignment *new = to_assign(name, value);
    struct assignment *a = a_tab[pos];
    if (a->name == NULL)
    {
        free(a);
        a_tab[pos] = new;
        return;
    }
    for (; a && a->next; a = a ->next)
        continue;
    a->next = new;
    return;
}
