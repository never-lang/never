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
#ifndef __WEAKFUNC_H__
#define __WEAKFUNC_H__

#include "func.h"

typedef struct func_list_weak_node
{
    func * value;
    struct func_list_weak_node * next;
    struct func_list_weak_node * prev;
} func_list_weak_node;

typedef struct func_list_weak
{
    int count;
    struct func_list_weak_node * tail;
    struct func_list_weak_node * head;
} func_list_weak;

func_list_weak_node * func_list_weak_node_new(func * value);
void func_list_weak_node_delete(func_list_weak_node * node);

func_list_weak * func_list_weak_new();
void func_list_weak_delete(func_list_weak * list);

void func_list_weak_add(func_list_weak * list, func * value);
func * func_list_weak_pop(func_list_weak * list);

#endif /* __WEAKFUNC_H__ */
