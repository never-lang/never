/**
 * Copyright 2020 Slawomir Maludzinski
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
#ifndef __ENUMRED_H__
#define __ENUMRED_H__

#include "never.h"

enum
{
    ENUMRED_SUCC = 0,
    ENUMRED_FAIL = 1
};

typedef struct symtab symtab;
typedef struct expr expr;
typedef struct expr_list expr_list;

int expr_enumerator_enumred(expr * value, int * result);
int expr_enumred(expr * value, int * result);
int enumerator_index_enumred(enumtype * enumtype_value, enumerator * value, int * index, int * result);
int enumerator_value_enumred(enumtype * enumtype_value, enumerator * value, int * index, int * result);
int enumerator_record_enumred(enumtype * enumtype_value, enumerator * value, int * index, int * result);
int enumerator_enumred(enumtype * enumtype_value, enumerator * value, int * index, int * result);
int enumerator_list_enumred(enumtype * enumtype_value, enumerator_list * list, int * result);
int enumtype_enumred(enumtype * value, int * result);
int decl_enumred(decl * value, int * result);
int decl_list_enumred(decl_list * list, int * result);

#if 0
int use_enumred(use * value, int * result);
int use_list_enumred(use_list * list, int * result);
int never_enumred(never * nev, int * result);
int module_enumred(module_decl * value, int * result);
#endif

#endif /* __CONSTRED_H__ */
