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
#include "match.h"
#include "expr.h"
#include <stdlib.h>

match_guard * match_guard_new_item(char * enum_id, char * item_id, expr * expr_value)
{
    match_guard * ret = malloc(sizeof(match_guard));
    
    ret->type = MATCH_GUARD_ITEM;
    ret->line_no = 0;
    ret->guard_item.enum_id = enum_id;
    ret->guard_item.item_id = item_id;
    ret->guard_item.enumtype_value = NULL;
    ret->guard_item.enumerator_value = NULL;
    ret->guard_item.expr_value = expr_value;
        
    return ret;
}

match_guard * match_guard_new_else(expr * expr_value)
{
    match_guard * ret = malloc(sizeof(match_guard));
    
    ret->type = MATCH_GUARD_ELSE;
    ret->line_no = 0;
    ret->guard_else.expr_value = expr_value;
    
    return ret;
}

void match_guard_delete(match_guard * value)
{
    switch (value->type)
    {
        case MATCH_GUARD_ITEM:
            if (value->guard_item.enum_id != NULL)
            {
                free(value->guard_item.enum_id);
            }
            if (value->guard_item.item_id != NULL)
            {
                free(value->guard_item.item_id);
            }
            if (value->guard_item.expr_value != NULL)
            {
                expr_delete(value->guard_item.expr_value);
            }
        break;
        case MATCH_GUARD_ELSE:
            if (value->guard_else.expr_value != NULL)
            {
                expr_delete(value->guard_else.expr_value);
            }
        break;
    }
    free(value);
}

expr * match_guard_get_expr(match_guard * value)
{
    expr * ret = NULL;
    
    switch (value->type)
    {
        case MATCH_GUARD_ITEM:
            ret = value->guard_item.expr_value;
        break;
        case MATCH_GUARD_ELSE:
            ret = value->guard_else.expr_value;
        break;
    }
    
    return ret;
}

match_guard_list_node * match_guard_list_node_new(match_guard * value)
{
    match_guard_list_node * node = (match_guard_list_node *)malloc(sizeof(match_guard_list_node));
    
    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void match_guard_list_node_delete(match_guard_list_node * node)
{
    if (node->value != NULL)
    {
        match_guard_delete(node->value);
    }
    free(node);
}

match_guard_list * match_guard_list_new()
{
    match_guard_list * list = (match_guard_list *)malloc(sizeof(match_guard_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void match_guard_list_delete(match_guard_list * list)
{
    match_guard_list_node * node = list->tail;

    while (node != NULL)
    {
        match_guard_list_node * tmp = node->next;
        match_guard_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void match_guard_list_add_beg(match_guard_list * list, match_guard * value)
{
    match_guard_list_node * node = match_guard_list_node_new(value);

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

void match_guard_list_add_end(match_guard_list * list, match_guard * value)
{
    match_guard_list_node * node = match_guard_list_node_new(value);

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

char match_guard_list_is_first(match_guard_list * list, match_guard_list_node * node)
{
    if (node == list->tail)
    {
        return 1;
    }
    
    return 0;
}

char match_guard_list_is_last(match_guard_list * list, match_guard_list_node * node)
{
    if (node == list->head)
    {
        return 1;
    }
    
    return 0;
}

const char * match_guard_type_str(match_guard_type type)
{
    switch (type)
    {
        case MATCH_GUARD_ITEM: return "MATCH_GUARD_ITEM";
        case MATCH_GUARD_ELSE: return "MATCH_GUARD_ELSE";
    }
    return "MATCH_GUARD_UNKNOWN";
}


