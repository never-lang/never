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
#include "dim.h"
#include "param.h"
#include <stdlib.h>

dim * dim_new(char * id)
{
    dim * value = malloc(sizeof(dim));

    value->id = param_new_int(id);
    value->line_no = 0;

    return value;
}

void dim_delete(dim * value)
{
    if (value->id != NULL)
    {
        param_delete(value->id);
    }
    free(value);
}

dim_list_node * dim_list_node_new(dim * value)
{
    dim_list_node * node = (dim_list_node *)malloc(sizeof(dim_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void dim_list_node_delete(dim_list_node * node)
{
    if (node->value != NULL)
    {
        dim_delete(node->value);
    }
    free(node);
}

dim_list * dim_list_new()
{
    dim_list * list = malloc(sizeof(dim_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void dim_list_delete(dim_list * list)
{
    dim_list_node * node = list->tail;

    while (node != NULL)
    {
        dim_list_node * tmp = node->next;
        dim_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void dim_list_add_beg(dim_list * list, dim * value)
{
    dim_list_node * node = dim_list_node_new(value);

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

void dim_list_add_end(dim_list * list, dim * value)
{
    dim_list_node * node = dim_list_node_new(value);

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
