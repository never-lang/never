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
#include "constred.h"
#include "utils.h"
#include <stdio.h>

int expr_constred(expr * value, int * result)
{
    switch (value->type)
    {
    case EXPR_INT:
    case EXPR_FLOAT:
    case EXPR_ID:
        /* cannot be reduced */
        break;
    case EXPR_NEG:
        expr_constred(value->left, result);

        if (value->left->type == EXPR_INT)
        {
            expr * left_value = value->left;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = -(left_value->int_value);

            expr_delete(left_value);
        }
        else if (value->left->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;

            value->type = EXPR_FLOAT;
            value->comb = COMB_TYPE_FLOAT;
            value->float_value = -(left_value->float_value);

            expr_delete(left_value);
        }
        break;
    case EXPR_ADD:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value + right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_FLOAT;
            value->comb = COMB_TYPE_FLOAT;
            value->float_value =
                left_value->float_value + right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_SUB:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value - right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_FLOAT;
            value->comb = COMB_TYPE_FLOAT;
            value->float_value =
                left_value->float_value - right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_MUL:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value * right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_FLOAT;
            value->comb = COMB_TYPE_FLOAT;
            value->float_value =
                left_value->float_value * right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_DIV:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            if (right_value->int_value == 0)
            {
                *result = CONSTRED_FAIL;
                print_error_msg(value->line_no, "division by zero\n");
                return 0;
            }

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value / right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            if (right_value->float_value == 0)
            {
                *result = CONSTRED_FAIL;
                print_error_msg(value->line_no, "division by zero\n");
                return 0;
            }

            value->type = EXPR_FLOAT;
            value->comb = COMB_TYPE_FLOAT;
            value->float_value =
                left_value->float_value / right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_MOD:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            if (right_value->int_value == 0)
            {
                *result = CONSTRED_FAIL;
                print_error_msg(value->line_no, "division by zero\n");
                return 0;
            }

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value % right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_LT:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value < right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value =
                left_value->float_value < right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_GT:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value > right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value =
                left_value->float_value > right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_LTE:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value <= right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value =
                left_value->float_value <= right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_GTE:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value >= right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value =
                left_value->float_value >= right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_EQ:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value == right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value =
                left_value->float_value == right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_NEQ:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value != right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value =
                left_value->float_value != right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_AND:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value && right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_OR:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value || right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_NOT:
        expr_constred(value->left, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = !(left_value->int_value);

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_SUP:
        expr_constred(value->left, result);

        if (value->left->type == EXPR_INT)
        {
            expr * left_value = value->left;

            value->type = EXPR_INT;
            value->comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value;

            expr_delete(left_value);
        }
        else if (value->left->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;

            value->type = EXPR_FLOAT;
            value->comb = COMB_TYPE_FLOAT;
            value->float_value = left_value->float_value;

            expr_delete(left_value);
        }
        break;
    case EXPR_COND:
        expr_constred(value->left, result);
        expr_constred(value->middle, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * middle_value = value->middle;
            expr * right_value = value->right;

            if (left_value->int_value != 0)
            {
                value->type = EXPR_SUP;
                value->left = middle_value;

                expr_delete(left_value);
                expr_delete(right_value);
            }
            else
            {
                value->type = EXPR_SUP;
                value->left = right_value;

                expr_delete(left_value);
                expr_delete(middle_value);
            }
        }
        break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            array_constread(value->array.array_value, result);
        }
        break;
    case EXPR_ARRAY_DEREF:
        expr_constred(value->array_deref.array_expr, result);
        expr_list_constred(value->array_deref.ref, result);
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        expr_constred(value->call.func_expr, result);
        if (value->call.params != NULL)
        {
            expr_list_constred(value->call.params, result);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_constred(value->func_value, result);
        }
        break;
    case EXPR_BUILD_IN:
        expr_list_constred(value->func_build_in.param, result);
        break;
    case EXPR_INT_TO_FLOAT:
        expr_constred(value->left, result);

        if (value->left->type == EXPR_INT)
        {
            expr * left_value = value->left;

            value->type = EXPR_FLOAT;
            value->float_value = (float)value->left->int_value;

            expr_delete(left_value);
        }
        break;
    case EXPR_FLOAT_TO_INT:
        expr_constred(value->left, result);

        if (value->left->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;

            value->type = EXPR_INT;
            value->int_value = (int)value->left->float_value;

            expr_delete(left_value);
        }
        break;
    }
    return 0;
}

int expr_list_constred(expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            expr_constred(value, result);
        }
        node = node->next;
    }
    return 0;
}

int array_constread(array * value, int * result)
{
    if (value->elements != NULL)
    {
        expr_list_constred(value->elements, result);
    }
    if (value->dims != NULL)
    {
        expr_list_constred(value->dims, result);
    }
    return 0;
}

int bind_constred(bind * value, int * result)
{
    if (value->expr_value != NULL)
    {
        expr_constred(value->expr_value, result);
    }
    return 0;
}

int bind_list_constred(bind_list * list, int * result)
{
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * value = node->value;
        if (value != NULL)
        {
            bind_constred(value, result);
        }

        node = node->next;
    }
    return 0;
}

int func_constred(func * value, int * result)
{
    if (value->body != NULL && value->body->binds != NULL)
    {
        bind_list_constred(value->body->binds, result);
    }
    if (value->body != NULL && value->body->funcs != NULL)
    {
        func_list_constred(value->body->funcs, result);
    }
    if (value->body != NULL && value->body->ret != NULL)
    {
        expr_constred(value->body->ret, result);
    }

    return 0;
}

int func_list_constred(func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * value = node->value;
        if (value != NULL)
        {
            func_constred(value, result);
        }

        node = node->next;
    }
    return 0;
}

int never_constred(never * nev)
{
    int result = CONSTRED_SUCC;

    if (nev->funcs)
    {
        func_list_constred(nev->funcs, &result);
    }

    return result;
}
