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
#include "freevar.h"
#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

freevar * freevar_new(char * id, int index)
{
    freevar * value = malloc(sizeof(freevar));

    value->type = FREEVAR_UNKNOWN;
    value->id = id;
    value->index = index;
    value->local_value = NULL;
    value->global_value = NULL;

    return value;
}

void freevar_delete(freevar * value) { free(value); }

freevar_list_node * freevar_list_node_new(freevar * value)
{
    freevar_list_node * node =
        (freevar_list_node *)malloc(sizeof(freevar_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void freevar_list_node_delete(freevar_list_node * node)
{
    if (node->value != NULL)
    {
        freevar_delete(node->value);
    }
    free(node);
}

freevar_list * freevar_list_new()
{
    freevar_list * list = (freevar_list *)malloc(sizeof(freevar_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void freevar_list_delete(freevar_list * list)
{
    freevar_list_node * node = list->tail;

    while (node != NULL)
    {
        freevar_list_node * tmp = node->next;
        freevar_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

freevar * freevar_list_add(freevar_list * list, char * id)
{
    freevar * freevar_value = NULL;
    freevar_list_node * node = list->tail;

    while (node != NULL)
    {
        freevar * value = node->value;
        if (value != NULL && strcmp(value->id, id) == 0)
        {
            return value;
        }
        node = node->next;
    }

    freevar_value = freevar_new(id, list->count);
    freevar_list_add_end(list, freevar_value);

    return freevar_value;
}

void freevar_list_add_beg(freevar_list * list, freevar * value)
{
    freevar_list_node * node = freevar_list_node_new(value);

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

void freevar_list_add_end(freevar_list * list, freevar * value)
{
    freevar_list_node * node = freevar_list_node_new(value);

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

void freevar_print(freevar * value)
{
    printf("freevar %s %d %s\n", freevar_type_str(value->type), value->index,
           value->id);

    switch (value->type)
    {
    case FREEVAR_LOCAL:
        if (value->local_value)
        {
            var_print(value->local_value);
        }
        break;
    case FREEVAR_GLOBAL:
        if (value->global_value)
        {
            freevar_print(value->global_value);
        }
        break;
    case FREEVAR_FUNC:
        if (value->func_value)
        {
            func_print(value->func_value);
        }
        break;
    }
}

void freevar_list_print(freevar_list * list)
{
    freevar_list_node * node = list->tail;
    while (node != NULL)
    {
        freevar * value = node->value;
        if (value != NULL)
        {
            freevar_print(value);
        }
        node = node->next;
    }
}

char * freevar_type_str(int type)
{
    switch (type)
    {
    case FREEVAR_UNKNOWN:
        return "FREEVAR_UNKNOWN";
    case FREEVAR_LOCAL:
        return "FREEVAR_LOCAL";
    case FREEVAR_GLOBAL:
        return "FREEVAR_GLOBAL";
    case FREEVAR_FUNC:
        return "FREEVAR_FUNC";
    }
    return "FREEVAR_???";
}
