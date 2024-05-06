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
#include "array.h"
#include <stdlib.h>

array * array_new(expr_list * elements, param * ret)
{
    array * value = (array *)malloc(sizeof(array));

    value->type = ARRAY_INIT;
    value->elements = elements;
    value->dims = NULL;
    value->ret = ret;
    value->line_no = 0;

    return value;
}

array * array_new_sub(expr_list * elements)
{
    array * value = (array *)malloc(sizeof(array));

    value->type = ARRAY_SUB;
    value->elements = elements;
    value->dims = NULL;
    value->ret = NULL;
    value->line_no = 0;

    return value;
}

array * array_new_dims(expr_list * dims, param * ret)
{
    array * value = (array *)malloc(sizeof(array));

    value->type = ARRAY_DIMS;
    value->elements = NULL;
    value->dims = dims;
    value->ret = ret;
    value->line_no = 0;

    return value;
}

void array_delete(array * value)
{
    if (value->elements != NULL)
    {
        expr_list_delete(value->elements);
    }
    if (value->dims != NULL)
    {
        expr_list_delete(value->dims);
    }
    if (value->ret != NULL)
    {
        param_delete(value->ret);
    }
    free(value);
}

int elements_to_depth_list(expr_list * elements, expr_list_weak * bfs_list,
                           int distance)
{
    expr_list_node * node = elements->tail;
    while (node != NULL)
    {
        expr * elem = node->value;
        if (elem != NULL)
        {
            expr_list_weak_add(bfs_list, elem, distance);
        }
        node = node->next;
    }

    return 0;
}

int array_to_depth_list(expr * value, expr_list_weak * bfs_list)
{
    expr_list_weak_add(bfs_list, value, 0);
    if (value->array.array_value->elements)
    {
        elements_to_depth_list(value->array.array_value->elements, bfs_list, 1);
    }

    expr_list_weak_node * node = bfs_list->head;
    while (node != NULL)
    {
        expr * node_value = node->value;

        if (node_value->type == EXPR_ARRAY &&
            node_value->array.array_value->type == ARRAY_SUB &&
            node_value->array.array_value->elements != NULL)
        {
            elements_to_depth_list(node_value->array.array_value->elements,
                                   bfs_list, node->distance + 1);
        }

        node = node->prev;
    }

    return 0;
}
