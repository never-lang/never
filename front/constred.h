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
#ifndef __CONSTRED_H__
#define __CONSTRED_H__

#include "never.h"

enum
{
    CONSTRED_SUCC = 0,
    CONSTRED_FAIL = 1
};

typedef struct match_guard match_guard;
typedef struct match_guard_list match_guard_list;

int expr_match_guard_constred(match_guard * match_value, int * result);
int expr_match_guard_list_constred(match_guard_list * list, int * result);
int expr_match_constred(expr * value, int * result);

int expr_neg_constred(expr * value, int * result);
int expr_add_constred(expr * value, int * result);
int expr_sub_constred(expr * value, int * result);
int expr_mul_constred(expr * value, int * result);
int expr_div_constred(expr * value, int * result);
int expr_mod_constred(expr * value, int * result);
int expr_lt_constred(expr * value, int * result);
int expr_gt_constred(expr * value, int * result);
int expr_lte_constred(expr * value, int * result);
int expr_gte_constred(expr * value, int * result);
int expr_eq_constred(expr * value, int * result);
int expr_neq_constred(expr * value, int * result);
int expr_and_constred(expr * value, int * result);
int expr_or_constred(expr * value, int * result);
int expr_bin_not_constred(expr * value, int * result);
int expr_bin_and_constred(expr * value, int * result);
int expr_bin_or_constred(expr * value, int * result);
int expr_bin_xor_constred(expr * value, int * result);
int expr_bin_shl_constred(expr * value, int * result);
int expr_bin_shr_constred(expr * value, int * result);
int expr_pipel_constred(expr * value, int * result);
int expr_not_constred(expr * value, int * result);
int expr_sup_constred(expr * value, int * result);
int expr_cond_constred(expr * value, int * result);
int expr_array_constred(expr * value, int * result);
int expr_array_deref_constred(expr * value, int * result);
int expr_call_constred(expr * value, int * result);
int expr_func_constred(expr * value, int * result);
int expr_range_dim_constred(expr * value, int * result);
int expr_range_constred(expr * value, int * result);
int expr_slice_constred(expr * value, int * result);
int expr_seq_constred(expr * value, int * result);
int expr_ass_constred(expr * value, int * result);
int expr_while_constred(expr * value, int * result);
int expr_for_constred(expr * value, int * result);
int expr_forin_constred(expr * value, int * result);
int expr_iflet_constred(expr * value, int * result);
int expr_build_in_constred(expr * value, int * result);
int expr_conv_constred(expr * value, int * result);
int expr_listcomp_constred(expr * value, int * result);
int expr_attr_constred(expr * value, int * result);
int expr_touple_constred(expr * value, int * result);

int expr_constred(expr * value, int * result);
int expr_list_constred(expr_list * list, int * result);
int seq_constred(seq * value, int * result);
int seq_list_constred(seq_list * list, int * result);
int qualifier_constred(qualifier * value, int * result);
int qualifier_list_constred(qualifier_list * list, int * result);
int listcomp_constred(listcomp * value, int * result);
int array_constred(array * value, int * result);
int bind_constred(bind * value, int * result);
int func_constred_native(func * value, int * result);
int func_constred(func * value, int * result);
int never_constred(never * nev, int * result);
int module_constred(module_decl * value, int * result);

#endif /* __CONSTRED_H__ */
