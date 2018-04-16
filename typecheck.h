#ifndef __TYPECHECK_H__
#define __TYPECHECK_H__

#include "never.h"

#define TYPECHECK_SUCC 0
#define TYPECHECK_FAIL 1

int expr_check_type(symtab * tab, expr * value, int * result);
int expr_list_check_type(symtab * tab, expr_list * list, int * result);
int func_check_type(symtab * tab, func * func_value, int * result);
int func_list_check_type(symtab * tab, func_list * list, int * result);
int never_check_type(never * nev, int * result);

int expr_call_check_call(symtab * tab, expr * value, int * result);
int expr_check_call(symtab * tab, expr * value, int * result);
int expr_list_check_call(symtab * tab, expr_list * list, int * result);
int func_check_call(symtab * tab, func * func_value, int * result);
int func_list_check_call(symtab * tab, func_list * list, int * result);
int never_check_func_call(never * nev, int * result);

int expr_check_undefined_ids(symtab * tab, expr * value, int * result);
int expr_list_check_undefined_ids(symtab * tab, expr_list * list, int * result);
int func_check_undefined_ids(symtab * tab, func * func_value, int * result);
int func_list_check_undefined_ids(symtab * tab, func_list * list, int * result);
int never_check_undefined_ids(never * nev, int * result);

int symtab_add_arg_from_arg_list(symtab * tab, arg_list * list, int * result);
int symtab_add_func_from_func_list(symtab * tab, func_list * list, int * result);
int symtab_add_entry_func(symtab * stab_parent, func * func_value, int * result);
int symtab_add_entry_func_list(symtab * stab_parent, func_list * list, int * result);
int symtab_add_entry_never(never * nev, int * result);

int print_symtabs_expr(expr * value);
int print_symtabs_func(func * func_value);
int print_symtabs_func_list(func_list * list);
int print_symtabs(never * nev);

int print_func_list(func_list * list, int depth);
int print_functions(never * nev);

#endif /* __TYPECHECK_H__ */

