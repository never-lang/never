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
#include "strutil.h"
#include "utils.h"
#include "iflet.h"
#include "match.h"
#include "forin.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int expr_match_guard_constred(match_guard * match_value, int * result)
{
    switch (match_value->type)
    {
        case MATCH_GUARD_ITEM:
            expr_constred(match_value->guard_item.expr_value, result);
        break;
        case MATCH_GUARD_RECORD:
            expr_constred(match_value->guard_record.expr_value, result);
        break;
        case MATCH_GUARD_ELSE:
            expr_constred(match_value->guard_else.expr_value, result);
        break;
    }
    
    return 0;
}

int expr_match_guard_list_constred(match_guard_list * list, int * result)
{
    match_guard_list_node * node = list->tail;

    while (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL)
        {
            expr_match_guard_constred(match_value, result);
        }
        node = node->next;
    }

    return 0;
}

int expr_match_constred(expr * value, int * result)
{
    expr_constred(value->match.expr_value, result);
    if (value->match.match_guards != NULL)
    {
        expr_match_guard_list_constred(value->match.match_guards, result);
    }
    return 0;
}

int expr_constred(expr * value, int * result)
{
    switch (value->type)
    {
    case EXPR_BOOL:
    case EXPR_INT:
    case EXPR_FLOAT:
    case EXPR_CHAR:
    case EXPR_STRING:
    case EXPR_ENUMTYPE:
    case EXPR_NIL:
    case EXPR_ID:
        /* cannot be reduced */
        break;
    case EXPR_NEG:
        expr_constred(value->left, result);

        if (value->left->type == EXPR_INT)
        {
            expr * left_value = value->left;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = -(left_value->int_value);

            expr_delete(left_value);
        }
        else if (value->left->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;

            value->type = EXPR_FLOAT;
            value->comb.comb = COMB_TYPE_FLOAT;
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
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value + right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT && value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_FLOAT;
            value->comb.comb = COMB_TYPE_FLOAT;
            value->float_value =
                left_value->float_value + right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_STRING && value->right->type == EXPR_STRING)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;
                        
            value->type = EXPR_STRING;
            value->comb.comb = COMB_TYPE_STRING;
            value->string_value = string_add(left_value->string_value,
                                             right_value->string_value);
            
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
            value->comb.comb = COMB_TYPE_INT;
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
            value->comb.comb = COMB_TYPE_FLOAT;
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
            value->comb.comb = COMB_TYPE_INT;
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
            value->comb.comb = COMB_TYPE_FLOAT;
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
                print_error_msg(value->line_no, "division by zero");
                return 0;
            }

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
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
                print_error_msg(value->line_no, "division by zero");
                return 0;
            }

            value->type = EXPR_FLOAT;
            value->comb.comb = COMB_TYPE_FLOAT;
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
                print_error_msg(value->line_no, "division by zero");
                return 0;
            }

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
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

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value < right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
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

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value > right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
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

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value <= right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
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

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value >= right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value =
                left_value->float_value >= right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_EQ:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_BOOL && value->right->type == EXPR_BOOL)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value == right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value == right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value =
                left_value->float_value == right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_NEQ:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_BOOL && value->right->type == EXPR_BOOL)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value != right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value != right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        else if (value->left->type == EXPR_FLOAT &&
                 value->right->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value =
                left_value->float_value != right_value->float_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_AND:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_BOOL && value->right->type == EXPR_BOOL)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value && right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_OR:
        expr_constred(value->left, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_BOOL && value->right->type == EXPR_BOOL)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value || right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_NOT:
        expr_constred(value->left, result);

        if (value->left->type == EXPR_BOOL)
        {
            expr * left_value = value->left;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = !(left_value->int_value);

            expr_delete(left_value);
        }
        break;
    case EXPR_SUP:
        expr_constred(value->left, result);

        if (value->left->type == EXPR_BOOL)
        {
            expr * left_value = value->left;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = left_value->int_value;

            expr_delete(left_value);
        }
        else if (value->left->type == EXPR_INT)
        {
            expr * left_value = value->left;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value;

            expr_delete(left_value);
        }
        else if (value->left->type == EXPR_FLOAT)
        {
            expr * left_value = value->left;

            value->type = EXPR_FLOAT;
            value->comb.comb = COMB_TYPE_FLOAT;
            value->float_value = left_value->float_value;

            expr_delete(left_value);
        }
        break;
    case EXPR_COND:
        expr_constred(value->left, result);
        expr_constred(value->middle, result);
        expr_constred(value->right, result);

        if (value->left->type == EXPR_BOOL)
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
    case EXPR_RANGE_DIM:
        if (value->range_dim.from)
        {
            expr_constred(value->range_dim.from, result);
        }
        if (value->range_dim.to)
        {
            expr_constred(value->range_dim.to, result);
        }
        break;
    case EXPR_RANGE:
        if (value->range.range_dims != NULL)
        {
            expr_list_constred(value->range.range_dims, result);
        }
        break;
    case EXPR_SLICE:
        if (value->slice.array_expr != NULL)
        {
            expr_constred(value->slice.array_expr, result);
        }
        if (value->slice.range_dims != NULL)
        {
            expr_list_constred(value->slice.range_dims, result);
        }
        break;
    case EXPR_SEQ:
        if (value->seq.list != NULL)
        {
            expr_list_constred(value->seq.list, result);
        }
        break;
    case EXPR_ASS:
        expr_constred(value->left, result);
        expr_constred(value->right, result);
        break;
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
        expr_constred(value->whileloop.cond, result);
        expr_constred(value->whileloop.do_value, result);
        break;
    case EXPR_FOR:
        expr_constred(value->forloop.init, result);
        expr_constred(value->forloop.cond, result);
        expr_constred(value->forloop.incr, result);
        expr_constred(value->forloop.do_value, result);
        break;
    case EXPR_FORIN:
        expr_constred(value->forin_value->in_value, result);
        expr_constred(value->forin_value->do_value, result);
        break;
    case EXPR_IFLET:
        expr_constred(value->iflet_value->expr_value, result);
        expr_constred(value->iflet_value->then_value, result);
        expr_constred(value->iflet_value->else_value, result);
        break;
    case EXPR_MATCH:
        expr_match_constred(value, result);
        break;
    case EXPR_BUILD_IN:
        if (value->func_build_in.param != NULL)
        {
            expr_list_constred(value->func_build_in.param, result);
        }
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
    case EXPR_LISTCOMP:
        if (value->listcomp_value != NULL)
        {
            listcomp_constred(value->listcomp_value, result);
        }
        break;
    case EXPR_ATTR:
        if (value->attr.record_value != NULL)
        {
            expr_constred(value->attr.record_value, result);
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

int qualifier_constred(qualifier * value, int * result)
{
    switch (value->type)
    {
        case QUALIFIER_UNKNOWN:
            assert(0);
        break;
        case QUALIFIER_GENERATOR:
            if (value->expr_value != NULL)
            {
                expr_constred(value->expr_value, result);
            }
        break;
        case QUALIFIER_FILTER:
            if (value->expr_value != NULL)
            {
                expr_constred(value->expr_value, result);
            }
        break;
    }

    return 0;
}

int qualifier_list_constred(qualifier_list * list, int * result)
{
    qualifier_list_node * node = list->tail;

    while (node != NULL)
    {
        qualifier * qualifier_value = node->value;
        if (qualifier_value != NULL)
        {
            qualifier_constred(qualifier_value, result);
        }
        node = node->next;
    }

    return 0;
}

int listcomp_constred(listcomp * value, int * result)
{
    if (value->list != NULL)
    {
        qualifier_list_constred(value->list, result);
    }
    if (value->expr_value != NULL)
    {
        expr_constred(value->expr_value, result);
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

int except_constred(except * value, int * result)
{
    if (value->expr_value != NULL)
    {
        expr_constred(value->expr_value, result);
    }
    return 0;
}

int except_list_constred(except_list * list, int * result)
{
    except_list_node * node = list->tail;
    while (node != NULL)
    {
        except * value = node->value;
        if (value != NULL)
        {
            except_constred(value, result);
        }
        node = node->next;
    }
    return 0;
}

int func_constred_ffi(func * value, int * result)
{
    return 0;
}

int func_constred_native(func * value, int * result)
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
    if (value->except != NULL && value->except->list != NULL)
    {
        except_list_constred(value->except->list, result);
    }
    if (value->except != NULL && value->except->all != NULL)
    {
        except_constred(value->except->all, result);
    }

    return 0;
}

int func_constred(func * value, int * result)
{
    switch (value->type)
    {
        case FUNC_TYPE_UNKNOWN:
            assert(0);
        break;
        case FUNC_TYPE_NATIVE:
            func_constred_native(value, result);
        break;
        case FUNC_TYPE_FFI:
            func_constred_ffi(value, result);
        break;
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

    if (nev->binds)
    {
        bind_list_constred(nev->binds, &result);
    }
    if (nev->funcs)
    {
        func_list_constred(nev->funcs, &result);
    }

    return result;
}
