#ifndef __TCMATCH_H__
#define __TCMATCH_H__

#include "never.h"

typedef struct match_guard_item match_guard_item;
typedef struct match_guard match_guard;
typedef struct match_guard_list match_guard_list;

int expr_comb_is_enum(expr * value, int * result);

int expr_match_guard_item_check_type(symtab * tab, match_guard * match_value,
                                     int * result);
int expr_match_guard_check_type(symtab * tab, match_guard * match_value,
                                func * func_value, unsigned int syn_level, int * result);
int expr_match_guard_list_check_type(symtab * tab, match_guard_list * list,
                                     func * func_value, unsigned int syn_level, int * result);

int expr_match_guard_left_cmp(expr * value, match_guard * match_value, int * result);
int expr_match_guard_list_left_cmp(expr * value, match_guard_list * list, int * result);

int expr_match_guard_exhaustive_item(match_guard * match_value, int * result);
int expr_match_guard_list_last_else(match_guard_list * list);
int expr_match_guard_list_exhaustive_items(match_guard_list * list, int * result);
int expr_match_guard_list_exhaustive(match_guard_list * list, int * result);

int expr_match_guard_list_right_cmp(expr * value, match_guard_list * list, int * result);

int expr_match_check_type(symtab * tab, expr * value, func * func_value,
                          unsigned int syn_level, int * result);

#endif /* __TCMATCH_H__ */
