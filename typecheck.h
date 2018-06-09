/**
 * Copyright 2018 Slawomir Maludzinski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef __TYPECHECK_H__
#define __TYPECHECK_H__

#include "never.h"

#define TYPECHECK_SUCC 0
#define TYPECHECK_FAIL 1

int expr_set_return_type(expr * value, var * ret);

int var_cmp(var * var_one, var * var_two);
int var_list_cmp(var_list * var_one, var_list * var_two);
int func_cmp(var_list * var_list_one, var * ret_one,
             var_list * var_list_two, var * ret_two);
int var_expr_cmp(var * var_value, expr * expr_value);
int var_expr_list_cmp(var_list * vars, expr_list * list);

int expr_id_check_type(symtab * tab, expr * value, int * result);
int expr_cond_check_type(symtab * tab, expr * value, int * result);
int expr_check_type(symtab * tab, expr * value, int * result);
int expr_list_check_type(symtab * tab, expr_list * list, int * result);
int func_check_type(symtab * tab, func * func_value, int * result);
int func_list_check_type(symtab * tab, func_list * list, int * result);
int never_check_type(never * nev, int * result);

int symtab_add_var_from_var_list(symtab * tab, var_list * list, int * result);
int symtab_add_func_from_func(symtab * tab, func * func_value, int * result);
int symtab_add_func_from_func_list(symtab * tab, func_list * list, int * result);
int symtab_add_entry_expr(symtab * stab, expr * value, int * result);
int symtab_add_entry_expr_list(symtab * stab_parent, expr_list * list, int * result);
int symtab_add_entry_func(symtab * stab_parent, func * func_value, int * result);
int symtab_add_entry_func_list(symtab * stab_parent, func_list * list, int * result);
int symtab_add_entry_never(never * nev, int * result);

int print_func_expr(expr * value, int depth);
int print_func_expr_list(expr_list * list, int depth);
int print_func(func * value, int depth);
int print_func_list(func_list * list, int depth);
int print_functions(never * nev);

int func_main_check_type(symtab * tab, int * result);

int never_sem_check(never * nev);

#endif /* __TYPECHECK_H__ */

