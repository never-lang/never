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
#include "ids.h"
#include <stdlib.h>

id * id_new(char * value)
{
    id * ret = (id *)malloc(sizeof(id));
    
    ret->id = value;
    ret->line_no = 0;
    
    return ret;
}

void id_delete(id * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    free(value);
}

id_list_node * id_list_node_new(id * value)
{
    id_list_node * node =
        (id_list_node *)malloc(sizeof(id_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void id_list_node_delete(id_list_node * node)
{
    if (node->value)
    {
        id_delete(node->value);
    }
    free(node);
}

id_list * id_list_new()
{
    id_list * list = (id_list *)malloc(sizeof(id_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void id_list_delete(id_list * list)
{
    id_list_node * node = list->tail;

    while (node != NULL)
    {
        id_list_node * tmp = node->next;
        id_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void id_list_add_beg(id_list * list, id * value)
{
    id_list_node * node = id_list_node_new(value);

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

void id_list_add_end(id_list * list, id * value)
{
    id_list_node * node = id_list_node_new(value);

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

