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
#ifndef __TYPES_H__
#define __TYPES_H__

#include "array.h"
#include "dim.h"
#include "expr.h"
#include "param.h"
#include "bind.h"
#include "func.h"
#include "never.h"
#include "except.h"
#include "listcomp.h"
#include "record.h"
#include "enumerator.h"
#include "enums.h"
#include "match.h"

typedef union token_value {
    int int_value;
    float float_value;
    char char_value;
    char * str_value;
    expr * expr_value;
    expr_list * expr_list_value;
    expr_list * expr_seq_value;
    param * param_value;
    param_list * param_list_value;
    dim * dim_value;
    dim_list * dim_list_value;
    array * array_value;
    listcomp * listcomp_value;
    qualifier * qualifier_value;
    qualifier_list * qualifier_list_value;
    bind * let_value;
    bind * var_value;
    bind * bind_value;
    bind_list * bind_list_value;
    match_guard * match_guard_value;
    match_guard_list * match_guard_list_value;
    func_decl * func_decl_value;
    func * func_value;
    func_list * func_list_value;
    func_body * func_body_value;
    func_except * func_except_value;
    except * except_value;
    except_list * except_list_value;
    enumerator * enumerator_value;
    enumerator_list * enumerator_list_value;
    enumtype * enumtype_value;
    record * record_value;
    decl * decl_value;
    decl_list * decl_list_value;
    never * never_value;
} token_value;

typedef struct token
{
    int line_no;
    token_value val;
} token;

#define YYSTYPE token

#endif /* __TYPES_H__ */
