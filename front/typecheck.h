/**
 * Copyright 2020 Slawomir Maludzinski
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
#include <stdbool.h>

#define TYPECHECK_SUCC 0
#define TYPECHECK_FAIL 1

typedef struct qualifier qualifier;
typedef struct qualifier_list qualifier_list;
typedef struct matchbind_list matchbind_list;

int enumtype_enumerator_check_type(symtab * stab, enumerator * value, int * index, int * result);
int enumtype_enumerator_list_check_type(symtab * stab, enumerator_list * list, int * result);
int record_enum_param_list(param_list * params);
int func_enum_param_list(param_list * params);
int enum_matchbind_list(matchbind_list * list);

int expr_set_comb_type(expr * value, param * param_value);
int expr_set_comb_type_symtab(expr * value, symtab_entry * entry, int * result);
int expr_qualifier_set_comb_type(expr * value, expr * expr_value);
int expr_conv_basic_type(expr * value, expr * expr_left, expr * expr_right);
int expr_conv_string_type(expr * value, expr * expr_left, expr * expr_right);
int expr_conv_enumerator(expr * value);
int expr_conv_enumtype(expr * value, expr * expr_left, expr * expr_right);
int expr_conv_ass_type(expr * value, expr * expr_left, expr * expr_right);

int array_cmp(int comb_dims_one, param * ret_one,
              int comb_dims_two, param * ret_two); 
int param_is_num(param * value);
int param_is_dynamic_array(param * value);
int param_expr_array_cmp(param * param_value, expr * expr_value);
int param_expr_cmp(param * param_value, expr * expr_value);
int param_expr_list_cmp(param_list * params, expr_list * list);

int expr_comb_is_enum(expr * value, int * result);
int expr_comb_cmp_and_set(expr * left, expr * right, expr * value, int * result);

int symtab_entry_exists(symtab_entry * entry, unsigned int line_no);
int symtab_add_param_from_basic_param(symtab * tab, param * param_value,
                                  unsigned int syn_level, int * result);
int symtab_add_param_from_param(symtab * tab, param * param_value,
                            unsigned int syn_level, int * result);
int symtab_add_param_from_param_list(symtab * tab, param_list * list,
                                 unsigned int syn_level, int * result);
int symtab_add_param_from_range(symtab * tab, range * value,
                                unsigned int syn_level, int * result);
int symtab_add_param_from_range_list(symtab * tab, range_list * list,
                                     unsigned int syn_level, int * result);
int symtab_add_bind_from_bind(symtab * tab, bind * bind_value,
                              unsigned int syn_level, int * result);
int symtab_add_bind_from_bind_list(symtab * tab, bind_list * list,
                                   unsigned int syn_level, int * result);
int symtab_add_func_from_func(symtab * tab, func * func_value,
                              unsigned int syn_level, int * result);
int symtab_add_func_from_func_list(symtab * tab, func_list * list,
                                   unsigned int syn_level, int * result);

int array_dims_check_type_expr(symtab * tab, expr * value, func * func_value,
                               unsigned syn_level, int * result);
int array_dims_check_type_expr_list(symtab * tab, expr_list * list,
                                    func * func_value, unsigned int syn_level, int * result);

int param_enum_record_check_type(symtab * tab, param * param_value,
                                 unsigned int syn_level, int * result);
int param_range_check_type(symtab * tab, range * value,
                           unsigned int syn_level, int * result);
int param_range_list_check_type(symtab * tab, range_list * list,
                                unsigned int syn_level, int * result);
int param_ffi_check_type(symtab * tab, param * param_value,
                         unsigned int syn_level, int * result);
int param_check_type(symtab * tab, param * param_value,
                     unsigned int syn_level, int * result);
int param_list_ffi_check_type(symtab * tab, param_list * list,
                              unsigned int syn_level, int * result);
int param_list_check_type(symtab * tab, param_list * list,
                          unsigned int syn_level, int * result);                     

int expr_id_check_type(symtab * tab, expr * value, int * result);
int expr_enumtype_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_neg_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_add_sub_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                            int * result);
int expr_mul_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_div_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_mod_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_lgte_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_and_or_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_not_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_bin_not_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_bin_op_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_ass_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                        int * result);
int expr_eq_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result);
int expr_array_deref_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                                int * result);
int expr_slice_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                          int * result);
int expr_range_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                          int * result);
int expr_range_dim_check_type(symtab * tab, expr * value, func * func_value,
                               unsigned int syn_level, int * result);
int expr_range_list_check_type(symtab * tab, expr_list * range_dims, func * func_value, 
                               unsigned int syn_level, int * result);
int expr_call_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                         int * result);
int expr_cond_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                         int * result);
int qualifier_check_type(symtab * tab, qualifier * value, func * func_value, unsigned int syn_level, 
                         int * result);
int qualifier_list_check_type(symtab * tab, qualifier_list * list, func * func_value,
                              unsigned int syn_level, int * result);
int expr_conv_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                         int * result);
int expr_listcomp_check_type(symtab * tab, listcomp * listcomp_value,
                             func * func_value, unsigned int syn_level, int * result);
int expr_attr_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                         int * result);
int expr_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                    int * result);
int expr_list_check_type(symtab * tab, expr_list * list, func * func_value, unsigned int syn_level,
                         int * result);
int expr_seq_check_type(symtab * tab, expr * value, func * func_value, unsigned syn_level,
                        int * result);
int bind_check_type(symtab * tab, bind * value, func * func_value, unsigned int syn_level,
                    int * result);
int bind_list_check_type(symtab * tab, bind_list * list, func * func_value, unsigned int syn_level,
                         int * result);
int except_check_type(symtab * tab, except * value, func * func_value,
                      unsigned int syn_level, int * result);
int except_list_check_type(symtab * tab, except_list * list, func * func_value,
                           unsigned int syn_level, int * result);                      
int func_except_check_type(symtab * tab, func_except * value, func * func_value, 
                           unsigned int syn_level, int * result);
int func_ffi_check_type(symtab * tab, func * func_value, unsigned int syn_level,
                        int * result);
int func_native_check_type(symtab * tab, func * func_value, unsigned int syn_level,
                           int * result);
int func_check_type(symtab * tab, func * func_value, unsigned int syn_level,
                    int * result);
int func_list_check_type(symtab * tab, func_list * list, unsigned int syn_level,
                         int * result);

int never_add_enumerator(enumtype * enumtype_value, enumerator * value, enumerator * enumerator_prev, int * result);
int never_add_enumerator_list(enumtype * enumtype_value, enumerator_list * enums, int * result);
int never_add_enumtype(symtab * stab, enumtype * value, int * result);
int never_add_record(symtab * stab, record * value, int * result);
int never_add_decl(symtab * stab, decl * value, int * result);
int never_add_decl_list(symtab * stab, decl_list * list, int * result);

int never_add_module_decl(module_decl * module_modules, module_decl * module_stdlib, symtab * stab, use * use_value, int * result);
int never_add_use(module_decl * module_modules, module_decl * module_stdlib, symtab * stab, use * value, int * result);
int never_add_use_list(module_decl * module_modules, module_decl * module_stdlib, symtab * stab, use_list * list, int * result);

int enumerator_index_check_type(symtab * stab, enumerator * value, int * result);
int enumerator_item_check_type(symtab * stab, enumtype * enumtype_value, enumerator * value, int * result);
int enumerator_value_check_type(symtab * stab, enumtype * enumtype_value, enumerator * value, int * result);
int enumerator_record_check_type(symtab * stab, enumtype * enumtype_value, enumerator * value, int * result);
int enumerator_check_type(symtab * stab, enumtype * enumtype_value, enumerator * value, int * result);
int enumerator_list_check_type(symtab * stab, enumtype * enumtype_value, enumerator_list * enums, int * result);
int enumtype_check_type(symtab * stab, enumtype * value, int * result);

int record_check_type(symtab * stab, record * record_value, int * result);

int decl_check_type(symtab * stab, decl * value, int * result);
int decl_index_reduce_check_value(symtab * stab, decl * value, int * result);
int decl_list_check_type(symtab * stab, decl_list * list, int * result);
int decl_list_index_reduce_check_value(symtab * stab, decl_list * list, int * result);

int func_entry_check_num_params(param_list * params);
int func_entry_check_type(func * func_value, int * result);
int func_list_entry_check_type(func_list * list, int * result);

int never_check_type(module_decl * module_modules, module_decl * module_stdlib, never * nev, int * result);

int module_decl_check_type(module_decl * modules_modules, module_decl * module_stdlib, module_decl * value, int * result);

int main_check_type(module_decl * module_modules, module_decl * module_stdlib, module_decl * module_nev, int * result);

#endif /* __TYPECHECK_H__ */

