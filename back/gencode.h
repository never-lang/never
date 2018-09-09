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
#ifndef __GENCODE_H__
#define __GENCODE_H__

#include "bytecode.h"
#include "never.h"
#include "weakfunc.h"

enum
{
    GENCODE_SUCC = 0,
    GENCODE_FAIL = 1
};

int func_enum_param_list(param_list * params);
int func_enum_bind_list(bind_list * list, int start);
int func_enum_func_list(func_list * list, int start);

int expr_id_gencode(unsigned int syn_level, func * func_value, expr * value,
                    int * result);
int expr_gencode(unsigned int syn_level, func * func_value, expr * value,
                 int * result);
int expr_list_gencode(unsigned int syn_level, func * func_value,
                      expr_list * list, int * result);

int array_gencode(unsigned int syn_level, func * func_value,
                  array * array_value, int * result);

int bind_gencode(unsigned int syn_level, func * func_value, bind * bind_value,
                 int * result);
int bind_list_gencode(unsigned int syn_level, func * func_value,
                      bind_list * list, int * result);                 

int func_gencode_freevars_freevar(func * func_value, freevar * freevar_value,
                                  int * result);
int func_gencode_freevars_expr(func * func_value, expr * value, int * result);
int func_gencode_freevars_expr_list(func * func_value, expr_list * list,
                                    int * result);
int func_gencode_freevars_bind(func * func_value, bind * bind_value,
                               int * result);
int func_gencode_freevars_bind_list(func * func_value, bind_list * list,
                                    int * result);
int func_gencode_freevars_func(func * func_value, func * subfunc_value,
                               int * result);
int func_gencode_freevars_func_list(func * func_value, func_list * list,
                                    int * result);
int func_gencode_freevars(func * func_value, int * result);

int func_gencode(unsigned int syn_level, func * func_value, int * result);
int func_list_gencode(unsigned int syn_level, func_list * list, int * result);

int never_gencode(never * nev);

int expr_float_emit(expr * value, int stack_level, bytecode_list * code,
                    int * result);

int func_freevar_id_local_emit(freevar * value, int stack_level,
                               bytecode_list * code, int * result);
int func_freevar_id_bind_emit(freevar * value, int stack_level,
                              bytecode_list * code, int * result);
int func_freevar_emit(freevar * value, int stack_level, bytecode_list * code,
                      int * result);
int func_freevar_list_emit(freevar_list * freevars, int stack_level,
                           bytecode_list * code, int * result);

int expr_id_local_emit(expr * value, int stack_level, bytecode_list * code,
                       int * result);
int expr_id_bind_emit(expr * value, int stack_level, bytecode_list * code,
                       int * result);
int expr_id_global_emit(expr * value, int stack_value, bytecode_list * code,
                        int * result);
int expr_id_func_top_emit(func * func_value, int stack_level,
                          bytecode_list * code, int * result);
int expr_id_func_emit(func * func_value, int stack_level, bytecode_list * code,
                      int * result);
int expr_id_func_nest_emit(func * func_value, int stack_level,
                           bytecode_list * code, int * result);

int expr_id_emit(expr * value, int stack_level, bytecode_list * code,
                 int * result);
int expr_neg_emit(expr * value, int stack_level, bytecode_list * code,
                  func_list_weak * list_weak, int * result);
int expr_add_emit(expr * value, int stack_level, bytecode_list * code,
                  func_list_weak * list_weak, int * result);
int expr_sub_emit(expr * value, int stack_level, bytecode_list * code,
                  func_list_weak * list_weak, int * result);
int expr_mul_emit(expr * value, int stack_level, bytecode_list * code,
                  func_list_weak * list_weak, int * result);

int expr_and_emit(expr * value, int stack_level, bytecode_list * code,
                  func_list_weak * list_weak, int * result);
int expr_or_emit(expr * value, int stack_level, bytecode_list * code,
                 func_list_weak * list_weak, int * result);
int expr_not_emit(expr * value, int stack_level, bytecode_list * code,
                  func_list_weak * list_weak, int * result);
int expr_ass_emit(expr * value, int stack_level, bytecode_list * code,
                  func_list_weak * list_weak, int * result);
int expr_cond_emit(expr * value, int stack_level, bytecode_list * code,
                   func_list_weak * list_weak, int * result);
int expr_while_emit(expr * value, int stack_level, bytecode_list * code, 
                    func_list_weak * list_weak, int * result);
int expr_do_while_emit(expr * value, int stack_level, bytecode_list * code, 
                       func_list_weak * list_weak, int * result);
int expr_for_emit(expr * value, int stack_level, bytecode_list * code, 
                  func_list_weak * list_weak, int * result);
int expr_call_emit(expr * value, int stack_level, bytecode_list * code,
                   func_list_weak * list_weak, int * result);
int expr_last_call_emit(expr * value, int stack_level, bytecode_list * code,
                        func_list_weak * list_weak, int * result);
int expr_emit(expr * value, int stack_level, bytecode_list * code,
              func_list_weak * list_weak, int * result);
int expr_list_emit(expr_list * list, int stack_level, bytecode_list * code,
                   func_list_weak * list_weak, int * result);
int expr_seq_emit(expr_list * list, int stack_level, bytecode_list * code,
                  func_list_weak * list_weak, int * result);
int array_init_elements_emit(expr_list_weak * depth_list, int * elements_count,
                             int stack_level, bytecode_list * code,
                             func_list_weak * list_weak, int * result);
int array_init_emit(expr * value, int stack_level, bytecode_list * code,
                    func_list_weak * list_weak, int * result);
int array_dims_emit(array * array_value, int stack_level, bytecode_list * code,
                    func_list_weak * list_weak, int * result);
int expr_array_emit(expr * value, int stack_level, bytecode_list * code,
                    func_list_weak * list_weak, int * result);
int expr_array_deref_emit(expr * value, int stack_level, bytecode_list * code,
                          func_list_weak * list_weak, int * result);
int bind_emit(bind * bind_value, int stack_level, bytecode_list * code,
              func_list_weak * list_weak, int * result);
int bind_list_emit(bind_list * list, int stack_level, bytecode_list * code,
                   func_list_weak * list_weak, int * result);
int except_all_emit(except * value, int stack_level, bytecode_list * code,
                    func_list_weak * list_weak, int * result);
int except_emit(except * value, int stack_level, bytecode_list * code,
                func_list_weak * list_weak, int * result);
int except_list_emit(except_list * list, int stack_level, bytecode_list * code,
                     func_list_weak * list_weak, int * result);
int func_except_emit(func_except * value, int stack_level, bytecode_list * code,
                     func_list_weak * list_weak, int * result);
int func_body_emit(func * func_value, bytecode_list * code,
                   func_list_weak * list_weak, int * result);
int func_emit(func * func_value, int stack_level, bytecode_list * code,
              func_list_weak * list_weak, int * result);
int func_list_emit(func_list * list, int stack_level, bytecode_list * code,
                   func_list_weak * list_weak, int * result);
int never_emit(never * nev, bytecode_list * code);

int func_main_emit(never * nev, int stack_level, bytecode_list * code,
                   int * result);

#endif /* __GENCODE_H__ */
