#ifndef __GENCODE_H__
#define __GENCODE_H__

#include "never.h"

int expr_gencode(symtab * tab, expr * value, int * result);
int expr_list_gencode(symtab * tab, expr_list * list, int * result);
int func_gencode(symtab * tab, func * func_value, int * result);
int func_list_gencode(symtab * tab, func_list * list, int * result);
int never_gencode(never * nev);

#endif /* __GENCODE_H__ */


