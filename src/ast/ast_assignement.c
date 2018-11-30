#define _GNU_SOURCE
#include "ast_assignement.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct assignment **init_assignment(void)
{
    struct assignment **a_tab = calloc(HASH_TAB_SIZE, sizeof(struct assignment *));
    return a_tab;
}

//return the position in the hash table
int hash_function(char *string)
{
    float r = 0;
    int i = 0;
    for ( ; *(string + i) && *(string + i) != '='; i++)
        r+= ((double)string[i] * TETA );
    return (int)(r / i) % HASH_TAB_SIZE;
}

struct assignment *to_assign(char *string)
{
    struct assignment *new = calloc(1, sizeof(struct assignment));
    int i = 0;
    int len = strlen(string);
    for (; *(string + i) && *(string + i) != '='; i++)
        continue;
    new->name = malloc(sizeof(char) * i);
    new->name = memcpy(new->name, string, i);
    new->name[i] = '\0';
    new->value = memcpy(new->value, string + i + 1, len - i);
    new->value[len - i] = '\0';
    return new;
}

int same_name(char *a, char *b)
{
    int i = 0;
    for ( ;  a[i] && a[i] != '=' && b[i] && a[i] == b[i]; i++)
        continue;
    if (!b[i] && a[i] == '=')
        return 1;
    return 0;
}

int already_exists(char *string, struct assignment **a_tab, int pos)
{
    struct assignment *a = a_tab[pos];
    for (; a; a = a->next)
    {
        if (same_name(string, a->name))
        {
            free(a->value);
            for (int i = 0; *(string + i) && *(string + i) != '='; i++)
                continue;
            size_t value_len = strlen(string) + 1;
            a->value = calloc(sizeof(char), value_len);
            a->value = memcpy(a->value, string, value_len);
            return 1;
        }
    }
    return 0;
}
struct assignment **add_assignment(char *string,
        struct assignment **a_tab)
{
    int pos = hash_function(string);
    if (already_exists(string, a_tab, pos))
        return a_tab;
    struct assignment *new = to_assign(string);
    struct assignment *a = a_tab[pos];
    if (a == NULL)
    {
        a_tab[pos] = new;
        return a_tab;
    }
    for (; a && a->next; a = a ->next)
        continue;
    a->next = new;
    return a_tab;
}

char *get_assign(char *name, struct assignment **a_tab)
{
    int pos = hash_function(name);
    struct assignment *a = a_tab[pos];
    for ( ; a ; a = a ->next)
    {
        if (!strcmp(name, a->name))
            return a->value;
    }
    return "";
}
