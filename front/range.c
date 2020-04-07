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
#include "range.h"
#include "param.h"
#include <stdlib.h>

range * range_new(param * from, param * to)
{
    range * value = malloc(sizeof(range));

    value->from = from;
    value->to = to;
    value->line_no = 0;

    return value;
}

void range_delete(range * value)
{
    if (value->from != NULL)
    {
        param_delete(value->from);
    }
    if (value->to != NULL)
    {
        param_delete(value->to);
    }
    free(value);
}

range_list_node * range_list_node_new(range * value)
{
    range_list_node * node = (range_list_node *)malloc(sizeof(range_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void range_list_node_delete(range_list_node * node)
{
    if (node->value != NULL)
    {
        range_delete(node->value);
    }
    free(node);
}

range_list * range_list_new()
{
    range_list * list = malloc(sizeof(range_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void range_list_delete(range_list * list)
{
    range_list_node * node = list->tail;

    while (node != NULL)
    {
        range_list_node * tmp = node->next;
        range_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void range_list_add_beg(range_list * list, range * value)
{
    range_list_node * node = range_list_node_new(value);

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

void range_list_add_end(range_list * list, range * value)
{
    range_list_node * node = range_list_node_new(value);

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
