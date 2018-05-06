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

int never_sem_check(never * nev);

#endif /* __TYPECHECK_H__ */

