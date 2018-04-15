#ifndef __TYPECHECK_H__
#define __TYPECHECK_H__

#include "never.h"

int symtab_add_arg_from_arg_list(symtab * tab, arg_list * list);
int symtab_add_func_from_func_list(symtab * tab, func_list * list);
int symtab_add_entry_func_list(func_list * list, symtab * stab_parent);
int symtab_add_entry_never(never * nev);

int print_symtabs_func_list(func_list * list);
int print_symtabs(never * nev);

int print_func_list(func_list * list, int depth);
int print_functions(never * nev);

#endif /* __TYPECHECK_H__ */

