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
#ifndef __CONSTRED_H__
#define __CONSTRED_H__

#include "never.h"

enum
{
    CONSTRED_SUCC = 0,
    CONSTRED_FAIL = 1
};

typedef struct match_guard match_guard;
typedef struct match_guard_list match_guard_list;

int expr_match_guard_constred(match_guard * match_value, int * result);
int expr_match_guard_list_constred(match_guard_list * list, int * result);
int expr_match_constred(expr * value, int * result);
int expr_conv_constred(expr * value, int * result);
int expr_constred(expr * value, int * result);
int expr_list_constred(expr_list * list, int * result);
int seq_constred(seq * value, int * result);
int seq_list_constred(seq_list * list, int * result);
int qualifier_constred(qualifier * value, int * result);
int qualifier_list_constred(qualifier_list * list, int * result);
int listcomp_constred(listcomp * value, int * result);
int array_constred(array * value, int * result);
int bind_constred(bind * value, int * result);
int func_constred_ffi(func * value, int * result);
int func_constred_native(func * value, int * result);
int func_constred(func * value, int * result);
int never_constred(never * nev, int * result);
int module_constred(module_decl * value, int * result);

#endif /* __CONSTRED_H__ */
