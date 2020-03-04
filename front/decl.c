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
#include "decl.h"
#include "enums.h"
#include "record.h"
#include <stdlib.h>

decl * decl_new_enumtype(enumtype * value)
{
    decl * ret = (decl *)malloc(sizeof(decl));
    
    ret->type = DECL_TYPE_ENUMTYPE;
    ret->enumtype_value = value;
    
    return ret;
}

decl * decl_new_record(record * value)
{
    decl * ret = (decl *)malloc(sizeof(decl));
    
    ret->type = DECL_TYPE_RECORD;
    ret->record_value = value;
    
    return ret;
}

void decl_delete(decl * value)
{
    switch (value->type)
    {
        case DECL_TYPE_ENUMTYPE:
            if (value->enumtype_value != NULL)
            {
                enumtype_delete(value->enumtype_value);
            }
        break;
        case DECL_TYPE_RECORD:
            if (value->record_value != NULL)
            {
                record_delete(value->record_value);
            }
        break;
    }
    free(value);
}

decl_list_node * decl_list_node_new(decl * value)
{
    decl_list_node * node =
        (decl_list_node *)malloc(sizeof(decl_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void decl_list_node_delete(decl_list_node * node)
{
    if (node->value)
    {
        decl_delete(node->value);
    }
    free(node);
}

decl_list * decl_list_new()
{
    decl_list * list = (decl_list *)malloc(sizeof(decl_list));

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void decl_list_delete(decl_list * list)
{
    decl_list_node * node = list->tail;

    while (node != NULL)
    {
        decl_list_node * tmp = node->next;
        decl_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void decl_list_add_beg(decl_list * list, decl * value)
{
    decl_list_node * node = decl_list_node_new(value);

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

void decl_list_add_end(decl_list * list, decl * value)
{
    decl_list_node * node = decl_list_node_new(value);

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


