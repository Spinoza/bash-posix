#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hardbs.h>

static int mpow(int a, int b)
{
    if (b == 0)
        return 1;

    while (b > 1)
    {
        a *= a;
        b--;
    }

    return a;
}

static char convert_octal(int conv)
{
    int res = 0;
    int b = 0;

    while (conv > 0)
    {
        int temp = conv%10;
        conv/=10;
        res += temp * mpow(8, b);
        b++;
    }

    char c = res;
    return c;
}

static char convert_hexa(int conv)
{
    int res = 0;
    int b = 0;

    while (conv > 0)
    {
        int temp = conv%10;
        conv/=10;
        res += temp * mpow(16, b);
        b++;
    }

    char c = res;
    return c;
}

int isforbidden(char c)
{
    if (c == '\\')
        return 1;
    if (c == '\0')
        return 1;
    if (c == '\a')
        return 1;
    if (c == '\n')
        return 1;
    if (c == '\f')
        return 1;
    if (c == '\v')
        return 1;
    if (c == '\t')
        return 1;
    if (c == '\b')
        return 1;

    return 0;
}

int back_zero(char *arg, char *printfin, int index, size_t *i)
{
    char cpy[3] = {"\0"};
    int filled = 0;
    int hasfilled = 0;
    for (;(*i < strlen(arg)) && (*i < 5); (*i)++)
    {
        cpy[filled] = arg[*i];
        filled++;
    }

    while (filled < 3)
    {
        cpy[filled] = '0';
        filled++;
        hasfilled = 1;
    }

    char c = convert_octal(atoi(cpy));
    printfin[index] = c;
    index++;
    if (hasfilled)
    {
        return index;
    }
    else
    {
        for (; *i < strlen(arg) && !isforbidden(arg[*i]); (*i)++)
        {
            printfin[index] = arg[*i];
            index++;
        }
        return index;
    }

}

int back_x(char *arg, char *printfin, int index, size_t *i)
{
    char cpy[2] = {"\0"};
    int filled = 0;
    int hasfilled = 0;
    for (; (*i < strlen(arg)) && (*i < 4); (*i)++)
    {
        cpy[filled] = arg[*i];
        filled++;
    }

    while (filled < 2)
    {
        cpy[filled] = '0';
        filled++;
        hasfilled = 1;
    }

    char c = convert_hexa(atoi(cpy));
    printfin[index] = c;
    index++;
    if (hasfilled)
    {
        return index;
    }
    else
    {
        for (; *i < strlen(arg) && !isforbidden(arg[*i]); (*i)++)
        {
            printfin[index] = arg[*i];
            index++;
        }
        return index;
    }

}

int back_u(char *arg, char *printfin, int index, size_t *i)
{
    char cpy[4] = {"\0"};
    int filled = 0;
    int hasfilled = 0;
    for (; (*i < strlen(arg)) && (*i < 8); (*i)++)
    {
        cpy[filled] = arg[*i];
        filled++;
    }

    while (filled < 4)
    {
        cpy[filled] = '0';
        filled++;
        hasfilled = 1;
    }

    char c = convert_hexa(atoi(cpy));
    printfin[index] = c;
    index++;
    if (hasfilled)
    {
        return index;
    }
    else
    {
        for (; *i < strlen(arg) && !isforbidden(arg[*i]); (*i)++)
        {
            printfin[index] = arg[*i];
            index++;
        }
        return index;
    }

}

int back_U(char *arg, char *printfin, int index, size_t *i)
{
    char cpy[8] = {"\0"};
    int filled = 0;
    int hasfilled = 0;
    for (; (*i < strlen(arg)) && (*i < 10); (*i)++)
    {
        cpy[filled] = arg[*i];
        filled++;
    }

    while (filled < 8)
    {
        cpy[filled] = '0';
        filled++;
        hasfilled = 1;
    }

    char c = convert_hexa(atoi(cpy));
    printfin[index] = c;
    index++;
    if (hasfilled)
    {
        return index;
    }
    else
    {
        for (;*i < strlen(arg) && !isforbidden(arg[*i]); (*i)++)
        {
            printfin[index] = arg[*i];
            index++;
        }
        return index;
    }

}
