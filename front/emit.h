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
#ifndef __EMIT_H__
#define __EMIT_H__

#include "module.h"
#include "never.h"
#include "weakfunc.h"

enum
{
    EMIT_SUCC = 0,
    EMIT_FAIL = 1
};

typedef struct match_guard_item match_guard_item;
typedef struct match_guard_record match_guard_record;
typedef struct match_guard_item_expr match_guard_item_expr;
typedef struct match_guard_record_expr match_guard_record_expr;
typedef struct match_guard_else_expr match_guard_else_expr;
typedef struct match_guard match_guard;
typedef struct match_guard_list match_guard_list;

int expr_int_emit(expr * value, int stack_level, module * module_value,
                  int * result);
int expr_long_emit(expr * value, int stack_level, module * module_value,
                   int * result);
int expr_float_emit(expr * value, int stack_level, module * module_value,
                    int * result);
int expr_double_emit(expr * value, int stack_level, module * module_value,
                     int * result);
int expr_char_emit(expr * value, int stack_level, module * module_value,
                   int * result);
int expr_string_emit(expr * value, int stack_level, module * module_value,
                     int * result);
int expr_nil_emit(expr * value, int stack_level, module * module_value,
                  int * result);
int expr_c_null_emit(expr * value, int stack_level, module * module_value,
                     int * result);

int func_freevar_id_local_emit(freevar * value, int stack_level,
                               module * module_value, int * result);
int func_freevar_id_bind_emit(freevar * value, int stack_level,
                              module * module_value, int * result);
int func_freevar_id_matchbind_emit(freevar * value, int stack_level, 
                                   module * module_value, int * result);
int func_freevar_id_qualifier_emit(freevar * value, int stack_level,
                                   module * module_value, int * result);
int func_freevar_id_forin_emit(freevar * value, int stack_level, 
                               module * module_value, int * result);
int func_freevar_emit(freevar * value, int stack_level, module * module_value,
                      int * result);
int func_freevar_list_emit(freevar_list * freevars, int stack_level,
                           module * module_value, int * result);

int expr_id_local_emit(expr * value, int stack_level, module * module_value,
                       int * result);
int expr_id_qualifier_emit(expr * value, int stack_level, module * module_value,
                           int * result);
int expr_id_forin_emit(expr * value, int stack_level, module * module_value,
                       int * result);
int expr_id_bind_emit(expr * value, int stack_level, module * module_value,
                       int * result);
int expr_id_matchbind_emit(expr * value, int stack_level, module * module_value,
                           int * result);
int expr_id_global_emit(expr * value, int stack_value, module * module_value,
                        int * result);
int expr_id_func_top_emit(func * func_value, int stack_level,
                          module * module_value, int * result);
int expr_id_func_emit(func * func_value, int stack_level, module * module_value,
                      int * result);
int expr_id_func_nest_emit(func * func_value, int stack_level,
                           module * module_value, int * result);

int expr_id_emit(expr * value, int stack_level, module * module_value,
                 int * result);
int expr_neg_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_add_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_sub_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_mul_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_div_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_mod_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_eq_emit(expr * value, int stack_level, module * module_value,
                 func_list_weak * list_weak, int * result);
int expr_neq_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_lt_emit(expr * value, int stack_level, module * module_value,
                 func_list_weak * list_weak, int * result);
int expr_gt_emit(expr * value, int stack_level, module * module_value,
                 func_list_weak * list_weak, int * result);
int expr_lte_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_gte_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_and_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_or_emit(expr * value, int stack_level, module * module_value,
                 func_list_weak * list_weak, int * result);
int expr_not_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_pipel_call_emit(expr * value, int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result);
int expr_pipel_last_call_emit(expr * value, int stack_level, module * module_value,
                              func_list_weak * list_weak, int * result);
int expr_pipel_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result);
int expr_ass_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_cond_emit(expr * value, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);
int expr_while_emit(expr * value, int stack_level, module * module_value, 
                    func_list_weak * list_weak, int * result);
int expr_do_while_emit(expr * value, int stack_level, module * module_value, 
                       func_list_weak * list_weak, int * result);
int expr_for_emit(expr * value, int stack_level, module * module_value, 
                  func_list_weak * list_weak, int * result);
int expr_forin_array_emit(expr * value, int stack_level, module * module_value, 
                          func_list_weak * list_weak, int * result);
int expr_forin_range_emit(expr * value, int stack_level, module * module_value, 
                          func_list_weak * list_weak, int * result);
int expr_forin_slice_emit(expr * value, int stack_level, module * module_value, 
                          func_list_weak * list_weak, int * result);
int expr_forin_emit(expr * value, int stack_level, module * module_value, 
                    func_list_weak * list_weak, int * result);
int expr_iflet_guard_item_emit(match_guard_item * guard_item,
                               int stack_level, module * module_value,
                               func_list_weak * list_weak, int * result);
int expr_iflet_guard_record_emit(match_guard_record * guard_record,
                                 int stack_level, module * module_value,
                                 func_list_weak * list_weak, int * result);
int expr_iflet_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result);
int expr_match_guard_item_emit(match_guard_item_expr * item_value, bytecode *label,
                               int stack_level, module * module_value,
                               func_list_weak * list_weak, int * result);
int expr_match_guard_record_emit(match_guard_record_expr * record_value, bytecode *label,
                                 int stack_level, module * module_value,
                                 func_list_weak * list_weak, int * result);
int expr_match_guard_else_emit(match_guard_else_expr * else_value, bytecode * label,
                               int stack_level, module * module_value,
                               func_list_weak * list_weak, int * result);
int expr_match_guard_emit(match_guard * match_value,
                          bytecode * label, int stack_level,
                          module * module_value, func_list_weak * list_weak,
                          int * result);
int expr_match_guard_list_emit(match_guard_list * list, int stack_level,
                               module * module_value, func_list_weak * list_weak,
                               int * result);
int expr_match_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result);
int expr_range_dim_emit(expr * value, int stack_level, module * module_value,
                        func_list_weak * list_weak, int * result);
int expr_range_dim_list_emit(expr_list * list, int stack_level, module * module_value,
                             func_list_weak * list_weak, int * result);
int expr_range_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result);
int expr_slice_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result);
int expr_call_emit(expr * value, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);
int expr_last_call_emit(expr * value, int stack_level, module * module_value,
                        func_list_weak * list_weak, int * result);
int expr_emit(expr * value, int stack_level, module * module_value,
              func_list_weak * list_weak, int * result);
int expr_list_emit(expr_list * list, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);
int expr_seq_emit(expr_list * list, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int expr_yeld_emit(listcomp * listcomp_value, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);
int generator_array_emit(listcomp * listcomp_value, qualifier_list_node * node,
                         int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result);
int generator_range_emit(listcomp * listcomp_value, qualifier_list_node * node,
                         int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result);
int generator_slice_emit(listcomp * listcomp_value, qualifier_list_node * node,
                         int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result);
int generator_emit(listcomp * listcomp_value, qualifier_list_node * node,
                   int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);
int filter_emit(listcomp * listcomp_value, qualifier_list_node * node,
                int stack_level, module * module_value,
                func_list_weak * list_weak, int * result);
int qualifier_emit(listcomp * listcomp_value, qualifier_list_node * node,
                   int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);
int qualifier_stack_emit(listcomp * listcomp_value, qualifier_list_node * node,
                         int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result);
int expr_conv_emit(expr * value, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);
int listcomp_emit(listcomp * listcomp_value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int array_init_elements_emit(expr_list_weak * depth_list, int * elements_count,
                             int stack_level, module * module_value,
                             func_list_weak * list_weak, int * result);
int array_init_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result);
int array_dims_emit(array * array_value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result);
int expr_array_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result);
int expr_array_deref_emit(expr * value, int stack_level, module * module_value,
                          func_list_weak * list_weak, int * result);
int expr_range_deref_emit(expr * value, int stack_level, module * module_value,
                          func_list_weak * list_weak, int * result);
int expr_slice_deref_emit(expr * value, int stack_level, module * module_value,
                          func_list_weak * list_weak, int * result);
int expr_string_deref_emit(expr * value, int stack_level, module * module_value,
                           func_list_weak * list_weak, int * result);
int expr_record_emit(expr * value, int stack_level, module * module_value,
                     func_list_weak * list_weak, int * result);
int expr_enumtype_emit(expr * value, int stack_level, module * module_value, int * result);
int expr_enumtype_record_emit(expr * value, int stack_level, module * module_value,
                              func_list_weak * list_weak, int * result);
int expr_record_attr_emit(expr * value, int stack_level, module * module_value,
                          func_list_weak * list_weak, int * result);
int bind_emit(bind * bind_value, int stack_level, module * module_value,
              func_list_weak * list_weak, int * result);
int bind_list_emit(bind_list * list, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);
int except_all_emit(except * value, func * func_value, int stack_level,
                    module * module_value, func_list_weak * list_weak, int * result);
int except_emit(except * value, func * func_value, int stack_level,
                module * module_value, func_list_weak * list_weak, int * result);
int except_list_emit(except_list * list, func * func_value, int stack_level,
                     module * module_value, func_list_weak * list_weak, int * result);
int except_implicit_emit(func * func_value, int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result);
int func_except_emit(func_except * value, func * func_value, int stack_level,
                     module * module_value, func_list_weak * list_weak, int * result);
unsigned int func_body_emit_ffi_param(param * value, module * module_value, int * result);
unsigned int func_body_emit_ffi_param_list(param_list * params, module * module_value, int * result);
int func_body_emit_ffi(func * func_value, module * module_value,
                       func_list_weak * list_weak, int * result);
int func_body_emit_native(func * func_value, module * module_value,
                          func_list_weak * list_weak, int * result);
int func_body_emit(func * func_value, module * module_value,
                   func_list_weak * list_weak, int * result);

int func_emit_ffi(func * func_value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result);
int func_emit_native(func * func_value, int stack_level, module * module_value,
                     func_list_weak * list_weak, int * result);
int func_emit(func * func_value, int stack_level, module * module_value,
              func_list_weak * list_weak, int * result);

int func_entry_params(func * func_value, module * module_value, int * result);
int func_list_entry_params(func_list * list, module * module_value, int * result);

int func_entry_emit(never * nev, int stack_level, module * module_value,
                   int * result);

int func_list_emit(func_list * list, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result);

int use_emit(use * value, int stack_level, int * index, module * module_value, func_list_weak * list_weak, int * result);
int use_list_emit(use_list * list, int stack_level, int * index, module * module_value, func_list_weak * list_weak, int * result);

int never_emit(never * nev, int stack_level, int * index, module * module_value, func_list_weak * list_weak, int * result);
int module_decl_emit(module_decl * value, int stack_level, int * index, module * module_value, func_list_weak * list_weak, int * result);
int main_emit(module_decl * module_modules, module_decl * module_main, module * module_value);

#endif /* __EMIT_H__ */


