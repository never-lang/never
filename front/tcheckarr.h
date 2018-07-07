#ifndef __TCHECKARR_H__
#define __TCHECKARR_H__

#include "expr.h"
#include "symtab.h"
#include "weakexpr.h"

int array_depth_list_well_formed(expr * expr_value, expr_list_weak * depth_list, int * result);
int array_set_dims(expr_list_weak * depth_list);
int array_well_formed(expr * value, int * result);

int array_check_type(symtab * tab, expr * value, int * result);
int expr_array_check_type(symtab * tab, expr * value, int * result);

#endif /* __TCHECKARR_H__ */

