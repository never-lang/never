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
#ifndef __TCMATCH_H__
#define __TCMATCH_H__

#include "never.h"

typedef struct match_guard_item match_guard_item;
typedef struct match_guard_item_expr match_guard_item_expr;
typedef struct match_guard_record match_guard_record;
typedef struct match_guard_record_expr match_guard_record_expr;
typedef struct match_guard match_guard;
typedef struct match_guard_list match_guard_list;

int symtab_add_matchbind_from_matchbind(symtab * tab, matchbind * matchbind_value,
                                        param * param_value, enumerator * enumerator_value,
                                        enumtype * enumtype_value, unsigned int syn_level, int * result);
int symtab_add_matchbind_from_matchbind_list(symtab * tab, match_guard_record * guard_record,
                                             unsigned int syn_level, int * result);

int expr_match_guard_item_check_type(symtab * tab, match_guard_item * guard_item,
                                     int * result);
int expr_match_guard_record_check_type(symtab * tab, match_guard_record * guard_item,
                                       int * result);
int expr_match_guard_check_type(symtab * tab, match_guard * match_value,
                                func * func_value, unsigned int syn_level, int * result);
int expr_match_guard_list_check_type(symtab * tab, match_guard_list * list,
                                     func * func_value, unsigned int syn_level, int * result);

int expr_match_guard_left_cmp(expr * value, match_guard * match_value, int * result);
int expr_match_guard_list_left_cmp(expr * value, match_guard_list * list, int * result);

unsigned int expr_match_gaurd_list_last_cnt(match_guard_list * list);
int expr_match_guard_list_last_else(match_guard_list * list);

int expr_match_guard_mark_item(match_guard * match_value);
int expr_match_guard_list_mark_items(expr * value, match_guard_list * list);
int expr_match_guard_unmark_items(expr * value);
int expr_match_guard_are_all_mark_items(expr * value, int * result);
int expr_match_guard_list_exhaustive(expr * value, match_guard_list * list, int * result);

int expr_match_guard_list_right_cmp(expr * value, match_guard_list * list, int * result);

int expr_match_check_type(symtab * tab, expr * value, func * func_value,
                          unsigned int syn_level, int * result);

#endif /* __TCMATCH_H__ */

