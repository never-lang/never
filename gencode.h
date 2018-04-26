#ifndef __GENCODE_H__
#define __GENCODE_H__

#include "never.h"

int expr_id_gencode(unsigned int syn_level, func * func_value, expr * value, int * result);
int expr_gencode(unsigned int syn_level, func * func_value, expr * value, int * result);
int expr_list_gencode(unsigned int syn_level, func * func_value, expr_list * list, int * result);

int func_gencode(unsigned int syn_level, func * func_value, int * result);
int func_list_gencode(unsigned int syn_level, func_list * list, int * result);

int never_gencode(never * nev);

#endif /* __GENCODE_H__ */


