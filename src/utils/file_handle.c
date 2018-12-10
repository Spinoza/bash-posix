#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "lexer.h"
#include "builtins.h"
#include "file_handle.h"
#include <err.h>

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
    char *copy = calloc(strlen(path) + 1, sizeof(char));
    memcpy(copy, path, strlen(path));
    char *path2 = strtok(copy, " \n;");
    FILE *file = fopen(path2, "r");
    free(copy);
    if (!file)
        return 0;

    fclose(file);
    return 1;
}

void fuse_lists(struct linked_list *ll, char *string)
{
    struct linked_list *temp = lexer_c(string);
    struct nL *head = ll->head;
    while (head->next->next)
    {
        head = head->next;
    }

    free(head->next->elem->name);
    head->next->elem->name = NULL;

    free(head->next->elem);
    head->next->elem = NULL;

    free(head->next);
    head->next = temp->head;
    free(temp);
    temp = NULL;
}

struct linked_list *read_fil(char *path)
{
    char *copy = calloc(strlen(path) + 1, sizeof(char));
    memcpy(copy, path, strlen(path));
    char *path2 = strtok(copy, " \n;");
    FILE *file = fopen(path2, "r");
    free(copy);
    char *line = NULL;
    size_t len = 0;
    ssize_t isread;

    isread = getline(&line, &len, file);
    line[strlen(line) - 1] = '\0';
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
        line[strlen(line) - 1] = '\0';
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

/*static int is_empty(char *line)
{
    if (!line)
        return 1;
    for (size_t j = 0; j < strlen(line); j++)
    {
        if (line[j] != ' ' && line[j] != '\n')
        {
            return 0;
        }
    }

    return 1;
}*/

struct linked_list *reading(FILE *file)
{
    struct linked_list *tokens;
    char *input = calloc(300, sizeof(char));
    size_t red = fread(input, sizeof(char), 300, file);
    size_t cumul = red;
    size_t toreac = 300;
    while (red > 0)
    {
        if (cumul >= toreac)
        {
            input = realloc(input, 300 * sizeof(char));
            if (!input)
            {
                errx(1, "error while reallocating.\n");
            }
            toreac += 300;
            for (size_t i = cumul; i < toreac; i++)
            {
                input[i] = '\0';
            }
        }

        red = fread(input + cumul, sizeof(char), 300, file);
        cumul += red;
    }

    tokens = lexer_c(input);
/*    char *line = NULL;
    size_t i = 0;
    int res = 0;
    res = getline(&line, &i, file);
    if (res == -1)
    {
        free(line);
        line = NULL;
        return NULL;
    }
    while (res != -1 && is_empty(line))
    {
        free(line);
        line = NULL;
        res = getline(&line, &i, file);
    }
    if (res == -1)
    {
        free(line);
        char *argE[] = { "exit", NULL };
        my_exit(argE, 0);
    }
    if (line[strlen(line) - 1] == '\n')
    {
        line[strlen(line) - 1] = '\0';
    }

    tokens = lexer_c(line);
    free(line);
    line = NULL;
    while ( (res = getline(&line, &i, file)) != -1)
    {
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }
        fuse_lists(tokens, line);
        free(line);
        line = NULL;
    }
    free(line);
*/
    free(input);
    return tokens;
}
