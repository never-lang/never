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
#include "bind.h"
#include <stdlib.h>
#include <stdio.h>

bind * bind_new_let(char * id, expr * expr_value)
{
    bind * value = (bind *)malloc(sizeof(bind));

    value->type = BIND_LET;
    value->id = id;
    value->expr_value = expr_value;
    value->index = 0;
    value->line_no = 0;

    return value;
}

bind * bind_new_var(char * id, expr * expr_value)
{
    bind * value = (bind *)malloc(sizeof(bind));

    value->type = BIND_VAR;
    value->id = id;
    value->expr_value = expr_value;
    value->index = 0;
    value->line_no = 0;

    return value;
}

void bind_delete(bind * value)
{
    if (value->id)
    {
        free(value->id);
    }
    if (value->expr_value)
    {
        expr_delete(value->expr_value);
    }

    free(value);
}

#if 0
TODO : remove bind_list
int bind_list_enum(bind_list * list, int start)
{
    int index = start;

    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * value = node->value;
        if (value != NULL)
        {
            value->index = index++;
        }
        node = node->next;
    }

    return 0;
}

bind_list_node * bind_list_node_new(bind * value)
{
    bind_list_node * node = (bind_list_node *)malloc(sizeof(bind_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void bind_list_node_delete(bind_list_node * node)
{
    if (node->value)
    {
        bind_delete(node->value);
    }
    free(node);
}

bind_list * bind_list_new()
{
    bind_list * list = (bind_list *)malloc(sizeof(bind_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void bind_list_delete(bind_list * list)
{
    bind_list_node * node = list->tail;

    while (node != NULL)
    {
        bind_list_node * tmp = node->next;
        bind_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void bind_list_add_beg(bind_list * list, bind * value)
{
    bind_list_node * node = bind_list_node_new(value);

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

void bind_list_add_end(bind_list * list, bind * value)
{
    bind_list_node * node = bind_list_node_new(value);

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
#endif

void bind_print(bind * value)
{
    printf("bind %s\n", value->id);
}



