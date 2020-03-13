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
#include "matchbind.h"
#include <stdlib.h>
#include <stdio.h>

matchbind * matchbind_new(char * value)
{
    matchbind * ret = (matchbind *)malloc(sizeof(matchbind));
    
    ret->id = value;
    ret->param_value = NULL;
    ret->enumerator_value = NULL;
    ret->enumtype_value = NULL;
    ret->index = -1;
    ret->stack_level = 0;
    ret->line_no = 0;
    
    return ret;
}

void matchbind_delete(matchbind * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    free(value);
}

void matchbind_print(matchbind * value)
{
    printf("matchbind %s\n", value->id);
}

matchbind_list_node * matchbind_list_node_new(matchbind * value)
{
    matchbind_list_node * node =
        (matchbind_list_node *)malloc(sizeof(matchbind_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void matchbind_list_node_delete(matchbind_list_node * node)
{
    if (node->value)
    {
        matchbind_delete(node->value);
    }
    free(node);
}

matchbind_list * matchbind_list_new()
{
    matchbind_list * list = (matchbind_list *)malloc(sizeof(matchbind_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void matchbind_list_delete(matchbind_list * list)
{
    matchbind_list_node * node = list->tail;

    while (node != NULL)
    {
        matchbind_list_node * tmp = node->next;
        matchbind_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void matchbind_list_add_beg(matchbind_list * list, matchbind * value)
{
    matchbind_list_node * node = matchbind_list_node_new(value);

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

void matchbind_list_add_end(matchbind_list * list, matchbind * value)
{
    matchbind_list_node * node = matchbind_list_node_new(value);

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

void matchbind_list_set_stack_level(matchbind_list * list, int stack_level)
{
    matchbind_list_node * node = list->tail;
    
    while (node != NULL)
    {
        matchbind * value = node->value;
        if (value != NULL)
        {
            value->stack_level = stack_level;
        }
        node = node->next;
    }
}


