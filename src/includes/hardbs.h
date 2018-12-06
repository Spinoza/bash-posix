#ifndef HARDBS_H
#define HARDBS_H

#include <wchar.h>
#include <locale.h>

int isforbidden(char c);

int back_U(char *arg, wchar_t *printfin, int index, int *i);
int back_u(char *arg, wchar_t *printfin, int index, int *i);
int back_x(char *arg, wchar_t *printfin, int index, int *i);
int back_zero(char *arg, wchar_t *printfin, int index, int *i);

#endif /* HARDBS_H */
