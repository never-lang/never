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

#include "expr.h"
#include "var.h"
#include "array.h"
#include "func.h"
#include "dim.h"
#include "never.h"

typedef union token_value
{
    int int_value;
    float float_value;
    char * str_value;
    expr * expr_value;
    expr_list * expr_list_value;
    var * var_value;
    var_list * var_list_value;
    dim * dim_value;
    dim_list * dim_list_value;
    array * array_value;
    func * func_value;
    func_list * func_list_value;
    func_body * func_body_value;
    never * never_value;
} token_value;

typedef struct token
{
    int line_no;
    token_value val;
} token;

#define YYSTYPE token

#endif /* __TYPES_H__ */

