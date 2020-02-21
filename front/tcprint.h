#ifndef __TCPRINT_H__
#define __TCPRINT_H__

#include "never.h"

typedef struct qualifier qualifier;
typedef struct qualifier_list qualifier_list;
typedef struct match_guard_item match_guard_item;
typedef struct match_guard match_guard;
typedef struct match_guard_list match_guard_list;

int print_func_expr(expr * value, int depth);
int print_func_expr_list(expr_list * list, int depth);
int print_func_qualifier(qualifier * value, int depth);
int print_func_qualifier_list(qualifier_list * list, int depth);
int print_func_listcomp(listcomp * value, int depth);
int print_func_array(array * value, int depth);
int print_func_bind(bind * value, int depth);
int print_func_bind_list(bind_list * list, int depth);
int print_func_except(except * value, int depth);
int print_func_except_list(except_list * list, int depth);
int print_func_attr(expr * value, int depth);
int print_func_ffi(func * value, int depth);
int print_func_match_guard(match_guard * value, int depth);
int print_func_match_guard_list(match_guard_list * list, int depth);
int print_func_match_expr(expr * value, int depth);
int print_func_native(func * value, int depth);
int print_func(func * value, int depth);
int print_func_list(func_list * list, int depth);
int print_functions(never * nev);

#endif /* __TCPRINT_H__ */

