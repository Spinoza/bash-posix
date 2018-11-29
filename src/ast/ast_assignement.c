#define _GNU_SOURCE
#define "linked_list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct a_tab *init_assignment(void)
{
    struct a_tab *a_tab = malloc(sizeof(struct a_tab));
    a_tab->a_list = malloc(HASH_TAB_SIZE * sizeof(struct assignement *));
    return a_tab;
}

int hash_function(char *string)
{
    float r = 0;
    int i = 0;
    for ( ; *(string + i); i++)
        r+= ((float)string[i] * TETA ) % HASH_TAB_SIZE;
    return (int)(r / i);
}

struct assignment *to_assign(char *string)
{
    struct assignment *new = malloc(sizeof(struct assignment));
    int i = 0;
    int len = strlen(string);
    for (; *(string + i) && *(string + i) != '='; i++);
    new->name = malloc(sizeof(char) * i);
    new->name = memcpy(new->name, string, i);
    new->name[i] = '\0';
    new->value = memcpy(new->value, string + i + 1, len - i);
    new->value[len - i] = '\0';
    return new;
}
int already_exists(struct assignment *to_assign, struct tab_a *tab)
{
    char *string = to_assign->name;
    int name_len = strlen(string) + 1;
    int value_len = strlen(to_assign->name) +1;
    for (int i =0; i < nb; i++) //Value already present
    {
        if (!strcmp(string, tab->assignment->name))
        {
            free(tab->value);
            tab->value = malloc(sizeof(char) * value_len);
            tab->value = memcpy(tab->value, to_assign->value, value_len);
            free(to_assign);
            return 1;
        }
    }
    return 0;
}
void add_assignment(struct assignment *to_assign, struct tab_a *tab)
{
    char *string = to_assign->name;
    int name_len = strlen(string) + 1;

    if (already_exists(to_assign, tab)) //just change the value field
        return;
    if (tab->nb == tab->capacity)
    {
        capacity *=2;
        void *tmp = realloc(tab->assignment,sizeof(struct assignment)
                * capacity);
        if (!tmp)
        {
            capacity /=2;
            fprintf(stderr,"42sh : Out of memory");
            return;
        }
        tab->assignment = tmp;
    }
    tab->assignment[tab->nb++] = to_assign;
}

char *get_assign(char *name, struct tab_a **tab)
{
    for (int i = 0; i < la->nb; i++)
    {
        if (!strcmp(name, tab->assignment[i]->name))
            return tab->assignment[i]->value;
    }
    return name;
}
