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
#include "enumred.h"
#include "expr.h"
#include "symtab.h"
#include "enums.h"
#include "enumerator.h"
#include "typecheck.h"
#include "strutil.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

int enumerator_enumred(expr * value, int * result)
{
    enumerator * enumerator_value = value->enumtype.id_enumerator_value;

    if (enumerator_value != NULL)
    {
        if (enumerator_value->mark == 1)
        {
            *result = ENUMRED_FAIL;
            print_error_msg(value->line_no, "could not reduce value to int, cyclic reference detected");
            return 0;
        }

        enumerator_value->mark = 1;

        if (enumerator_value->expr_value != NULL)
        {
            expr_enumred(enumerator_value->expr_value, result);
        }

        enumerator_value->mark = 0;

        if (enumerator_value->expr_value->type == EXPR_INT)
        {
            int int_value = enumerator_value->expr_value->int_value;

            if (value->enumtype.enum_id)
            {
                expr_delete(value->enumtype.enum_id);
            }
            if (value->enumtype.item_id)
            {
                free(value->enumtype.item_id);
            }

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = int_value;
        }
        else
        {
            *result = ENUMRED_FAIL;
            print_error_msg(value->line_no, "could not reduce enumerator index to integer, is %s", expr_type_str(value->type));
            return 0;
        }            
    }
    else
    {
        *result = ENUMRED_FAIL;
        print_error_msg(value->line_no, "enumerator is null");
    }
    

    return 0;
}

int expr_enumred(expr * value, int * result)
{
    switch (value->type)
    {
    case EXPR_BOOL:
    case EXPR_INT:
    case EXPR_LONG:
    case EXPR_FLOAT:
    case EXPR_DOUBLE:
    case EXPR_CHAR:
    case EXPR_STRING:
    case EXPR_NIL:
    case EXPR_C_NULL:
    case EXPR_ID:
    /* cannot be reduced */
    break;
    case EXPR_ENUMTYPE:
        enumerator_enumred(value, result);
    break;
    case EXPR_NEG:
        expr_enumred(value->left, result);

        if (value->left->type == EXPR_INT)
        {
            expr * left_value = value->left;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = -(left_value->int_value);

            expr_delete(left_value);
        }
        break;
    case EXPR_ADD:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_SUB:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_MUL:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_DIV:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            if (right_value->int_value == 0)
            {
                *result = ENUMRED_FAIL;
                print_error_msg(value->line_no, "division by zero");
                return 0;
            }

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value / right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_MOD:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            if (right_value->int_value == 0)
            {
                *result = ENUMRED_FAIL;
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
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_GT:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_LTE:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_GTE:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_EQ:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_NEQ:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        break;
    case EXPR_AND:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

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
        expr_enumred(value->left, result);

        if (value->left->type == EXPR_BOOL)
        {
            expr * left_value = value->left;

            value->type = EXPR_BOOL;
            value->comb.comb = COMB_TYPE_BOOL;
            value->int_value = !(left_value->int_value);

            expr_delete(left_value);
        }
        break;
    case EXPR_BIN_NOT:
        expr_enumred(value->left, result);

        if (value->left->type == EXPR_INT)
        {
            expr * left_value = value->left;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = ~(left_value->int_value);

            expr_delete(left_value);
        }
        break;
    case EXPR_BIN_AND:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value & right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_BIN_OR:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value | right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_BIN_XOR:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value ^ right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_BIN_SHL:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value << right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_BIN_SHR:
        expr_enumred(value->left, result);
        expr_enumred(value->right, result);

        if (value->left->type == EXPR_INT && value->right->type == EXPR_INT)
        {
            expr * left_value = value->left;
            expr * right_value = value->right;

            value->type = EXPR_INT;
            value->comb.comb = COMB_TYPE_INT;
            value->int_value = left_value->int_value >> right_value->int_value;

            expr_delete(left_value);
            expr_delete(right_value);
        }
        break;
    case EXPR_SUP:
        expr_enumred(value->left, result);

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
        else if (value->left->type == EXPR_ENUMTYPE)
        {
            expr * left_value = value->left;

            value->type = EXPR_ENUMTYPE;
            value->comb = left_value->comb;
            value->enumtype = left_value->enumtype;

            free(left_value);
        }
        break;
    case EXPR_COND:
        expr_enumred(value->left, result);
        expr_enumred(value->middle, result);
        expr_enumred(value->right, result);

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

                expr_enumred(value, result);
            }
            else
            {
                value->type = EXPR_SUP;
                value->left = right_value;

                expr_delete(left_value);
                expr_delete(middle_value);

                expr_enumred(value, result);
            }
        }
        break;
    case EXPR_CONV:
        if (value->conv.type == CONV_ENUMTYPE_RECORD_TO_INT)
        {
            expr_enumred(value->conv.expr_value, result);

            if (value->conv.expr_value->type == EXPR_INT)
            {
                expr * left_value = value->conv.expr_value;

                value->type = EXPR_INT;
                value->comb.comb = COMB_TYPE_INT;
                value->int_value = left_value->int_value;

                expr_delete(left_value);
            }
        }
        break;
    case EXPR_PIPEL:
    case EXPR_ARRAY:
    case EXPR_ARRAY_DEREF:
    case EXPR_CALL:
    case EXPR_LAST_CALL:
    case EXPR_FUNC:
    case EXPR_RANGE:
    case EXPR_RANGE_DIM:
    case EXPR_SLICE:
    case EXPR_SEQ:
    case EXPR_ASS:
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
    case EXPR_FOR:
    case EXPR_FORIN:
    case EXPR_IFLET:
    case EXPR_MATCH:
    case EXPR_BUILD_IN:
    case EXPR_LISTCOMP:
    case EXPR_ATTR:
        *result = ENUMRED_FAIL;
        print_error_msg(value->line_no, "enumred %s not supported", expr_type_str(value->type));
        break;
    }
    return 0;
}
