/**
 * Copyright 2018 Slawomir Maludzinski
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
#ifndef __GENCODE_H__
#define __GENCODE_H__

#include "module.h"
#include "never.h"
#include "weakfunc.h"

enum
{
    GENCODE_SUCC = 0,
    GENCODE_FAIL = 1
};

int expr_id_gencode(unsigned int syn_level, func * func_value, symtab * tab,
                    expr * value, int * result);

int func_gencode_freevars_freevar(func * func_value, symtab * stab, freevar * freevar_value,
                                  int * result);
int func_gencode_freevars_expr(func * func_value, symtab * stab, expr * value, int * result);
int func_gencode_freevars_expr_list(func * func_value, symtab * stab, expr_list * list,
                                    int * result);
int func_gencode_freevars_qualifier(func * func_value, symtab * stab,
                                    qualifier * value, int * result);
int func_gencode_freevars_qualifier_list(func * func_value, symtab * stab,
                                         qualifier_list * list, int * result);
int func_gencode_freevars_listcomp(func * func_value, symtab * stab,
                                   listcomp * value, int * result);
int func_gencode_freevars_array(func * func_value, symtab * stab, array * array_value,
                                int * result);
int func_gencode_freevars_bind(func * func_value, symtab * stab, bind * bind_value,
                               int * result);
int func_gencode_freevars_bind_list(func * func_value, symtab * stab, bind_list * list,
                                    int * result);
int func_gencode_freevars_bind_list(func * func_value, symtab * stab, bind_list * list,
                                    int * result);
int func_gencode_freevars_except(func * func_value, symtab * stab, except * except_value,
                                 int * result);
int func_gencode_freevars_except_list(func * func_value, symtab * stab, except_list * list,
                                      int * result);
int func_gencode_freevars_func_except(func * func_value, symtab * stab, func_except * value,
                                 int * result);
int func_gencode_freevars_func(func * func_value, symtab * stab, func * subfunc_value,
                               int * result);
int func_gencode_freevars_func_list(func * func_value, symtab * stab, func_list * list,
                                    int * result);
int func_gencode_freevars(func * func_value, symtab * stab, int * result);

#endif /* __GENCODE_H__ */
