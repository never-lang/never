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
#include "tcheckarr.h"
#include "typecheck.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int array_depth_list_well_formed(array * array_value, expr_list_weak * depth_list,
                                 int * result)
{
    int first_distance = -1;
    int curr_distance = -1;
    int first_comb_elems = 0;
    param * ret = array_value->ret;

    expr_list_weak_node * node = depth_list->tail;
    curr_distance = first_distance = node->distance;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            if (node->distance == first_distance)
            {
                if (param_expr_cmp(ret, value) == TYPECHECK_FAIL)
                {
                    *result = TYPECHECK_FAIL;
                    print_error_msg(
                        value->line_no, "incorrect types in array %s %s\n",
                        param_type_str(ret->type), expr_type_str(value->type));
                }
            }
            else
            {
                if (node->distance != curr_distance)
                {
                    curr_distance = node->distance;

                    if (value->type == EXPR_ARRAY &&
                        (value->array.array_value->type == ARRAY_SUB ||
                         value->array.array_value->type == ARRAY_INIT))
                    {
                        first_comb_elems =
                            value->array.array_value->elements->count;
                    }
                    else
                    {
                        *result = TYPECHECK_FAIL;
                        print_error_msg(value->line_no,
                                        "incorrect types in array %s %s\n",
                                        param_type_str(ret->type),
                                        expr_type_str(value->type));
                    }
                }
                else
                {
                    if (value->type == EXPR_ARRAY &&
                        (value->array.array_value->type == ARRAY_SUB ||
                         value->array.array_value->type == ARRAY_INIT))
                    {
                        if (value->array.array_value->elements->count !=
                            first_comb_elems)
                        {
                            *result = TYPECHECK_FAIL;
                            print_error_msg(
                                value->line_no,
                                "incorrect dimesions in array %s %s\n",
                                param_type_str(ret->type),
                                expr_type_str(value->type));
                        }
                    }
                    else
                    {
                        *result = TYPECHECK_FAIL;
                        print_error_msg(value->line_no,
                                        "incorrect types in array %s %s\n",
                                        param_type_str(ret->type),
                                        expr_type_str(value->type));
                    }
                }
            }
        }
        node = node->next;
    }

    return 0;
}

int array_set_dims(expr_list_weak * depth_list)
{
    int dim = -1;
    int elems = 0;
    int distance = 0;
    expr_list_weak_node * node = NULL;
    expr_list * dims = expr_list_new();

    node = depth_list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            if (node->distance != distance)
            {
                dim++;
                distance = node->distance;

                if (value->type == EXPR_ARRAY &&
                    value->array.array_value->type == ARRAY_SUB)
                {
                    elems = value->array.array_value->elements->count;
                    expr_list_add_beg(dims, expr_new_int(elems));
                }
            }
        }
        node = node->next;
    }

    node = depth_list->head;

    assert(node->value->type == EXPR_ARRAY &&
           node->value->array.array_value->type == ARRAY_INIT);

    elems = node->value->array.array_value->elements->count;
    expr_list_add_beg(dims, expr_new_int(elems));

    node->value->array.array_value->dims = dims;

    return 0;
}

int array_well_formed(expr * value, int * result)
{
    expr_list_weak * depth_list = expr_list_weak_new();
    array_to_depth_list(value, depth_list);

    array_depth_list_well_formed(value->array.array_value, depth_list, result);
    if (*result == TYPECHECK_SUCC)
    {
        array_set_dims(depth_list);
    }

    expr_list_weak_delete(depth_list);

    return 0;
}

int array_check_type(symtab * tab, expr * value, unsigned int syn_level,
                     int * result)
{
    if (value->array.array_value->type == ARRAY_INIT)
    {
        int arr_result = TYPECHECK_SUCC;

        if (value->array.array_value->elements != NULL)
        {
            expr_list_check_type(tab, value->array.array_value->elements,
                                 syn_level, result);
        }

        if (value->array.array_value->ret != NULL)
        {
            param_check_type(tab, value->array.array_value->ret, syn_level, result);
        }

        array_well_formed(value, &arr_result);
        if (arr_result == TYPECHECK_SUCC)
        {
            value->comb.comb = COMB_TYPE_ARRAY;
            value->comb.comb_ret = value->array.array_value->ret;
            value->comb.comb_dims = value->array.array_value->dims->count;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "array is not well formed\n");
        }
    }
    else if (value->array.array_value->type == ARRAY_SUB)
    {
        if (value->array.array_value->elements != NULL)
        {
            expr_list_check_type(tab, value->array.array_value->elements,
                                 syn_level, result);
        }
    }
    else if (value->array.array_value->type == ARRAY_DIMS)
    {
        if (value->array.array_value->dims != NULL)
        {
            array_dims_check_type_expr_list(tab, value->array.array_value->dims,
                                            syn_level, result);
        }

        if (param_is_dynamic_array(value->array.array_value->ret) == TYPECHECK_FAIL)
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "dynamic array of %s is not supported\n",
                            param_type_str(value->array.array_value->ret->type));
        }

        param_check_type(tab, value->array.array_value->ret, syn_level, result);

        if (*result == TYPECHECK_SUCC)
        {
            value->comb.comb = COMB_TYPE_ARRAY;
            value->comb.comb_ret = value->array.array_value->ret;
            value->comb.comb_dims = value->array.array_value->dims->count;
        }
        else
        {
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "array is not well formed\n");
        }
    }

    return 0;
}

int expr_array_check_type(symtab * tab, expr * value, unsigned int syn_level,
                          int * result)
{
    array_check_type(tab, value, syn_level, result);

    return 0;
}
