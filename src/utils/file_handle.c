#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "lexer.h"
#include "file_handle.h"

static size_t mstrlen(char *string)
{
    return *string ? 1 + mstrlen(string + 1) : 0;
}

static struct nL *find_actual(struct linked_list *ll, struct nL *actual)
{
    struct nL *begin = NULL;
    if (!actual)
    {
        begin = ll->head;
    }

    else
    {
        begin = actual;
    }

    while (begin->next && begin->next->elem->type != ENDOF)
    {
        begin = begin->next;
    }

    return begin;
}

int is_file(char *path)
{
    char *copy = calloc(mstrlen(path) + 1, sizeof(char));
    memcpy(copy, path, mstrlen(path));
    char *path2 = strtok(copy, " \n;");
    FILE *file = fopen(path2, "r");
    free(copy);
    if (!file)
        return 0;

    fclose(file);
    return 1;
}

struct linked_list *read_fil(char *path)
{
    char *copy = calloc(mstrlen(path) + 1, sizeof(char));
    memcpy(copy, path, mstrlen(path));
    char *path2 = strtok(copy, " \n;");
    FILE *file = fopen(path2, "r");
    free(copy);
    char *line = NULL;
    size_t len = 0;
    ssize_t isread;

    isread = getline(&line, &len, file);
    line[mstrlen(line) - 1] = '\0';
    struct linked_list *toret = lexer_c(line);
    if (isread == -1)
    {
        free(line);
        return toret;
    }
    free(line);
    line = NULL;
    struct nL *actual = NULL;
    actual = find_actual(toret, actual);
    while ((isread = getline(&line, &len, file)) != -1)
    {
        line[mstrlen(line) - 1] = '\0';
        struct linked_list *temp = lexer_c(line);
        actual->next = temp->head;
        temp->head->prev = actual;
        free(temp);
        temp = NULL;
        free(line);
        line = NULL;
        actual = find_actual(toret, actual);
    }
    fclose(file);
    return toret;
}
