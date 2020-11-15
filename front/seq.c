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
#include "seq.h"
#include "bind.h"
#include "func.h"
#include "expr.h"
#include "symtab.h"
#include <stdlib.h>

seq_item * seq_item_new_bind(bind * bind_value)
{
    seq_item * ret = (seq_item *)malloc(sizeof(seq_item));

    ret->type = SEQ_TYPE_BIND;
    ret->bind_value = bind_value;
    ret->line_no = 0;

    return ret;
}

seq_item * seq_item_new_func(func * func_value)
{
    seq_item * ret = (seq_item *)malloc(sizeof(seq_item));

    ret->type = SEQ_TYPE_FUNC;
    ret->func_value = func_value;
    ret->line_no = 0;

    return ret;
}

seq_item * seq_item_new_expr(expr * expr_value)
{
    seq_item * ret = (seq_item *)malloc(sizeof(seq_item));

    ret->type = SEQ_TYPE_EXPR;
    ret->expr_value = expr_value;
    ret->line_no = 0;

    return ret;
}

void seq_item_delete(seq_item * value)
{
    switch (value->type)
    {
        case SEQ_TYPE_BIND:
            if (value->bind_value)
            {
                bind_delete(value->bind_value);
            }
        break;
        case SEQ_TYPE_FUNC:
            if (value->func_value)
            {
                func_delete(value->func_value);
            }
        break;
        case SEQ_TYPE_EXPR:
           if (value->expr_value)
            {
                expr_delete(value->expr_value);
            }
        break;
        case SEQ_TYPE_UNKNOWN:
        break;
    }
    free(value);
}

seq_list_node * seq_list_node_new(seq_item * value)
{
    seq_list_node * node = (seq_list_node *)malloc(sizeof(seq_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void seq_list_node_delete(seq_list_node * node)
{
    if (node->value)
    {
        seq_item_delete(node->value);
    }
    free(node);    
}

seq_list * seq_list_new()
{
    seq_list * list = (seq_list *)malloc(sizeof(seq_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void seq_list_delete(seq_list * list)
{
    seq_list_node * node = list->tail;

    while (node != NULL)
    {
        seq_list_node * tmp = node->next;
        seq_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

seq * seq_new(seq_list * list)
{
    seq * ret = (seq *)malloc(sizeof(seq));

    ret->list = list;
    ret->stab = NULL;
    ret->line_no = 0;

    return ret;
}

void seq_delete(seq * value)
{
    if (value->list)
    {
        seq_list_delete(value->list);
    }
    if (value->stab)
    {
        symtab_delete(value->stab);
    }
    free(value);
}

const char * seq_type_str(seq_type type)
{
    switch (type)
    {
        case SEQ_TYPE_BIND: return "seq bind";
        case SEQ_TYPE_FUNC: return "seq func";
        case SEQ_TYPE_EXPR: return "seq expr";
        case SEQ_TYPE_UNKNOWN: return "seq unknown";
    }
    return "seq unknown";
}

void seq_list_add_beg(seq_list * list, seq_item * value)
{
    seq_list_node * node = seq_list_node_new(value);

    list->count++;
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

void seq_list_add_end(seq_list * list, seq_item * value)
{
    seq_list_node * node = seq_list_node_new(value);

    list->count++;
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
