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
#include "expr.h"
#include "seq.h"
#include "array.h"
#include "func.h"
#include "listcomp.h"
#include "iflet.h"
#include "forin.h"
#include "match.h"
#include "symtab.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

expr * expr_new_bool(int int_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_BOOL;
    ret->int_value = int_value;
    ret->comb.comb = COMB_TYPE_BOOL;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_int(int int_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_INT;
    ret->int_value = int_value;
    ret->comb.comb = COMB_TYPE_INT;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_long(long long long_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_LONG;
    ret->long_value = long_value;
    ret->comb.comb = COMB_TYPE_LONG;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_char(char char_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_CHAR;
    ret->char_value = char_value;
    ret->comb.comb = COMB_TYPE_CHAR;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    
    return ret;
}

expr * expr_new_float(float float_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_FLOAT;
    ret->float_value = float_value;
    ret->comb.comb = COMB_TYPE_FLOAT;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_double(double double_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_DOUBLE;
    ret->double_value = double_value;
    ret->comb.comb = COMB_TYPE_DOUBLE;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_string(char * string_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_STRING;
    ret->string_value = string_value;
    ret->comb.comb = COMB_TYPE_STRING;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    
    return ret;
}

expr * expr_new_enumtype(expr * enum_id, char * item_id)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_ENUMTYPE;
    ret->enumtype.called = 0;
    ret->enumtype.enum_id = enum_id;
    ret->enumtype.item_id = item_id;
    ret->enumtype.id_enumerator_value = NULL;
    ret->enumtype.id_enumtype_value = NULL;
    ret->comb.comb = COMB_TYPE_ENUMTYPE;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    
    return ret;
}

expr * expr_new_id(char * id)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_ID;
    ret->id.id = id;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_nil()
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_NIL;
    ret->comb.comb = COMB_TYPE_NIL;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    
    return ret;
}

expr * expr_new_c_null()
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_C_NULL;
    ret->comb.comb = COMB_TYPE_C_PTR;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    
    return ret;
}

expr * expr_new_one(int type, expr * left)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = type;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    ret->left = left;
    ret->middle = NULL;
    ret->right = NULL;

    return ret;
}

expr * expr_new_two(int type, expr * left, expr * right)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = type;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    ret->left = left;
    ret->middle = NULL;
    ret->right = right;

    return ret;
}

expr * expr_new_three(int type, expr * left, expr * middle, expr * right)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = type;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    ret->left = left;
    ret->middle = middle;
    ret->right = right;

    return ret;
}

expr * expr_new_array(array * value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_ARRAY;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->comb.comb_params = NULL;
    ret->comb.comb_ret = NULL;
    ret->line_no = 0;
    ret->array.array_value = value;

    return ret;
}

expr * expr_new_array_deref(expr * array_expr, expr_list * ref)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_ARRAY_DEREF;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    ret->array_deref.array_expr = array_expr;
    ret->array_deref.ref = ref;

    return ret;
}

expr * expr_new_range_dim(expr * from, expr * to)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_RANGE_DIM;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    ret->range_dim.from = from;
    ret->range_dim.to = to;

    return ret;
}

expr * expr_new_range(expr_list * range_dims)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_RANGE;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    ret->range.ret = param_new_int(NULL);
    ret->range.range_dims = range_dims;

    return ret;
}

expr * expr_new_slice(expr * array_expr, expr_list * range_dims)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_SLICE;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->line_no = 0;
    ret->slice.array_expr = array_expr;
    ret->slice.range_dims = range_dims;

    return ret;
}

expr * expr_new_seq(seq * value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_SEQ;
    ret->line_no = value != NULL ? value->line_no : 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->seq_value = value;

    return ret;
}

expr * expr_new_func(func * value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_FUNC;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->func_value = value;

    return ret;
}

expr * expr_new_call(expr * func_expr, expr_list * params)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_CALL;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->call.func_expr = func_expr;
    ret->call.params = params;

    return ret;
}

expr * expr_new_build_in(unsigned int id, expr_list * params, param * param_ret)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_BUILD_IN;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->func_build_in.id = id;
    ret->func_build_in.param = params;
    ret->func_build_in.ret = param_ret;

    return ret;
}

expr * expr_new_ass(expr * left, expr * right)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_ASS;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->left = left;
    ret->right = right;
    
    return ret;
}

expr * expr_new_while(expr * cond, expr * do_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_WHILE;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->whileloop.cond = cond;
    ret->whileloop.do_value = do_value;
    
    return ret;
}

expr * expr_new_do_while(expr * cond, expr * do_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_DO_WHILE;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->whileloop.cond = cond;
    ret->whileloop.do_value = do_value;
    
    return ret;
}

expr * expr_new_for(expr * init, expr * cond, expr * incr, expr * do_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_FOR;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->forloop.init = init;
    ret->forloop.cond = cond;
    ret->forloop.incr = incr;
    ret->forloop.do_value = do_value;

    return ret;
}

expr * expr_new_for_in(char * id, expr * in_value, expr * do_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_FORIN;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->forin_value = forin_new(id, in_value, do_value);

    return ret;
}

expr * expr_new_iflet(iflet * iflet_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_IFLET;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->iflet_value = iflet_value;
    
    return ret;
}

expr * expr_new_match(expr * expr_value, match_guard_list * match_guards)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_MATCH;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->match.expr_value = expr_value;
    ret->match.match_guards = match_guards;

    return ret;
}

expr * expr_new_listcomp(listcomp * listcomp_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_LISTCOMP;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->listcomp_value = listcomp_value;    
        
    return ret;
}

expr * expr_new_attr(expr * record_value, expr * id)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_ATTR;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->comb.comb_const = COMB_CONST_TYPE_CONST;
    ret->comb.comb_lr = COMB_LR_TYPE_RIGHT;
    ret->attr.record_value = record_value;
    ret->attr.id = id;
    
    return ret;
}

comb_type conv_to_comb_type(conv_type conv)
{
    switch (conv)
    {
        case CONV_UNKNOWN: return CONV_UNKNOWN;
        case CONV_INT_TO_LONG: return COMB_TYPE_LONG;
        case CONV_INT_TO_FLOAT: return COMB_TYPE_FLOAT;
        case CONV_INT_TO_DOUBLE: return COMB_TYPE_DOUBLE;

        case CONV_LONG_TO_INT: return COMB_TYPE_INT;
        case CONV_LONG_TO_FLOAT: return COMB_TYPE_FLOAT;
        case CONV_LONG_TO_DOUBLE: return COMB_TYPE_DOUBLE;

        case CONV_FLOAT_TO_INT: return COMB_TYPE_INT;
        case CONV_FLOAT_TO_LONG: return COMB_TYPE_LONG;
        case CONV_FLOAT_TO_DOUBLE: return COMB_TYPE_DOUBLE;

        case CONV_DOUBLE_TO_INT: return COMB_TYPE_INT;
        case CONV_DOUBLE_TO_LONG: return COMB_TYPE_LONG;
        case CONV_DOUBLE_TO_FLOAT: return COMB_TYPE_FLOAT;

        case CONV_ENUMTYPE_RECORD_TO_INT: return COMB_TYPE_INT;
    }

    return CONV_UNKNOWN;
}

expr * expr_conv(expr * expr_value, conv_type conv)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    *ret = *expr_value;

    expr_value->type = EXPR_CONV;
    expr_value->conv.type = conv;
    expr_value->conv.expr_value = ret;
    expr_value->comb.comb = conv_to_comb_type(conv);

    return ret;
}

void expr_delete(expr * value)
{
    switch (value->type)
    {
    case EXPR_BOOL:
    case EXPR_INT:
    case EXPR_LONG:
    case EXPR_FLOAT:
    case EXPR_DOUBLE:
    case EXPR_CHAR:
    case EXPR_NIL:
    case EXPR_C_NULL:
        break;
    case EXPR_STRING:
        if (value->string_value != NULL)
        {
            free(value->string_value);
        }
        break;
    case EXPR_ENUMTYPE:
        if (value->enumtype.enum_id != NULL)
        {
            expr_delete(value->enumtype.enum_id);
        }
        if (value->enumtype.item_id != NULL)
        {
            free(value->enumtype.item_id);
        }
        break;
    case EXPR_ID:
        free(value->id.id);
        break;
    case EXPR_NEG:
    case EXPR_NOT:
    case EXPR_BIN_NOT:
        expr_delete(value->left);
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
    case EXPR_AND:
    case EXPR_OR:
    case EXPR_PIPEL:
    case EXPR_BIN_AND:
    case EXPR_BIN_OR:
    case EXPR_BIN_XOR:
    case EXPR_BIN_SHL:
    case EXPR_BIN_SHR:
        expr_delete(value->left);
        expr_delete(value->right);
        break;
    case EXPR_SUP: /* ( expr ) */
        expr_delete(value->left);
        break;
    case EXPR_COND:
        expr_delete(value->left);
        expr_delete(value->middle);
        expr_delete(value->right);
        break;
    case EXPR_ARRAY:
        if (value->array.array_value)
        {
            array_delete(value->array.array_value);
        }
        break;
    case EXPR_ARRAY_DEREF:
        expr_delete(value->array_deref.array_expr);
        expr_list_delete(value->array_deref.ref);
        break;
    case EXPR_RANGE_DIM:
        if (value->range_dim.from)
        {
            expr_delete(value->range_dim.from);
        }
        if (value->range_dim.to)
        {
            expr_delete(value->range_dim.to);
        }
        break;
    case EXPR_RANGE:
        if (value->range.ret)
        {
            param_delete(value->range.ret);
        }
        if (value->range.range_dims)
        {
            expr_list_delete(value->range.range_dims);
        }
        break;
    case EXPR_SLICE:
        if (value->slice.array_expr != NULL)
        {
            expr_delete(value->slice.array_expr);
        }
        if (value->slice.range_dims != NULL)
        {
            expr_list_delete(value->slice.range_dims);
        }
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        if (value->call.func_expr != NULL)
        {
            expr_delete(value->call.func_expr);
        }
        if (value->call.params != NULL)
        {
            expr_list_delete(value->call.params);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value != NULL)
        {
            func_delete(value->func_value);
        }
        break;
    case EXPR_SEQ:
        if (value->seq_value != NULL)
        {
            seq_delete(value->seq_value);
        }
        break;
    case EXPR_BUILD_IN:
        if (value->func_build_in.param != NULL)
        {
            expr_list_delete(value->func_build_in.param);
        }
        if (value->func_build_in.ret != NULL)
        {
            param_delete(value->func_build_in.ret);
        }
        break;
    case EXPR_ASS:
        if (value->left != NULL)
        {
            expr_delete(value->left);
        }
        if (value->right != NULL)
        {
            expr_delete(value->right);
        }
        break;
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
        if (value->whileloop.cond != NULL)
        {
            expr_delete(value->whileloop.cond);
        }
        if (value->whileloop.do_value != NULL)
        {
            expr_delete(value->whileloop.do_value);
        }
        break;
    case EXPR_FOR:
        if (value->forloop.init != NULL)
        {
            expr_delete(value->forloop.init);
        }
        if (value->forloop.cond != NULL)
        {
            expr_delete(value->forloop.cond);
        }
        if (value->forloop.incr != NULL)
        {
            expr_delete(value->forloop.incr);
        }
        if (value->forloop.do_value != NULL)
        {
            expr_delete(value->forloop.do_value);
        }
        break;
    case EXPR_FORIN:
        if (value->forin_value != NULL)
        {
            forin_delete(value->forin_value);
        }
        break;
    case EXPR_IFLET:
        if (value->iflet_value != NULL)
        {
            iflet_delete(value->iflet_value);
        }
        break;
    case EXPR_MATCH:
        if (value->match.expr_value != NULL)
        {
            expr_delete(value->match.expr_value);
        }
        if (value->match.match_guards != NULL)
        {
            match_guard_list_delete(value->match.match_guards);
        }
        break;
    case EXPR_CONV:
        expr_delete(value->conv.expr_value);
        break;
    case EXPR_LISTCOMP:
        listcomp_delete(value->listcomp_value);
        break;
    case EXPR_ATTR:
        if (value->attr.id != NULL)
        {
            expr_delete(value->attr.id);
        }
        if (value->attr.record_value != NULL)
        {
            expr_delete(value->attr.record_value);
        }
        break;
    }
    free(value);
}

expr_list_node * expr_list_node_new(expr * value)
{
    expr_list_node * node = (expr_list_node *)malloc(sizeof(expr_list_node));

    node->value = value;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

void expr_list_node_delete(expr_list_node * node)
{
    if (node->value != NULL)
    {
        expr_delete(node->value);
    }
    free(node);
}

expr_list * expr_list_new()
{
    expr_list * list = (expr_list *)malloc(sizeof(expr_list));

    list->count = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void expr_list_delete(expr_list * list)
{
    expr_list_node * node = list->tail;

    while (node != NULL)
    {
        expr_list_node * tmp = node->next;
        expr_list_node_delete(node);
        node = tmp;
    }
    free(list);
}

void expr_list_add_beg(expr_list * list, expr * value)
{
    expr_list_node * node = expr_list_node_new(value);

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

void expr_list_add_end(expr_list * list, expr * value)
{
    expr_list_node * node = expr_list_node_new(value);

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

const char * expr_type_str(expr_type type)
{
    switch (type)
    {
    case EXPR_BOOL: return "bool";
    case EXPR_INT: return "int";
    case EXPR_LONG: return "long";
    case EXPR_FLOAT: return "float";
    case EXPR_DOUBLE: return "double";
    case EXPR_CHAR: return "char";
    case EXPR_STRING: return "string";
    case EXPR_ENUMTYPE: return "enumtype";
    case EXPR_ID: return "id";
    case EXPR_NEG: return "neg";
    case EXPR_ADD: return "add";
    case EXPR_SUB: return "sub";
    case EXPR_MUL: return "mul";
    case EXPR_DIV: return "div";
    case EXPR_MOD: return "mod";
    case EXPR_LT: return "lt";
    case EXPR_GT: return "gt";
    case EXPR_LTE: return "lte";
    case EXPR_GTE: return "gte";
    case EXPR_EQ: return "eq";
    case EXPR_NEQ: return "neq";
    case EXPR_AND: return "and";
    case EXPR_OR: return "or";
    case EXPR_PIPEL: return "pipel";
    case EXPR_NOT: return "not";
    case EXPR_BIN_NOT: return "binary not";
    case EXPR_BIN_AND: return "binary and";
    case EXPR_BIN_OR: return "binary or";
    case EXPR_BIN_XOR: return "binary xor";
    case EXPR_BIN_SHL: return "binary shift left";
    case EXPR_BIN_SHR: return "binary shift right";
    case EXPR_SUP: return "sup";
    case EXPR_COND: return "cond";
    case EXPR_ARRAY: return "array";
    case EXPR_ARRAY_DEREF: return "array deref";
    case EXPR_CALL: return "call";
    case EXPR_LAST_CALL: return "last call";
    case EXPR_FUNC: return "func";
    case EXPR_RANGE_DIM: return "range dim";
    case EXPR_RANGE: return "range";
    case EXPR_SLICE: return "slice";
    case EXPR_SEQ: return "seq";
    case EXPR_ASS: return "ass";
    case EXPR_WHILE: return "while";
    case EXPR_DO_WHILE: return "do while";
    case EXPR_FOR: return "for";
    case EXPR_FORIN: return "for in";
    case EXPR_IFLET: return "if let";
    case EXPR_MATCH: return "match";
    case EXPR_BUILD_IN: return "build id";
    case EXPR_CONV: return "conv";
    case EXPR_LISTCOMP: return "listcomp";
    case EXPR_ATTR: return "attr";
    case EXPR_NIL: return "nil";
    case EXPR_C_NULL: return "c_null";
    }
    
    return "unknown";
}

const char * comb_type_str(comb_type type)
{
    switch (type)
    {
    case COMB_TYPE_UNKNOWN:
        return "unknown";
    case COMB_TYPE_ERR:
        return "error";
    case COMB_TYPE_NIL:
        return "nil";
    case COMB_TYPE_BOOL:
        return "bool";
    case COMB_TYPE_INT:
        return "int";
    case COMB_TYPE_LONG:
        return "long";
    case COMB_TYPE_FLOAT:
        return "float";
    case COMB_TYPE_DOUBLE:
        return "double";
    case COMB_TYPE_ENUMTYPE:
        return "enum";
    case COMB_TYPE_ENUMTYPE_ID:
        return "enum type";
    case COMB_TYPE_CHAR:
        return "char";
    case COMB_TYPE_STRING:
        return "string";
    case COMB_TYPE_VOID:
        return "void";
    case COMB_TYPE_C_PTR:
        return "c_ptr";
    case COMB_TYPE_ARRAY:
        return "array";
    case COMB_TYPE_RANGE:
        return "range";
    case COMB_TYPE_SLICE:
        return "slice";
    case COMB_TYPE_FUNC:
        return "func";
    case COMB_TYPE_RECORD:
        return "record";
    case COMB_TYPE_RECORD_ID:
        return "record type";
    case COMB_TYPE_MODULE:
        return "module";
    }
    return "unknown comb type!";
}

const char * conv_type_str(conv_type conv)
{
    switch (conv)
    {
    case CONV_UNKNOWN: return "unknown";
    case CONV_INT_TO_LONG: return "int to long";
    case CONV_INT_TO_FLOAT: return "int to float";
    case CONV_INT_TO_DOUBLE: return "int to double";
    case CONV_LONG_TO_INT: return "long to int";
    case CONV_LONG_TO_FLOAT: return "long to float";
    case CONV_LONG_TO_DOUBLE: return "long to double";
    case CONV_FLOAT_TO_INT: return "float to int";
    case CONV_FLOAT_TO_LONG: return "float to long";
    case CONV_FLOAT_TO_DOUBLE: return "float to double";
    case CONV_DOUBLE_TO_INT: return "double to int";
    case CONV_DOUBLE_TO_LONG: return "double to long";
    case CONV_DOUBLE_TO_FLOAT: return "double to float";
    case CONV_ENUMTYPE_RECORD_TO_INT: return "enumtype record to int";
    }

    return "unknown";
}
