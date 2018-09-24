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
#include "weakexpr.h"
#include <stdlib.h>

expr_list_weak_node * expr_list_weak_node_new(expr * value, int distance)
{
    expr_list_weak_node * node =
        (expr_list_weak_node *)malloc(sizeof(expr_list_weak_node));

    node->value = value;
    node->distance = distance;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

void expr_list_weak_node_delete(expr_list_weak_node * node) { free(node); }

expr_list_weak * expr_list_weak_new()
{
    expr_list_weak * list = (expr_list_weak *)malloc(sizeof(expr_list_weak));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void expr_list_weak_delete(expr_list_weak * list)
{
    expr_list_weak_node * node = list->tail;
    while (node != NULL)
    {
        expr_list_weak_node * tmp = node->next;
        expr_list_weak_node_delete(node);
        node = tmp;
    }
    free(list);
}

void expr_list_weak_add(expr_list_weak * list, expr * value, int distance)
{
    expr_list_weak_node * node = expr_list_weak_node_new(value, distance);

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

expr_list_weak_node * expr_list_weak_pop(expr_list_weak * list)
{
    expr_list_weak_node * head = NULL;

    head = list->head;
    if (head != NULL)
    {
        list->head = head->prev;
        if (list->head == NULL)
        {
            list->tail = NULL;
        }
        else
        {
            list->head->next = NULL;
        }
        list->count--;
    }

    return head;
}
