#ifndef __GENCODE_H__
#define __GENCODE_H__

#include "never.h"

enum
{
    GENCODE_SUCC = 0,
    GENCODE_FAIL = 1
};

int expr_id_gencode(unsigned int syn_level, func * func_value, expr * value, int * result);
int expr_gencode(unsigned int syn_level, func * func_value, expr * value, int * result);
int expr_list_gencode(unsigned int syn_level, func * func_value, expr_list * list, int * result);

int func_gencode_freevars_expr(func * func_value, expr * value, int * result);
int func_gencode_freevars_expr_list(func * func_value, expr_list * list, int * result);
int func_gencode_freevars_freevar(func * func_value, freevar * freevar_value, int * result);
int func_gencode_freevars_func(func * func_value, func * subfunc_value, int * result);
int func_gencode_freevars_func_list(func * func_value, func_list * list, int * result);
int func_gencode_freevars(func * func_value, int * result);

int func_gencode(unsigned int syn_level, func * func_value, int * result);
int func_list_gencode(unsigned int syn_level, func_list * list, int * result);

int never_gencode(never * nev);

int expr_emit(expr * value, int * result);
int expr_list_emit(expr_list * list, int * result);
int func_emit(func * func_value, int * result);
int func_list_emit(func_list * list, int * result);
int never_emit(never * nev);

#endif /* __GENCODE_H__ */


