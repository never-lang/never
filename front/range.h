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
#ifndef __RANGE_H__
#define __RANGE_H__

#include "param.h"

typedef struct range
{
    param * from;
    param * to;
    unsigned int line_no;
} range;

typedef struct range_list_node
{
    range * value;
    struct range_list_node * prev;
    struct range_list_node * next;
} range_list_node;

typedef struct range_list
{
    unsigned int count;
    range_list_node * head;
    range_list_node * tail;
} range_list;

range * range_new(param * from, param * to);
void range_delete(range * value);
void range_init_const(range * value, param_const_type const_type);

range_list_node * range_list_node_new(range * value);
void range_list_node_delete(range_list_node * node);

range_list * range_list_new();
void range_list_delete(range_list * list);

void range_list_add_beg(range_list * list, range * value);
void range_list_add_end(range_list * list, range * value);

void range_dim_set_range(range_list * list, param * range_value);
void range_dim_set_slice(range_list * list, param * slice_value);
void range_list_init_const(range_list * list, param_const_type const_type);

#endif /* __RANGE_H__ */
