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

int expr_set_return_type(expr * value, param * ret);

int param_cmp(param * param_one, param * param_two);
int param_list_cmp(param_list * param_one, param_list * param_two);
int func_cmp(param_list * param_list_one, param * ret_one, param_list * param_list_two,
             param * ret_two);

int param_expr_array_cmp(param * param_value, expr * expr_value);
int param_expr_cmp(param * param_value, expr * expr_value);
int param_expr_list_cmp(param_list * params, expr_list * list);

int array_dims_check_type_expr(symtab * tab, expr * value, unsigned syn_level,
                               int * result);
int array_dims_check_type_expr_list(symtab * tab, expr_list * list,
                                    unsigned int syn_level, int * result);

int expr_id_check_type(symtab * tab, expr * value, int * result);
int expr_neg_check_type(symtab * tab, expr * value, unsigned int syn_level, int * result);
int expr_add_sub_check_type(symtab * tab, expr * value, unsigned int syn_level,
                            int * result);
int expr_mul_check_type(symtab * tab, expr * value, unsigned int syn_level, int * result);
int expr_div_check_type(symtab * tab, expr * value, unsigned int syn_level, int * result);
int expr_array_deref_check_type(symtab * tab, expr * value, unsigned int syn_level,
                                int * result);
int expr_call_check_type(symtab * tab, expr * value, unsigned int syn_level,
                         int * result);
int expr_cond_check_type(symtab * tab, expr * value, unsigned int syn_level,
                         int * result);
int expr_check_type(symtab * tab, expr * value, unsigned int syn_level,
                    int * result);
int expr_list_check_type(symtab * tab, expr_list * list, unsigned int syn_level,
                         int * result);
int bind_check_type(symtab * tab, bind * value, unsigned int syn_level,
                    int * result);
int bind_list_check_type(symtab * tab, bind_list * list, unsigned int syn_level,
                         int * result);
int func_check_type(symtab * tab, func * func_value, unsigned int syn_level,
                    int * result);
int func_list_check_type(symtab * tab, func_list * list, unsigned int syn_level,
                         int * result);
int never_check_type(never * nev, int * result);


int symtab_add_param_from_basic_param(symtab * tab, param * param_value,
                                  unsigned int syn_level, int * result);
int symtab_add_param_from_param(symtab * tab, param * param_value,
                            unsigned int syn_level, int * result);
int symtab_add_param_from_param_list(symtab * tab, param_list * list,
                                 unsigned int syn_level, int * result);
int symtab_add_bind_from_bind(symtab * tab, bind * bind_value,
                              unsigned int syn_level, int * result);
int symtab_add_bind_from_bind_list(symtab * tab, bind_list * list,
                                   unsigned int syn_level, int * result);
int symtab_add_func_from_func(symtab * tab, func * func_value,
                              unsigned int syn_level, int * result);
int symtab_add_func_from_func_list(symtab * tab, func_list * list,
                                   unsigned int syn_level, int * result);

int print_func_array(array * value, int depth);
int print_func_expr(expr * value, int depth);
int print_func_expr_list(expr_list * list, int depth);
int print_bind(bind * value, int depth);
int print_bind_list(bind_list * list, int depth);
int print_func(func * value, int depth);
int print_func_list(func_list * list, int depth);
int print_functions(never * nev);

int func_main_check_num_params(param_list * params);
int func_main_check_type(symtab * tab, int * result);

int never_sem_check(never * nev);

#endif /* __TYPECHECK_H__ */
