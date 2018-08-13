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
#include "gencode.h"
#include "freevar.h"
#include "symtab.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>

/* GP old, FP old, IP old */
#define NUM_FRAME_PTRS 3

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

/**
 * free variables
 */
int expr_id_gencode(unsigned int syn_level, func * func_value, expr * value,
                    int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(func_value->stab, value->id.id, SYMTAB_NESTED);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value != NULL)
        {
            func * sup_func_value = entry->func_value;
            if (sup_func_value)
            {
                if (entry->syn_level == 0)
                {
                    value->id.id_type_value = ID_TYPE_FUNC_TOP;
                    value->id.id_func_value = sup_func_value;
                }
                else if (syn_level - 1 == entry->syn_level &&
                         func_value == sup_func_value) /* recursive call */
                {
                    value->id.id_type_value = ID_TYPE_FUNC_NEST;
                    value->id.id_func_value = sup_func_value;
                }
                else if (syn_level == entry->syn_level)
                {
                    value->id.id_type_value = ID_TYPE_FUNC;
                    value->id.id_func_value = sup_func_value;
                }
                else
                {
                    freevar * freevar_value = NULL;
                    if (func_value->freevars == NULL)
                    {
                        func_value->freevars = freevar_list_new();
                    }

                    freevar_value =
                        freevar_list_add(func_value->freevars, value->id.id);

                    freevar_value->type = FREEVAR_FUNC;
                    freevar_value->func_value = sup_func_value;

                    value->id.id_type_value = ID_TYPE_GLOBAL;
                    value->id.id_freevar_value = freevar_value;
                }
            }
        }
        else if (entry->type == SYMTAB_PARAM && entry->param_value != NULL)
        {
            param * param_value = entry->param_value;
            if (param_value->type == PARAM_INT || param_value->type == PARAM_FLOAT ||
                param_value->type == PARAM_DIM || param_value->type == PARAM_ARRAY ||
                param_value->type == PARAM_FUNC)
            {
                if (syn_level == entry->syn_level)
                {
                    value->id.id_type_value = ID_TYPE_LOCAL;
                    value->id.id_param_value = param_value;
                }
                else
                {
                    freevar * freevar_value = NULL;
                    if (func_value->freevars == NULL)
                    {
                        func_value->freevars = freevar_list_new();
                    }

                    freevar_value =
                        freevar_list_add(func_value->freevars, value->id.id);

                    value->id.id_type_value = ID_TYPE_GLOBAL;
                    value->id.id_freevar_value = freevar_value;
                }
            }
            else
            {
                printf("unknown param type %d\n", param_value->type);
                assert(0);
            }
        }
        else if (entry->type == SYMTAB_BIND && entry->bind_value != NULL)
        {
            bind * bind_value = entry->bind_value;
            if (bind_value->type == BIND_LET || bind_value->type == BIND_VAR)
            {
                if (syn_level == entry->syn_level)
                {
                    value->id.id_type_value = ID_TYPE_BIND;
                    value->id.id_bind_value = bind_value;
                }
                else
                {
                    freevar * freevar_value = NULL;
                    if (func_value->freevars == NULL)
                    {
                        func_value->freevars = freevar_list_new();
                    }
                    
                    freevar_value =
                        freevar_list_add(func_value->freevars, value->id.id);
                        
                        value->id.id_type_value = ID_TYPE_GLOBAL;
                        value->id.id_freevar_value = freevar_value;
                }
            }
            else
            {
                fprintf(stderr, "unknown bind type %d\n", bind_value->type);
            }            
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        *result = 1;
        print_error_msg(
            value->line_no,
            "cannot find variable %s, at this stage it is very bad\n",
            value->id);
        assert(0);
    }

    return 0;
}

int expr_gencode(unsigned int syn_level, func * func_value, expr * value,
                 int * result)
{
    switch (value->type)
    {
    case EXPR_INT:
        break;
    case EXPR_FLOAT:
        /* printf("gencode EXPR_FLOAT %f\n", value->float_value); */
        break;
    case EXPR_ID:
        expr_id_gencode(syn_level, func_value, value, result);
        break;
    case EXPR_NEG:
        expr_gencode(syn_level, func_value, value->left, result);
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
        expr_gencode(syn_level, func_value, value->left, result);
        expr_gencode(syn_level, func_value, value->right, result);
        break;
    case EXPR_AND:
    case EXPR_OR:
        expr_gencode(syn_level, func_value, value->left, result);
        expr_gencode(syn_level, func_value, value->right, result);
        break;
    case EXPR_NOT:
        expr_gencode(syn_level, func_value, value->left, result);
        break;
    case EXPR_SUP:
        expr_gencode(syn_level, func_value, value->left, result);
        break;
    case EXPR_COND:
        expr_gencode(syn_level, func_value, value->left, result);
        expr_gencode(syn_level, func_value, value->middle, result);
        expr_gencode(syn_level, func_value, value->right, result);
        break;
    case EXPR_ARRAY:
        if (value->array.array_value)
        {
            array_gencode(syn_level, func_value, value->array.array_value,
                          result);
        }
        break;
    case EXPR_ARRAY_DEREF:
        expr_gencode(syn_level, func_value, value->array_deref.array_expr,
                     result);
        if (value->array_deref.ref != NULL)
        {
            expr_list_gencode(syn_level, func_value, value->array_deref.ref,
                              result);
        }
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        expr_gencode(syn_level, func_value, value->call.func_expr, result);
        if (value->call.params != NULL)
        {
            expr_list_gencode(syn_level, func_value, value->call.params, result);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value != NULL)
        {
            func_gencode(syn_level + 2, value->func_value, result);
        }
        break;
    case EXPR_SEQ:
        assert(0);
        break;
    case EXPR_BUILD_IN:
        expr_list_gencode(syn_level, func_value, value->func_build_in.param,
                          result);
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        expr_gencode(syn_level, func_value, value->left, result);
        break;
    }
    return 0;
}

int expr_list_gencode(unsigned int syn_level, func * func_value,
                      expr_list * list, int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            expr_gencode(syn_level, func_value, value, result);
        }
        node = node->next;
    }
    return 0;
}

int array_gencode(unsigned int syn_level, func * func_value,
                  array * array_value, int * result)
{
    if (array_value->type == ARRAY_INIT || array_value->type == ARRAY_SUB)
    {
        expr_list_gencode(syn_level, func_value, array_value->elements, result);
    }
    else if (array_value->type == ARRAY_DIMS)
    {
        expr_list_gencode(syn_level, func_value, array_value->dims, result);
    }

    return 0;
}

int bind_gencode(unsigned int syn_level, func * func_value, bind * bind_value,
                 int * result)
{
    switch (bind_value->type)
    {
        case BIND_UNKNOWN:
            fprintf(stderr, "unknown bind type\n");
            assert(0);
        break;
        case BIND_LET:
        case BIND_VAR:
            if (bind_value->expr_value != NULL)
            {
                expr_gencode(syn_level, func_value, bind_value->expr_value, result);
            }
        break;
    }

    return 0;
}

int bind_list_gencode(unsigned int syn_level, func * func_value,
                      bind_list * list, int * result)
{
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * bind_value = node->value;
        if (bind_value)
        {
            bind_gencode(syn_level, func_value, bind_value, result);
        }
        node = node->next;
    }
    return 0;
}

/**
 * free variables
 */
int func_gencode_freevars_freevar(func * func_value, freevar * freevar_value,
                                  int * result)
{
    /** search in symtab */
    /** if found then mark as local */
    /** otherwise mark as global and set index */
    symtab_entry * entry = NULL;

    entry = symtab_lookup(func_value->stab, freevar_value->id, SYMTAB_FLAT);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value)
        {
            freevar_value->type = FREEVAR_FUNC;
            freevar_value->func_value = entry->func_value;
        }
        else if (entry->type == SYMTAB_PARAM && entry->param_value)
        {
            freevar_value->type = FREEVAR_LOCAL;
            freevar_value->local_value = entry->param_value;
        }
        else if (entry->type == SYMTAB_BIND && entry->bind_value)
        {
            freevar_value->type = FREEVAR_BIND;
            freevar_value->bind_value = entry->bind_value;
        }
        else
        {
            assert(0);
        }
    }
    else
    {
        freevar * freevar_sup_value = NULL;
        if (func_value->freevars == NULL)
        {
            func_value->freevars = freevar_list_new();
        }

        freevar_sup_value =
            freevar_list_add(func_value->freevars, freevar_value->id);

        freevar_value->type = FREEVAR_GLOBAL;
        freevar_value->global_value = freevar_sup_value;
    }

    return 0;
}

int func_gencode_freevars_expr(func * func_value, expr * value, int * result)
{
    switch (value->type)
    {
    case EXPR_INT:
        /* not possible */
        break;
    case EXPR_FLOAT:
        /* not possible */
        break;
    case EXPR_ID:
        /* not possible */
        break;
    case EXPR_NEG:
        func_gencode_freevars_expr(func_value, value->left, result);
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
        func_gencode_freevars_expr(func_value, value->left, result);
        func_gencode_freevars_expr(func_value, value->right, result);
        break;
    case EXPR_AND:
    case EXPR_OR:
        func_gencode_freevars_expr(func_value, value->left, result);
        func_gencode_freevars_expr(func_value, value->right, result);
        break;
    case EXPR_NOT:
        func_gencode_freevars_expr(func_value, value->left, result);
        break;
    case EXPR_SUP:
        func_gencode_freevars_expr(func_value, value->left, result);
        break;
    case EXPR_COND:
        func_gencode_freevars_expr(func_value, value->left, result);
        func_gencode_freevars_expr(func_value, value->middle, result);
        func_gencode_freevars_expr(func_value, value->right, result);
        break;
    case EXPR_ARRAY:
        if (value->array.array_value->dims)
        {
            func_gencode_freevars_expr_list(
                func_value, value->array.array_value->dims, result);
        }
        if (value->array.array_value->elements)
        {
            func_gencode_freevars_expr_list(
                func_value, value->array.array_value->elements, result);
        }
        break;
    case EXPR_ARRAY_DEREF:
        func_gencode_freevars_expr(func_value, value->array_deref.array_expr,
                                   result);
        if (value->array_deref.ref != NULL)
        {
            func_gencode_freevars_expr_list(func_value, value->array_deref.ref,
                                            result);
        }
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        func_gencode_freevars_expr(func_value, value->call.func_expr, result);
        if (value->call.params)
        {
            func_gencode_freevars_expr_list(func_value, value->call.params,
                                            result);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_gencode_freevars_func(func_value, value->func_value, result);
        }
        break;
    case EXPR_SEQ:
        assert(0);
        break;
    case EXPR_BUILD_IN:
        func_gencode_freevars_expr_list(func_value, value->func_build_in.param,
                                        result);
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        func_gencode_freevars_expr(func_value, value->left, result);
        break;
    }

    return 0;
}

int func_gencode_freevars_expr_list(func * func_value, expr_list * list,
                                    int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            func_gencode_freevars_expr(func_value, value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars_bind(func * func_value, bind * bind_value,
                               int * result)
{
    if (bind_value->expr_value)
    {
        func_gencode_freevars_expr(func_value, bind_value->expr_value, result);
    }
    return 0;
}

int func_gencode_freevars_bind_list(func * func_value, bind_list * list,
                                    int * result)
{
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * bind_value = node->value;
        if (bind_value != NULL)
        {
            func_gencode_freevars_bind(func_value, bind_value, result);
        }
        node = node->next;
    }
    return 0;
}

int func_gencode_freevars_func(func * func_value, func * subfunc_value,
                               int * result)
{
    if (subfunc_value->freevars)
    {
        freevar_list_node * node = subfunc_value->freevars->tail;
        while (node != NULL)
        {
            freevar * freevar_value = node->value;
            if (freevar_value != NULL)
            {
                func_gencode_freevars_freevar(func_value, freevar_value,
                                              result);
            }
            node = node->next;
        }
    }

    return 0;
}

int func_gencode_freevars_func_list(func * func_value, func_list * list,
                                    int * result)
{
    func_list_node * node = list->tail;

    while (node != NULL)
    {
        func * subfunc_value = node->value;
        if (subfunc_value != NULL)
        {
            func_gencode_freevars_func(func_value, subfunc_value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars(func * func_value, int * result)
{
    if (func_value->body && func_value->body->binds)
    {
        func_gencode_freevars_bind_list(func_value, func_value->body->binds,
                                        result);
    }
    if (func_value->body && func_value->body->funcs)
    {
        func_gencode_freevars_func_list(func_value, func_value->body->funcs,
                                        result);
    }
    if (func_value->body && func_value->body->ret)
    {
        func_gencode_freevars_expr(func_value, func_value->body->ret, result);
    }

    return 0;
}

int func_gencode(unsigned int syn_level, func * func_value, int * result)
{
    int start = 1;
    if (func_value->params != NULL)
    {
        func_enum_param_list(func_value->params);
    }
    if (func_value->body && func_value->body->binds != NULL)
    {
        func_enum_bind_list(func_value->body->binds, start);
        start += func_value->body->binds->count;
    }
    if (func_value->body && func_value->body->funcs != NULL)
    {
        func_enum_func_list(func_value->body->funcs, start);
    }

    if (func_value->body && func_value->body->binds)
    {
        bind_list_gencode(syn_level, func_value, func_value->body->binds,
                          result);
    }
    if (func_value->body && func_value->body->funcs)
    {
        func_list_gencode(syn_level, func_value->body->funcs, result);
    }
    if (func_value->body && func_value->body->ret)
    {
        expr_gencode(syn_level, func_value, func_value->body->ret, result);
    }

    /** set subfunction local/global indexes **/
    func_gencode_freevars(func_value, result);

    return 0;
}

int func_list_gencode(unsigned int syn_level, func_list * list, int * result)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value)
        {
            func_gencode(syn_level + 1, func_value, result);
        }
        node = node->next;
    }
    return 0;
}

int never_gencode(never * nev)
{
    int gencode_res = GENCODE_SUCC;
    unsigned int syn_level = 0;

    if (nev->funcs)
    {
        func_list_gencode(syn_level, nev->funcs, &gencode_res);
    }

    return gencode_res;
}

/**
 * emit code
 */
int expr_int_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_INT;
    bc.integer.value = value->int_value;

    bytecode_add(code, &bc);

    return 0;
}

int expr_float_emit(expr * value, int stack_level, bytecode_list * code,
                    int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_FLOAT;
    bc.real.value = value->float_value;

    bytecode_add(code, &bc);

    return 0;
}

int func_freevar_id_local_emit(freevar * value, int stack_level,
                               bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    if (value->local_value->type == PARAM_DIM)
    {
        bc.type = BYTECODE_ID_DIM_LOCAL;
        bc.id_dim_local.stack_level = stack_level;
        bc.id_dim_local.index = value->local_value->array->index;
        bc.id_dim_local.dim_index = value->local_value->index;
    }
    else
    {
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->local_value->index;
    }

    bytecode_add(code, &bc);

    return 0;
}

int func_freevar_id_bind_emit(freevar * value, int stack_level,
                              bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level;
    bc.id_local.index = value->bind_value->index;

    bytecode_add(code, &bc);

    return 0;
}

int func_freevar_emit(freevar * value, int stack_level, bytecode_list * code,
                      int * result)
{
    bytecode bc = { 0 };

    switch (value->type)
    {
    case FREEVAR_UNKNOWN:
        print_error_msg(0, "unknown freevar %s during emit\n", value->id);
        assert(0);
        break;
    case FREEVAR_LOCAL:
        func_freevar_id_local_emit(value, stack_level, code, result);
        break;
    case FREEVAR_BIND:
        func_freevar_id_bind_emit(value, stack_level, code, result);
        break;
    case FREEVAR_GLOBAL:
        bc.type = BYTECODE_ID_GLOBAL;
        bc.id_global.index = value->global_value->index;

        bytecode_add(code, &bc);
        break;
    case FREEVAR_FUNC:
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->func_value->index;

        bytecode_add(code, &bc);
        break;
    }

    return 0;
}

int func_freevar_list_emit(freevar_list * freevars, int stack_level,
                           bytecode_list * code, int * result)
{
    int e = 0;
    freevar_list_node * node = freevars->tail;
    while (node != NULL)
    {
        freevar * value = node->value;
        if (value != NULL)
        {
            func_freevar_emit(value, stack_level + e++, code, result);
        }
        node = node->next;
    }

    return 0;
}

int expr_id_local_emit(expr * value, int stack_level, bytecode_list * code,
                       int * result)
{
    bytecode bc = { 0 };

    if (value->id.id_param_value->type == PARAM_DIM)
    {
        bc.type = BYTECODE_ID_DIM_LOCAL;
        bc.id_dim_local.stack_level = stack_level;
        bc.id_dim_local.index = value->id.id_param_value->array->index;
        bc.id_dim_local.dim_index = value->id.id_param_value->index;
    }
    else
    {
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->id.id_param_value->index;
    }

    bytecode_add(code, &bc);

    return 0;
}

int expr_id_bind_emit(expr * value, int stack_level, bytecode_list * code,
                       int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level;
    bc.id_local.index = value->id.id_bind_value->index;

    bytecode_add(code, &bc);

    return 0;
}

int expr_id_global_emit(expr * value, int stack_value, bytecode_list * code,
                        int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_GLOBAL;
    bc.id_global.index = value->id.id_freevar_value->index;

    bytecode_add(code, &bc);

    return 0;
}

int expr_id_func_top_emit(func * func_value, int stack_level,
                          bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = 0;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(code, &bc);

    return 0;
}

int expr_id_func_emit(func * func_value, int stack_level, bytecode_list * code,
                      int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level;
    bc.id_local.index = func_value->index;

    bytecode_add(code, &bc);

    return 0;
}

int expr_id_func_nest_emit(func * func_value, int stack_level,
                           bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_COPYGLOB;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(code, &bc);

    return 0;
}

int expr_id_emit(expr * value, int stack_level, bytecode_list * code,
                 int * result)
{
    switch (value->id.id_type_value)
    {
    case ID_TYPE_UNKNOWN:
        print_error_msg(value->line_no,
                        "not recognized id, at this stage it is very bad\n");
        assert(0);
        break;
    case ID_TYPE_LOCAL:
        expr_id_local_emit(value, stack_level, code, result);
        break;
    case ID_TYPE_GLOBAL:
        expr_id_global_emit(value, stack_level, code, result);
        break;
    case ID_TYPE_BIND:
        expr_id_bind_emit(value, stack_level, code, result);
        break;        
    case ID_TYPE_FUNC_TOP:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_top_emit(value->id.id_func_value, stack_level, code,
                                  result);
        }
        break;
    case ID_TYPE_FUNC:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_emit(value->id.id_func_value, stack_level, code,
                              result);
        }
        break;
    case ID_TYPE_FUNC_NEST:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_nest_emit(value->id.id_func_value, stack_level, code,
                                   result);
        }
        break;
    }
    return 0;
}

int expr_neg_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };
    
    expr_emit(value->left, stack_level, code, result);

    if (value->comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_NEG_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_NEG_FLOAT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->comb_ret->type == PARAM_INT)
    {
        bc.type = BYTECODE_OP_NEG_ARR_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->comb_ret->type == PARAM_FLOAT)
    {
        bc.type = BYTECODE_OP_NEG_ARR_FLOAT;
        bytecode_add(code, &bc);
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(value->line_no, "cannot neg type %s\n",
                        comb_type_str(value->comb));
        assert(0);
    }

    return 0;
}

int expr_add_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };
    
    expr_emit(value->left, stack_level, code, result);
    expr_emit(value->right, stack_level + 1, code, result);

    if (value->comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_ADD_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_ADD_FLOAT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->comb_ret->type == PARAM_INT)
    {
        bc.type = BYTECODE_OP_ADD_ARR_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->comb_ret->type == PARAM_FLOAT)
    {
        bc.type = BYTECODE_OP_ADD_ARR_FLOAT;
        bytecode_add(code, &bc);
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(value->line_no, "cannot add type %s\n",
                        comb_type_str(value->comb));
        assert(0);
    }

    return 0;
}

int expr_sub_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, code, result);
    expr_emit(value->right, stack_level + 1, code, result);

    if (value->comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_SUB_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_SUB_FLOAT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->comb_ret->type == PARAM_INT)
    {
        bc.type = BYTECODE_OP_SUB_ARR_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->comb_ret->type == PARAM_FLOAT)
    {
        bc.type = BYTECODE_OP_SUB_ARR_FLOAT;
        bytecode_add(code, &bc);
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(value->line_no, "cannot sub type %s\n",
                        comb_type_str(value->comb));
        assert(0);
    }

    return 0;
}

int expr_mul_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, code, result);
    expr_emit(value->right, stack_level + 1, code, result);

    if (value->comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_MUL_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_MUL_FLOAT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->left->comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_MUL_ARR_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->left->comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_MUL_ARR_FLOAT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->comb_ret->type == PARAM_INT &&
             value->left->comb == COMB_TYPE_ARRAY &&
             value->right->comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_MUL_ARR_ARR_INT;
        bytecode_add(code, &bc);
    }
    else if (value->comb == COMB_TYPE_ARRAY &&
             value->comb_ret->type == PARAM_FLOAT &&
             value->left->comb == COMB_TYPE_ARRAY &&
             value->right->comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_MUL_ARR_ARR_FLOAT;
        bytecode_add(code, &bc);
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(value->line_no, "cannot mul type %s\n",
                        comb_type_str(value->comb));
        assert(0);
    }

    return 0;
}

int expr_and_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };
    bytecode *condzA, *condzB, *jumpE;
    bytecode *labelEF, *labelE;

    expr_emit(value->left, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condzA = bytecode_add(code, &bc);

    expr_emit(value->right, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condzB = bytecode_add(code, &bc);

    bc.type = BYTECODE_INT;
    bc.integer.value = 1;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpE = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelEF = bytecode_add(code, &bc);
    condzA->jump.offset = labelEF->addr - condzA->addr;
    condzB->jump.offset = labelEF->addr - condzB->addr;

    bc.type = BYTECODE_INT;
    bc.integer.value = 0;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(code, &bc);
    jumpE->jump.offset = labelE->addr - jumpE->addr;

    return 0;
}

int expr_or_emit(expr * value, int stack_level, bytecode_list * code,
                 int * result)
{
    bytecode bc = { 0 };
    bytecode *condzA, *condzB, *jumpET, *jumpE;
    bytecode *labelB, *labelET, *labelEF, *labelE;

    expr_emit(value->left, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condzA = bytecode_add(code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpET = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(code, &bc);
    condzA->jump.offset = labelB->addr - condzA->addr;

    expr_emit(value->right, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condzB = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelET = bytecode_add(code, &bc);
    jumpET->jump.offset = labelET->addr - jumpET->addr;

    bc.type = BYTECODE_INT;
    bc.integer.value = 1;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpE = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelEF = bytecode_add(code, &bc);
    condzB->jump.offset = labelEF->addr - condzB->addr;

    bc.type = BYTECODE_INT;
    bc.integer.value = 0;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(code, &bc);
    jumpE->jump.offset = labelE->addr - jumpE->addr;

    return 0;
}

int expr_not_emit(expr * value, int stack_level, bytecode_list * code,
                  int * result)
{
    bytecode bc = { 0 };
    expr_emit(value->left, stack_level, code, result);

    if (value->comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_NOT_INT;
        bytecode_add(code, &bc);
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(value->line_no, "cannot not type %s\n",
                        comb_type_str(value->comb));
        assert(0);
    }

    return 0;
}

int expr_cond_emit(expr * value, int stack_level, bytecode_list * code,
                   int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condz;
    bytecode *labelA, *labelB;

    expr_emit(value->left, stack_level, code, result);

    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(code, &bc);

    expr_emit(value->middle, stack_level, code, result);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(code, &bc);
    condz->jump.offset = labelA->addr - condz->addr;

    expr_emit(value->right, stack_level, code, result);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(code, &bc);
    cond->jump.offset = labelB->addr - cond->addr;

    return 0;
}

int expr_call_emit(expr * value, int stack_level, bytecode_list * code,
                   int * result)
{
    int v = NUM_FRAME_PTRS;
    bytecode bc = { 0 };
    bytecode *mark, *label;

    bc.type = BYTECODE_MARK;
    mark = bytecode_add(code, &bc);

    if (value->call.params)
    {
        expr_list_emit(value->call.params, stack_level + v, code, result);
        v += value->call.params->count;
    }
    expr_emit(value->call.func_expr, stack_level + v, code, result);

    bc.type = BYTECODE_CALL;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    label = bytecode_add(code, &bc);
    mark->mark.addr = label->addr;

    return 0;
}

int expr_last_call_emit(expr * value, int stack_level, bytecode_list * code,
                        int * result)
{
    int v = 0;
    bytecode bc = { 0 };

    if (value->call.params)
    {
        expr_list_emit(value->call.params, stack_level + v, code, result);
        v += value->call.params->count;
    }
    expr_emit(value->call.func_expr, stack_level + v, code, result);

    bc.type = BYTECODE_SLIDE;
    bc.slide.q = stack_level + v;
    bc.slide.m = v + 1;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_CALL;
    bytecode_add(code, &bc);

    return 0;
}

int expr_emit(expr * value, int stack_level, bytecode_list * code, int * result)
{
    bytecode bc = { 0 };

    switch (value->type)
    {
    case EXPR_INT:
        expr_int_emit(value, stack_level, code, result);
        break;
    case EXPR_FLOAT:
        expr_float_emit(value, stack_level, code, result);
        break;
    case EXPR_ID:
        expr_id_emit(value, stack_level, code, result);
        break;
    case EXPR_NEG:
        expr_neg_emit(value, stack_level, code, result);
        break;
    case EXPR_ADD:
        expr_add_emit(value, stack_level, code, result);
        break;
    case EXPR_SUB:
        expr_sub_emit(value, stack_level, code, result);
        break;
    case EXPR_MUL:
        expr_mul_emit(value, stack_level, code, result);    
        break;
    case EXPR_DIV:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        bc.type = BYTECODE_LINE;
        bc.line.no = value->line_no;
        bytecode_add(code, &bc);

        if (value->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_DIV_INT;
            bytecode_add(code, &bc);
        }
        else if (value->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_DIV_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot div type %s\n",
                            comb_type_str(value->comb));
            assert(0);
        }
        break;
    case EXPR_MOD:
        expr_emit(value->right, stack_level, code, result);
        expr_emit(value->left, stack_level + 1, code, result);

        bc.type = BYTECODE_LINE;
        bc.line.no = value->line_no;
        bytecode_add(code, &bc);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_MOD_INT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot mod type %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
            assert(0);
        }
        break;
    case EXPR_LT:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_LT_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_LT_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot lt different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
            assert(0);
        }
        break;
    case EXPR_GT:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_GT_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_GT_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot gt different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
            assert(0);
        }
        break;
    case EXPR_LTE:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_LTE_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_LTE_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no,
                            "cannot lte different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
            assert(0);
        }
        break;
    case EXPR_GTE:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_GTE_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_GTE_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no,
                            "cannot gte different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
            assert(0);
        }
        break;
    case EXPR_EQ:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_EQ_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_EQ_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot eq different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
            assert(0);
        }
        break;
    case EXPR_NEQ:
        expr_emit(value->left, stack_level, code, result);
        expr_emit(value->right, stack_level + 1, code, result);

        if (value->left->comb == COMB_TYPE_INT &&
            value->right->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_OP_NEQ_INT;
            bytecode_add(code, &bc);
        }
        else if (value->left->comb == COMB_TYPE_FLOAT &&
                 value->right->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_OP_NEQ_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no,
                            "cannot neq different types %s %s\n",
                            comb_type_str(value->left->comb),
                            comb_type_str(value->right->comb));
            assert(0);
        }

        break;
    case EXPR_AND:
        expr_and_emit(value, stack_level, code, result);
        break;
    case EXPR_OR:
        expr_or_emit(value, stack_level, code, result);
        break;
    case EXPR_NOT:
        expr_not_emit(value, stack_level, code, result);
        break;
    case EXPR_SUP:
        expr_emit(value->left, stack_level, code, result);
        break;
    case EXPR_COND:
        expr_cond_emit(value, stack_level, code, result);
        break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            expr_array_emit(value, stack_level, code, result);
        }
        break;
    case EXPR_ARRAY_DEREF:
        expr_array_deref_emit(value, stack_level, code, result);
        break;
    case EXPR_CALL:
        expr_call_emit(value, stack_level, code, result);
        break;
    case EXPR_LAST_CALL:
        expr_last_call_emit(value, stack_level, code, result);
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_emit(value->func_value, stack_level, code, result);
        }
        break;
    case EXPR_SEQ:
        assert(0);
        break;
    case EXPR_BUILD_IN:
        expr_list_emit(value->func_build_in.param, stack_level, code, result);

        bc.type = BYTECODE_BUILD_IN;
        bc.build_in.id = value->func_build_in.id;
        bytecode_add(code, &bc);
        break;
    case EXPR_INT_TO_FLOAT:
        expr_emit(value->left, stack_level, code, result);

        if (value->left->comb == COMB_TYPE_INT)
        {
            bc.type = BYTECODE_INT_TO_FLOAT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot convert type %s to float\n",
                            comb_type_str(value->left->comb));
            assert(0);
        }

        break;
    case EXPR_FLOAT_TO_INT:
        expr_emit(value->left, stack_level, code, result);

        if (value->left->comb == COMB_TYPE_FLOAT)
        {
            bc.type = BYTECODE_FLOAT_TO_INT;
            bytecode_add(code, &bc);
        }
        else
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "cannot convert type %s to int\n",
                            comb_type_str(value->left->comb));
            assert(0);
        }
        break;
    }
    return 0;
}

int expr_list_emit(expr_list * list, int stack_level, bytecode_list * code,
                   int * result)
{
    int e = 0;
    expr_list_node * node = list->head;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_emit(value, stack_level + e++, code, result);
        }
        node = node->prev;
    }

    return 0;
}

int array_dims_emit(array * array_value, int stack_level, bytecode_list * code,
                    int * result)
{
    bytecode bc = { 0 };
    expr_list_emit(array_value->dims, stack_level, code, result);

    bc.type = BYTECODE_MK_ARRAY;
    bc.mk_array.dims = array_value->dims->count;

    bytecode_add(code, &bc);

    return 0;
}

int array_init_elements_emit(expr_list_weak * depth_list, int * elements_count,
                             int stack_level, bytecode_list * code,
                             int * result)
{
    int elem_dist = -1;
    *elements_count = 0;

    expr_list_weak_node * node = depth_list->tail;
    elem_dist = node->distance;

    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL && node->distance == elem_dist)
        {
            expr_emit(value, stack_level + (*elements_count)++, code, result);
        }
        else
        {
            break;
        }
        node = node->next;
    }

    return 0;
}

int array_init_emit(expr * value, int stack_level, bytecode_list * code,
                    int * result)
{
    bytecode bc = { 0 };
    int elements_count = 0;
    array * array_value = value->array.array_value;

    expr_list_weak * depth_list = expr_list_weak_new();
    array_to_depth_list(value, depth_list);

    array_init_elements_emit(depth_list, &elements_count, stack_level, code,
                             result);
    expr_list_weak_delete(depth_list);

    expr_list_emit(array_value->dims, stack_level + elements_count, code,
                   result);

    bc.type = BYTECODE_MK_INIT_ARRAY;
    bc.mk_array.dims = array_value->dims->count;

    bytecode_add(code, &bc);

    return 0;
}

int expr_array_emit(expr * value, int stack_level, bytecode_list * code,
                    int * result)
{
    array * array_value = value->array.array_value;

    if (array_value->type == ARRAY_INIT)
    {
        array_init_emit(value, stack_level, code, result);
    }
    else if (array_value->type == ARRAY_SUB)
    {
        assert(0);
    }
    else if (array_value->type == ARRAY_DIMS)
    {
        array_dims_emit(array_value, stack_level, code, result);
    }

    return 0;
}

int expr_array_deref_emit(expr * value, int stack_level, bytecode_list * code,
                          int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->array_deref.array_expr, stack_level, code, result);
    expr_list_emit(value->array_deref.ref, stack_level + 1, code, result);

    bc.type = BYTECODE_ARRAY_DEREF;
    bc.array_deref.dims = value->array_deref.ref->count;

    bytecode_add(code, &bc);

    return 0;
}

int bind_emit(bind * bind_value, int stack_level, bytecode_list * code,
              int * result)
{
    switch (bind_value->type)
    {
        case BIND_UNKNOWN:
            fprintf(stderr, "unknown bind type\n");
            assert(0);
        break;
        case BIND_LET:
        case BIND_VAR:
            if (bind_value->expr_value != NULL)
            {
                expr_emit(bind_value->expr_value, stack_level, code, result);
            }
        break;
    }
    return 0;
}

int bind_list_emit(bind_list * list, int stack_level, bytecode_list * code,
                   int * result)
{
    int b = 0;
    
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * bind_value = node->value;
        if (bind_value != NULL)
        {
            bind_emit(bind_value, stack_level + b++, code, result);
        }
        node = node->next;
    }
    return 0;
}

int func_emit(func * func_value, int stack_level, bytecode_list * code,
              int * result)
{
    int param_count = 0;
    int freevar_count = 0;
    int func_count = 0;
    bytecode bc = { 0 };
    bytecode *jump, *labelA, *labelB;

    bc.type = BYTECODE_FUNC_DEF;
    bytecode_add(code, &bc);

    if (func_value->line_no > 0)
    {
        bc.type = BYTECODE_LINE;
        bc.line.no = func_value->line_no;
        bytecode_add(code, &bc);
    }

    if (func_value->freevars != NULL)
    {
        func_freevar_list_emit(func_value->freevars, stack_level, code, result);
        freevar_count = func_value->freevars->count;
    }

    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = freevar_count;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_JUMP;
    jump = bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(code, &bc);
    func_value->addr = labelA->addr;

    if (func_value->body && func_value->body->binds)
    {
        bind_list_emit(func_value->body->binds, 0, code, result);
        func_count += func_value->body->binds->count;
    }
    if (func_value->body && func_value->body->funcs)
    {
        func_list_emit(func_value->body->funcs, func_count, code, result);
        func_count += func_value->body->funcs->count;
    }

    if (func_value->body && func_value->body->ret)
    {
        expr_emit(func_value->body->ret, func_count, code, result);
    }

    if (bc.line.no > 0)
    {
        bc.type = BYTECODE_LINE;
        bc.line.no = func_value->body->ret->line_no;
        bytecode_add(code, &bc);
    }

    if (func_value->params != NULL)
    {
        param_count = func_value->params->count;
    }

    bc.type = BYTECODE_RET;
    bc.ret.count = param_count;
    bytecode_add(code, &bc);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(code, &bc);
    jump->jump.offset = labelB->addr - jump->addr;

    return 0;
}

int func_main_emit(never * nev, int stack_level, bytecode_list * code,
                   int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(nev->stab, "main", SYMTAB_FLAT);
    if (entry != NULL && entry->type == SYMTAB_FUNC)
    {
        bytecode bc = { 0 };
        bytecode *mark, *label;

        bc.type = BYTECODE_MARK;
        mark = bytecode_add(code, &bc);

        bc.type = BYTECODE_PUSH_PARAM;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_GLOBAL_VEC;
        bc.global_vec.count = 0;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_ID_FUNC_FUNC;
        bc.id_func.func_value = entry->func_value;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_CALL;
        bytecode_add(code, &bc);

        bc.type = BYTECODE_LABEL;
        label = bytecode_add(code, &bc);
        mark->mark.addr = label->addr;

        bc.type = BYTECODE_HALT;
        label = bytecode_add(code, &bc);
        mark->mark.addr = label->addr;
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(0, "no main function defined\n");
    }
    return 0;
}

int func_list_emit(func_list * list, int stack_level, bytecode_list * code,
                   int * result)
{
    bytecode bc = { 0 };
    unsigned int n = list->count;

    bc.type = BYTECODE_ALLOC;
    bc.alloc.n = n;
    bytecode_add(code, &bc);

    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            func_emit(func_value, stack_level + list->count, code, result);

            bc.type = BYTECODE_REWRITE;
            bc.rewrite.j = n--;
            bytecode_add(code, &bc);
        }
        node = node->next;
    }
    return 0;
}

int never_emit(never * nev, bytecode_list * code)
{
    int stack_level = 0;
    int gencode_res = 0;

    if (nev->funcs)
    {
        func_list_emit(nev->funcs, stack_level, code, &gencode_res);
        func_main_emit(nev, stack_level, code, &gencode_res);
    }

    return gencode_res;
}
