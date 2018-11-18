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
#include "record.h"
#include "param.h"
#include "symtab.h"
#include <stdlib.h>
#include <assert.h>

record * record_new(char * id, param_list * params)
{
    record * value = (record *)malloc(sizeof(record));
    
    value->id = id;
    value->params = params;
    value->stab = symtab_new(32, SYMTAB_TYPE_FUNC, NULL);
    
    return value;
}

void record_delete(record * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    if (value->params != NULL)
    {
        param_list_delete(value->params);
    }
    if (value->stab != NULL)
    {
        symtab_delete(value->stab);
    }

    free(value);
}

param * record_find_param(record * record_value, char * id)
{
    param * ret = NULL;
    symtab_entry * entry = NULL;

    if (record_value->stab != NULL && id != NULL)
    {    
        entry = symtab_lookup(record_value->stab, id, SYMTAB_LOOKUP_LOCAL);
    }

    if (entry != NULL && entry->type == SYMTAB_PARAM)
    {
        ret = entry->param_value;
    }
    else if (entry != NULL && entry->type != SYMTAB_PARAM)
    {
        assert(0);
    }

    return ret;
}

record_list_node * record_list_node_new(record * value)
{
    record_list_node * node =
        (record_list_node *)malloc(sizeof(record_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void record_list_node_delete(record_list_node * node)
{
    if (node->value)
    {
        record_delete(node->value);
    }
    free(node);
}

record_list * record_list_new()
{
    record_list * list = (record_list *)malloc(sizeof(record_list));

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void record_list_delete(record_list * list)
{
    record_list_node * node = list->tail;

    while (node != NULL)
    {
        record_list_node * tmp = node->next;
        record_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void record_list_add_beg(record_list * list, record * value)
{
    record_list_node * node = record_list_node_new(value);

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

void record_list_add_end(record_list * list, record * value)
{
    record_list_node * node = record_list_node_new(value);

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

