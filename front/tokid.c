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
#include "tokid.h"
#include <stdlib.h>

tokid * tokid_new(char * id)
{
    tokid * value = (tokid *)malloc(sizeof(tokid));
    
    value->id = id;
    value->index = 0;
    value->line_no = 0;
    
    return value;
}

void tokid_delete(tokid * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    free(value);
}

tokid_list_node * tokid_list_node_new(tokid * value)
{
    tokid_list_node * node =
        (tokid_list_node *)malloc(sizeof(tokid_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void tokid_list_node_delete(tokid_list_node * node)
{
    if (node->value)
    {
        tokid_delete(node->value);
    }
    free(node);
}

tokid_list * tokid_list_new()
{
    tokid_list * list = (tokid_list *)malloc(sizeof(tokid_list));

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void tokid_list_delete(tokid_list * list)
{
    tokid_list_node * node = list->tail;

    while (node != NULL)
    {
        tokid_list_node * tmp = node->next;
        tokid_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void tokid_list_add_beg(tokid_list * list, tokid * value)
{
    tokid_list_node * node = tokid_list_node_new(value);

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

void tokid_list_add_end(tokid_list * list, tokid * value)
{
    tokid_list_node * node = tokid_list_node_new(value);

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

