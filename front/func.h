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
#ifndef __FUNC_H__
#define __FUNC_H__

#include "expr.h"
#include "freevar.h"
#include "symtab.h"
#include "param.h"
#include "bind.h"

typedef struct func_body func_body;
typedef struct func_list func_list;

typedef struct func
{
    char * id;
    int index;
    param_list * params;
    param * ret;
    func_body * body;
    struct freevar_list * freevars;
    struct symtab * stab;
    unsigned int addr;
    unsigned int line_no;
} func;

typedef struct func_body
{
    bind_list * binds;
    func_list * funcs;
    struct expr * ret;
} func_body;

typedef struct func_list_node
{
    func * value;
    struct func_list_node * prev;
    struct func_list_node * next;
} func_list_node;

typedef struct func_list
{
    unsigned int count;
    func_list_node * head;
    func_list_node * tail;
} func_list;

func * func_new(char * id, param_list * params, param * ret, func_body * body);
void func_delete(func * value);

void func_print(func * value);

func_body * func_body_new(bind_list * binds, func_list * funcs, expr_list * ret);
func_body * func_body_new_expr(bind_list * binds, func_list * funcs, expr * ret);
void func_body_delete(func_body * body);

func_list_node * func_list_node_new(func * value);
void func_list_node_delete(func_list_node * node);

func_list * func_list_new();
void func_list_delete(func_list * list);

void func_list_add_beg(func_list * list, func * value);
void func_list_add_end(func_list * list, func * value);

#endif /* __FUNC_H__ */

