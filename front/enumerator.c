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
#include "enumerator.h"
#include <stdlib.h>

enumerator * enumerator_new(char * id)
{
    enumerator * value = (enumerator *)malloc(sizeof(enumerator));
    
    value->id = id;
    value->index = 0;
    value->line_no = 0;
    
    return value;
}

void enumerator_delete(enumerator * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    free(value);
}

enumerator_list_node * enumerator_list_node_new(enumerator * value)
{
    enumerator_list_node * node =
        (enumerator_list_node *)malloc(sizeof(enumerator_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void enumerator_list_node_delete(enumerator_list_node * node)
{
    if (node->value)
    {
        enumerator_delete(node->value);
    }
    free(node);
}

enumerator_list * enumerator_list_new()
{
    enumerator_list * list = (enumerator_list *)malloc(sizeof(enumerator_list));

    list->head = NULL;
    list->tail = NULL;

    return list;
}

void enumerator_list_delete(enumerator_list * list)
{
    enumerator_list_node * node = list->tail;

    while (node != NULL)
    {
        enumerator_list_node * tmp = node->next;
        enumerator_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void enumerator_list_add_beg(enumerator_list * list, enumerator * value)
{
    enumerator_list_node * node = enumerator_list_node_new(value);

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

void enumerator_list_add_end(enumerator_list * list, enumerator * value)
{
    enumerator_list_node * node = enumerator_list_node_new(value);

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

