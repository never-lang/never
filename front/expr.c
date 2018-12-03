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
#include "array.h"
#include "func.h"
#include "listcomp.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

expr * expr_new_int(int int_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_INT;
    ret->int_value = int_value;
    ret->comb.comb = COMB_TYPE_INT;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_float(float float_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_FLOAT;
    ret->float_value = float_value;
    ret->comb.comb = COMB_TYPE_FLOAT;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_string(char * string_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_STRING;
    ret->string_value = string_value;
    ret->comb.comb = COMB_TYPE_STRING;
    ret->line_no = 0;
    
    return ret;
}

expr * expr_new_id(char * id)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_ID;
    ret->id.id = id;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->line_no = 0;

    return ret;
}

expr * expr_new_nil()
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_NIL;
    ret->comb.comb = COMB_TYPE_NIL;
    ret->line_no = 0;
    
    return ret;
}

expr * expr_new_one(int type, expr * left)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = type;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
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
    ret->line_no = 0;
    ret->array_deref.array_expr = array_expr;
    ret->array_deref.ref = ref;

    return ret;
}

expr * expr_new_seq(expr_list * list)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_SEQ;
    /* ret->line_no = list->head->value->line_no; */
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->seq.list = list;
    
    return ret;
}

expr * expr_new_func(func * value)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_FUNC;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->func_value = value;

    return ret;
}

expr * expr_new_call(expr * func_expr, expr_list * params)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    ret->type = EXPR_CALL;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
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
    ret->forloop.init = init;
    ret->forloop.cond = cond;
    ret->forloop.incr = incr;
    ret->forloop.do_value = do_value;

    return ret;
}

expr * expr_new_listcomp(listcomp * listcomp_value)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_LISTCOMP;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    ret->listcomp_value = listcomp_value;    
        
    return ret;
}

expr * expr_new_attr(expr * record_value, char * id)
{
    expr * ret = (expr *)malloc(sizeof(expr));
    
    ret->type = EXPR_ATTR;
    ret->attr.id = id;
    ret->attr.record_value = record_value;
    ret->attr.id_param_value = NULL;
    ret->line_no = 0;
    ret->comb.comb = COMB_TYPE_UNKNOWN;
    
    return ret;
}

expr * expr_conv(expr * expr_value, expr_type conv)
{
    expr * ret = (expr *)malloc(sizeof(expr));

    assert(conv == EXPR_INT_TO_FLOAT || conv == EXPR_FLOAT_TO_INT);

    *ret = *expr_value;

    expr_value->type = conv;
    expr_value->comb.comb =
        (conv == EXPR_INT_TO_FLOAT) ? COMB_TYPE_FLOAT : COMB_TYPE_INT;
    expr_value->left = ret;

    return ret;
}

void expr_delete(expr * value)
{
    switch (value->type)
    {
    case EXPR_INT:
    case EXPR_FLOAT:
    case EXPR_NIL:
        break;
    case EXPR_STRING:
        if (value->string_value != NULL)
        {
            free(value->string_value);
        }
        break;
    case EXPR_ID:
        free(value->id.id);
        break;
    case EXPR_NEG:
    case EXPR_NOT:
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
        if (value->seq.list != NULL)
        {
            expr_list_delete(value->seq.list);
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
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        expr_delete(value->left);
        break;
    case EXPR_LISTCOMP:
        listcomp_delete(value->listcomp_value);
        break;
    case EXPR_ATTR:
        if (value->attr.id != NULL)
        {
            free(value->attr.id);
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
    case EXPR_INT:
        return "int";
    case EXPR_FLOAT:
        return "float";
    case EXPR_ID:
        return "id";
    case EXPR_ARRAY:
        return "array";
    case EXPR_FUNC:
        return "func";
    default:
        return "expr_type to add";
    }
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
    case COMB_TYPE_FLOAT:
        return "float";
    case COMB_TYPE_STRING:
        return "string";
    case COMB_TYPE_ARRAY:
        return "array";
    case COMB_TYPE_FUNC:
        return "func";
    case COMB_TYPE_RECORD_ID:
        return "record id";
    case COMB_TYPE_RECORD:
        return "record";
    }
    return "unknown comb type!";
}
