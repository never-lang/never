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
#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "expr.h"
#include "param.h"
#include "weakexpr.h"

typedef enum array_type
{
    ARRAY_UNKNOWN = 0,
    ARRAY_INIT = 1,
    ARRAY_SUB = 2,
    ARRAY_DIMS = 3
} array_type;

typedef struct array
{
    array_type type;
    expr_list * elements;
    expr_list * dims;
    param * ret;
    unsigned int line_no;
} array;

array * array_new(expr_list * elements, param * ret);
array * array_new_sub(expr_list * elements);
array * array_new_dims(expr_list * dims, param * ret);

void array_delete(array * value);

int elements_to_depth_list(expr * value, expr_list_weak * bfs_list,
                           int distance);
int array_to_depth_list(expr * value, expr_list_weak * depth_list);

#endif /* __ARRAY_H__ */
