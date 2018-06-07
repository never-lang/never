#ifndef __CONSTRED_H__
#define __CONSTRED_H__

#include "never.h"

enum
{
    CONSTRED_SUCC = 0,
    CONSTRED_FAIL = 1
};

int expr_constred(expr * value, int * result);
int expr_list_constred(expr_list * list, int * result);
int func_constred(func * value, int * result);
int func_list_constred(func_list * list, int * result);
int never_constred(never * nev);

#endif /* __CONSTRED_H__ */

