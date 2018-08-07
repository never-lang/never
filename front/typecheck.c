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
#include "typecheck.h"
#include "symtab.h"
#include "tailrec.h"
#include "tcheckarr.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>

int expr_set_return_type(expr * value, param * ret)
{
    if (ret == NULL)
    {
        value->comb = COMB_TYPE_VOID;
    }
    else if (ret->type == PARAM_INT)
    {
        value->comb = COMB_TYPE_INT;
    }
    else if (ret->type == PARAM_FLOAT)
    {
        value->comb = COMB_TYPE_FLOAT;
    }
    else if (ret->type == PARAM_DIM)
    {
        value->comb = COMB_TYPE_INT;
    }
    else if (ret->type == PARAM_ARRAY)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = ret->dims->count;
        value->comb_ret = ret->ret;
    }
    else if (ret->type == PARAM_FUNC)
    {
        value->comb = COMB_TYPE_FUNC;
        value->comb_params = ret->params;
        value->comb_ret = ret->ret;
    }
    else
    {
        value->comb = COMB_TYPE_ERR;
    }
    return 0;
}

/*
 * check types
 */
int param_cmp(param * param_one, param * param_two)
{
    if (param_one == NULL && param_two == NULL)
    {
        return TYPECHECK_SUCC;
    }
    if ((param_one == NULL && param_two != NULL) ||
        (param_one != NULL && param_two == NULL))
    {
        return TYPECHECK_FAIL;
    }
    if (param_one->type == PARAM_INT && param_two->type == PARAM_INT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_one->type == PARAM_FLOAT && param_two->type == PARAM_FLOAT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_one->type == PARAM_ARRAY && param_two->type == PARAM_ARRAY)
    {
        return (param_one->dims->count == param_two->dims->count &&
                param_cmp(param_one->ret, param_two->ret));
    }
    else if (param_one->type == PARAM_FUNC && param_two->type == PARAM_FUNC)
    {
        return func_cmp(param_one->params, param_one->ret, param_two->params,
                        param_one->ret);
    }
    else
    {
        return TYPECHECK_FAIL;
    }
}

int param_list_cmp(param_list * param_one, param_list * param_two)
{
    if (param_one == NULL && param_two == NULL)
    {
        return TYPECHECK_SUCC;
    }
    if ((param_one == NULL && param_two != NULL) ||
        (param_one != NULL && param_two == NULL))
    {
        return TYPECHECK_FAIL;
    }

    if (param_one->count != param_two->count)
    {
        return TYPECHECK_FAIL;
    }

    param_list_node * param_one_node = param_one->tail;
    param_list_node * param_two_node = param_two->tail;
    while (param_one_node != NULL && param_two_node != NULL)
    {
        param * param_one_value = param_one_node->value;
        param * param_two_value = param_two_node->value;

        if (param_cmp(param_one_value, param_two_value) == TYPECHECK_FAIL)
        {
            return TYPECHECK_FAIL;
        }

        param_one_node = param_one_node->next;
        param_two_node = param_two_node->next;
    }

    return TYPECHECK_SUCC;
}

int func_cmp(param_list * param_list_one, param * ret_one, param_list * param_list_two,
             param * ret_two)
{
    if (param_list_cmp(param_list_one, param_list_two) == TYPECHECK_SUCC &&
        param_cmp(ret_one, ret_two) == TYPECHECK_SUCC)
    {
        return TYPECHECK_SUCC;
    }
    else
    {
        return TYPECHECK_FAIL;
    }
}

int param_expr_array_cmp(param * param_value, expr * expr_value)
{
    if (param_value->dims->count != expr_value->comb_dims)
    {
        return TYPECHECK_FAIL;
    }
    return param_cmp(param_value->ret, expr_value->comb_ret);
}

int param_expr_cmp(param * param_value, expr * expr_value)
{
    if (param_value == NULL && expr_value == NULL)
    {
        return TYPECHECK_SUCC;
    }
    if ((param_value != NULL && expr_value == NULL) ||
        (param_value == NULL && expr_value != NULL))
    {
        return TYPECHECK_FAIL;
    }

    if (param_value->type == PARAM_INT && expr_value->comb == COMB_TYPE_INT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_INT && expr_value->comb == COMB_TYPE_FLOAT)
    {
        expr_conv(expr_value, EXPR_FLOAT_TO_INT);

        print_warning_msg(expr_value->line_no, "converted float to int\n");
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_FLOAT && expr_value->comb == COMB_TYPE_INT)
    {
        expr_conv(expr_value, EXPR_INT_TO_FLOAT);

        print_warning_msg(expr_value->line_no, "converted int to float\n");
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_FLOAT &&
             expr_value->comb == COMB_TYPE_FLOAT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_DIM && expr_value->comb == COMB_TYPE_INT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_DIM && expr_value->comb == COMB_TYPE_FLOAT)
    {
        expr_conv(expr_value, EXPR_FLOAT_TO_INT);

        print_warning_msg(expr_value->line_no, "converted float to int\n");
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_ARRAY &&
             expr_value->comb == COMB_TYPE_ARRAY)
    {
        return param_expr_array_cmp(param_value, expr_value);
    }
    else if (param_value->type == PARAM_FUNC && expr_value->comb == COMB_TYPE_FUNC)
    {
        return func_cmp(param_value->params, param_value->ret, expr_value->comb_params,
                        expr_value->comb_ret);
    }
    else
    {
        return TYPECHECK_FAIL;
    }
}

int param_expr_list_cmp(param_list * params, expr_list * list)
{
    if (params == NULL && list == NULL)
    {
        return TYPECHECK_SUCC;
    }
    if ((params != NULL && list == NULL) || (params == NULL && list != NULL))
    {
        return TYPECHECK_FAIL;
    }
    if (params->count != list->count)
    {
        return TYPECHECK_FAIL;
    }

    param_list_node * param_node = params->tail;
    expr_list_node * expr_node = list->tail;
    while (param_node != NULL && expr_node != NULL)
    {
        param * param_value = param_node->value;
        expr * expr_value = expr_node->value;

        if (param_expr_cmp(param_value, expr_value) == TYPECHECK_FAIL)
        {
            return TYPECHECK_FAIL;
        }

        param_node = param_node->next;
        expr_node = expr_node->next;
    }

    return TYPECHECK_SUCC;
}

int expr_id_check_type(symtab * tab, expr * value, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(tab, value->id.id, SYMTAB_NESTED);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value != NULL)
        {
            func * func_value = entry->func_value;

            value->comb = COMB_TYPE_FUNC;
            value->comb_params = func_value->params;
            value->comb_ret = func_value->ret;
        }
        else if (entry->type == SYMTAB_VAR && entry->param_value != NULL)
        {
            param * param_value = entry->param_value;
            if (param_value->type == PARAM_INT)
            {
                value->comb = COMB_TYPE_INT;
            }
            else if (param_value->type == PARAM_FLOAT)
            {
                value->comb = COMB_TYPE_FLOAT;
            }
            else if (param_value->type == PARAM_DIM)
            {
                value->comb = COMB_TYPE_INT;
            }
            else if (param_value->type == PARAM_ARRAY)
            {
                value->comb = COMB_TYPE_ARRAY;
                value->comb_dims = param_value->dims->count;
                value->comb_ret = param_value->ret;
            }
            else if (param_value->type == PARAM_FUNC)
            {
                value->comb = COMB_TYPE_FUNC;
                value->comb_params = param_value->params;
                value->comb_ret = param_value->ret;
            }
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(value->line_no, "cannot find paramiable %s\n",
                        value->id.id);
    }
    return 0;
}

int expr_neg_check_type(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value->left, result);
    if (value->left->comb == COMB_TYPE_INT)
    {
        value->comb = COMB_TYPE_INT;
    }
    else if (value->left->comb == COMB_TYPE_FLOAT)
    {
        value->comb = COMB_TYPE_FLOAT;
    }
    else if (value->left->comb == COMB_TYPE_ARRAY &&
             value->left->comb_ret->type == PARAM_INT)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->left->comb_dims;
        value->comb_ret = value->left->comb_ret;
    }
    else if (value->left->comb == COMB_TYPE_ARRAY &&
             value->left->comb_ret->type == PARAM_FLOAT)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->left->comb_dims;
        value->comb_ret = value->left->comb_ret;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no, "cannot negate type %s\n",
                        comb_type_str(value->left->comb));
    }
    
    return 0;
}

int expr_add_sub_check_type(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value->left, result);
    expr_check_type(tab, value->right, result);
    if (value->left->comb == COMB_TYPE_INT &&
        value->right->comb == COMB_TYPE_INT)
    {
        value->comb = COMB_TYPE_INT;
    }
    else if (value->left->comb == COMB_TYPE_INT &&
             value->right->comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value->left, EXPR_INT_TO_FLOAT);
        value->comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float\n");
    }
    else if (value->left->comb == COMB_TYPE_FLOAT &&
             value->right->comb == COMB_TYPE_INT)
    {
        expr_conv(value->right, EXPR_INT_TO_FLOAT);
        value->comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float\n");
    }
    else if (value->left->comb == COMB_TYPE_FLOAT &&
             value->right->comb == COMB_TYPE_FLOAT)
    {
        value->comb = COMB_TYPE_FLOAT;
    }
    else if (value->left->comb == COMB_TYPE_ARRAY &&
             value->left->comb_ret->type == PARAM_INT &&
             value->right->comb == COMB_TYPE_ARRAY &&
             value->right->comb_ret->type == PARAM_INT &&
             value->left->comb_dims == value->right->comb_dims)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->right->comb_dims;
        value->comb_ret = value->right->comb_ret;
    }
    else if (value->left->comb == COMB_TYPE_ARRAY &&
             value->left->comb_ret->type == PARAM_FLOAT &&
             value->right->comb == COMB_TYPE_ARRAY &&
             value->right->comb_ret->type == PARAM_FLOAT &&
             value->left->comb_dims == value->right->comb_dims)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->right->comb_dims;
        value->comb_ret = value->right->comb_ret;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot exec arithmetic operation on types %s %s\n",
                        comb_type_str(value->left->comb),
                        comb_type_str(value->right->comb));
    }

    return 0;
}

int expr_mul_check_type(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value->left, result);
    expr_check_type(tab, value->right, result);
    if (value->left->comb == COMB_TYPE_INT &&
        value->right->comb == COMB_TYPE_INT)
    {
        value->comb = COMB_TYPE_INT;
    }
    else if (value->left->comb == COMB_TYPE_INT &&
             value->right->comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value->left, EXPR_INT_TO_FLOAT);
        value->comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float\n");
    }
    else if (value->left->comb == COMB_TYPE_FLOAT &&
             value->right->comb == COMB_TYPE_INT)
    {
        expr_conv(value->right, EXPR_INT_TO_FLOAT);
        value->comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float\n");
    }
    else if (value->left->comb == COMB_TYPE_FLOAT &&
             value->right->comb == COMB_TYPE_FLOAT)
    {
        value->comb = COMB_TYPE_FLOAT;
    }
    else if (value->left->comb == COMB_TYPE_INT &&
             value->right->comb == COMB_TYPE_ARRAY &&
             value->right->comb_ret->type == PARAM_INT)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->right->comb_dims;
        value->comb_ret = value->right->comb_ret;
    }
    else if (value->left->comb == COMB_TYPE_FLOAT &&
             value->right->comb == COMB_TYPE_ARRAY &&
             value->right->comb_ret->type == PARAM_FLOAT)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->right->comb_dims;
        value->comb_ret = value->right->comb_ret;
    }
    else if (value->left->comb == COMB_TYPE_INT &&
             value->right->comb == COMB_TYPE_ARRAY &&
             value->right->comb_ret->type == PARAM_FLOAT)
    {
        expr_conv(value->left, EXPR_INT_TO_FLOAT);
    
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->right->comb_dims;
        value->comb_ret = value->right->comb_ret;

        print_warning_msg(value->line_no, "converted int to float\n");
    }
    else if (value->left->comb == COMB_TYPE_FLOAT &&
             value->right->comb == COMB_TYPE_ARRAY &&
             value->right->comb_ret->type == PARAM_INT)
    {
        expr_conv(value->left, EXPR_FLOAT_TO_INT);
    
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->right->comb_dims;
        value->comb_ret = value->right->comb_ret;

        print_warning_msg(value->line_no, "converted float to int\n");
    }
    else if (value->left->comb == COMB_TYPE_ARRAY &&
             value->right->comb == COMB_TYPE_ARRAY &&
             value->left->comb_ret->type == PARAM_INT &&
             value->right->comb_ret->type == PARAM_INT &&
             value->left->comb_dims == 2 &&
             value->left->comb_dims == value->right->comb_dims)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->left->comb_dims;
        value->comb_ret = value->left->comb_ret;
    }
    else if (value->left->comb == COMB_TYPE_ARRAY &&
             value->right->comb == COMB_TYPE_ARRAY &&
             value->left->comb_ret->type == PARAM_FLOAT &&
             value->right->comb_ret->type == PARAM_FLOAT &&
             value->left->comb_dims == 2 &&
             value->left->comb_dims == value->right->comb_dims)
    {
        value->comb = COMB_TYPE_ARRAY;
        value->comb_dims = value->left->comb_dims;
        value->comb_ret = value->left->comb_ret;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot exec arithmetic operation on types %s %s\n",
                        comb_type_str(value->left->comb),
                        comb_type_str(value->right->comb));
    }

    return 0;
}

int expr_div_check_type(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value->left, result);
    expr_check_type(tab, value->right, result);
    if (value->left->comb == COMB_TYPE_INT &&
        value->right->comb == COMB_TYPE_INT)
    {
        value->comb = COMB_TYPE_INT;
    }
    else if (value->left->comb == COMB_TYPE_INT &&
             value->right->comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value->left, EXPR_INT_TO_FLOAT);
        value->comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float\n");
    }
    else if (value->left->comb == COMB_TYPE_FLOAT &&
             value->right->comb == COMB_TYPE_INT)
    {
        expr_conv(value->right, EXPR_INT_TO_FLOAT);
        value->comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float\n");
    }
    else if (value->left->comb == COMB_TYPE_FLOAT &&
             value->right->comb == COMB_TYPE_FLOAT)
    {
        value->comb = COMB_TYPE_FLOAT;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot exec arithmetic operation on types %s %s\n",
                        comb_type_str(value->left->comb),
                        comb_type_str(value->right->comb));
    }

    return 0;
}

int expr_cond_check_type(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value->left, result);
    expr_check_type(tab, value->middle, result);
    expr_check_type(tab, value->right, result);

    if (value->left->comb == COMB_TYPE_INT)
    {
        if (value->middle->comb == value->right->comb)
        {
            if (value->middle->comb == COMB_TYPE_FUNC)
            {
                if (func_cmp(value->middle->comb_params, value->middle->comb_ret,
                             value->right->comb_params,
                             value->right->comb_ret) == TYPECHECK_SUCC)
                {
                    value->comb = COMB_TYPE_FUNC;
                    value->comb_params = value->middle->comb_params;
                    value->comb_ret = value->middle->comb_ret;
                }
                else
                {
                    *result = TYPECHECK_FAIL;
                    print_error_msg(value->line_no,
                                    "functions are different %s:%u %s:%u\n",
                                    value->middle->id, value->middle->line_no,
                                    value->right->id, value->right->line_no);
                }
            }
            else
            {
                value->comb = value->middle->comb;
            }
        }
        else
        {
            print_error_msg(
                value->line_no,
                "types on conditional expression do not match %s %s\n",
                comb_type_str(value->middle->comb),
                comb_type_str(value->right->comb));
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot execute conditional operator on %s\n",
                        comb_type_str(value->left->comb));
    }
    return 0;
}

int array_dims_check_type_expr(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value, result);

    if (value->comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value, EXPR_FLOAT_TO_INT);
        print_warning_msg(value->line_no, "converted float to int\n");
    }
    else if (value->comb != COMB_TYPE_INT)
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(value->line_no,
                        "incorrect types %s passed to deref array\n",
                        comb_type_str(value->comb));
    }
    return 0;
}

int array_dims_check_type_expr_list(symtab * tab, expr_list * list,
                                    int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            array_dims_check_type_expr(tab, value, result);
        }
        node = node->next;
    }

    return TYPECHECK_SUCC;
}

int expr_array_deref_check_type(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value->array_deref.array_expr, result);
    if (value->array_deref.ref != NULL)
    {
        expr_list_check_type(tab, value->array_deref.ref, result);
    }

    if (value->array_deref.array_expr->comb == COMB_TYPE_ARRAY)
    {
        if (value->array_deref.array_expr->comb_dims !=
            value->array_deref.ref->count)
        {
            *result = TYPECHECK_FAIL;
            value->comb = COMB_TYPE_ERR;
            print_error_msg(
                value->line_no,
                "incorrect number of dimesions passed to deref array\n");
        }
        else
        {
            array_dims_check_type_expr_list(tab, value->array_deref.ref,
                                            result);
            if (*result == TYPECHECK_FAIL)
            {
                value->comb = COMB_TYPE_ERR;
                print_error_msg(
                    value->line_no,
                    "incorrect types of arguments passed to deref array\n");
            }
            else
            {
                expr_set_return_type(value,
                                     value->array_deref.array_expr->comb_ret);
            }
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no, "cannot deref %s\n",
                        comb_type_str(value->array_deref.array_expr->comb));
    }

    return 0;
}

int expr_call_check_type(symtab * tab, expr * value, int * result)
{
    expr_check_type(tab, value->call.func_expr, result);
    if (value->call.params != NULL)
    {
        expr_list_check_type(tab, value->call.params, result);
    }

    switch (value->call.func_expr->comb)
    {
    case COMB_TYPE_FUNC:
        if (param_expr_list_cmp(value->call.func_expr->comb_params,
                              value->call.params) == TYPECHECK_SUCC)
        {
            expr_set_return_type(value, value->call.func_expr->comb_ret);
        }
        else
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(value->line_no, "function call type mismatch\n");
        }
        break;
    case COMB_TYPE_INT:
    case COMB_TYPE_FLOAT:
    case COMB_TYPE_ARRAY:
    case COMB_TYPE_BOOL:
    case COMB_TYPE_UNKNOWN:
    case COMB_TYPE_ERR:
    case COMB_TYPE_VOID:
        print_error_msg(value->line_no, "cannot execute function on type %s\n",
                        comb_type_str(value->call.func_expr->comb));
        break;
    }

    return 0;
}

int expr_check_type(symtab * tab, expr * value, int * result)
{
    switch (value->type)
    {
    case EXPR_INT:
        value->comb = COMB_TYPE_INT;
        break;
    case EXPR_FLOAT:
        value->comb = COMB_TYPE_FLOAT;
        break;
    case EXPR_ID:
        expr_id_check_type(tab, value, result);
        break;
    case EXPR_NEG:
        expr_neg_check_type(tab, value, result);
    break;
    case EXPR_ADD:
    case EXPR_SUB:
        expr_add_sub_check_type(tab, value, result);
    break;
    case EXPR_MUL:
        expr_mul_check_type(tab, value, result);
    break;
    case EXPR_DIV:
        expr_div_check_type(tab, value, result);
    break;
    case EXPR_MOD:
        expr_check_type(tab, value->left, result);
        expr_check_type(tab, value->right, result);
        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            value->comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "cannot exec mod operation on types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
    {
        expr_check_type(tab, value->left, result);
        expr_check_type(tab, value->right, result);
        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            value->comb = COMB_TYPE_INT;
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            value->comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot compare types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
    }
    break;
    case EXPR_EQ:
    case EXPR_NEQ:
        expr_check_type(tab, value->left, result);
        expr_check_type(tab, value->right, result);
        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            value->comb = COMB_TYPE_INT;
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            value->comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot compare types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_AND:
    case EXPR_OR:
        expr_check_type(tab, value->left, result);
        expr_check_type(tab, value->right, result);
        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            value->comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot compare types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
        }
        break;
    case EXPR_NOT:
        expr_check_type(tab, value->left, result);
        if (value->left->comb == COMB_TYPE_INT)
        {
            value->comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot ne types %s\n",
                            comb_type_str(value->left->comb));
        }
        break;
    case EXPR_SUP:
        expr_check_type(tab, value->left, result);

        value->comb = value->left->comb;
        value->comb_params = value->left->comb_params;
        value->comb_ret = value->left->comb_ret;
        value->comb_dims = value->left->comb_dims;
        break;
    case EXPR_COND:
        expr_cond_check_type(tab, value, result);
        break;
    case EXPR_ARRAY:
        expr_array_check_type(tab, value, result);
        break;
    case EXPR_ARRAY_DEREF:
        expr_array_deref_check_type(tab, value, result);
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        expr_call_check_type(tab, value, result);
        break;
    case EXPR_FUNC:
    {
        if (value->func_value)
        {
            func_check_type(value->func_value->stab, value->func_value, result);

            value->comb = COMB_TYPE_FUNC;
            value->comb_params = value->func_value->params;
            value->comb_ret = value->func_value->ret;
        }
    }
    break;
    case EXPR_BUILD_IN:
        expr_list_check_type(tab, value->func_build_in.param, result);
        expr_set_return_type(value, value->func_build_in.ret);
        break;
    case EXPR_INT_TO_FLOAT:
        expr_check_type(tab, value->left, result);
        if (value->left->comb == COMB_TYPE_INT)
        {
            value->comb = COMB_TYPE_FLOAT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot convert type %s to float\n",
                            value->line_no, comb_type_str(value->left->comb));
        }
        break;
    case EXPR_FLOAT_TO_INT:
        expr_check_type(tab, value->left, result);
        if (value->left->comb == COMB_TYPE_FLOAT)
        {
            value->comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot convert type %s to int\n",
                            value->line_no, comb_type_str(value->left->comb));
        }
        break;
    }
    return 0;
}

int expr_list_check_type(symtab * tab, expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_check_type(tab, value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_check_type(symtab * tab, func * func_value, int * result)
{
    if (func_value->body && func_value->body->funcs)
    {
        func_list_check_type(tab, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_check_type(tab, func_value->body->ret, result);

        if (param_expr_cmp(func_value->ret, func_value->body->ret) ==
            TYPECHECK_FAIL)
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(func_value->line_no,
                            "incorrect return type in function %s\n",
                            func_value->id);
        }
    }

    return 0;
}

int func_list_check_type(symtab * tab, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_check_type(func_value->stab, func_value, result);
        }
        node = node->next;
    }

    return 0;
}

int never_check_type(never * nev, int * result)
{
    func_list_check_type(nev->stab, nev->funcs, result);

    return 0;
}

/*
 * Add symbols to symtab
 */
int symtab_add_param_from_basic_param(symtab * tab, param * param_value,
                                  unsigned int syn_level, int * result)
{
    symtab_entry * entry = symtab_lookup(tab, param_value->id, SYMTAB_FLAT);
    if (entry == NULL)
    {
        symtab_add_param(tab, param_value, syn_level);
    }
    else
    {
        *result = TYPECHECK_FAIL;
        if (entry->type == SYMTAB_FUNC)
        {
            func * al_func = entry->func_value;
            print_error_msg(param_value->line_no,
                            "function %s already defined at line %u\n",
                            entry->id, al_func->line_no);
        }
        else if (entry->type == SYMTAB_VAR)
        {
            param * al_param = entry->param_value;
            print_error_msg(param_value->line_no,
                            "parameter %s already defined at line %u\n",
                            entry->id, al_param->line_no);
        }
    }
    return 0;
}

int symtab_add_param_from_param(symtab * tab, param * param_value,
                            unsigned int syn_level, int * result)
{
    if (param_value->type == PARAM_ARRAY)
    {
        symtab_add_param_from_basic_param(tab, param_value, syn_level, result);
        symtab_add_param_from_param_list(tab, param_value->dims, syn_level, result);
    }
    else
    {
        symtab_add_param_from_basic_param(tab, param_value, syn_level, result);
    }
    return 0;
}

int symtab_add_param_from_param_list(symtab * tab, param_list * list,
                                 unsigned int syn_level, int * result)
{
    param_list_node * node = list->tail;
    while (node != NULL)
    {
        param * param_value = node->value;
        if (param_value && param_value->id)
        {
            symtab_add_param_from_param(tab, param_value, syn_level, result);
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_func_from_func(symtab * tab, func * func_value,
                              unsigned int syn_level, int * result)
{
    symtab_entry * entry = symtab_lookup(tab, func_value->id, SYMTAB_FLAT);
    if (entry == NULL)
    {
        symtab_add_func(tab, func_value, syn_level);
    }
    else
    {
        *result = TYPECHECK_FAIL;
        if (entry->type == SYMTAB_FUNC)
        {
            func * al_func = entry->func_value;
            print_error_msg(func_value->line_no,
                            "function %s already defined at line %u\n",
                            entry->id, al_func->line_no);
        }
        else if (entry->type == SYMTAB_VAR)
        {
            param * al_param = entry->param_value;
            print_error_msg(func_value->line_no,
                            "parameter %s already defined at line %u\n",
                            entry->id, al_param->line_no);
        }
    }

    return 0;
}

int symtab_add_func_from_func_list(symtab * tab, func_list * list,
                                   unsigned int syn_level, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value && func_value->id)
        {
            symtab_add_func_from_func(tab, func_value, syn_level, result);
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_entry_expr(symtab * stab, expr * value, unsigned int syn_level,
                          int * result)
{
    switch (value->type)
    {
    case EXPR_INT:
    case EXPR_FLOAT:
        break;
    case EXPR_ID:
        break;
    case EXPR_NEG:
        symtab_add_entry_expr(stab, value->left, syn_level, result);
        break;
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_MOD:
    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
    case EXPR_EQ:
    case EXPR_NEQ:
        symtab_add_entry_expr(stab, value->left, syn_level, result);
        symtab_add_entry_expr(stab, value->right, syn_level, result);
        break;
    case EXPR_AND:
    case EXPR_OR:
        symtab_add_entry_expr(stab, value->left, syn_level, result);
        symtab_add_entry_expr(stab, value->right, syn_level, result);
        break;
    case EXPR_NOT:
        symtab_add_entry_expr(stab, value->left, syn_level, result);
        break;
    case EXPR_SUP:
        symtab_add_entry_expr(stab, value->left, syn_level, result);
        break;
    case EXPR_COND:
        symtab_add_entry_expr(stab, value->left, syn_level, result);
        symtab_add_entry_expr(stab, value->middle, syn_level, result);
        symtab_add_entry_expr(stab, value->right, syn_level, result);
        break;
    case EXPR_ARRAY:
        symtab_add_entry_array(stab, value->array.array_value, syn_level,
                               result);
        break;
    case EXPR_ARRAY_DEREF:
        symtab_add_entry_expr(stab, value->array_deref.array_expr, syn_level,
                              result);
        symtab_add_entry_expr_list(stab, value->array_deref.ref, syn_level, 
                                   result);
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        symtab_add_entry_expr(stab, value->call.func_expr, syn_level, result);
        if (value->call.params != NULL)
        {
            symtab_add_entry_expr_list(stab, value->call.params, syn_level,
                                       result);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            symtab_add_entry_func(stab, value->func_value, syn_level + 2,
                                  result);
        }
        break;
    case EXPR_BUILD_IN:
        symtab_add_entry_expr_list(stab, value->func_build_in.param, syn_level,
                                   result);
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        symtab_add_entry_expr(stab, value->left, syn_level, result);
        break;
    }
    return 0;
}

int symtab_add_entry_expr_list(symtab * stab_parent, expr_list * list,
                               unsigned int syn_level, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            symtab_add_entry_expr(stab_parent, value, syn_level, result);
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_entry_array(symtab * stab_parent, array * array_value,
                           unsigned int syn_level, int * result)
{
    if (array_value->dims != NULL)
    {
        symtab_add_entry_expr_list(stab_parent, array_value->dims,
                                   syn_level, result);
    }
    if (array_value->elements != NULL)
    {
        symtab_add_entry_expr_list(stab_parent, array_value->elements,
                                   syn_level, result);
    }
    return 0;
}

int symtab_add_entry_func(symtab * stab_parent, func * func_value,
                          unsigned int syn_level, int * result)
{
    if (func_value->stab == NULL)
    {
        func_value->stab = symtab_new(32, stab_parent);
    }
    if (func_value->id)
    {
        symtab_add_func_from_func(func_value->stab, func_value, syn_level - 1,
                                  result);
    }
    if (func_value->params)
    {
        symtab_add_param_from_param_list(func_value->stab, func_value->params,
                                     syn_level, result);
    }
    if (func_value->body && func_value->body->funcs)
    {
        symtab_add_func_from_func_list(func_value->stab,
                                       func_value->body->funcs, syn_level,
                                       result);
    }
    if (func_value->body && func_value->body->funcs)
    {
        symtab_add_entry_func_list(func_value->stab, func_value->body->funcs,
                                   syn_level, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        symtab_add_entry_expr(func_value->stab, func_value->body->ret,
                              syn_level, result);
    }

    return 0;
}

int symtab_add_entry_func_list(symtab * stab_parent, func_list * list,
                               unsigned int syn_level, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
        {
            symtab_add_entry_func(stab_parent, func_value, syn_level + 1,
                                  result);
        }
        node = node->next;
    }

    return 0;
}

int symtab_add_entry_never(never * nev, int * result)
{
    unsigned int syn_level = 0;
    
    if (nev->stab == NULL)
    {
        nev->stab = symtab_new(32, NULL);
    }

    symtab_add_func_from_func_list(nev->stab, nev->funcs, syn_level, result);
    symtab_add_entry_func_list(nev->stab, nev->funcs, syn_level, result);

    return 0;
}

/**
 * print functions
 */
int print_func_expr(expr * value, int depth)
{
    switch (value->type)
    {
    case EXPR_INT:
    case EXPR_FLOAT:
        /* no symtabs possible */
        break;
    case EXPR_ID:
        /* no symtabs possible */
        break;
    case EXPR_NEG:
        print_func_expr(value->left, depth);
        break;
    case EXPR_ADD:
    case EXPR_SUB:
    case EXPR_MUL:
    case EXPR_DIV:
    case EXPR_MOD:
    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
    case EXPR_EQ:
    case EXPR_NEQ:
        print_func_expr(value->left, depth);
        print_func_expr(value->right, depth);
        break;
    case EXPR_AND:
    case EXPR_OR:
        print_func_expr(value->left, depth);
        print_func_expr(value->right, depth);
        break;
    case EXPR_NOT:
        print_func_expr(value->left, depth);
        break;
    case EXPR_SUP:
        print_func_expr(value->left, depth);
        break;
    case EXPR_COND:
        print_func_expr(value->left, depth);
        print_func_expr(value->middle, depth);
        print_func_expr(value->right, depth);
        break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            print_func_array(value->array.array_value, depth);
        }
        break;
    case EXPR_ARRAY_DEREF:
        print_func_expr(value->array_deref.array_expr, depth);
        print_func_expr_list(value->array_deref.ref, depth);
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        print_func_expr(value->call.func_expr, depth);
        if (value->call.params != NULL)
        {
            print_func_expr_list(value->call.params, depth);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            print_func(value->func_value, depth + 1);
        }
        break;
    case EXPR_BUILD_IN:
        print_func_expr_list(value->func_build_in.param, depth + 1);
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        print_func_expr(value->left, depth);
        break;
    }
    return 0;
}

int print_func_expr_list(expr_list * list, int depth)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            print_func_expr(value, depth);
        }
        node = node->next;
    }
    return 0;
}

int print_func_array(array * value, int depth)
{
    if (value->elements != NULL)
    {
        print_func_expr_list(value->elements, depth);
    }
    if (value->dims != NULL)
    {
        print_func_expr_list(value->dims, depth);
    }

    return 0;
}

int print_func(func * value, int depth)
{
    if (value->id != NULL)
    {
        printf("\nfunction (%d): %d %s@%u\n", depth, value->index, value->id,
               value->addr);
    }
    if (value->stab)
    {
        symtab_print(value->stab);
    }
    if (value->freevars != NULL)
    {
        freevar_list_print(value->freevars);
    }
    if (value->body != NULL && value->body->funcs != NULL)
    {
        print_func_list(value->body->funcs, depth + 1);
    }
    if (value->body && value->body->ret)
    {
        print_func_expr(value->body->ret, depth);
    }

    return 0;
}

int print_func_list(func_list * list, int depth)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * value = node->value;
        if (value != NULL)
        {
            print_func(value, depth);
        }

        node = node->next;
    }
    return 0;
}

int print_functions(never * nev)
{
    if (nev->funcs)
    {
        print_func_list(nev->funcs, 1);
    }
    return 0;
}

int func_main_check_num_params(param_list * params)
{
    param_list_node * node = params->tail;
    while (node != NULL)
    {
        param * value = node->value;
        if (value->type != PARAM_INT && value->type != PARAM_FLOAT)
        {
            return 0;
        }
        node = node->next;
    }
    return 1;
}

int func_main_check_type(symtab * tab, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(tab, "main", SYMTAB_FLAT);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value != NULL)
        {
            func * func_value = entry->func_value;
            if (func_value->params != NULL &&
                func_main_check_num_params(func_value->params) == 0)
            {
                print_error_msg(
                    func_value->line_no,
                    "function main can take only numerical parameters\n",
                    func_value->params->count);
                *result = TYPECHECK_FAIL;
            }
            if (func_value->ret == NULL)
            {
                print_error_msg(func_value->line_no,
                                "incorrect function main return type\n");
                *result = TYPECHECK_FAIL;
            }
            else
            {
                if (func_value->ret->type != PARAM_INT &&
                    func_value->ret->type != PARAM_FLOAT)
                {
                    print_error_msg(func_value->line_no,
                                    "incorrect function main return type\n");
                    *result = TYPECHECK_FAIL;
                }
            }
        }
        else
        {
            print_error_msg(0, "incorrect function main, expected function\n");
            *result = TYPECHECK_FAIL;
        }
    }
    else
    {
        print_error_msg(0, "function main is not defined\n");
        *result = TYPECHECK_FAIL;
    }

    return 0;
}

int never_sem_check(never * nev)
{
    int typecheck_res = TYPECHECK_SUCC;

    /* printf("---- add symbol table entries ---\n"); */
    symtab_add_entry_never(nev, &typecheck_res);

    /* printf("---- check types ---\n"); */
    never_check_type(nev, &typecheck_res);

    /* printf("---- check function main\n"); */
    func_main_check_type(nev->stab, &typecheck_res);

    return typecheck_res;
}
