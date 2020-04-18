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
#include "param.h"
#include "dim.h"
#include "range.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

param * param_new_bool(char * id)
{
    param * value = (param *)malloc(sizeof(param));
    
    value->type = PARAM_BOOL;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->params = NULL;
    value->ret = NULL;
    value->line_no = 0;
    
    return value;
}

param * param_new_int(char * id)
{
    param * value = (param *)malloc(sizeof(param));

    value->type = PARAM_INT;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->params = NULL;
    value->ret = NULL;
    value->line_no = 0;

    return value;
}

param * param_new_float(char * id)
{
    param * value = (param *)malloc(sizeof(param));

    value->type = PARAM_FLOAT;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->params = NULL;
    value->ret = NULL;
    value->line_no = 0;

    return value;
}

param * param_new_char(char * id)
{
    param * value = (param *)malloc(sizeof(param));
    
    value->type = PARAM_CHAR;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->params = NULL;
    value->ret = NULL;
    value->line_no = 0;
    
    return value;
}

param * param_new_string(char * id)
{
    param * value = (param *)malloc(sizeof(param));
    
    value->type = PARAM_STRING;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->params = NULL;
    value->ret = NULL;
    value->line_no = 0;

    return value;
}

param * param_new_dim(char * id)
{
    param * value = (param *)malloc(sizeof(param));

    value->type = PARAM_DIM;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->array = NULL;
    value->ret = NULL;
    value->line_no = 0;

    return value;
}

param * param_new_array(char * id, param_list * dims, param * ret)
{
    param * value = (param *)malloc(sizeof(param));

    value->type = PARAM_ARRAY;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->dims = dims;
    value->ret = ret;
    value->line_no = 0;

    if (dims != NULL)
    {
        param_dim_set_array(dims, value);
    }

    return value;
}

param * param_new_range(range_list * ranges)
{
    param * value = (param *)malloc(sizeof(param));

    value->type = PARAM_RANGE;
    value->index = -1;
    value->id = NULL;
    value->record_id = NULL;
    value->ranges = ranges;
    value->line_no = 0;

    value->ret = param_new_int(NULL);

    if (value->ranges != NULL)
    {
        range_dim_set_range(value->ranges, value);
    }

    return value;
}

param * param_new_range_dim(char * id)
{
    param * value = (param *)malloc(sizeof(param));

    value->type = PARAM_RANGE_DIM;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->ret = NULL;
    value->line_no = 0;

    return value;
}

param * param_new_slice(char * id, range_list * ranges, param * ret)
{
    param * value = (param *)malloc(sizeof(param));

    value->type = PARAM_SLICE;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->ranges = ranges;
    value->ret = ret;
    value->line_no = 0;

    if (value->ranges != NULL)
    {
        range_dim_set_slice(value->ranges, value);
    }

    return value;
}

param * param_new_record(char * id, char * record_id)
{
    param * value = (param *)malloc(sizeof(param));
    
    value->type = PARAM_RECORD;
    value->index = -1;
    value->id = id;
    value->record_id = record_id;
    value->record_value = NULL;
    value->ret = NULL;
    value->line_no = 0;

    return value;
}

param * param_new_func(char * id, param_list * params, param * ret)
{
    param * value = (param *)malloc(sizeof(param));

    value->type = PARAM_FUNC;
    value->index = -1;
    value->id = id;
    value->record_id = NULL;
    value->params = params;
    value->ret = ret;
    value->line_no = 0;

    return value;
}

void param_delete(param * value)
{
    if (value->id)
    {
        free(value->id);
    }

    switch (value->type)
    {
        case PARAM_BOOL:
        case PARAM_INT:
        case PARAM_FLOAT:
        case PARAM_CHAR:
        case PARAM_STRING:
        case PARAM_DIM:
        case PARAM_RANGE_DIM:
        case PARAM_SLICE_DIM:
        break;
        case PARAM_ARRAY:
            if (value->dims != NULL)
            {
                param_list_delete(value->dims);
            }
        break;
        case PARAM_RANGE:
        case PARAM_SLICE:
            if (value->ranges != NULL)
            {
                range_list_delete(value->ranges);
            }
        break;
        case PARAM_RECORD:
            if (value->record_id)
            {
                free(value->record_id);
            }
        break;
        case PARAM_ENUMTYPE:
        break;
        case PARAM_FUNC:
            if (value->params != NULL)
            {
                param_list_delete(value->params);
            }
        break;
    }

    if (value->ret)
    {
        param_delete(value->ret);
    }

    free(value);
}

param_list_node * param_list_node_new(param * value)
{
    param_list_node * node = (param_list_node *)malloc(sizeof(param_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void param_list_node_delete(param_list_node * node)
{
    if (node->value)
    {
        param_delete(node->value);
    }
    free(node);
}

param_list * param_list_new()
{
    param_list * list = (param_list *)malloc(sizeof(param_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void param_list_delete(param_list * list)
{
    param_list_node * node = list->tail;

    while (node != NULL)
    {
        param_list_node * tmp = node->next;
        param_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void param_list_add_beg(param_list * list, param * value)
{
    param_list_node * node = param_list_node_new(value);

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

void param_list_add_end(param_list * list, param * value)
{
    param_list_node * node = param_list_node_new(value);

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

param * param_list_find(param_list * list, char * id)
{
    param * ret = NULL;

    param_list_node * node = list->tail;
    while (node != NULL)
    {
        param * value = node->value;
        if (value != NULL)
        {
            if (strcmp(value->id, id) == 0)
            {
                ret = value;
                break; 
            }
        }

        node = node->next;
    }
    
    return ret;
}

void param_dim_set_array(param_list * dims, param * array)
{
    int index = 0;
    param_list_node * node = dims->tail;
    while (node != NULL)
    {
        param * value = node->value;
        if (value != NULL)
        {
            value->index = index++;
            value->array = array;
        }
        node = node->next;
    }
}

void param_print(param * value)
{
    if (value == NULL)
        return;

    printf("param %s %d %s\n", param_type_str(value->type), value->index,
           value->id);
    param_print(value->ret);
}

void param_list_print(param_list * list)
{
    param_list_node * node = list->tail;
    while (node != NULL)
    {
        param * value = node->value;
        if (value != NULL)
        {
            param_print(value);
        }

        node = node->next;
    }
}

char * param_type_str(param_type type)
{
    switch (type)
    {
    case PARAM_BOOL:
        return "PARAM_BOOL";
    case PARAM_INT:
        return "PARAM_INT";
    case PARAM_FLOAT:
        return "PARAM_FLOAT";
    case PARAM_ENUMTYPE:
        return "PARAM_ENUMTYPE";
    case PARAM_CHAR:
        return "PARAM_CHAR";
    case PARAM_STRING:
        return "PARAM_STRING";
    case PARAM_RECORD:
        return "PARAM_RECORD";
    case PARAM_DIM:
        return "PARAM_DIM";
    case PARAM_RANGE_DIM:
        return "PARAM_RANGE_DIM";
    case PARAM_ARRAY:
        return "PARAM_ARRAY";
    case PARAM_RANGE:
        return "PARAM_RANGE";
    case PARAM_SLICE:
        return "PARAM_SLICE";
    case PARAM_SLICE_DIM:
        return "PARAM_SLICE_DIM";
    case PARAM_FUNC:
        return "PARAM_FUNC";
    }
    return "PARAM_???";
}
