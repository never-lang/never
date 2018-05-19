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
#include <stdlib.h>
#include <stdio.h>
#include "func.h"

func * func_new(char * id, var_list * vars, var * ret, func_body * body)
{
    func * value = (func *)malloc(sizeof(func));
    
    value->id = id;
    value->vars = vars;
    value->ret = ret;
    value->body = body;
    value->freevars = NULL;
    value->stab = NULL;
    value->addr = 0;
    value->line_no = 0;
    
    return value;
}

void func_delete(func * value)
{
    if (value->id)
    {
        free(value->id);
    }
    if (value->vars)
    {
        var_list_delete(value->vars);
    }
    if (value->ret)
    {
        var_delete(value->ret);
    }
    if (value->body)
    {
        func_body_delete(value->body);
    }
    if (value->freevars)
    {
        freevar_list_delete(value->freevars);
    }
    if (value->stab)
    {
        symtab_delete(value->stab);
    }
    
    free(value);
}

void func_print(func * value)
{
    printf("func %s@%u\n", value->id, value->addr);
}

func_body * func_body_new(func_list * funcs, expr * ret)
{
    func_body * body = (func_body *)malloc(sizeof(func_body));
    
    body->funcs = funcs;
    body->ret = ret;
    
    return body;
}

void func_body_delete(func_body * body)
{
    if (body->funcs)
    {
        func_list_delete(body->funcs);
    }
    if (body->ret)
    {
        expr_delete(body->ret);
    }
    free(body);
}

func_list_node * func_list_node_new(func * value)
{
    func_list_node * node = (func_list_node *)malloc(sizeof(func_list_node));
    
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void func_list_node_delete(func_list_node * node)
{
    if (node->value)
    {
        func_delete(node->value);
    }
    free(node);
}

func_list * func_list_new()
{
    func_list * list = (func_list *)malloc(sizeof(func_list));
    
    list->head = NULL;
    list->tail = NULL;
    
    return list;
}

void func_list_delete(func_list * list)
{
    func_list_node * node = list->tail;
    
    while (node != NULL)
    {
        func_list_node * tmp = node->next;
        func_list_node_delete(node);
        node = tmp;
    }
    free(list);    
}

void func_list_add_beg(func_list * list, func * value)
{
    func_list_node * node = func_list_node_new(value);
    
    if (list->head == NULL && list->tail == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->tail->prev = node;
        node->next = list->tail;
        list->tail = node;
    }
}

void func_list_add_end(func_list * list, func * value)
{
    func_list_node * node = func_list_node_new(value);
    
    if (list->head == NULL && list->tail == NULL)
    {
        list->head = list->tail = node;
    }
    else
    {
        list->head->next = node;
        node->prev = list->head;
        list->head = node;
    }
}


