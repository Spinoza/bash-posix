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

int back_zero(char *arg, char *printfin, int index)
{
    char cpy[3] = {"\0"};
    int filled = 0;
    int hasfilled = 0;
    for (size_t i = 2; (i < strlen(arg)) && (i < 5); i++)
    {
        cpy[filled] = arg[i];
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
        for (size_t i = 5; i < strlen(arg); i++)
        {
            printfin[index] = arg[i];
            index++;
        }
        return index;
    }

}

int back_x(char *arg, char *printfin, int index)
{
    char cpy[2] = {"\0"};
    int filled = 0;
    int hasfilled = 0;
    for (size_t i = 2; (i < strlen(arg)) && (i < 4); i++)
    {
        cpy[filled] = arg[i];
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
        for (size_t i = 4; i < strlen(arg); i++)
        {
            printfin[index] = arg[i];
            index++;
        }
        return index;
    }

}

int back_u(char *arg, char *printfin, int index)
{
    char cpy[4] = {"\0"};
    int filled = 0;
    int hasfilled = 0;
    for (size_t i = 4; (i < strlen(arg)) && (i < 8); i++)
    {
        cpy[filled] = arg[i];
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
        for (size_t i = 8; i < strlen(arg); i++)
        {
            printfin[index] = arg[i];
            index++;
        }
        return index;
    }

}

int back_U(char *arg, char *printfin, int index)
{
    char cpy[8] = {"\0"};
    int filled = 0;
    int hasfilled = 0;
    for (size_t i = 2; (i < strlen(arg)) && (i < 10); i++)
    {
        cpy[filled] = arg[i];
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
        for (size_t i = 10; i < strlen(arg); i++)
        {
            printfin[index] = arg[i];
            index++;
        }
        return index;
    }

}
