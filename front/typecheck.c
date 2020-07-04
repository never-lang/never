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
#include "typecheck.h"
#include "gencode.h"
#include "symtab.h"
#include "tailrec.h"
#include "tcheckarr.h"
#include "utils.h"
#include "listcomp.h"
#include "tciflet.h"
#include "tcmatch.h"
#include "range.h"
#include "forin.h"
#include "tcforin.h"
#include "module_decl.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int enumtype_enum_enumerator_list(enumerator_list * list)
{
    int index = 0;
    enumerator_list_node * node = NULL;
    
    node = list->tail;
    while (node != NULL)
    {
        enumerator * value = node->value;
        if (value != NULL)
        {
            value->index = index++;
        }
        node = node->next;
    }
    return 0;
}

int record_enum_param_list(param_list * params)
{
    int index = 0;
    param_list_node * node = NULL;
    
    node = params->tail;
    while (node != NULL)
    {
        param * value = node->value;
        if (value != NULL)
        {
            value->index = index++;
        }
        node = node->next;
    }
    return 0;
}

int func_enum_param_list(param_list * params)
{
    int index = 0;
    param_list_node * node = NULL;

    node = params->tail;
    while (node != NULL)
    {
        param * value = node->value;
        if (value != NULL)
        {
            value->index = -(index++);
        }
        node = node->next;
    }
    return 0;
}

int func_enum_bind_list(bind_list * list, int start)
{
    int index = start;

    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * value = node->value;
        if (value != NULL)
        {
            value->index = index++;
        }
        node = node->next;
    }

    return 0;
}

int func_enum_func_list(func_list * list, int start)
{
    int index = start;

    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * value = node->value;
        if (value != NULL)
        {
            value->index = index++;
        }
        node = node->next;
    }

    return 0;
}

int enum_matchbind_list(matchbind_list * list)
{
    int index = 0;
    
    matchbind_list_node * node = list->tail;
    while (node != NULL)
    {
        matchbind * value = node->value;
        if (value != NULL)
        {
            value->index = index++;
        }
        node = node->next;
    }

    return 0;
}

int expr_set_comb_type(expr * value, param * param_value)
{
    if (param_value == NULL)
    {
        value->comb.comb = COMB_TYPE_ERR;
        return 0;
    }

    switch (param_value->type)
    {
        case PARAM_BOOL:
            value->comb.comb = COMB_TYPE_BOOL;
        break;
        case PARAM_INT:
            value->comb.comb = COMB_TYPE_INT;
        break;
        case PARAM_FLOAT:
            value->comb.comb = COMB_TYPE_FLOAT;
        break;
        case PARAM_ENUMTYPE:
            value->comb.comb = COMB_TYPE_ENUMTYPE;
            value->comb.comb_enumtype = param_value->enumtype_value;
        break;
        case PARAM_CHAR:
            value->comb.comb = COMB_TYPE_CHAR;
        break;
        case PARAM_STRING:
            value->comb.comb = COMB_TYPE_STRING;
        break;
        case PARAM_VOID:
            value->comb.comb = COMB_TYPE_VOID;
        break;
        case PARAM_C_PTR:
            value->comb.comb = COMB_TYPE_C_PTR;
        break;
        case PARAM_DIM:
            value->comb.comb = COMB_TYPE_INT;
        break;
        case PARAM_ARRAY:
            value->comb.comb = COMB_TYPE_ARRAY;
            value->comb.comb_dims = param_value->dims->count;
            value->comb.comb_ret = param_value->ret;
        break;
        case PARAM_RANGE:
            value->comb.comb = COMB_TYPE_RANGE;
            value->comb.comb_dims = param_value->ranges->count;
            value->comb.comb_ret = param_value->ret;
        break;
        case PARAM_SLICE_DIM:
        case PARAM_RANGE_DIM:
            value->comb.comb = COMB_TYPE_INT;
        break;
        case PARAM_SLICE:
            value->comb.comb = COMB_TYPE_SLICE;
            value->comb.comb_dims = param_value->ranges->count;
            value->comb.comb_ret = param_value->ret;
        break;
        case PARAM_RECORD:
            value->comb.comb = COMB_TYPE_RECORD;
            value->comb.comb_record = param_value->record_value;
        break;
        case PARAM_FUNC:
            value->comb.comb = COMB_TYPE_FUNC;
            value->comb.comb_params = param_value->params;
            value->comb.comb_ret = param_value->ret;
        break;
    }

    return 0;
}

int expr_set_comb_type_symtab(expr * value, symtab_entry * entry, int * result)
{
    switch (entry->type)
    {
        case SYMTAB_FUNC:
            if (entry->func_value != NULL)
            {
                func * func_value = entry->func_value;

                value->comb.comb = COMB_TYPE_FUNC;
                value->comb.comb_params = func_value->decl->params;
                value->comb.comb_ret = func_value->decl->ret;
            }
        break;
        case SYMTAB_PARAM:
            if (entry->param_value != NULL)
            {
                expr_set_comb_type(value, entry->param_value);
            }
        break;
        case SYMTAB_BIND:
            if (entry->bind_value != NULL)
            {
                value->comb = entry->bind_value->expr_value->comb;
            }
        break;
        case SYMTAB_MATCHBIND:
            if (entry->matchbind_value != NULL)
            {
                expr_set_comb_type(value, entry->matchbind_value->param_value);
            }
        break;
        case SYMTAB_QUALIFIER:
            if (entry->qualifier_value != NULL)
            {
                expr_qualifier_set_comb_type(value, entry->qualifier_value->expr_value);
            }
        break;
        case SYMTAB_FORIN:
            if (entry->forin_value != NULL)
            {
                value->comb = entry->forin_value->iter->comb;
            }
        break;
        case SYMTAB_RECORD:
            if (entry->record_value != NULL)
            {
                value->comb.comb = COMB_TYPE_RECORD_ID;
                value->comb.comb_record = entry->record_value;
            }
        break;
        case SYMTAB_ENUMTYPE:
            if (entry->enumtype_value != NULL)
            {
                enumtype * al_enumtype = entry->enumtype_value;
                value->comb.comb = COMB_TYPE_ENUMTYPE;
                value->comb.comb_enumtype = al_enumtype;
            }
        break;
        case SYMTAB_ENUMERATOR:
            if (entry->enumerator_value != NULL)
            {
                *result = TYPECHECK_FAIL;
                value->comb.comb = COMB_TYPE_ERR;

                print_error_msg(value->line_no, "found enumerator %s", value->id.id);
            }
        break;
        case SYMTAB_MODULE_DECL:
            if (entry->module_decl_value != NULL)
            {
                value->comb.comb = COMB_TYPE_MODULE;
                value->comb.comb_module_decl = entry->module_decl_value;
            }
        break;
    }

    return 0;
}

int expr_qualifier_set_comb_type(expr * value, expr * expr_value)
{
    if (expr_value->comb.comb == COMB_TYPE_ARRAY &&
        expr_value->comb.comb_dims == 1)
    {
        expr_set_comb_type(value, expr_value->comb.comb_ret);
    }
    else if (expr_value->comb.comb == COMB_TYPE_SLICE &&
             expr_value->comb.comb_dims == 1)
    {
        expr_set_comb_type(value, expr_value->comb.comb_ret);
    }
    else if (expr_value->comb.comb == COMB_TYPE_RANGE &&
             expr_value->comb.comb_dims == 1)
    {
        value->comb.comb = COMB_TYPE_INT;
    }
    else
    {
        assert(0);
    }

    return 0;
}

int array_cmp(int comb_dims_one, param * ret_one,
              int comb_dims_two, param * ret_two)
{
    if (comb_dims_one == comb_dims_two &&
        param_cmp(ret_one, ret_two) == PARAM_CMP_SUCC)
    {
        return TYPECHECK_SUCC;
    }
    else
    {
        return TYPECHECK_FAIL;
    }
}

int param_is_num(param * value)
{
    if (value->type == PARAM_BOOL ||
        value->type == PARAM_INT ||
        value->type == PARAM_FLOAT)
    {
        return TYPECHECK_SUCC;
    }

    return TYPECHECK_FAIL;
}

int param_is_dynamic_array(param * value)
{
    if (value->type == PARAM_BOOL ||
        value->type == PARAM_INT ||
        value->type == PARAM_FLOAT ||
        value->type == PARAM_ENUMTYPE ||
        value->type == PARAM_CHAR ||
        value->type == PARAM_STRING ||
        value->type == PARAM_ARRAY ||
        value->type == PARAM_RANGE ||
        value->type == PARAM_SLICE ||
        value->type == PARAM_RECORD ||
        value->type == PARAM_FUNC)
    {
        return TYPECHECK_SUCC;
    }
    
    return TYPECHECK_FAIL;
}

int param_expr_array_cmp(param * param_value, expr * expr_value)
{
    if (param_value->dims->count != expr_value->comb.comb_dims)
    {
        return TYPECHECK_FAIL;
    }
    return param_cmp(param_value->ret, expr_value->comb.comb_ret);
}

int param_expr_range_cmp(param * param_value, expr * expr_value)
{
    if (param_value->ranges->count == expr_value->comb.comb_dims)
    {
        return TYPECHECK_SUCC;
    }
    return TYPECHECK_FAIL;
}

int param_expr_slice_cmp(param * param_value, expr * expr_value)
{
    if (param_value->ranges->count != expr_value->comb.comb_dims)
    {
        return TYPECHECK_FAIL;
    }
    return param_cmp(param_value->ret, expr_value->comb.comb_ret);
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

    if (param_value->type == PARAM_BOOL && expr_value->comb.comb == COMB_TYPE_BOOL)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_INT && expr_value->comb.comb == COMB_TYPE_INT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_INT && expr_value->comb.comb == COMB_TYPE_FLOAT)
    {
        expr_conv(expr_value, EXPR_FLOAT_TO_INT);

        print_warning_msg(expr_value->line_no, "converted float to int");
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_FLOAT && expr_value->comb.comb == COMB_TYPE_INT)
    {
        expr_conv(expr_value, EXPR_INT_TO_FLOAT);

        print_warning_msg(expr_value->line_no, "converted int to float");
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_FLOAT && expr_value->comb.comb == COMB_TYPE_FLOAT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_CHAR && expr_value->comb.comb == COMB_TYPE_CHAR)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_STRING && expr_value->comb.comb == COMB_TYPE_STRING)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_C_PTR && expr_value->comb.comb == COMB_TYPE_C_PTR)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_DIM && expr_value->comb.comb == COMB_TYPE_INT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_DIM && expr_value->comb.comb == COMB_TYPE_FLOAT)
    {
        expr_conv(expr_value, EXPR_FLOAT_TO_INT);

        print_warning_msg(expr_value->line_no, "converted float to int");
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_ARRAY &&
             expr_value->comb.comb == COMB_TYPE_ARRAY)
    {
        return param_expr_array_cmp(param_value, expr_value);
    }
    else if (param_value->type == PARAM_RANGE &&
             expr_value->comb.comb == COMB_TYPE_RANGE)
    {
        return param_expr_range_cmp(param_value, expr_value);
    }
    else if (param_value->type == PARAM_RANGE_DIM && expr_value->comb.comb == COMB_TYPE_INT)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_SLICE && expr_value->comb.comb == COMB_TYPE_SLICE)
    {
        return param_expr_slice_cmp(param_value, expr_value);
    }
    else if (param_value->type == PARAM_RECORD &&
            (expr_value->comb.comb == COMB_TYPE_RECORD ||
             expr_value->comb.comb == COMB_TYPE_RECORD_ID))
    {
        if (param_value->record_value == expr_value->comb.comb_record)
        {
            return TYPECHECK_SUCC;
        }
        else
        {
            return TYPECHECK_FAIL;
        }
    }
    else if (param_value->type == PARAM_ENUMTYPE && expr_value->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        if (param_value->enumtype_value == expr_value->comb.comb_enumtype)
        {
            return TYPECHECK_SUCC;
        }
        else
        {
            return TYPECHECK_FAIL;
        }
    }
    else if (param_value->type == PARAM_RECORD && expr_value->comb.comb == COMB_TYPE_NIL)
    {
        return TYPECHECK_SUCC;
    }
    else if (param_value->type == PARAM_FUNC && expr_value->comb.comb == COMB_TYPE_FUNC)
    {
        return func_cmp(param_value->params, param_value->ret, expr_value->comb.comb_params,
                        expr_value->comb.comb_ret);
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

int expr_comb_is_enum(expr * value, int * result)
{
    if (value->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        return TYPECHECK_SUCC;
    }

    *result = TYPECHECK_FAIL;
    print_error_msg(value->line_no,
                    "expression is %s not enum name",
                    comb_type_str(value->comb.comb));

    return TYPECHECK_FAIL;
}

int expr_comb_cmp_and_set(expr * left, expr * right, expr * value, int * result)
{
    if (left->comb.comb == COMB_TYPE_BOOL &&
        right->comb.comb == COMB_TYPE_BOOL)
    {
        value->comb.comb = left->comb.comb;
    }
    else if (left->comb.comb == COMB_TYPE_INT &&
             right->comb.comb == COMB_TYPE_INT)
    {
        value->comb.comb = left->comb.comb;
    }
    else if (left->comb.comb == COMB_TYPE_FLOAT &&
             right->comb.comb == COMB_TYPE_FLOAT)
    {
        value->comb.comb = left->comb.comb;
    }
    else if (left->comb.comb == COMB_TYPE_CHAR &&
             right->comb.comb == COMB_TYPE_CHAR)
    {
        value->comb.comb = left->comb.comb;
    }
    else if (left->comb.comb == COMB_TYPE_STRING &&
             right->comb.comb == COMB_TYPE_STRING)
    {
        value->comb.comb = left->comb.comb;
    }
    else if (left->comb.comb == COMB_TYPE_ARRAY &&
             right->comb.comb == COMB_TYPE_ARRAY)
    {
         if (array_cmp(left->comb.comb_dims,
                       left->comb.comb_ret,
                       right->comb.comb_dims,
                       right->comb.comb_ret) == TYPECHECK_SUCC)
         {
             value->comb.comb = COMB_TYPE_ARRAY;
             value->comb.comb_dims = left->comb.comb_dims;
             value->comb.comb_ret = left->comb.comb_ret;
         }
         else
         {
             *result = TYPECHECK_FAIL;
             value->comb.comb = COMB_TYPE_ERR;
             print_error_msg(value->line_no,
                             "arrays are different first line %u second line %u",
                             left->line_no,
                             right->line_no);
         }
    }
    else if (left->comb.comb == COMB_TYPE_RANGE &&
             right->comb.comb == COMB_TYPE_RANGE)
    {
        if (value->left->comb.comb_dims == value->right->comb.comb_dims)
        {
            value->comb.comb = COMB_TYPE_RANGE;
            value->comb.comb_dims = value->left->comb.comb_dims;
            value->comb.comb_ret = value->left->comb.comb_ret;
        }
    }
    else if (left->comb.comb == COMB_TYPE_SLICE &&
             right->comb.comb == COMB_TYPE_SLICE)
    {
         if (array_cmp(left->comb.comb_dims,
                       left->comb.comb_ret,
                       right->comb.comb_dims,
                       right->comb.comb_ret) == TYPECHECK_SUCC)
         {
             value->comb.comb = COMB_TYPE_SLICE;
             value->comb.comb_dims = left->comb.comb_dims;
             value->comb.comb_ret = left->comb.comb_ret;
         }
         else
         {
             *result = TYPECHECK_FAIL;
             value->comb.comb = COMB_TYPE_ERR;
             print_error_msg(value->line_no,
                             "slices are different first line %u second line %u",
                             left->line_no,
                             right->line_no);
         }
    }
    else if (left->comb.comb == COMB_TYPE_FUNC &&
             right->comb.comb == COMB_TYPE_FUNC)
    {
        if (func_cmp(left->comb.comb_params,
                     left->comb.comb_ret,
                     right->comb.comb_params,
                     right->comb.comb_ret) == TYPECHECK_SUCC)
        {
            value->comb.comb = COMB_TYPE_FUNC;
            value->comb.comb_params = left->comb.comb_params;
            value->comb.comb_ret = left->comb.comb_ret;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "functions are different %s:%u %s:%u",
                            left->id, left->line_no,
                            right->id, right->line_no);
        }
    }
    else if (left->comb.comb == COMB_TYPE_ENUMTYPE &&
             right->comb.comb == COMB_TYPE_ENUMTYPE &&
             left->comb.comb_enumtype == right->comb.comb_enumtype)
    {
        value->comb.comb = COMB_TYPE_ENUMTYPE;
        value->comb.comb_enumtype = left->comb.comb_enumtype;
    }
    else if ((left->comb.comb == COMB_TYPE_RECORD ||
              left->comb.comb == COMB_TYPE_RECORD_ID) &&
             (right->comb.comb == COMB_TYPE_RECORD ||
              right->comb.comb == COMB_TYPE_RECORD_ID) &&
             left->comb.comb_record == right->comb.comb_record)
    {
        value->comb.comb = COMB_TYPE_RECORD;
        value->comb.comb_record = left->comb.comb_record;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "types on conditional expression do not match %s %s",
                        comb_type_str(left->comb.comb),
                        comb_type_str(right->comb.comb));
    }

    return 0;
}

/*
 * Add symbols to symtab
 */
int symtab_entry_exists(symtab_entry * entry, unsigned int line_no)
{
    switch (entry->type)
    {
        case SYMTAB_FUNC:
        {
            func * al_func = entry->func_value;
            print_error_msg(line_no,
                            "function %s already defined at line %u",
                            entry->id, al_func->line_no);
        }
        break;
        case SYMTAB_PARAM:
        {
            param * al_param = entry->param_value;
            print_error_msg(line_no,
                            "parameter %s already defined at line %u",
                            entry->id, al_param->line_no);
        }
        break;
        case SYMTAB_BIND:
        {
            bind * al_bind = entry->bind_value;
            print_error_msg(line_no,
                            "bind %s already defined at line %u",
                            entry->id, al_bind->line_no);
        }
        break;
        case SYMTAB_MATCHBIND:
        {
            matchbind * al_matchbind = entry->matchbind_value;
            print_error_msg(line_no,
                            "match bind %s already defined at line %u",
                            entry->id, al_matchbind->line_no);
        }
        break;
        case SYMTAB_QUALIFIER:
        {
            qualifier * al_qualifier = entry->qualifier_value;
            print_error_msg(line_no,
                            "qualifier %s already defined at line %u",
                            entry->id, al_qualifier->line_no);
        }
        break;
        case SYMTAB_FORIN:
        {
            forin * al_forin = entry->forin_value;
            print_error_msg(line_no,
                            "forin iterator %s already defined at line %u",
                            entry->id, al_forin->line_no);
        }
        break;
        case SYMTAB_ENUMERATOR:
        {
            enumerator * al_enumerator = entry->enumerator_value;
            print_error_msg(line_no,
                            "enum item %s already defined at line %u",
                            entry->id, al_enumerator->line_no);
        }
        break;
        case SYMTAB_ENUMTYPE:
        {
            enumtype * al_enumtype = entry->enumtype_value;
            print_error_msg(line_no,
                            "enum %s already defined at line %u",
                            entry->id, al_enumtype->line_no);
        }
        break;
        case SYMTAB_RECORD:
        {
            record * al_record = entry->record_value;
            print_error_msg(line_no,
                            "record %s already defined at line %u",
                            entry->id, al_record->line_no);
        }
        break;
        case SYMTAB_MODULE_DECL:
        {
            module_decl * al_module_decl = entry->module_decl_value;
            print_error_msg(line_no,
                            "module decl %s already defined at line %u",
                            entry->id, al_module_decl->line_no);
        }
        break;
    }

    return 0;
}
  
int symtab_add_param_from_basic_param(symtab * tab, param * param_value,
                                      unsigned int syn_level, int * result)
{
    if (param_value->id == NULL)
    {
        return 0;
    }

    symtab_entry * entry = symtab_lookup(tab, param_value->id, SYMTAB_LOOKUP_LOCAL);
    if (entry == NULL)
    {
        symtab_add_param(tab, param_value, syn_level);
    }
    else
    {
        *result = TYPECHECK_FAIL;
        symtab_entry_exists(entry, param_value->line_no);
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
    else if (param_value->type == PARAM_SLICE)
    {
        symtab_add_param_from_basic_param(tab, param_value, syn_level, result);
        symtab_add_param_from_range_list(tab, param_value->ranges, syn_level, result);
    }
    else if (param_value->type == PARAM_RANGE)
    {
        symtab_add_param_from_basic_param(tab, param_value, syn_level, result);
        symtab_add_param_from_range_list(tab, param_value->ranges, syn_level, result);
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
        if (param_value)
        {
            symtab_add_param_from_param(tab, param_value, syn_level, result);
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_param_from_range(symtab * tab, range * value,
                                unsigned int syn_level, int * result)
{
    symtab_add_param_from_basic_param(tab, value->from, syn_level, result);
    symtab_add_param_from_basic_param(tab, value->to, syn_level, result);

    return 0;
}                                

int symtab_add_param_from_range_list(symtab * tab, range_list * list,
                                     unsigned int syn_level, int * result)
{
    range_list_node * node = list->tail;

    while (node != NULL)
    {
        range * value = node->value;
        if (value != NULL)
        {
            symtab_add_param_from_range(tab, value, syn_level, result);
        }
        node = node->next;
    }

    return 0;
}                                     

int symtab_add_bind_from_bind(symtab * tab, bind * bind_value,
                              unsigned int syn_level, int * result)
{
    symtab_entry * entry = symtab_lookup(tab, bind_value->id, SYMTAB_LOOKUP_LOCAL);
    if (entry == NULL)
    {
        symtab_add_bind(tab, bind_value, syn_level);
    }
    else
    {
        *result = TYPECHECK_FAIL;
        symtab_entry_exists(entry, bind_value->line_no);
    }
    return 0;
}

int symtab_add_bind_from_bind_list(symtab * tab, bind_list * list,
                                   unsigned int syn_level, int * result)
{
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * bind_value = node->value;
        if (bind_value && bind_value->id)
        {
            symtab_add_bind_from_bind(tab, bind_value, syn_level, result);
        }
        node = node->next;
    }
    return 0;
}

int symtab_add_qualifier_from_qualifier(symtab * tab, qualifier * value,
                                        unsigned int syn_level, int * result)
{
    symtab_entry * entry = symtab_lookup(tab, value->id, SYMTAB_LOOKUP_LOCAL);
    if (entry == NULL)
    {
        symtab_add_qualifier(tab, value, syn_level);
    }
    else
    {
        *result = TYPECHECK_FAIL;
        symtab_entry_exists(entry, value->line_no);
    }

    return 0;
}

int symtab_add_func_from_func(symtab * tab, func * func_value,
                              unsigned int syn_level, int * result)
{
    symtab_entry * entry = symtab_lookup(tab, func_value->decl->id,
                                         SYMTAB_LOOKUP_LOCAL);
    if (entry == NULL)
    {
        symtab_add_func(tab, func_value, syn_level);
    }
    else
    {
        *result = TYPECHECK_FAIL;
        symtab_entry_exists(entry, func_value->line_no);
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
        if (func_value && func_value->decl->id)
        {
            symtab_add_func_from_func(tab, func_value, syn_level, result);
        }
        node = node->next;
    }
    return 0;
}

/*
 * check types
 */
int param_enum_record_check_type(symtab * tab, param * param_value,
                                 unsigned int syn_level, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(tab, param_value->record_id, SYMTAB_LOOKUP_GLOBAL);
    if (entry == NULL)
    {
        *result = TYPECHECK_FAIL;
        param_value->record_value = NULL;

        print_error_msg(param_value->line_no, "cannot find record or enum %s",
                        param_value->record_id);
    }
    else if (entry->type == SYMTAB_RECORD)
    {
        param_value->record_value = entry->record_value; 
    }
    else if (entry->type == SYMTAB_ENUMTYPE)
    {
        param_value->type = PARAM_ENUMTYPE;
        param_value->enumtype_value = entry->enumtype_value;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        param_value->record_value = NULL;

        print_error_msg(param_value->line_no, "expected record or enum but %s found",
                        symtab_entry_type_str(entry->type));
    }

    return 0;
}                                       

int param_range_check_type(symtab * tab, range * value,
                           unsigned int syn_level, int * result)
{
    return 0;
}                           

int param_range_list_check_type(symtab * tab, range_list * list,
                                unsigned int syn_level, int * result)
{
    range_list_node * node = list->tail;
    while (node != NULL)
    {
        range * range_value = node->value;
        if (range_value != NULL)
        {
            param_range_check_type(tab, range_value, syn_level, result);
        }
        node = node->next;
    }

    return 0;
}                          

int param_ffi_check_type(symtab * tab, param * param_value,
                         unsigned int syn_level, int * result)
{
    switch (param_value->type)
    {
        case PARAM_BOOL:
        case PARAM_CHAR:
        case PARAM_INT:
        case PARAM_FLOAT:
        case PARAM_STRING:
        case PARAM_VOID:
        case PARAM_C_PTR:
        /* these types are supported for ffi */
        break;
        case PARAM_RECORD:
            param_enum_record_check_type(tab, param_value, syn_level, result);
        break;

        case PARAM_DIM:
        case PARAM_ARRAY:
        case PARAM_RANGE_DIM:
        case PARAM_RANGE:
        case PARAM_SLICE:
        case PARAM_SLICE_DIM:
        case PARAM_ENUMTYPE:
        case PARAM_FUNC:
            *result = TYPECHECK_FAIL;
            print_error_msg(param_value->line_no, "ffi type %s not supported",
                            param_type_str(param_value->type));
        break;
    }
    
    return 0;
}

int param_check_type(symtab * tab, param * param_value,
                     unsigned int syn_level, int * result)
{
    switch (param_value->type)
    {
        case PARAM_BOOL:
        case PARAM_INT:
        case PARAM_FLOAT:
        case PARAM_CHAR:
        case PARAM_STRING:
        case PARAM_C_PTR:
        break;
        case PARAM_ENUMTYPE:
            param_enum_record_check_type(tab, param_value, syn_level, result);
        break;
        case PARAM_VOID:
            *result = TYPECHECK_FAIL;
            print_error_msg(param_value->line_no, "void type supported only in ffi\n");
        break;
        case PARAM_DIM:
        break;
        case PARAM_ARRAY:
            if (param_value->dims != NULL)
            {
                param_list_check_type(tab, param_value->dims, syn_level, result);
            }
            if (param_value->ret != NULL)
            {
                param_check_type(tab, param_value->ret, syn_level, result);
            }
        break;
        case PARAM_RANGE:
            if (param_value->ranges != NULL)
            {
                param_range_list_check_type(tab, param_value->ranges, syn_level, result);
            }
        break;
        case PARAM_RANGE_DIM:
        case PARAM_SLICE_DIM:
        break;
        case PARAM_SLICE:
            if (param_value->ranges != NULL)
            {
                param_range_list_check_type(tab, param_value->ranges, syn_level, result);
            }
            if (param_value->ret != NULL)
            {
                param_check_type(tab, param_value->ret, syn_level, result);
            }
        break;
        case PARAM_RECORD:
            param_enum_record_check_type(tab, param_value, syn_level, result);
        break;
        case PARAM_FUNC:
            if (param_value->params != NULL)
            {
                param_list_check_type(tab, param_value->params, syn_level, result);
            }
            if (param_value->ret != NULL)
            {
                param_check_type(tab, param_value->ret, syn_level, result);
            }
        break;
    }

    return 0;
}

int param_list_ffi_check_type(symtab * tab, param_list * list,
                              unsigned int syn_level, int * result)
{
    param_list_node * node = list->tail;
    while (node != NULL)
    {
        param * param_value = node->value;
        if (param_value != NULL)
        {
            param_ffi_check_type(tab, param_value, syn_level, result);
        }
        node = node->next;
    }
    return 0;
}

int param_list_check_type(symtab * tab, param_list * list,
                          unsigned int syn_level, int * result)
{
    param_list_node * node = list->tail;
    while (node != NULL)
    {
        param * param_value = node->value;
        if (param_value != NULL)
        {
            param_check_type(tab, param_value, syn_level, result);
        }
        node = node->next;
    }
    return 0;
}
 
int expr_id_check_type(symtab * tab, expr * value, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(tab, value->id.id, SYMTAB_LOOKUP_GLOBAL);
    if (entry != NULL)
    {
        expr_set_comb_type_symtab(value, entry, result);
    }
    else
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(value->line_no, "cannot find identifier %s",
                        value->id.id);
    }
    return 0;
}

int expr_enumtype_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result)
{
    if (value->enumtype.enum_id != NULL)
    {
        expr_check_type(tab, value->enumtype.enum_id, func_value, syn_level, result);
    }

    if (value->enumtype.enum_id->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        enumtype * enumtype_value = value->enumtype.enum_id->comb.comb_enumtype;
        enumerator * enumerator_value = enumtype_find_enumerator(enumtype_value, value->enumtype.item_id);

        if (enumerator_value != NULL)
        {
            value->comb.comb = COMB_TYPE_ENUMTYPE;
            value->comb.comb_enumtype = enumtype_value;
            
            value->enumtype.id_enumerator_value = enumerator_value;
            value->enumtype.id_enumtype_value = enumtype_value;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot find enum %s::%s",
                            enumtype_value->id,
                            value->enumtype.item_id);
        }
    }

    return 0;
}

int expr_neg_check_type(symtab * tab, expr * value, func * func_value, 
                        unsigned int syn_level, int * result)
{
    expr_check_type(tab, value->left, func_value, syn_level, result);
    if (value->left->comb.comb == COMB_TYPE_INT)
    {
        value->comb.comb = COMB_TYPE_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT)
    {
        value->comb.comb = COMB_TYPE_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb_ret->type == PARAM_INT)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->left->comb.comb_dims;
        value->comb.comb_ret = value->left->comb.comb_ret;
    }
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb_ret->type == PARAM_FLOAT)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->left->comb.comb_dims;
        value->comb.comb_ret = value->left->comb.comb_ret;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no, "cannot negate type %s",
                        comb_type_str(value->left->comb.comb));
    }
    
    return 0;
}

int expr_add_sub_check_type(symtab * tab, expr * value, func * func_value, 
                            unsigned int syn_level, int * result)
{
    expr_check_type(tab, value->left, func_value, syn_level, result);
    expr_check_type(tab, value->right, func_value, syn_level, result);
    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        value->comb.comb = COMB_TYPE_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value->left, EXPR_INT_TO_FLOAT);
        value->comb.comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        expr_conv(value->right, EXPR_INT_TO_FLOAT);
        value->comb.comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        value->comb.comb = COMB_TYPE_FLOAT;
    }
    else if (value->type == EXPR_ADD &&
             value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        value->comb.comb = COMB_TYPE_STRING;
    }
    else if (value->type == EXPR_ADD &&
             value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        value->comb.comb = COMB_TYPE_STRING;
    }
    else if (value->type == EXPR_ADD &&
             value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        value->comb.comb = COMB_TYPE_STRING;
    }
    else if (value->type == EXPR_ADD &&
             value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        value->comb.comb = COMB_TYPE_STRING;
    }
    else if (value->type == EXPR_ADD &&
             value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        value->comb.comb = COMB_TYPE_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb_ret->type == PARAM_INT &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb_ret->type == PARAM_INT &&
             value->left->comb.comb_dims == value->right->comb.comb_dims)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->right->comb.comb_dims;
        value->comb.comb_ret = value->right->comb.comb_ret;
    }
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb_ret->type == PARAM_FLOAT &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb_ret->type == PARAM_FLOAT &&
             value->left->comb.comb_dims == value->right->comb.comb_dims)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->right->comb.comb_dims;
        value->comb.comb_ret = value->right->comb.comb_ret;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot exec arithmetic operation on types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
    }

    return 0;
}

int expr_mul_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                        int * result)
{
    expr_check_type(tab, value->left, func_value, syn_level, result);
    expr_check_type(tab, value->right, func_value, syn_level, result);
    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        value->comb.comb = COMB_TYPE_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value->left, EXPR_INT_TO_FLOAT);
        value->comb.comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        expr_conv(value->right, EXPR_INT_TO_FLOAT);
        value->comb.comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        value->comb.comb = COMB_TYPE_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb_ret->type == PARAM_INT)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->right->comb.comb_dims;
        value->comb.comb_ret = value->right->comb.comb_ret;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb_ret->type == PARAM_FLOAT)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->right->comb.comb_dims;
        value->comb.comb_ret = value->right->comb.comb_ret;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb_ret->type == PARAM_FLOAT)
    {
        expr_conv(value->left, EXPR_INT_TO_FLOAT);
    
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->right->comb.comb_dims;
        value->comb.comb_ret = value->right->comb.comb_ret;

        print_warning_msg(value->line_no, "converted int to float");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb_ret->type == PARAM_INT)
    {
        expr_conv(value->left, EXPR_FLOAT_TO_INT);
    
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->right->comb.comb_dims;
        value->comb.comb_ret = value->right->comb.comb_ret;

        print_warning_msg(value->line_no, "converted float to int");
    }
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb_ret->type == PARAM_INT &&
             value->right->comb.comb_ret->type == PARAM_INT &&
             value->left->comb.comb_dims == 2 &&
             value->left->comb.comb_dims == value->right->comb.comb_dims)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->left->comb.comb_dims;
        value->comb.comb_ret = value->left->comb.comb_ret;
    }
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb_ret->type == PARAM_FLOAT &&
             value->right->comb.comb_ret->type == PARAM_FLOAT &&
             value->left->comb.comb_dims == 2 &&
             value->left->comb.comb_dims == value->right->comb.comb_dims)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->left->comb.comb_dims;
        value->comb.comb_ret = value->left->comb.comb_ret;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot exec arithmetic operation on types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
    }

    return 0;
}

int expr_div_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                        int * result)
{
    expr_check_type(tab, value->left, func_value, syn_level, result);
    expr_check_type(tab, value->right, func_value, syn_level, result);
    
    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        value->comb.comb = COMB_TYPE_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value->left, EXPR_INT_TO_FLOAT);
        value->comb.comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        expr_conv(value->right, EXPR_INT_TO_FLOAT);
        value->comb.comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        value->comb.comb = COMB_TYPE_FLOAT;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot exec arithmetic operation on types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
    }

    return 0;
}

int expr_ass_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                        int * result)
{
    expr_check_type(tab, value->left, func_value, syn_level, result);
    expr_check_type(tab, value->right, func_value, syn_level, result);

    if (value->left->type != EXPR_ID &&
        value->left->type != EXPR_ATTR &&
        value->left->type != EXPR_ARRAY_DEREF)
    {
        print_warning_msg(value->line_no,
                          "expression on assignment left side is not an identifier but %s",
                          expr_type_str(value->left->type));
    }

    if (value->left->comb.comb == COMB_TYPE_BOOL &&
        value->right->comb.comb == COMB_TYPE_BOOL)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        value->comb.comb = COMB_TYPE_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value->right, EXPR_FLOAT_TO_INT);
        value->comb.comb = COMB_TYPE_INT;

        print_warning_msg(value->line_no, "converted float to int");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        expr_conv(value->right, EXPR_INT_TO_FLOAT);
        value->comb.comb = COMB_TYPE_FLOAT;

        print_warning_msg(value->line_no, "converted int to float");
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        value->comb.comb = COMB_TYPE_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_ENUMTYPE &&
             value->right->comb.comb == COMB_TYPE_ENUMTYPE &&
             value->left->comb.comb_enumtype == value->right->comb.comb_enumtype)
    {
        value->comb.comb = COMB_TYPE_ENUMTYPE;
        value->comb.comb_enumtype = value->right->comb.comb_enumtype;
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
             value->right->comb.comb == COMB_TYPE_CHAR)
    {
        value->comb.comb = COMB_TYPE_CHAR;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        value->comb.comb = COMB_TYPE_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_C_PTR &&
             value->right->comb.comb == COMB_TYPE_C_PTR)
    {
        value->comb.comb = COMB_TYPE_C_PTR;
    }
    else if ((value->left->comb.comb == COMB_TYPE_RECORD ||
              value->left->comb.comb == COMB_TYPE_RECORD_ID) &&
             (value->right->comb.comb == COMB_TYPE_RECORD || 
              value->right->comb.comb == COMB_TYPE_RECORD_ID ) &&
             value->left->comb.comb_record == value->right->comb.comb_record)
    {
        value->comb.comb = COMB_TYPE_RECORD;
        value->comb.comb_record = value->left->comb.comb_record;
    }
    else if ((value->left->comb.comb == COMB_TYPE_RECORD ||
              value->left->comb.comb == COMB_TYPE_RECORD_ID) &&
             value->right->comb.comb == COMB_TYPE_NIL)
    {
        value->comb.comb = COMB_TYPE_RECORD;
        value->comb.comb_record = value->left->comb.comb_record;
    }
    else if (value->left->comb.comb == COMB_TYPE_FUNC &&
             value->right->comb.comb == COMB_TYPE_FUNC &&
             func_cmp(value->left->comb.comb_params,
                      value->left->comb.comb_ret,
                      value->right->comb.comb_params,
                      value->right->comb.comb_ret) == TYPECHECK_SUCC)
    {
        value->comb.comb = COMB_TYPE_FUNC;
        value->comb.comb_params = value->left->comb.comb_params;
        value->comb.comb_ret = value->left->comb.comb_ret;
    }
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_ARRAY &&
             array_cmp(value->left->comb.comb_dims,
                       value->left->comb.comb_ret,
                       value->right->comb.comb_dims,
                       value->right->comb.comb_ret) == TYPECHECK_SUCC)
    {
        value->comb.comb = COMB_TYPE_ARRAY;
        value->comb.comb_dims = value->left->comb.comb_dims;
        value->comb.comb_ret = value->left->comb.comb_ret;
    }
    else if (value->left->comb.comb == COMB_TYPE_RANGE &&
             value->right->comb.comb == COMB_TYPE_RANGE &&
             value->left->comb.comb_dims == value->right->comb.comb_dims)
    {
        value->comb.comb = COMB_TYPE_RANGE;
        value->comb.comb_dims = value->left->comb.comb_dims;
        value->comb.comb_ret = value->left->comb.comb_ret;
    }
    else if (value->left->comb.comb == COMB_TYPE_SLICE &&
             value->right->comb.comb == COMB_TYPE_SLICE &&
             array_cmp(value->left->comb.comb_dims,
                       value->left->comb.comb_ret,
                       value->right->comb.comb_dims,
                       value->right->comb.comb_ret) == TYPECHECK_SUCC)
    {
        value->comb.comb = COMB_TYPE_SLICE;
        value->comb.comb_dims = value->left->comb.comb_dims;
        value->comb.comb_ret = value->left->comb.comb_ret;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot assign different types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
    }

    return 0;
}

int expr_eq_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level, int * result)
{
    expr_check_type(tab, value->left, func_value, syn_level, result);
    expr_check_type(tab, value->right, func_value, syn_level, result);

    if (value->left->comb.comb == COMB_TYPE_NIL &&
             value->right->comb.comb == COMB_TYPE_NIL)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_C_PTR &&
             value->right->comb.comb == COMB_TYPE_C_PTR)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_BOOL &&
             value->right->comb.comb == COMB_TYPE_BOOL)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_ENUMTYPE &&
             value->right->comb.comb == COMB_TYPE_ENUMTYPE &&
             value->left->comb.comb_enumtype == value->right->comb.comb_enumtype &&
             value->left->comb.comb_enumtype->type == ENUMTYPE_TYPE_ITEM)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
             value->right->comb.comb == COMB_TYPE_CHAR)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if ((value->left->comb.comb == COMB_TYPE_STRING &&
              value->right->comb.comb == COMB_TYPE_NIL) ||
             (value->left->comb.comb == COMB_TYPE_NIL &&
              value->right->comb.comb == COMB_TYPE_STRING))
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if ((value->left->comb.comb == COMB_TYPE_ARRAY &&
              value->right->comb.comb == COMB_TYPE_NIL) ||
             (value->left->comb.comb == COMB_TYPE_NIL &&
              value->right->comb.comb == COMB_TYPE_ARRAY))
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if ((value->left->comb.comb == COMB_TYPE_RECORD ||
              value->left->comb.comb == COMB_TYPE_RECORD_ID) &&
              value->right->comb.comb == COMB_TYPE_NIL)
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
            (value->right->comb.comb == COMB_TYPE_RECORD ||
             value->right->comb.comb == COMB_TYPE_RECORD_ID))
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else if ((value->left->comb.comb == COMB_TYPE_FUNC &&
              value->right->comb.comb == COMB_TYPE_NIL) ||
             (value->left->comb.comb == COMB_TYPE_NIL &&
              value->right->comb.comb == COMB_TYPE_FUNC))
    {
        value->comb.comb = COMB_TYPE_BOOL;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no, "cannot compare types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
    }

    return 0;
}

int expr_cond_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                         int * result)
{
    expr_check_type(tab, value->left, func_value, syn_level, result);
    expr_check_type(tab, value->middle, func_value, syn_level, result);
    expr_check_type(tab, value->right, func_value, syn_level, result);

    if (value->left->comb.comb != COMB_TYPE_BOOL)
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "cannot execute conditional operator on %s",
                        comb_type_str(value->left->comb.comb));
        return 0;
    }

    expr_comb_cmp_and_set(value->middle, value->right, value, result);
    
    return 0;
}

int array_dims_check_type_expr(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                               int * result)
{
    int res = TYPECHECK_SUCC;

    expr_check_type(tab, value, func_value, syn_level, result);
    if (value->comb.comb == COMB_TYPE_FLOAT)
    {
        expr_conv(value, EXPR_FLOAT_TO_INT);
        print_warning_msg(value->line_no, "converted float to int");
    }
    else if (value->comb.comb != COMB_TYPE_INT)
    {
        *result = res = TYPECHECK_FAIL;
        print_error_msg(value->line_no,
                        "incorrect types %s passed to deref array",
                        comb_type_str(value->comb.comb));
    }
    return res;
}

int array_dims_check_type_expr_list(symtab * tab, expr_list * list,
                                    func * func_value, unsigned int syn_level, int * result)
{
    int res = TYPECHECK_SUCC;
    
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            if (array_dims_check_type_expr(tab, value, func_value, syn_level, result) == TYPECHECK_FAIL)
            {
                res = TYPECHECK_FAIL;
            }
        }
        node = node->next;
    }

    return res;
}

int expr_array_deref_check_type(symtab * tab, expr * value,
                                func * func_value, unsigned int syn_level, int * result)
{
    expr_check_type(tab, value->array_deref.array_expr, func_value, syn_level, result);
    if (value->array_deref.ref != NULL)
    {
        expr_list_check_type(tab, value->array_deref.ref, func_value, syn_level, result);
    }

    if (value->array_deref.array_expr->comb.comb == COMB_TYPE_ARRAY)
    {
        if (value->array_deref.array_expr->comb.comb_dims == value->array_deref.ref->count)
        {
            if (array_dims_check_type_expr_list(tab, value->array_deref.ref,
                                                func_value, syn_level, result) == TYPECHECK_SUCC)
            {
                expr_set_comb_type(value,
                                   value->array_deref.array_expr->comb.comb_ret);
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb.comb = COMB_TYPE_ERR;
                print_error_msg(value->line_no,
                                "incorrect types of arguments passed to deref array");
            }
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(
                value->line_no,
                "incorrect number of dimesions passed to deref array");

        }
    }
    else if (value->array_deref.array_expr->comb.comb == COMB_TYPE_RANGE)
    {
        if (value->array_deref.array_expr->comb.comb_dims == value->array_deref.ref->count)
        {
            if (array_dims_check_type_expr_list(tab, value->array_deref.ref,
                                                func_value, syn_level, result) == TYPECHECK_SUCC)
            {
                value->comb.comb = COMB_TYPE_ARRAY;
                value->comb.comb_dims = 1;
                value->comb.comb_ret = value->array_deref.array_expr->comb.comb_ret;
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb.comb = COMB_TYPE_ERR;
                print_error_msg(value->line_no,
                                "incorrect types of arguments passed to deref range");
            }
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "incorrect number of dimensions passed to deref range");
        }
    }
    else if (value->array_deref.array_expr->comb.comb == COMB_TYPE_SLICE)
    {
        if (value->array_deref.array_expr->comb.comb_dims == 
            value->array_deref.ref->count)
        {
            if (array_dims_check_type_expr_list(tab, value->array_deref.ref,
                                                func_value, syn_level, result) == TYPECHECK_SUCC)
            {
                expr_set_comb_type(value, value->array_deref.array_expr->comb.comb_ret);
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb.comb = COMB_TYPE_ERR;
                print_error_msg(value->line_no,
                                "incorrect types of arguments passed to deref slice");
            }
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "incorrect number of dimensions passed to deref slice");

        }
    }
    else if (value->array_deref.array_expr->comb.comb == COMB_TYPE_STRING)
    {
        if (value->array_deref.ref->count == 1)
        {
            if (array_dims_check_type_expr_list(tab, value->array_deref.ref,
                                                func_value, syn_level, result) == TYPECHECK_SUCC)
            {
                value->comb.comb = COMB_TYPE_CHAR;
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb.comb = COMB_TYPE_ERR;
                print_error_msg(value->line_no,
                                "incorrect types to deref string");
            }
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(
                value->line_no,
                "strings can be deref only using one dimesion");
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no, "cannot deref %s",
                        comb_type_str(value->array_deref.array_expr->comb.comb));
    }

    return 0;
}

int expr_slice_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                                int * result)
{
    expr_check_type(tab, value->slice.array_expr, func_value, syn_level, result);
    expr_list_check_type(tab, value->slice.range_dims, func_value, syn_level, result);

    if (value->slice.array_expr->comb.comb == COMB_TYPE_ARRAY)
    {
        if (value->slice.array_expr->comb.comb_dims == value->slice.range_dims->count)
        {
            value->comb.comb = COMB_TYPE_SLICE;
            value->comb.comb_dims = value->slice.array_expr->comb.comb_dims;
            value->comb.comb_ret = value->slice.array_expr->comb.comb_ret;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "incorrect number of dimensions passed to slice array");
        }
    }
    else if (value->slice.array_expr->comb.comb == COMB_TYPE_RANGE)
    {
        if (value->slice.array_expr->comb.comb_dims == value->slice.range_dims->count)
        {
            value->comb.comb = COMB_TYPE_RANGE;
            value->comb.comb_dims = value->slice.array_expr->comb.comb_dims;
            value->comb.comb_ret = value->slice.array_expr->comb.comb_ret;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "incorrect number of dimensions passed to slice range");
        }
    }
    else if (value->slice.array_expr->comb.comb == COMB_TYPE_SLICE)
    {
        if (value->slice.array_expr->comb.comb_dims == value->slice.range_dims->count)
        {
            value->comb.comb = COMB_TYPE_SLICE;
            value->comb.comb_dims = value->slice.array_expr->comb.comb_dims;
            value->comb.comb_ret = value->slice.array_expr->comb.comb_ret;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "incorrect number of dimensions passed to slice slice");
        }
    }
    else if (value->slice.array_expr->comb.comb == COMB_TYPE_STRING)
    {
        if (value->slice.range_dims->count == 1)
        {
            value->comb.comb = COMB_TYPE_STRING;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "incorrect number of dimensions passed to slice string");
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no, "cannot deref %s",
                        comb_type_str(value->slice.array_expr->comb.comb));
    }

    return 0;
}                                

int expr_range_check_type(symtab * tab, expr * value, func * func_value,
                          unsigned int syn_level, int * result)
{
    expr_range_list_check_type(tab, value->range.range_dims, func_value, syn_level, result);

    value->comb.comb = COMB_TYPE_RANGE;
    value->comb.comb_dims = value->range.range_dims->count;
    value->comb.comb_ret = value->range.ret;

    return 0;
}

int expr_range_dim_check_type(symtab * tab, expr * value, func * func_value,
                               unsigned int syn_level, int * result)
{
    assert (value->type == EXPR_RANGE_DIM);

    expr_check_type(tab, value->range_dim.from, func_value, syn_level, result);
    expr_check_type(tab, value->range_dim.to, func_value, syn_level, result);

    if (value->range_dim.from->comb.comb != COMB_TYPE_INT)
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(value->range_dim.from->line_no,
                        "expected range from of type int but got %s",
                        comb_type_str(value->range_dim.from->comb.comb));
    }

    if (value->range_dim.to->comb.comb != COMB_TYPE_INT)
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(value->range_dim.to->line_no,
                        "expected range to of type int but got %s",
                        comb_type_str(value->range_dim.to->comb.comb));
    }

    return 0;
}

int expr_range_list_check_type(symtab * tab, expr_list * range_dims, func * func_value, 
                               unsigned int syn_level, int * result)
{
    expr_list_node * node = range_dims->tail;

    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_range_dim_check_type(tab, value, func_value, syn_level, result);
        }
        node = node->next;
    }

    return 0;
}

int expr_call_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                         int * result)
{
    expr_check_type(tab, value->call.func_expr, func_value, syn_level, result);
    if (value->call.params != NULL)
    {
        expr_list_check_type(tab, value->call.params, func_value, syn_level, result);
    }

    switch (value->call.func_expr->comb.comb)
    {
    case COMB_TYPE_FUNC:
        if (param_expr_list_cmp(value->call.func_expr->comb.comb_params,
                                value->call.params) == TYPECHECK_SUCC)
        {
            expr_set_comb_type(value, value->call.func_expr->comb.comb_ret);
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;

            print_error_msg(value->line_no, "function call type mismatch");
        }
        break;
    case COMB_TYPE_RECORD_ID:
        if (param_expr_list_cmp(value->call.func_expr->comb.comb_record->params,
                                value->call.params) == TYPECHECK_SUCC)
        {
            value->comb.comb = COMB_TYPE_RECORD;
            value->comb.comb_record = value->call.func_expr->comb.comb_record;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            value->comb.comb_record = NULL;            

            print_error_msg(value->line_no, "record create type mismatch");
        }

        break;
    case COMB_TYPE_ENUMTYPE:
        if (value->call.func_expr->type == EXPR_ENUMTYPE &&
            value->call.func_expr->enumtype.id_enumtype_value != NULL &&
            value->call.func_expr->enumtype.id_enumerator_value != NULL &&
            value->call.func_expr->enumtype.id_enumerator_value->type == ENUMERATOR_TYPE_RECORD &&
            value->call.func_expr->enumtype.id_enumerator_value->record_value != NULL &&
            param_expr_list_cmp(value->call.func_expr->enumtype.id_enumerator_value->record_value->params,
                                value->call.params) == TYPECHECK_SUCC)
        {
            value->call.func_expr->enumtype.called = 1;

            value->comb.comb = COMB_TYPE_ENUMTYPE;
            value->comb.comb_enumtype = value->call.func_expr->comb.comb_enumtype;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            value->comb.comb_enumtype = NULL;
                    
            print_error_msg(value->line_no, "enum record type mismatch");
        }
        break;
    case COMB_TYPE_BOOL:
    case COMB_TYPE_INT:
    case COMB_TYPE_FLOAT:
    case COMB_TYPE_CHAR:
    case COMB_TYPE_STRING:
    case COMB_TYPE_VOID:
    case COMB_TYPE_C_PTR:
    case COMB_TYPE_ARRAY:
    case COMB_TYPE_RANGE:
    case COMB_TYPE_SLICE:
    case COMB_TYPE_UNKNOWN:
    case COMB_TYPE_ERR:
    case COMB_TYPE_NIL:
    case COMB_TYPE_RECORD:
    case COMB_TYPE_MODULE:
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            
            print_error_msg(value->line_no, "cannot execute function on type %s",
                            comb_type_str(value->call.func_expr->comb.comb));
        }
        break;
    }

    return 0;
}

int qualifier_check_type(symtab * tab, qualifier * value, func * func_value, unsigned int syn_level, 
                         int * result)
{
    switch (value->type)
    {
        case QUALIFIER_UNKNOWN:
            assert(0);
        break;
        case QUALIFIER_GENERATOR:
            if (value->expr_value != NULL)
            {
                expr_check_type(tab, value->expr_value, func_value, syn_level, result);
                
                if (!((value->expr_value->comb.comb == COMB_TYPE_ARRAY &&
                       value->expr_value->comb.comb_dims == 1) ||
                      (value->expr_value->comb.comb == COMB_TYPE_RANGE &&
                       value->expr_value->comb.comb_dims == 1) ||
                      (value->expr_value->comb.comb == COMB_TYPE_SLICE &&
                       value->expr_value->comb.comb_dims == 1)))
                {
                    *result = TYPECHECK_FAIL;
                    print_error_msg(value->line_no,
                                    "generators over one dimensional arrays");
                }
            }
            symtab_add_qualifier_from_qualifier(tab, value, syn_level, result);
        break;
        case QUALIFIER_FILTER:
            if (value->expr_value != NULL)
            {
                expr_check_type(tab, value->expr_value, func_value, syn_level, result);
                if (value->expr_value->comb.comb != COMB_TYPE_BOOL)
                {
                    *result = TYPECHECK_FAIL;
                    print_error_msg(value->line_no,
                                    "filter should be bool type is %s",
                                     comb_type_str(value->expr_value->comb.comb));
                }
            }
        break;
    }

    return 0;
}

int qualifier_list_check_type(symtab * tab, qualifier_list * list,
                              func * func_value, unsigned int syn_level, int * result)
{
    qualifier_list_node * node = list->tail;

    while (node != NULL)
    {
        qualifier * qualifier_value = node->value;
        if (qualifier_value != NULL)
        {
            qualifier_check_type(tab, qualifier_value, func_value, syn_level, result);
        }
        node = node->next;
    }

    return 0;
}

int expr_listcomp_check_type(symtab * tab, listcomp * listcomp_value,
                             func * func_value, unsigned int syn_level, int * result)
{
    if (listcomp_value->stab == NULL)
    {
        listcomp_value->stab = symtab_new(8, SYMTAB_TYPE_BLOCK, tab);
    }

    qualifier_list_check_type(listcomp_value->stab, listcomp_value->list,
                              func_value, syn_level, result);

    expr_check_type(listcomp_value->stab, listcomp_value->expr_value, func_value, syn_level,
                    result);

    if (listcomp_value->ret != NULL)
    {
        param_check_type(listcomp_value->stab, listcomp_value->ret, syn_level, result);
    }

    if (param_expr_cmp(listcomp_value->ret, listcomp_value->expr_value)
                       == TYPECHECK_FAIL)
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(listcomp_value->line_no,
                        "incorrect return type in list comprehension %s %s",
                        comb_type_str(listcomp_value->expr_value->comb.comb),
                        param_type_str(listcomp_value->ret->type));
    }

    return 0;
}

int expr_attr_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                         int * result)
{
    if (value->attr.record_value != NULL)
    {
        expr_check_type(tab, value->attr.record_value, func_value, syn_level, result);
    }

    if (value->attr.record_value->comb.comb == COMB_TYPE_RECORD ||
        value->attr.record_value->comb.comb == COMB_TYPE_RECORD_ID)
    {
        record * record_value = value->attr.record_value->comb.comb_record;
        if (record_value != NULL && value->attr.id != NULL)
        {
            param * param_value = record_find_param(record_value, value->attr.id);
            if (param_value != NULL)
            {
                value->attr.id_param_value = param_value;

                expr_set_comb_type(value, param_value);
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb.comb = COMB_TYPE_ERR;
                print_error_msg(value->line_no, "cannot find attribute %s in record %s",
                                value->attr.id, record_value->id);
            }
        }
    }
    else if (value->attr.record_value->comb.comb == COMB_TYPE_MODULE)
    {
        module_decl * module_decl_value = value->attr.record_value->comb.comb_module_decl;
        if (module_decl_value != NULL && value->attr.id != NULL)
        {
            never * nev = NULL;

            if (module_decl_value->type == MODULE_DECL_TYPE_MOD)
            {
                nev = module_decl_value->nev;
            }
            else if (module_decl_value->type == MODULE_DECL_TYPE_REF)
            {
                /* TODO: remove */
                printf("getting cycle\n");
                nev = module_decl_value->nev;
            }
            assert(nev != NULL);

            symtab_entry * entry = symtab_lookup(nev->stab, value->attr.id, SYMTAB_LOOKUP_LOCAL);
            if (entry != NULL)
            {
                /* TODO: remove */
                printf("MODULE %s %d\n\n", module_decl_value->id, module_decl_value->type);
                printf("id %s\n", value->attr.id);

                expr_set_comb_type_symtab(value, entry, result);
            }
            else
            {
                *result = TYPECHECK_FAIL;
                value->comb.comb = COMB_TYPE_ERR;
                print_error_msg(value->line_no, "cannot find attribute %s in module %s",
                                value->attr.id, module_decl_value->id);
            }
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot find module or attribute");
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no, "cannot get record attribute of type %s",
                        comb_type_str(value->attr.record_value->comb.comb));
    }

    return 0;
}        

int expr_check_type(symtab * tab, expr * value, func * func_value, unsigned int syn_level,
                    int * result)
{
    switch (value->type)
    {
    case EXPR_BOOL:
        value->comb.comb = COMB_TYPE_BOOL;
        break;
    case EXPR_INT:
        value->comb.comb = COMB_TYPE_INT;
        break;
    case EXPR_FLOAT:
        value->comb.comb = COMB_TYPE_FLOAT;
        break;
    case EXPR_CHAR:
        value->comb.comb = COMB_TYPE_CHAR;
        break;
    case EXPR_STRING:
        value->comb.comb = COMB_TYPE_STRING;
        break;
    case EXPR_ENUMTYPE:
        expr_enumtype_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_ID:
        expr_id_check_type(tab, value, result);
        if (*result == TYPECHECK_SUCC)
        {
            expr_id_gencode(syn_level, func_value, tab, value, result);
        }
        break;
    case EXPR_NIL:
        value->comb.comb = COMB_TYPE_NIL;
        break;
    case EXPR_C_NULL:
        value->comb.comb = COMB_TYPE_C_PTR;
        break;
    case EXPR_NEG:
        expr_neg_check_type(tab, value, func_value, syn_level, result);
    break;
    case EXPR_ADD:
    case EXPR_SUB:
        expr_add_sub_check_type(tab, value, func_value, syn_level, result);
    break;
    case EXPR_MUL:
        expr_mul_check_type(tab, value, func_value, syn_level, result);
    break;
    case EXPR_DIV:
        expr_div_check_type(tab, value, func_value, syn_level, result);
    break;
    case EXPR_MOD:
        expr_check_type(tab, value->left, func_value, syn_level, result);
        expr_check_type(tab, value->right, func_value, syn_level, result);
        if (value->left->comb.comb == COMB_TYPE_INT &&
            value->right->comb.comb == COMB_TYPE_INT)
        {
            value->comb.comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "cannot exec mod operation on types %s %s",
                            comb_type_str(value->left->comb.comb),
                            comb_type_str(value->right->comb.comb));
        }
        break;
    case EXPR_LT:
    case EXPR_GT:
    case EXPR_LTE:
    case EXPR_GTE:
    {
        expr_check_type(tab, value->left, func_value, syn_level, result);
        expr_check_type(tab, value->right, func_value, syn_level, result);
        if (value->left->comb.comb == COMB_TYPE_INT &&
            value->right->comb.comb == COMB_TYPE_INT)
        {
            value->comb.comb = COMB_TYPE_BOOL;
        }
        else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
                 value->right->comb.comb == COMB_TYPE_FLOAT)
        {
            value->comb.comb = COMB_TYPE_BOOL;
        }
        else if (value->left->comb.comb == COMB_TYPE_CHAR &&
                 value->right->comb.comb == COMB_TYPE_CHAR)
        {
            value->comb.comb = COMB_TYPE_BOOL;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot compare types %s %s",
                            comb_type_str(value->left->comb.comb),
                            comb_type_str(value->right->comb.comb));
        }
    }
    break;
    case EXPR_EQ:
    case EXPR_NEQ:
        expr_eq_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_AND:
    case EXPR_OR:
        expr_check_type(tab, value->left, func_value, syn_level, result);
        expr_check_type(tab, value->right, func_value, syn_level, result);
        if (value->left->comb.comb == COMB_TYPE_BOOL &&
            value->right->comb.comb == COMB_TYPE_BOOL)
        {
            value->comb.comb = COMB_TYPE_BOOL;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot compare types %s %s",
                            comb_type_str(value->left->comb.comb),
                            comb_type_str(value->right->comb.comb));
        }
        break;
    case EXPR_NOT:
        expr_check_type(tab, value->left, func_value, syn_level, result);
        if (value->left->comb.comb == COMB_TYPE_BOOL)
        {
            value->comb.comb = COMB_TYPE_BOOL;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot ne types %s",
                            comb_type_str(value->left->comb.comb));
        }
        break;
    case EXPR_SUP:
        expr_check_type(tab, value->left, func_value, syn_level, result);
        value->comb = value->left->comb;
        break;
    case EXPR_COND:
        expr_cond_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_ARRAY:
        expr_array_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_ARRAY_DEREF:
        expr_array_deref_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_RANGE_DIM:
        expr_range_dim_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_RANGE:
        if (value->range.range_dims != NULL)
        {
            expr_range_check_type(tab, value, func_value, syn_level, result);
        }
        break;
    case EXPR_SLICE:
        expr_slice_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        expr_call_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_check_type(tab, value->func_value, syn_level + 2, result);

            value->comb.comb = COMB_TYPE_FUNC;
            value->comb.comb_params = value->func_value->decl->params;
            value->comb.comb_ret = value->func_value->decl->ret;
        }
        break;
    case EXPR_SEQ:
        if (value->seq.list != NULL)
        {
            expr_seq_check_type(tab, value, func_value, syn_level, result);
        }
        break;
    case EXPR_ASS:
        expr_ass_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
        expr_check_type(tab, value->whileloop.cond, func_value, syn_level, result);
        expr_check_type(tab, value->whileloop.do_value, func_value, syn_level, result);
        
        if (value->whileloop.cond->comb.comb == COMB_TYPE_BOOL)
        {
            value->comb.comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "while loop condition type is %s",
                            comb_type_str(value->whileloop.cond->comb.comb));
        }
        break;
    case EXPR_FOR:
        expr_check_type(tab, value->forloop.init, func_value, syn_level, result);
        expr_check_type(tab, value->forloop.cond, func_value, syn_level, result);
        expr_check_type(tab, value->forloop.incr, func_value, syn_level, result);
        expr_check_type(tab, value->forloop.do_value, func_value, syn_level, result);
        
        if (value->forloop.cond->comb.comb == COMB_TYPE_BOOL)
        {
            value->comb.comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no,
                            "for loop condition is %s",
                            comb_type_str(value->forloop.cond->comb.comb));
        }
        break;
    case EXPR_FORIN:
        expr_forin_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_IFLET:
        expr_iflet_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_MATCH:
        expr_match_check_type(tab, value, func_value, syn_level, result);
        break;
    case EXPR_BUILD_IN:
        if (value->func_build_in.param != NULL)
        {
            expr_list_check_type(tab, value->func_build_in.param, func_value, syn_level, result);
        }
        expr_set_comb_type(value, value->func_build_in.ret);
        break;
    case EXPR_INT_TO_FLOAT:
        expr_check_type(tab, value->left, func_value, syn_level, result);
        if (value->left->comb.comb == COMB_TYPE_INT)
        {
            value->comb.comb = COMB_TYPE_FLOAT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot convert type %s to float",
                            value->line_no, comb_type_str(value->left->comb.comb));
        }
        break;
    case EXPR_FLOAT_TO_INT:
        expr_check_type(tab, value->left, func_value, syn_level, result);
        if (value->left->comb.comb == COMB_TYPE_FLOAT)
        {
            value->comb.comb = COMB_TYPE_INT;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "cannot convert type %s to int",
                            value->line_no, comb_type_str(value->left->comb.comb));
        }
        break;
    case EXPR_LISTCOMP:
        expr_listcomp_check_type(tab, value->listcomp_value, func_value, syn_level, result);
        if (*result == TYPECHECK_SUCC)
        {
            value->comb.comb = COMB_TYPE_ARRAY;
            value->comb.comb_ret = value->listcomp_value->ret;
            value->comb.comb_dims = 1;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            value->comb.comb = COMB_TYPE_ERR;
            print_error_msg(value->line_no, "list comprehension is not well formed");
        }
        break;
    case EXPR_ATTR:
        expr_attr_check_type(tab, value, func_value, syn_level, result);
        break;
    }
    return 0;
}

int expr_list_check_type(symtab * tab, expr_list * list, func * func_value, unsigned int syn_level,
                         int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_check_type(tab, value, func_value, syn_level, result);
        }
        node = node->next;
    }

    return 0;
}

int expr_seq_check_type(symtab * tab, expr * value, func * func_value, unsigned syn_level,
                        int * result)
{
    expr_list_node * node = NULL;

    expr_list_check_type(tab, value->seq.list, func_value, syn_level, result);

    node = value->seq.list->head;
    if (node != NULL && node->value != NULL)
    {
        expr * expr_last = node->value;

        value->comb = expr_last->comb;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;

        print_error_msg(value->line_no,
                        "no type in sequence %s", expr_type_str(value->type));
    }

    return 0;
}

int bind_check_type(symtab * tab, bind * value, func * func_value, unsigned int syn_level, 
                    int * result)
{
    switch (value->type)
    {
        case BIND_UNKNOWN:
            assert(0);
        break;
        case BIND_LET:
        case BIND_VAR:
            if (value->expr_value != NULL)
            {
                expr_check_type(tab, value->expr_value, func_value, syn_level, result);
                symtab_add_bind_from_bind(tab, value, syn_level, result);
            }
        break;
    }

    return 0;
}

int bind_list_check_type(symtab * tab, bind_list * list, func * func_value, unsigned int syn_level,
                         int * result)
{
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * bind_value = node->value;
        if (bind_value != NULL)
        {
            bind_check_type(tab, bind_value, func_value, syn_level, result);
        }
        node = node->next;
    }

    return 0;
}

int except_check_id(except * value, int * result)
{
    if (strcmp(value->id, EXCEPT_NO_INDEX_OOB_NAME) == 0)
    {
        value->no = EXCEPT_NO_INDEX_OOB;
    }
    else if (strcmp(value->id, EXCEPT_NO_ARR_SIZE_NAME) == 0)
    {
        value->no = EXCEPT_NO_ARR_SIZE;
    }
    else if (strcmp(value->id, EXCEPT_NO_DIVISION_NAME) == 0)
    {
        value->no = EXCEPT_NO_DIVISION;
    }
    else if (strcmp(value->id, EXCEPT_NO_INVALID_NAME) == 0)
    {
        value->no = EXCEPT_NO_INVALID;
    }
    else if (strcmp(value->id, EXCEPT_NO_OVERFLOW_NAME) == 0)
    {
        value->no = EXCEPT_NO_OVERFLOW;
    }
    else if (strcmp(value->id, EXCEPT_NO_UNDERFLOW_NAME) == 0)
    {
        value->no = EXCEPT_NO_UNDERFLOW;
    }
    else if (strcmp(value->id, EXCEPT_NO_INEXACT_NAME) == 0)
    {
        value->no = EXCEPT_NO_INEXACT;
    }
    else if (strcmp(value->id, EXCEPT_NIL_POINTER_NAME) == 0)
    {
        value->no = EXCEPT_NIL_POINTER;
    }
    else if (strcmp(value->id, EXCEPT_FFI_FAIL_NAME) == 0)
    {
        value->no = EXCEPT_FFI_FAIL;
    }
    else
    {
        value->no = EXCEPT_NO_UNKNOWN;
        *result = TYPECHECK_FAIL;
        print_error_msg(value->line_no,
                        "unknown exception %s", value->id);
    }
    return 0;
}

int except_check_type(symtab * tab, except * value,
                      func * func_value, unsigned int syn_level, int * result)
{
    if (value->id != NULL)
    {
        except_check_id(value, result);
    }

    if (value->expr_value != NULL)
    {
        expr_check_type(tab, value->expr_value, func_value, syn_level, result);
    }
    if (param_expr_cmp(func_value->decl->ret, value->expr_value) == TYPECHECK_FAIL)
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(value->line_no,
                        "incorrect return type in function %s",
                        func_value->decl->id);
    }

    return 0;
}

int except_list_check_type(symtab * tab, except_list * list, func * func_value,
                           unsigned int syn_level, int * result)
{
    except_list_node * node = list->tail;
    while (node != NULL)
    {
        except * value = node->value;
        if (value != NULL)
        {
            except_check_type(tab, value, func_value, syn_level, result);
        }
        node = node->next;
    }

    return 0;
}

int func_except_check_type(symtab * tab, func_except * value, func * func_value,
                           unsigned int syn_level, int * result)
{
    if (value->list != NULL)
    {
        except_list_check_type(tab, value->list, func_value, syn_level, result);
    }
    if (value->all != NULL)
    {
        except_check_type(tab, value->all, func_value, syn_level, result);
    }

    return 0;
}

int func_ffi_check_type(symtab * tab, func * func_value, unsigned int syn_level,
                        int * result)
{
#ifndef NO_FFI
    if (func_value->stab == NULL)
    {
        func_value->stab = symtab_new(32, SYMTAB_TYPE_FUNC, tab);
    }

    if (func_value->decl->params != NULL)
    {
        param_list_ffi_check_type(func_value->stab, func_value->decl->params, syn_level,
                                  result);
    }
    if (func_value->decl->ret != NULL)
    {
        param_ffi_check_type(func_value->stab, func_value->decl->ret, syn_level, result);
    }
#else
    *result = TYPECHECK_FAIL;
    print_error_msg(func_value->line_no,
                    "FFI not supported with JS %s", func_value->decl->id);
#endif

    return 0;
}                           

int func_native_check_type(symtab * tab, func * func_value, unsigned int syn_level,
                           int * result)
{
    int start = 1;

    if (func_value->stab == NULL)
    {
        func_value->stab = symtab_new(32, SYMTAB_TYPE_FUNC, tab);
    }
    if (func_value->decl->id)
    {
        symtab_add_func_from_func(func_value->stab, func_value, syn_level - 1,
                                  result);
    }
    if (func_value->decl->params != NULL)
    {
        func_enum_param_list(func_value->decl->params);
    }
    if (func_value->decl->params != NULL)
    {
        param_list_check_type(func_value->stab, func_value->decl->params, syn_level,
                              result);
    }
    if (func_value->decl->ret != NULL)
    {
        param_check_type(func_value->stab, func_value->decl->ret, syn_level, result);
    }
    if (func_value->decl->params)
    {
        symtab_add_param_from_param_list(func_value->stab, func_value->decl->params,
                                         syn_level, result);
    }
    if (func_value->except)
    {
        func_except_check_type(func_value->stab, func_value->except, func_value,
                               syn_level, result);
    }
    if (func_value->body && func_value->body->binds != NULL)
    {
        func_enum_bind_list(func_value->body->binds, start);
        start += func_value->body->binds->count;
    }
    if (func_value->body && func_value->body->binds)
    {
        bind_list_check_type(func_value->stab, func_value->body->binds,
                             func_value, syn_level, result);
    }
    if (func_value->body && func_value->body->funcs != NULL)
    {
        func_enum_func_list(func_value->body->funcs, start);
    }
    if (func_value->body && func_value->body->funcs)
    {
        symtab_add_func_from_func_list(func_value->stab,
                                       func_value->body->funcs, syn_level,
                                       result);
    }
    if (func_value->body && func_value->body->funcs)
    {
        func_list_check_type(func_value->stab, func_value->body->funcs,
                             syn_level, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_check_type(func_value->stab, func_value->body->ret, func_value, syn_level,
                        result);

        if (param_expr_cmp(func_value->decl->ret, func_value->body->ret) ==
            TYPECHECK_FAIL)
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(func_value->line_no,
                            "incorrect return type in function %s",
                            func_value->decl->id);
        }
    }

    /** set subfunction local/global indexes **/
    func_gencode_freevars(func_value, func_value->stab, result);

    return 0;
}

int func_check_type(symtab * tab, func * func_value, unsigned int syn_level,
                    int * result)
{
    switch (func_value->type)
    {
        case FUNC_TYPE_UNKNOWN:
            assert(0);
        break;
        case FUNC_TYPE_NATIVE:
            func_native_check_type(tab, func_value, syn_level, result);
        break;
        case FUNC_TYPE_FFI:
            func_ffi_check_type(tab, func_value, syn_level, result);
        break;
    }

    return 0;
}
                    
int func_list_check_type(symtab * tab, func_list * list, unsigned int syn_level,
                         int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_check_type(tab, func_value, syn_level + 1, result);
        }
        node = node->next;
    }

    return 0;
}

int never_add_enumtype(symtab * stab, enumtype * value, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(stab, value->id, SYMTAB_LOOKUP_GLOBAL);
    if (entry != NULL)
    {
        *result = TYPECHECK_FAIL;
        symtab_entry_exists(entry, value->line_no);
    }
    else
    {
        symtab_add_enumtype(stab, value, 0);
    }

    return 0;
}

int never_add_record(symtab * stab, record * record_value, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(stab, record_value->id, SYMTAB_LOOKUP_GLOBAL);
    if (entry != NULL)
    {
        *result = TYPECHECK_FAIL;
        symtab_entry_exists(entry, record_value->line_no);
    }
    else
    {
        symtab_add_record(stab, record_value, 0);
    }

    return 0;
}

int never_add_decl(symtab * stab, decl * value, int * result)
{
    switch (value->type)
    {
        case DECL_TYPE_ENUMTYPE:
            if (value->enumtype_value != NULL)
            {
                never_add_enumtype(stab, value->enumtype_value, result);
            }
        break;
        case DECL_TYPE_RECORD:
            if (value->record_value != NULL)
            {
                never_add_record(stab, value->record_value, result);
            }
        break;
    }
    
    return 0;
}

int never_add_decl_list(symtab * stab, decl_list * list, int * result)
{
    decl_list_node * node = list->tail;
    
    while (node != NULL)
    {
        decl * value = node->value;
        if (value != NULL)
        {
            never_add_decl(stab, value, result);
        }
        node = node->next;
    }

    return 0;
}

int never_add_module_decl(symtab * mtab, symtab * stab, char * use_id, module_decl * value, int * result)
{
    if (value->id != NULL && use_id != NULL)
    {
        if (strcmp(value->id, use_id) != 0)
        {
            print_error_msg(value->line_no,
                            "use %s and module id %s are different",
                            use_id, value->id);
            *result = TYPECHECK_FAIL;
        }
    }

    switch (value->type)
    {
        case MODULE_DECL_TYPE_MOD:
        {
            module_decl_check_type(mtab, value, result);
        }
        break;
        case MODULE_DECL_TYPE_REF:
        {
            value->id = use_id;
            module_decl_check_type(mtab, value, result);
        }
        break;
    }

    switch (value->type)
    {
        case MODULE_DECL_TYPE_MOD:
        {
            symtab_entry * entry = NULL;

            entry = symtab_lookup(stab, use_id, SYMTAB_LOOKUP_LOCAL);
            if (entry == NULL)
            {
                symtab_add_module_decl(stab, value, 0);
            }
            else
            {
                module_decl * al_module_decl = entry->module_decl_value;                
                *result = TYPECHECK_FAIL;
                print_error_msg(value->line_no,
                                "same use %s already used at %u",
                                use_id, al_module_decl->line_no);
            }
        }
        break;
        case MODULE_DECL_TYPE_REF:
        {
            symtab_entry * entry = NULL;
            symtab_entry * mentry = NULL;

            entry = symtab_lookup(stab, use_id, SYMTAB_LOOKUP_LOCAL);
            if (entry != NULL)
            {
                module_decl * al_module_decl = entry->module_decl_value;
                *result = TYPECHECK_FAIL;
                print_error_msg(value->line_no,
                                "same use %s already used at %u",
                                use_id, al_module_decl->line_no);
            }

            mentry = symtab_lookup(mtab, use_id, SYMTAB_LOOKUP_LOCAL);
            if (mentry != NULL)
            {
                module_decl * al_module_decl = mentry->module_decl_value;
                symtab_add_module_decl(stab, al_module_decl, 0);
            }
        }
        break;
    }

    return 0;
}

int never_add_use(symtab * mtab, symtab * stab, use * value, int * result)
{
    if (value->id != NULL && value->decl != NULL)
    {
        never_add_module_decl(mtab, stab, value->id, value->decl, result);
    }

    return 0;
}

int never_add_use_list(symtab * mtab, symtab * stab, use_list * list, int * result)
{
    use_list_node * node = list->tail;

    while (node != NULL)
    {
        use * value = node->value;
        if (value != NULL)
        {
            never_add_use(mtab, stab, value, result);
        }
        node = node->next;
    }

    return 0;
}

int enumerator_item_check_type(symtab * stab, enumerator * value, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(stab, value->id, SYMTAB_LOOKUP_GLOBAL);
    if (entry != NULL)
    {
        *result = TYPECHECK_FAIL;
        symtab_entry_exists(entry, value->line_no);
    }
    else
    {
        symtab_add_enumerator(stab, value, 0);
    }

    return 0;
}

int enumerator_record_check_type(symtab * gtab, symtab * stab, 
                                 enumerator * value, int * result)
{
    enumerator_item_check_type(stab, value, result);
    
    if (value->record_value != NULL)
    {
        record_check_type(gtab, value->record_value, result);
    }

    return 0;
}

int enumerator_check_type(symtab * gtab, symtab * stab, enumtype * enumtype_value, enumerator * value,
                          int * result)
{
    switch (value->type)
    {
        case ENUMERATOR_TYPE_ITEM:
            enumerator_item_check_type(stab, value, result);
        break;
        case ENUMERATOR_TYPE_RECORD:
            enumtype_value->type = ENUMTYPE_TYPE_RECORD;
            enumerator_record_check_type(gtab, stab, value, result);
        break;
    }

    return 0;
}

int enumerator_list_check_type(symtab * gtab, symtab * stab,
                               enumtype * enumtype_value, int * result)
{
    enumerator_list_node * node = enumtype_value->enums->tail;
    while (node != NULL)
    {
        enumerator * enumerator_value = node->value;
        if (enumerator_value != NULL)
        {
            enumerator_check_type(gtab, stab, enumtype_value, enumerator_value, result);
        }
        node = node->next;
    }

    return 0;
}

int enumtype_check_type(symtab * stab, enumtype * value, int * result)
{
    if (value->enums != NULL)
    {
        enumtype_enum_enumerator_list(value->enums);
    }

    if (value->enums != NULL)
    {
        enumerator_list_check_type(stab, value->stab, value, result);
    }

    return 0;
}            

int record_check_type(symtab * stab, record * record_value, int * result)
{
    if (record_value->params != NULL)
    {
        record_enum_param_list(record_value->params);
    }

    if (record_value->params != NULL)
    {
        param_list_check_type(stab, record_value->params, 0, result);
    }
    
    if (record_value->stab != NULL && record_value->params != NULL)
    {
        symtab_add_param_from_param_list(record_value->stab,
                                         record_value->params, 0, result);
    }
    
    return 0;
}

int decl_check_type(symtab * stab, decl * value, int * result)
{
    switch (value->type)
    {
        case DECL_TYPE_ENUMTYPE:
            if (value->enumtype_value != NULL)
            {
                enumtype_check_type(stab, value->enumtype_value, result);
            }
        break;
        case DECL_TYPE_RECORD:
            if (value->record_value != NULL)
            {
                record_check_type(stab, value->record_value, result);
            }
        break;
    }

    return 0;
}

int decl_list_check_type(symtab * stab, decl_list * list, int * result)
{
    decl_list_node * node = list->tail;
    
    while (node != NULL)
    {
        decl * decl_value = node->value;
        if (decl_value != NULL)
        {
            decl_check_type(stab, decl_value, result);
        }
        node = node->next;
    }

    return 0;
}

int never_check_type(symtab * mtab, never * nev, int * result)
{
    int start = 0;
    unsigned int syn_level = 0;
    
    if (nev->uses != NULL)
    {
        never_add_use_list(mtab, nev->stab, nev->uses, result);
    }

    if (nev->stab != NULL && nev->decls != NULL)
    {
        /* add decls to symtab */
        never_add_decl_list(nev->stab, nev->decls, result);

        /* check decls */
        decl_list_check_type(nev->stab, nev->decls, result);
    }

    symtab_add_func_from_func_list(nev->stab, nev->funcs, syn_level, result);

    if (nev->stab != NULL && nev->binds != NULL)
    {
        func_enum_bind_list(nev->binds, start);
        bind_list_check_type(nev->stab, nev->binds, NULL, syn_level, result);
        start += nev->binds->count;
    }

    func_enum_func_list(nev->funcs, start); 
    func_list_check_type(nev->stab, nev->funcs, syn_level, result);

    return 0;
}

int func_entry_check_num_params(param_list * params)
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

int func_entry_check_type(func * func_value, int * result)
{
    int is_entry = 1;

    if (func_value->decl->params != NULL &&
        func_entry_check_num_params(func_value->decl->params) == 0)
    {
        is_entry = 0;
    }
    if (func_value->decl->ret == NULL || param_is_num(func_value->decl->ret) == TYPECHECK_FAIL)
    {
        is_entry = 0;
    }

    if (is_entry)
    {
        func_value->entry = 1;
    }

    return 0;
}

int func_list_entry_check_type(func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_entry_check_type(func_value, result);
        }
        node = node->next;
    }

    return 0;
}

int never_sem_check(symtab * mtab, never * nev, int * result)
{
    if (nev->stab == NULL)
    {
        nev->stab = symtab_new(32, SYMTAB_TYPE_FUNC, mtab);
    }

    never_check_type(mtab, nev, result);

    func_list_entry_check_type(nev->funcs, result);

    return 0;
}

int module_decl_check_type(symtab * mtab, module_decl * value, int * result)
{
    if (value->id != NULL)
    {
        symtab_entry * mentry = NULL;

        mentry = symtab_lookup(mtab, value->id, SYMTAB_LOOKUP_LOCAL);
        if (mentry == NULL)
        {
            symtab_add_module_decl(mtab, value, 0);
        }
        else
        {
            module_decl * al_module_decl = mentry->module_decl_value;
            if (al_module_decl->type == MODULE_DECL_TYPE_REF &&
                value->type == MODULE_DECL_TYPE_MOD)
            {
                al_module_decl->nev = value->nev;
            }
        }
    }

    if (value->type == MODULE_DECL_TYPE_MOD && value->nev != NULL)
    {
        never_sem_check(mtab, value->nev, result);
    }

    return 0;
}

int main_check_type(never * nev, int * result)
{
    symtab * mtab = symtab_new(32, SYMTAB_TYPE_FUNC, NULL);

    never_sem_check(mtab, nev, result);

    /* TODO: move it outside */
    symtab_delete(mtab);

    return 0;
}
