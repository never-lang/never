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

#if 0
    if (value->ret != NULL)
    {
        param_init_const(value->ret, PARAM_CONST_TYPE_VAR);
    }
#endif

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

#if 0
    if (value->ret != NULL)
    {
        param_init_const(value->ret, PARAM_CONST_TYPE_VAR);
    }
#endif

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

int array_to_depth_list(expr * value, expr_list_weak * depth_list)
{
    expr_list_weak * bfs_list = expr_list_weak_new();

    expr_list_weak_add(bfs_list, value, 0);
    elements_to_depth_list(value->array.array_value->elements, bfs_list, 1);

    while (bfs_list->count > 0)
    {
        expr_list_weak_node * head = expr_list_weak_pop(bfs_list);
        expr * value = head->value;

        if (value->type == EXPR_ARRAY &&
            value->array.array_value->type == ARRAY_SUB)
        {
            elements_to_depth_list(value->array.array_value->elements,
                                   bfs_list, head->distance + 1);
        }

        expr_list_weak_add(depth_list, value, head->distance + 1);
        expr_list_weak_node_delete(head);
    }

    expr_list_weak_delete(bfs_list);

    return 0;
}
