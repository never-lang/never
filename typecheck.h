#ifndef __TYPECHECK_H__
#define __TYPECHECK_H__

#include "never.h"

int expr_check_undefined_ids(symtab * tab, expr * value);
int expr_list_check_undefined_ids(symtab * tab, expr_list * list);
int func_check_undefined_ids(symtab * tab, func * func_value);
int func_list_check_undefined_ids(symtab * tab, func_list * list);
int never_check_undefined_ids(never * nev);

int symtab_add_arg_from_arg_list(symtab * tab, arg_list * list);
int symtab_add_func_from_func_list(symtab * tab, func_list * list);
int symtab_add_entry_func(symtab * stab_parent, func * func_value);
int symtab_add_entry_func_list(symtab * stab_parent, func_list * list);
int symtab_add_entry_never(never * nev);

int print_symtabs_expr(expr * value);
int print_symtabs_func(func * func_value);
int print_symtabs_func_list(func_list * list);
int print_symtabs(never * nev);

int print_func_list(func_list * list, int depth);
int print_functions(never * nev);

#endif /* __TYPECHECK_H__ */

