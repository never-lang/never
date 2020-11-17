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
#include "emit.h"
#include "freevar.h"
#include "symtab.h"
#include "iflet.h"
#include "object.h"
#include "match.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* GP old, FP old, IP old, line_no, PP old */
#define NUM_FRAME_PTRS 5

/**
 * emit code
 */
int expr_int_emit(expr * value, int stack_level, module * module_value,
                  int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_INT;
    bc.int_t.value = value->int_value;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_long_emit(expr * value, int stack_level, module * module_value,
                  int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_LONG;
    bc.long_t.value = value->long_value;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_float_emit(expr * value, int stack_level, module * module_value,
                    int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_FLOAT;
    bc.float_t.value = value->float_value;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_double_emit(expr * value, int stack_level, module * module_value,
                    int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_DOUBLE;
    bc.double_t.value = value->double_value;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_nil_emit(expr * value, int stack_level, module * module_value,
                  int * result)
{
    bytecode bc = { 0 };
    
    bc.type = BYTECODE_NIL_RECORD_REF;
    bytecode_add(module_value->code, &bc);
    
    return 0;
}                  

int expr_c_null_emit(expr * value, int stack_level, module * module_value,
                     int * result)
{
    bytecode bc = { 0 };
    
    bc.type = BYTECODE_C_NULL;
    bytecode_add(module_value->code, &bc);
    
    return 0;
}                  

int expr_char_emit(expr * value, int stack_level, module * module_value,
                   int * result)
{
    bytecode bc = { 0 };
    
    bc.type = BYTECODE_CHAR;
    bc.char_t.value = value->char_value;
    
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_string_emit(expr * value, int stack_level, module * module_value,
                     int * result)
{
    bytecode bc = { 0 };
    
    bc.type = BYTECODE_STRING;
    bc.string.index = strtab_add_string(module_value->strtab_value, value->string_value);

    bytecode_add(module_value->code, &bc);

    return 0;
}

int func_freevar_id_local_emit(freevar * value, int stack_level,
                               module * module_value, int * result)
{
    bytecode bc = { 0 };

    if (value->src.param_value->type == PARAM_DIM)
    {
        bc.type = BYTECODE_ID_DIM_LOCAL;
        bc.id_dim_local.stack_level = stack_level;
        bc.id_dim_local.index = value->src.param_value->array->index;
        bc.id_dim_local.dim_index = value->src.param_value->index;
    }
    else if (value->src.param_value->type == PARAM_RANGE_DIM)
    {
        bc.type = BYTECODE_VECREF_VEC_DEREF;
        bc.attr.stack_level = stack_level - value->src.param_value->range->index;
        bc.attr.index = value->src.param_value->index;
    }
    else if (value->src.param_value->type == PARAM_SLICE_DIM)
    {
        bc.type = BYTECODE_ID_DIM_SLICE;
        bc.id_dim_slice.stack_level = stack_level;
        bc.id_dim_slice.index = value->src.param_value->slice->index;
        bc.id_dim_slice.dim_index = value->src.param_value->index;
    }
    else
    {
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->src.param_value->index;
    }

    bytecode_add(module_value->code, &bc);

    return 0;
}

int func_freevar_id_bind_emit(freevar * value, int stack_level,
                              module * module_value, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level;
    bc.id_local.index = value->src.bind_value->index;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int func_freevar_id_matchbind_emit(freevar * value, int stack_level, 
                                   module * module_value, int * result)
{
    bytecode bc = { 0 };
    
    bc.type = BYTECODE_VECREF_VEC_DEREF;
    bc.attr.stack_level = stack_level - value->src.matchbind_value->stack_level;
    bc.attr.index = value->src.matchbind_value->index + 1;
    
    bytecode_add(module_value->code, &bc);

    return 0;
}

int func_freevar_id_qualifier_emit(freevar * value, int stack_level,
                                   module * module_value, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level - value->src.qualifier_value->stack_level;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);
    
    return 0;
}

int func_freevar_id_forin_emit(freevar * value, int stack_level, 
                               module * module_value, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level - value->src.forin_value->stack_level;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int func_freevar_emit(freevar * value, int stack_level, module * module_value,
                      int * result)
{
    bytecode bc = { 0 };

    switch (value->src.type)
    {
    case FREEVAR_UNKNOWN:
        print_error_msg(0, "unknown freevar %s during emit", value->id);
        assert(0);
        break;
    case FREEVAR_PARAM:
        func_freevar_id_local_emit(value, stack_level, module_value, result);
        break;
    case FREEVAR_BIND:
        func_freevar_id_bind_emit(value, stack_level, module_value, result);
        break;
    case FREEVAR_MATCHBIND:
        func_freevar_id_matchbind_emit(value, stack_level, module_value, result);
        break;
    case FREEVAR_QUALIFIER:
        func_freevar_id_qualifier_emit(value, stack_level, module_value, result);
        break;
    case FREEVAR_FORIN:
        func_freevar_id_forin_emit(value, stack_level, module_value, result);
        break;
    case FREEVAR_FREEVAR:
        bc.type = BYTECODE_ID_GLOBAL;
        bc.id_global.index = value->src.freevar_value->index;

        bytecode_add(module_value->code, &bc);
        break;
    case FREEVAR_FUNC:
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->src.func_value->index;

        /* TODO: remove printf("freevar_func %s %d %d\n", value->src.func_value->decl->id, stack_level, value->src.func_value->index); */

        bytecode_add(module_value->code, &bc);
        break;
    }

    return 0;
}

int func_freevar_list_emit(freevar_list * freevars, int stack_level,
                           module * module_value, int * result)
{
    int e = 0;
    freevar_list_node * node = freevars->tail;
    while (node != NULL)
    {
        freevar * value = node->value;
        if (value != NULL)
        {
            func_freevar_emit(value, stack_level + e++, module_value, result);
        }
        node = node->next;
    }

    return 0;
}

int expr_id_local_emit(expr * value, int stack_level, module * module_value,
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
    else if (value->id.id_param_value->type == PARAM_RANGE_DIM)
    {
        bc.type = BYTECODE_VECREF_VEC_DEREF;
        bc.attr.stack_level = stack_level - value->id.id_param_value->range->index;
        bc.attr.index = value->id.id_param_value->index;
    }
    else if (value->id.id_param_value->type == PARAM_SLICE_DIM)
    {
        bc.type = BYTECODE_ID_DIM_SLICE;
        bc.id_dim_slice.stack_level = stack_level;
        bc.id_dim_slice.index = value->id.id_param_value->slice->index;
        bc.id_dim_slice.dim_index = value->id.id_param_value->index;
    }
    else
    {
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = stack_level;
        bc.id_local.index = value->id.id_param_value->index;
    }

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_id_qualifier_emit(expr * value, int stack_level, module * module_value,
                           int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level - value->id.id_qualifier_value->stack_level;
    bc.id_local.index = 0;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_id_forin_emit(expr * value, int stack_level, module * module_value,
                       int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level - value->id.id_forin_value->stack_level;
    bc.id_local.index = 0;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_id_bind_top_emit(expr * value, int stack_level, module * module_value,
                          int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_TOP;
    bc.id_top.index = value->id.id_bind_value->index;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_id_bind_emit(expr * value, int stack_level, module * module_value,
                       int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level;
    bc.id_local.index = value->id.id_bind_value->index;
    bytecode_add(module_value->code, &bc);

    /* printf("id %s sl %d index %d\n", value->id.id, stack_level, value->id.id_bind_value->index); */

    return 0;
}

int expr_id_matchbind_emit(expr * value, int stack_level, module * module_value,
                           int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_VECREF_VEC_DEREF;
    bc.attr.stack_level = stack_level - value->id.id_matchbind_value->stack_level;
    bc.attr.index = value->id.id_matchbind_value->index + 1;

    bytecode_add(module_value->code, &bc);

    return 0;
}        

int expr_id_global_emit(expr * value, int stack_value, module * module_value,
                        int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_GLOBAL;
    bc.id_global.index = value->id.id_freevar_value->index;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_id_func_top_emit(func * func_value, int stack_level,
                          module * module_value, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_id_func_emit(func * func_value, int stack_level, module * module_value,
                      int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = stack_level;
    bc.id_local.index = func_value->index;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_id_func_nest_emit(func * func_value, int stack_level,
                           module * module_value, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_COPYGLOB;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_id_emit(expr * value, int stack_level, module * module_value,
                 int * result)
{
    switch (value->id.id_type_value)
    {
    case ID_TYPE_UNKNOWN:
        print_error_msg(value->line_no,
                        "not recognized id, at this stage it is very bad");
        assert(0);
        break;
    case ID_TYPE_LOCAL:
        expr_id_local_emit(value, stack_level, module_value, result);
        break;
    case ID_TYPE_GLOBAL:
        expr_id_global_emit(value, stack_level, module_value, result);
        break;
    case ID_TYPE_BIND_TOP:
        expr_id_bind_top_emit(value, stack_level, module_value, result);
        break;
    case ID_TYPE_BIND:
        expr_id_bind_emit(value, stack_level, module_value, result);
        break;
    case ID_TYPE_MATCHBIND:
        expr_id_matchbind_emit(value, stack_level, module_value, result);
        break;
    case ID_TYPE_QUALIFIER:
        expr_id_qualifier_emit(value, stack_level, module_value, result);
        break;    
    case ID_TYPE_FORIN:
        expr_id_forin_emit(value, stack_level, module_value, result);
        break;
    case ID_TYPE_FUNC_TOP:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_top_emit(value->id.id_func_value, stack_level, module_value,
                                  result);
        }
        break;
    case ID_TYPE_FUNC:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_emit(value->id.id_func_value, stack_level, module_value,
                              result);
        }
        break;
    case ID_TYPE_FUNC_NEST:
        if (value->id.id_func_value != NULL)
        {
            expr_id_func_nest_emit(value->id.id_func_value, stack_level, module_value,
                                   result);
        }
        break;
    case ID_TYPE_RECORD:
        expr_nil_emit(value, stack_level, module_value, result);
        break;
    case ID_TYPE_ENUMTYPE:
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot emit code for enumtype");
        break;
    case ID_TYPE_MODULE:
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot emit code for module");
        break;
    }
    return 0;
}

int expr_neg_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    
    expr_emit(value->left, stack_level, module_value, list_weak, result);

    if (value->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_NEG_INT;
    }
    else if (value->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_NEG_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_NEG_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_NEG_DOUBLE;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_INT)
    {
        bc.type = BYTECODE_OP_NEG_ARR_INT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_LONG)
    {
        bc.type = BYTECODE_OP_NEG_ARR_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_FLOAT)
    {
        bc.type = BYTECODE_OP_NEG_ARR_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_DOUBLE)
    {
        bc.type = BYTECODE_OP_NEG_ARR_DOUBLE;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot neg type %s",
                        comb_type_str(value->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_eq_emit(expr * value, int stack_level, module * module_value,
                 func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_NIL &&
        value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_EQ_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_BOOL &&
             value->right->comb.comb == COMB_TYPE_BOOL)
    {
        bc.type = BYTECODE_OP_EQ_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_EQ_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
             value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_EQ_LONG;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_EQ_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_DOUBLE &&
             value->right->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_EQ_DOUBLE;
    }
    else if (value->left->comb.comb == COMB_TYPE_ENUMTYPE &&
             value->right->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        switch (value->left->comb.comb_enumtype->type)
        {
        case ENUMTYPE_TYPE_ITEM:
            bc.type = BYTECODE_OP_EQ_INT;
        break;
        case ENUMTYPE_TYPE_RECORD:
            *result = EMIT_FAIL;
            print_error_msg(value->line_no,
                            "cannot compare enum record type\n");
            assert(0);
        break;
        }
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
             value->right->comb.comb == COMB_TYPE_CHAR)
    {
        bc.type = BYTECODE_OP_EQ_CHAR;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_EQ_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_EQ_STRING_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_EQ_NIL_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_C_PTR &&
             value->right->comb.comb == COMB_TYPE_C_PTR)
    {
        bc.type = BYTECODE_OP_EQ_C_PTR;
    }             
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_EQ_ARRAY_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
             value->right->comb.comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_EQ_NIL_ARRAY;
    }
    else if ((value->left->comb.comb == COMB_TYPE_RECORD ||
              value->left->comb.comb == COMB_TYPE_RECORD_ID) &&
              value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_EQ_RECORD_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
            (value->right->comb.comb == COMB_TYPE_RECORD ||
             value->right->comb.comb == COMB_TYPE_RECORD_ID))
    {
        bc.type = BYTECODE_OP_EQ_NIL_RECORD;
    }
    else if (value->left->comb.comb == COMB_TYPE_FUNC &&
             value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_EQ_FUNC_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
             value->right->comb.comb == COMB_TYPE_FUNC)
    {
        bc.type = BYTECODE_OP_EQ_NIL_FUNC;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot eq different types %s %s\n",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_neq_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_NIL &&
        value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_NEQ_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_BOOL &&
             value->right->comb.comb == COMB_TYPE_BOOL)
    {
        bc.type = BYTECODE_OP_EQ_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_NEQ_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
             value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_NEQ_LONG;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_NEQ_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_DOUBLE &&
             value->right->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_NEQ_DOUBLE;
    }
    else if (value->left->comb.comb == COMB_TYPE_ENUMTYPE &&
             value->right->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        switch (value->left->comb.comb_enumtype->type)
        {
        case ENUMTYPE_TYPE_ITEM:
            bc.type = BYTECODE_OP_NEQ_INT;
        break;
        case ENUMTYPE_TYPE_RECORD:
            *result = EMIT_FAIL;
            print_error_msg(value->line_no,
                            "cannot compare enum record type\n");
            assert(0);
        break;
        }
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
             value->right->comb.comb == COMB_TYPE_CHAR)
    {
        bc.type = BYTECODE_OP_NEQ_CHAR;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_NEQ_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_NEQ_STRING_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_NEQ_NIL_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_C_PTR &&
             value->right->comb.comb == COMB_TYPE_C_PTR)
    {
        bc.type = BYTECODE_OP_NEQ_C_PTR;
    }             
    else if (value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_NEQ_ARRAY_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
             value->right->comb.comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_NEQ_NIL_ARRAY;
    }
    else if ((value->left->comb.comb == COMB_TYPE_RECORD ||
              value->left->comb.comb == COMB_TYPE_RECORD_ID) &&
              value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_NEQ_RECORD_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
            (value->right->comb.comb == COMB_TYPE_RECORD ||
             value->right->comb.comb == COMB_TYPE_RECORD_ID))
    {
        bc.type = BYTECODE_OP_NEQ_NIL_RECORD;
    }
    else if (value->left->comb.comb == COMB_TYPE_FUNC &&
             value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_NEQ_FUNC_NIL;
    }
    else if (value->left->comb.comb == COMB_TYPE_NIL &&
             value->right->comb.comb == COMB_TYPE_FUNC)
    {
        bc.type = BYTECODE_OP_NEQ_NIL_FUNC;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no,
                        "cannot neq different types %s %s\n",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}                  

int expr_add_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    
    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_ADD_INT;
    }
    else if (value->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_ADD_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_ADD_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_ADD_DOUBLE;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_ADD_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_INT &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_ADD_INT_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_ADD_STRING_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_ADD_FLOAT_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_ADD_STRING_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_ADD_LONG_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_ADD_STRING_LONG;
    }
    else if (value->left->comb.comb == COMB_TYPE_DOUBLE &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_ADD_DOUBLE_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_ADD_STRING_DOUBLE;
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
             value->right->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_ADD_CHAR_STRING;
    }
    else if (value->left->comb.comb == COMB_TYPE_STRING &&
             value->right->comb.comb == COMB_TYPE_CHAR)
    {
        bc.type = BYTECODE_OP_ADD_STRING_CHAR;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_INT)
    {
        bc.type = BYTECODE_OP_ADD_ARR_INT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_LONG)
    {
        bc.type = BYTECODE_OP_ADD_ARR_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_FLOAT)
    {
        bc.type = BYTECODE_OP_ADD_ARR_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_DOUBLE)
    {
        bc.type = BYTECODE_OP_ADD_ARR_DOUBLE;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot add type %s",
                        comb_type_str(value->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_sub_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_SUB_INT;
    }
    else if (value->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_SUB_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_SUB_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_SUB_DOUBLE;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_INT)
    {
        bc.type = BYTECODE_OP_SUB_ARR_INT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_LONG)
    {
        bc.type = BYTECODE_OP_SUB_ARR_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_FLOAT)
    {
        bc.type = BYTECODE_OP_SUB_ARR_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_DOUBLE)
    {
        bc.type = BYTECODE_OP_SUB_ARR_DOUBLE;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot sub type %s",
                        comb_type_str(value->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_mul_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_MUL_INT;
    }
    else if (value->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_MUL_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_MUL_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_MUL_DOUBLE;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_MUL_ARR_INT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_MUL_ARR_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_MUL_ARR_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->left->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_MUL_ARR_DOUBLE;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_INT &&
             value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_MUL_ARR_ARR_INT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_LONG &&
             value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_MUL_ARR_ARR_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_FLOAT &&
             value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_MUL_ARR_ARR_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY &&
             value->comb.comb_ret->type == PARAM_DOUBLE &&
             value->left->comb.comb == COMB_TYPE_ARRAY &&
             value->right->comb.comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_MUL_ARR_ARR_DOUBLE;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot mul type %s",
                        comb_type_str(value->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_div_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_LINE;
    bc.line.no = value->line_no;
    bytecode_add(module_value->code, &bc);

    if (value->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_DIV_INT;
    }
    else if (value->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_DIV_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_DIV_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_DIV_DOUBLE;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot div type %s",
                        comb_type_str(value->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}                  

int expr_mod_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_LINE;
    bc.line.no = value->line_no;
    bytecode_add(module_value->code, &bc);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_MOD_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
             value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_MOD_LONG;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot mod type %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}                  

int expr_lt_emit(expr * value, int stack_level, module * module_value,
                 func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_LT_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
                value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_LT_LONG;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
                value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_LT_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_DOUBLE &&
                value->right->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_LT_DOUBLE;
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
                value->right->comb.comb == COMB_TYPE_CHAR)
    {
        bc.type = BYTECODE_OP_LT_CHAR;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot lt different types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_gt_emit(expr * value, int stack_level, module * module_value,
                 func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_GT_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
                value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_GT_LONG;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
                value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_GT_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_DOUBLE &&
                value->right->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_GT_DOUBLE;
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
                value->right->comb.comb == COMB_TYPE_CHAR)
    {
        bc.type = BYTECODE_OP_GT_CHAR;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot gt different types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_lte_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_LTE_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
             value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_LTE_LONG;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_LTE_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_DOUBLE &&
             value->right->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_LTE_DOUBLE;
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
             value->right->comb.comb == COMB_TYPE_CHAR)
    {
        bc.type = BYTECODE_OP_LTE_CHAR;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no,
                        "cannot lte different types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_gte_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_GTE_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
                value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_GTE_LONG;
    }
    else if (value->left->comb.comb == COMB_TYPE_FLOAT &&
             value->right->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_GTE_FLOAT;
    }
    else if (value->left->comb.comb == COMB_TYPE_DOUBLE &&
                value->right->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_GTE_DOUBLE;
    }
    else if (value->left->comb.comb == COMB_TYPE_CHAR &&
                value->right->comb.comb == COMB_TYPE_CHAR)
    {
        bc.type = BYTECODE_OP_GTE_CHAR;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no,
                        "cannot gte different types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_and_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *condzA, *condzB, *jumpE;
    bytecode *labelEF, *labelE;

    expr_emit(value->left, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMPZ;
    condzA = bytecode_add(module_value->code, &bc);

    expr_emit(value->right, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMPZ;
    condzB = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_INT;
    bc.int_t.value = 1;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpE = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelEF = bytecode_add(module_value->code, &bc);
    condzA->jump.offset = labelEF->addr - condzA->addr;
    condzB->jump.offset = labelEF->addr - condzB->addr;

    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    jumpE->jump.offset = labelE->addr - jumpE->addr;

    return 0;
}

int expr_or_emit(expr * value, int stack_level, module * module_value,
                 func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *condzA, *condzB, *jumpET, *jumpE;
    bytecode *labelB, *labelET, *labelEF, *labelE;

    expr_emit(value->left, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMPZ;
    condzA = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpET = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(module_value->code, &bc);
    condzA->jump.offset = labelB->addr - condzA->addr;

    expr_emit(value->right, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMPZ;
    condzB = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelET = bytecode_add(module_value->code, &bc);
    jumpET->jump.offset = labelET->addr - jumpET->addr;

    bc.type = BYTECODE_INT;
    bc.int_t.value = 1;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMP;
    jumpE = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelEF = bytecode_add(module_value->code, &bc);
    condzB->jump.offset = labelEF->addr - condzB->addr;

    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    jumpE->jump.offset = labelE->addr - jumpE->addr;

    return 0;
}

int expr_not_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    expr_emit(value->left, stack_level, module_value, list_weak, result);
        
    if (value->comb.comb == COMB_TYPE_BOOL)
    {
        bc.type = BYTECODE_OP_NOT_INT;
    }
    else if (value->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_NOT_INT;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot not type %s",
                        comb_type_str(value->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_pipel_call_emit(expr * value, int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *mark, *label;

    assert(value->right->type == EXPR_CALL);

    bc.type = BYTECODE_LINE;
    bc.line.no = value->line_no;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_MARK;
    mark = bytecode_add(module_value->code, &bc);

    int v = NUM_FRAME_PTRS;
    expr_emit(value->left, stack_level + v++, module_value, list_weak, result);

    if (value->right->call.params != NULL)
    {
        expr_list_emit(value->right->call.params, stack_level + v, module_value, list_weak, result);
        v += value->right->call.params->count;

        bc.type = BYTECODE_DUP;
        bc.dup.n = value->right->call.params->count + 1;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_SLIDE;
        bc.slide.m = value->right->call.params->count + 1;
        bc.slide.q = 1;
        bytecode_add(module_value->code, &bc);
    }

    expr_emit(value->right->call.func_expr, stack_level + v, module_value, list_weak, result);

    bc.type = BYTECODE_CALL;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    label = bytecode_add(module_value->code, &bc);
    mark->mark.addr = label->addr;

    return 0;    
}

int expr_pipel_last_call_emit(expr * value, int stack_level, module * module_value,
                              func_list_weak * list_weak, int * result)
{
    int v = 0;
    bytecode bc = { 0 };

    assert(value->right->type == EXPR_LAST_CALL);

    expr_emit(value->left, stack_level + v++, module_value, list_weak, result);

    if (value->right->call.params != NULL)
    {
        expr_list_emit(value->right->call.params, stack_level + v, module_value, list_weak, result);
        v += value->right->call.params->count;

        bc.type = BYTECODE_DUP;
        bc.dup.n = value->right->call.params->count + 1;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_SLIDE;
        bc.slide.m = value->right->call.params->count + 1;
        bc.slide.q = 1;
        bytecode_add(module_value->code, &bc);
    }
    expr_emit(value->right->call.func_expr, stack_level + v, module_value, list_weak, result);

    bc.type = BYTECODE_SLIDE;
    bc.slide.q = stack_level + v;
    bc.slide.m = v + 1;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_CALL;
    bytecode_add(module_value->code, &bc);

    return 0;
}
int expr_bin_not_emit(expr * value, int stack_level, module * module_value,
                      func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    expr_emit(value->left, stack_level, module_value, list_weak, result);
        
    if (value->left->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_BIN_NOT_INT;
        bytecode_add(module_value->code, &bc);
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_BIN_NOT_LONG;
        bytecode_add(module_value->code, &bc);
    }
    else if (value->left->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        bc.type = BYTECODE_OP_BIN_NOT_INT;
        bytecode_add(module_value->code, &bc);
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot bin not type %s",
                        comb_type_str(value->comb.comb));
        assert(0);
    }

    return 0;
}

int expr_bin_and_emit(expr * value, int stack_level, module * module_value,
                      func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_BIN_AND_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
                value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_BIN_AND_LONG;
    }
    else if ((value->left->comb.comb == COMB_TYPE_ENUMTYPE || value->left->comb.comb == COMB_TYPE_INT)
                                                           &&
             (value->right->comb.comb == COMB_TYPE_ENUMTYPE || value->right->comb.comb == COMB_TYPE_INT))
    {
        bc.type = BYTECODE_OP_BIN_AND_INT;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no,
                        "cannot bin and types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_bin_or_emit(expr * value, int stack_level, module * module_value,
                      func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_BIN_OR_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
                value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_BIN_OR_LONG;
    }
    else if ((value->left->comb.comb == COMB_TYPE_ENUMTYPE || value->left->comb.comb == COMB_TYPE_INT)
                                                           &&
             (value->right->comb.comb == COMB_TYPE_ENUMTYPE || value->right->comb.comb == COMB_TYPE_INT))
    {
        bc.type = BYTECODE_OP_BIN_OR_INT;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no,
                        "cannot bin or types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_bin_xor_emit(expr * value, int stack_level, module * module_value,
                      func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_BIN_XOR_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
                value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_BIN_XOR_LONG;
    }
    else if ((value->left->comb.comb == COMB_TYPE_ENUMTYPE || value->left->comb.comb == COMB_TYPE_INT)
                                                           &&
             (value->right->comb.comb == COMB_TYPE_ENUMTYPE || value->right->comb.comb == COMB_TYPE_INT))
    {
        bc.type = BYTECODE_OP_BIN_XOR_INT;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no,
                        "cannot bin xor types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_bin_shl_emit(expr * value, int stack_level, module * module_value,
                      func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_BIN_SHL_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
                value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_BIN_SHL_LONG;
    }
    else if ((value->left->comb.comb == COMB_TYPE_ENUMTYPE || value->left->comb.comb == COMB_TYPE_INT)
                                                           &&
             (value->right->comb.comb == COMB_TYPE_ENUMTYPE || value->right->comb.comb == COMB_TYPE_INT))
    {
        bc.type = BYTECODE_OP_BIN_SHL_INT;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no,
                        "cannot bin shl types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_bin_shr_emit(expr * value, int stack_level, module * module_value,
                      func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if (value->left->comb.comb == COMB_TYPE_INT &&
        value->right->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_BIN_SHR_INT;
    }
    else if (value->left->comb.comb == COMB_TYPE_LONG &&
                value->right->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_BIN_SHR_LONG;
    }
    else if ((value->left->comb.comb == COMB_TYPE_ENUMTYPE || value->left->comb.comb == COMB_TYPE_INT)
                                                           &&
             (value->right->comb.comb == COMB_TYPE_ENUMTYPE || value->right->comb.comb == COMB_TYPE_INT))
    {
        bc.type = BYTECODE_OP_BIN_SHR_INT;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no,
                        "cannot bin shr types %s %s",
                        comb_type_str(value->left->comb.comb),
                        comb_type_str(value->right->comb.comb));
        assert(0);
    }
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_pipel_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result)
{
    if (value->right->type == EXPR_CALL)
    {
        expr_pipel_call_emit(value, stack_level, module_value, list_weak, result);
    }
    else if (value->right->type == EXPR_LAST_CALL)
    {
        expr_pipel_last_call_emit(value, stack_level, module_value, list_weak, result);
    }
    else
    {
        /* typechecker should have caught this */
        assert(0);
    }

    return 0;
}

int expr_ass_emit(expr * value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    
    expr_emit(value->left, stack_level, module_value, list_weak, result);
    expr_emit(value->right, stack_level + 1, module_value, list_weak, result);

    if ((value->left->comb.comb == COMB_TYPE_RECORD ||
         value->left->comb.comb == COMB_TYPE_RECORD_ID) &&
         value->right->comb.comb == COMB_TYPE_NIL)
    {
        bc.type = BYTECODE_OP_ASS_RECORD_NIL;
    }
    else if (value->comb.comb == COMB_TYPE_BOOL)
    {
        bc.type = BYTECODE_OP_ASS_INT;
    }
    else if (value->comb.comb == COMB_TYPE_INT)
    {
        bc.type = BYTECODE_OP_ASS_INT;
    }
    else if (value->comb.comb == COMB_TYPE_LONG)
    {
        bc.type = BYTECODE_OP_ASS_LONG;
    }
    else if (value->comb.comb == COMB_TYPE_FLOAT)
    {
        bc.type = BYTECODE_OP_ASS_FLOAT;
    }
    else if (value->comb.comb == COMB_TYPE_DOUBLE)
    {
        bc.type = BYTECODE_OP_ASS_DOUBLE;
    }
    else if (value->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        switch (value->comb.comb_enumtype->type)
        {
        case ENUMTYPE_TYPE_ITEM:
            bc.type = BYTECODE_OP_ASS_INT;
        break;
        case ENUMTYPE_TYPE_RECORD:
            bc.type = BYTECODE_OP_ASS_RECORD;
        break;
        }
    }
    else if (value->comb.comb == COMB_TYPE_CHAR)
    {
        bc.type = BYTECODE_OP_ASS_CHAR;
    }
    else if (value->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_OP_ASS_STRING;
    }
    else if (value->comb.comb == COMB_TYPE_C_PTR)
    {
        bc.type = BYTECODE_OP_ASS_C_PTR;
    }
    else if (value->comb.comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_OP_ASS_ARRAY;
    }
    else if (value->comb.comb == COMB_TYPE_RANGE)
    {
        bc.type = BYTECODE_OP_ASS_RECORD;
    }
    else if (value->comb.comb == COMB_TYPE_SLICE)
    {
        bc.type = BYTECODE_OP_ASS_RECORD;
    }
    else if (value->comb.comb == COMB_TYPE_FUNC)
    {
        bc.type = BYTECODE_OP_ASS_FUNC;
    }
    else if (value->comb.comb == COMB_TYPE_RECORD)
    {
        bc.type = BYTECODE_OP_ASS_RECORD;
    }
    else
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot assign type %s",
                        comb_type_str(value->comb.comb));
        assert(0);
    }

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_cond_emit(expr * value, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condz;
    bytecode *labelA, *labelB;

    expr_emit(value->left, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);

    expr_emit(value->middle, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelA->addr - condz->addr;

    expr_emit(value->right, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(module_value->code, &bc);
    cond->jump.offset = labelB->addr - cond->addr;

    return 0;
}

int expr_while_emit(expr * value, int stack_level, module * module_value, 
                    func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condz;
    bytecode *labelA, *labelB;
    
    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(module_value->code, &bc);
    
    expr_emit(value->whileloop.cond, stack_level, module_value, list_weak, result);
    
    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);
    
    expr_emit(value->whileloop.do_value, stack_level, module_value, list_weak, result);
    
    /* pop previous value of stack */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 1;            
    bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);
    cond->jump.offset = labelA->addr - cond->addr;
    
    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelB->addr - condz->addr;

    /* while loop returns int 0 */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_do_while_emit(expr * value, int stack_level, module * module_value, 
                       func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condz;
    bytecode *labelA, *labelB;
    
    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(module_value->code, &bc);

    expr_emit(value->whileloop.do_value, stack_level, module_value, list_weak, result);
    
    /* pop previous value of stack */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 1;            
    bytecode_add(module_value->code, &bc);

    expr_emit(value->whileloop.cond, stack_level, module_value, list_weak, result);
    
    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);
    cond->jump.offset = labelA->addr - cond->addr;

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelB->addr - condz->addr;

    /* do while loop returns int 0 */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_for_emit(expr * value, int stack_level, module * module_value, 
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condz;
    bytecode *labelA, *labelB;

    expr_emit(value->forloop.init, stack_level, module_value, list_weak, result);
    
    /* pop previous value of stack */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 1;            
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(module_value->code, &bc);

    expr_emit(value->forloop.cond, stack_level, module_value, list_weak, result);
    
    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);
        
    expr_emit(value->forloop.do_value, stack_level, module_value, list_weak, result);

    /* pop previous value of stack */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 1;            
    bytecode_add(module_value->code, &bc);

    expr_emit(value->forloop.incr, stack_level, module_value, list_weak, result);    

    /* pop previous value of stack */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 1;            
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);
    cond->jump.offset = labelA->addr - cond->addr;

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelB->addr - condz->addr;

    /* for loop returns int 0 */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_forin_array_emit(expr * value, int stack_level, module * module_value, 
                          func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode * labelA, * labelE;
    bytecode * condz, * cond;

    expr_emit(value->forin_value->in_value, stack_level, module_value, list_weak, result);

    /* loop counter */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(module_value->code, &bc);

    /* loop all elements */
    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = 0;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ID_DIM_LOCAL;
    bc.id_dim_local.stack_level = 2;
    bc.id_dim_local.index = 0;
    bc.id_dim_local.dim_index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_LT_INT;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);

    /* push value */
    /* remember stack level to get value later */
    value->forin_value->stack_level = stack_level + 3;

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = 1;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = 1;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ARRAYREF_DEREF;
    bc.array_deref.dims = 1;
    bytecode_add(module_value->code, &bc);

    expr_emit(value->forin_value->do_value, stack_level + 3, module_value, list_weak, result);

    /* pop value */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 2;
    bytecode_add(module_value->code, &bc);

    /* inc loop counter */
    bc.type = BYTECODE_OP_INC_INT;
    bc.id_local.stack_level = 0;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);

    /* jump to beginning */
    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);
    cond->jump.offset = labelA->addr - cond->addr;

    /* loop end */
    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelE->addr - condz->addr;

    /* pop list and counter */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 2;
    bytecode_add(module_value->code, &bc);

    /* for loop returns int 0 */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_forin_range_emit(expr * value, int stack_level, module * module_value, 
                          func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode * condd;
    bytecode * conda, * condb;
    bytecode * condea, * condeb;
    bytecode * labelA, * labelB;
    bytecode * labelD;
    bytecode * labelE;

    expr_emit(value->forin_value->in_value, stack_level, module_value, list_weak, result);

    /* deref range so it is faster to access */
    bc.type = BYTECODE_VECREF_DEREF;
    bytecode_add(module_value->code, &bc);

    /* loop counter set to from value */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_ASS_INT;
    bytecode_add(module_value->code, &bc);

    /* get loop direction */
    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 2;
    bc.attr.index = 1;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_LT_INT;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMPZ;
    condd = bytecode_add(module_value->code, &bc);

    /* FROM < TO */
        bc.type = BYTECODE_LABEL;
        labelA = bytecode_add(module_value->code, &bc);

        /* for all */
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_VEC_DEREF;
        bc.attr.stack_level = 2;
        bc.attr.index = 1;
        bytecode_add(module_value->code, &bc);
    
        bc.type = BYTECODE_OP_LTE_INT;
        bytecode_add(module_value->code, &bc);

        /* exit loop if all passed */
        bc.type = BYTECODE_JUMPZ;
        condea = bytecode_add(module_value->code, &bc);

        /* push value */
        bc.type = BYTECODE_OP_DUP_INT;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        /* remember stack level to get value later */
        value->forin_value->stack_level = stack_level + 3;

        expr_emit(value->forin_value->do_value, stack_level + 3, module_value, list_weak, result);

        /* pop value */
        bc.type = BYTECODE_SLIDE;
        bc.slide.m = 0;
        bc.slide.q = 2;
        bytecode_add(module_value->code, &bc);

        /* inc loop counter */
        bc.type = BYTECODE_OP_INC_INT;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        /* jump to beginning */
        bc.type = BYTECODE_JUMP;
        conda = bytecode_add(module_value->code, &bc);
        conda->jump.offset = labelA->addr - conda->addr;

    /* FROM > TO */
    bc.type = BYTECODE_LABEL;
    labelD = bytecode_add(module_value->code, &bc);
    condd->jump.offset = labelD->addr - condd->addr;

        bc.type = BYTECODE_LABEL;
        labelB = bytecode_add(module_value->code, &bc);

        /* for all */
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_VEC_DEREF;
        bc.attr.stack_level = 2;
        bc.attr.index = 1;
        bytecode_add(module_value->code, &bc);
    
        bc.type = BYTECODE_OP_GTE_INT;
        bytecode_add(module_value->code, &bc);

        /* exit loop if all passed */
        bc.type = BYTECODE_JUMPZ;
        condeb = bytecode_add(module_value->code, &bc);

        /* push value */
        bc.type = BYTECODE_OP_DUP_INT;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        /* remember stack level to get value later */
        value->forin_value->stack_level = stack_level + 3;

        expr_emit(value->forin_value->do_value, stack_level + 3, module_value, list_weak, result);

        /* pop value */
        bc.type = BYTECODE_SLIDE;
        bc.slide.m = 0;
        bc.slide.q = 2;
        bytecode_add(module_value->code, &bc);

        /* dec loop counter */
        bc.type = BYTECODE_OP_DEC_INT;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        /* jump to beginning */
        bc.type = BYTECODE_JUMP;
        condb = bytecode_add(module_value->code, &bc);
        condb->jump.offset = labelB->addr - condb->addr;

    /* exit */
    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condea->jump.offset = labelE->addr - condea->addr;
    condeb->jump.offset = labelE->addr - condeb->addr;

    /* pop range and counter */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 2;
    bytecode_add(module_value->code, &bc);

    /* for loop returns int 0 */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_forin_slice_emit(expr * value, int stack_level, module * module_value, 
                          func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode * condd;
    bytecode * conda, * condb;
    bytecode * condea, * condeb;
    bytecode * labelA, * labelB;
    bytecode * labelD;
    bytecode * labelE;

    expr_emit(value->forin_value->in_value, stack_level, module_value, list_weak, result);

    /* push array */
    bc.type = BYTECODE_VECREF_VEC_DEREF;
    bc.attr.stack_level = 0;
    bc.attr.index = SLICE_ARRAY_INDEX;
    bytecode_add(module_value->code, &bc);

    /* push range */
    bc.type = BYTECODE_VECREF_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = SLICE_RANGE_INDEX;
    bytecode_add(module_value->code, &bc);

    /* loop counter set to from value */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_ASS_INT;
    bytecode_add(module_value->code, &bc);

    /* get loop direction */
    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 2;
    bc.attr.index = 1;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_LT_INT;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMPZ;
    condd = bytecode_add(module_value->code, &bc);

    /* FROM  < TO */
       bc.type = BYTECODE_LABEL;
       labelA = bytecode_add(module_value->code, &bc);

       /* for all */
       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 0;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_VEC_DEREF;
       bc.attr.stack_level = 2;
       bc.attr.index = 1;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_OP_LTE_INT;
       bytecode_add(module_value->code, &bc);

       /* exit loop if all passed */
       bc.type = BYTECODE_JUMPZ;
       condea = bytecode_add(module_value->code, &bc);

       /* push value */
       /* remember stack level to get value later */
       value->forin_value->stack_level = stack_level + 5;

       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 2;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 1;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_ARRAY_DEREF;
       bc.array_deref.dims = 1;
       bytecode_add(module_value->code, &bc);

       expr_emit(value->forin_value->do_value, stack_level + 5, module_value, list_weak, result);

       /* pop value */
       bc.type = BYTECODE_SLIDE;
       bc.slide.m = 0;
       bc.slide.q = 2;
       bytecode_add(module_value->code, &bc);

       /* inc loop counter */
       bc.type = BYTECODE_OP_INC_INT;
       bc.id_local.stack_level = 0;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       /* jump to beginning */
       bc.type = BYTECODE_JUMP;
       conda = bytecode_add(module_value->code, &bc);
       conda->jump.offset = labelA->addr - conda->addr;

    /* FROM > TO */
    bc.type = BYTECODE_LABEL;
    labelD = bytecode_add(module_value->code, &bc);
    condd->jump.offset = labelD->addr - condd->addr;

       bc.type = BYTECODE_LABEL;
       labelB = bytecode_add(module_value->code, &bc);

       /* for all */
       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 0;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_VEC_DEREF;
       bc.attr.stack_level = 2;
       bc.attr.index = 1;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_OP_GTE_INT;
       bytecode_add(module_value->code, &bc);

       /* exit loop if all passed */
       bc.type = BYTECODE_JUMPZ;
       condeb = bytecode_add(module_value->code, &bc);

       /* push value */
       /* remember stack level to get value later */
       value->forin_value->stack_level = stack_level + 5;

       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 2;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 1;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_ARRAY_DEREF;
       bc.array_deref.dims = 1;
       bytecode_add(module_value->code, &bc);

       expr_emit(value->forin_value->do_value, stack_level + 5, module_value, list_weak, result);

       /* pop value */
       bc.type = BYTECODE_SLIDE;
       bc.slide.m = 0;
       bc.slide.q = 2;
       bytecode_add(module_value->code, &bc);

       /* inc loop counter */
       bc.type = BYTECODE_OP_DEC_INT;
       bc.id_local.stack_level = 0;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       /* jump to beginning */
       bc.type = BYTECODE_JUMP;
       condb = bytecode_add(module_value->code, &bc);
       condb->jump.offset = labelB->addr - condb->addr;

    /* exit */
    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condea->jump.offset = labelE->addr - condea->addr;
    condeb->jump.offset = labelE->addr - condeb->addr;

    /* pop slice, array, range, counter */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 4;
    bytecode_add(module_value->code, &bc);

    /* for loop returns int 0 */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_forin_emit(expr * value, int stack_level, module * module_value, 
                    func_list_weak * list_weak, int * result)
{
    if (value->forin_value->in_value->comb.comb == COMB_TYPE_ARRAY &&
        value->forin_value->in_value->comb.comb_dims == 1)
    {
        expr_forin_array_emit(value, stack_level, module_value, list_weak, result);
    }
    else if (value->forin_value->in_value->comb.comb == COMB_TYPE_RANGE &&
             value->forin_value->in_value->comb.comb_dims == 1)
    {
        expr_forin_range_emit(value, stack_level, module_value, list_weak, result);
    }
    else if (value->forin_value->in_value->comb.comb == COMB_TYPE_SLICE &&
            value->forin_value->in_value->comb.comb_dims == 1)
    {
        expr_forin_slice_emit(value, stack_level, module_value, list_weak, result);
    }
    else
    {
        assert(0);
    }

    return 0;
}

int expr_iflet_guard_item_emit(match_guard_item * guard_item,
                               int stack_level, module * module_value,
                               func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    
    switch (guard_item->enumtype_value->type)
    {
        case ENUMTYPE_TYPE_ITEM:
            bc.type = BYTECODE_DUP;
            bc.dup.n = 1;
            bytecode_add(module_value->code, &bc);
        break;
        case ENUMTYPE_TYPE_RECORD:
            bc.type = BYTECODE_VECREF_VEC_DEREF;
            bc.attr.stack_level = 0;
            bc.attr.index = 0;
            bytecode_add(module_value->code, &bc);
        break;
    }

    bc.type = BYTECODE_INT;
    bc.int_t.value = guard_item->enumerator_value->index;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_iflet_guard_record_emit(match_guard_record * guard_record,
                                 int stack_level, module * module_value,
                                 func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    if (guard_record->matchbinds != NULL)
    {
        matchbind_list_set_stack_level(guard_record->matchbinds, stack_level);
    }
    
    switch (guard_record->enumtype_value->type)
    {
        case ENUMTYPE_TYPE_ITEM:
            bc.type = BYTECODE_DUP;
            bc.dup.n = 1;
            bytecode_add(module_value->code, &bc);
        break;
        case ENUMTYPE_TYPE_RECORD:
            bc.type = BYTECODE_VECREF_VEC_DEREF;
            bc.attr.stack_level = 0;
            bc.attr.index = 0;
            bytecode_add(module_value->code, &bc);
        break;
    }

    bc.type = BYTECODE_INT;
    bc.int_t.value = guard_record->enumerator_value->index;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_iflet_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condE;
    bytecode *label, *labelE;

    bc.type = BYTECODE_LINE;
    bc.line.no = value->line_no;
    bytecode_add(module_value->code, &bc);

    expr_emit(value->iflet_value->expr_value, stack_level, module_value, list_weak, result);

    switch (value->iflet_value->type)
    {
        case IFLET_TYPE_ITEM:
            expr_iflet_guard_item_emit(value->iflet_value->guard_item,
                                       stack_level + 1, module_value, list_weak, result);
        break;
        case IFLET_TYPE_RECORD:
            expr_iflet_guard_record_emit(value->iflet_value->guard_record,
                                         stack_level + 1, module_value, list_weak, result);
        break;
    }

    bc.type = BYTECODE_OP_EQ_INT;
    bytecode_add(module_value->code, &bc);
            
    bc.type = BYTECODE_JUMPZ;
    condE = bytecode_add(module_value->code, &bc);
    
    expr_emit(value->iflet_value->then_value, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;          
    labelE = bytecode_add(module_value->code, &bc);
    condE->jump.offset = labelE->addr - condE->addr;

    expr_emit(value->iflet_value->else_value, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_LABEL;          
    label = bytecode_add(module_value->code, &bc);
    cond->jump.offset = label->addr - cond->addr;

    /* pop previous value of stack */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 1;
    bc.slide.q = 1;            
    bytecode_add(module_value->code, &bc);

    return 0;
}        

int expr_match_guard_item_emit(match_guard_item_expr * item_value, bytecode *label,
                               int stack_level, module * module_value,
                               func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condz;
    bytecode *labelN;

    switch (item_value->guard->enumtype_value->type)
    {
        case ENUMTYPE_TYPE_ITEM:
            bc.type = BYTECODE_DUP;
            bc.dup.n = 1;
            bytecode_add(module_value->code, &bc);
        break;
        case ENUMTYPE_TYPE_RECORD:
            bc.type = BYTECODE_VECREF_VEC_DEREF;
            bc.attr.stack_level = 0;
            bc.attr.index = 0;
            bytecode_add(module_value->code, &bc);
        break;
    }

    bc.type = BYTECODE_INT;
    bc.int_t.value = item_value->guard->enumerator_value->index;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_EQ_INT;
    bytecode_add(module_value->code, &bc);
            
    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);

    expr_emit(item_value->expr_value, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);
    cond->jump.offset = label->addr - cond->addr;                      
            
    bc.type = BYTECODE_LABEL;          
    labelN = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelN->addr - condz->addr;

    return 0;
}

int expr_match_guard_record_emit(match_guard_record_expr * record_value, bytecode * label,
                                 int stack_level, module * module_value,
                                 func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *cond, *condz;
    bytecode *labelN;

    if (record_value->guard->matchbinds != NULL)
    {
        matchbind_list_set_stack_level(record_value->guard->matchbinds, stack_level);
    }

    switch (record_value->guard->enumtype_value->type)
    {
        case ENUMTYPE_TYPE_ITEM:
            bc.type = BYTECODE_DUP;
            bc.dup.n = 1;
            bytecode_add(module_value->code, &bc);
        break;
        case ENUMTYPE_TYPE_RECORD:
            bc.type = BYTECODE_VECREF_VEC_DEREF;
            bc.attr.stack_level = 0;
            bc.attr.index = 0;
            bytecode_add(module_value->code, &bc);
        break;
    }

    bc.type = BYTECODE_INT;
    bc.int_t.value = record_value->guard->enumerator_value->index;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_EQ_INT;
    bytecode_add(module_value->code, &bc);
            
    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);

    expr_emit(record_value->expr_value, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);
    cond->jump.offset = label->addr - cond->addr;                      
            
    bc.type = BYTECODE_LABEL;          
    labelN = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelN->addr - condz->addr;

    return 0;
}

int expr_match_guard_else_emit(match_guard_else_expr * else_value, bytecode * label,
                               int stack_level, module * module_value,
                               func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *cond;

    expr_emit(else_value->expr_value, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);
    cond->jump.offset = label->addr - cond->addr;                      

    return 0;
}                                                    

/**
 * match.expr_value
 * match.match_guards
 * jump labelA
 * :labelB
 *    jump labelE
 * :labelA
 *    #match guard item
 *    dup
 *    push item_value
 *    eq
 *    jumpz labelN
 *        expr
 *        jump labelB
 *    labelN:
 *    #match guard else
 *        expr
 *        jump labelB
 * :labelE
 */
int expr_match_guard_emit(match_guard * match_value, bytecode * label,
                          int stack_level,
                          module * module_value, func_list_weak * list_weak,
                          int * result)
{
    switch (match_value->type)
    {
        case MATCH_GUARD_ITEM:
            expr_match_guard_item_emit(&match_value->guard_item, label,
                                       stack_level, module_value, list_weak,
                                       result);
        break;
        case MATCH_GUARD_RECORD:
            expr_match_guard_record_emit(&match_value->guard_record, label,
                                         stack_level, module_value, list_weak,
                                         result);
        break;
        case MATCH_GUARD_ELSE:
            expr_match_guard_else_emit(&match_value->guard_else, label,
                                       stack_level, module_value, list_weak,
                                       result);
        break;
    }

    return 0;
}                          

int expr_match_guard_list_emit(match_guard_list * list, int stack_level,
                               module * module_value, func_list_weak * list_weak,
                               int * result)
{
    bytecode bc = { 0 };
    bytecode *labelA, *labelB, *labelE;
    bytecode *condA, *condE;

    bc.type = BYTECODE_JUMP;
    condA = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelB = bytecode_add(module_value->code, &bc);

    /* pop previous value of stack */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 1;
    bc.slide.q = 1;            
    bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_JUMP;
    condE = bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(module_value->code, &bc);
    condA->jump.offset = labelA->addr - condA->addr;

    match_guard_list_node * node = node = list->tail;
    while (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL)
        {
            expr_match_guard_emit(match_value, labelB,
                                  stack_level, module_value, list_weak, result);
        }
        node = node->next;
    }

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condE->jump.offset = labelE->addr - condE->addr;

    return 0;
}                                   

int expr_match_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_LINE;
    bc.line.no = value->line_no;
    bytecode_add(module_value->code, &bc);

    expr_emit(value->match.expr_value, stack_level, module_value, list_weak, result);

    if (value->match.match_guards != NULL)
    {
        expr_match_guard_list_emit(value->match.match_guards, stack_level + 1,
                                   module_value, list_weak, result);
    }

    return 0;
}                    

int expr_range_dim_emit(expr * value, int stack_level, module * module_value,
                        func_list_weak * list_weak, int * result)
{
    assert(value->type == EXPR_RANGE_DIM);

    expr_emit(value->range_dim.to, stack_level, module_value, list_weak, result);
    expr_emit(value->range_dim.from, stack_level + 1, module_value, list_weak, result);

    return 0;
}                        

int expr_range_dim_list_emit(expr_list * list, int stack_level, module * module_value,
                             func_list_weak * list_weak, int * result)
{
    int r = 0;

    expr_list_node * node = list->head;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_range_dim_emit(value, stack_level + r, module_value, list_weak, result);
            r += 2;
        }
        node = node->prev;
    }

    return 0;
}

int expr_range_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_LINE;
    bc.line.no = value->line_no;
    bytecode_add(module_value->code, &bc);

    expr_range_dim_list_emit(value->range.range_dims, stack_level, module_value, list_weak, result);

    bc.type = BYTECODE_MK_RANGE;
    bc.mk_range.dims = value->range.range_dims->count;
    bytecode_add(module_value->code, &bc);

    return 0;
}                    

int expr_slice_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    bc.type = BYTECODE_LINE;
    bc.line.no = value->line_no;
    bytecode_add(module_value->code, &bc);

    /* array, range, slice or string */
    expr_emit(value->slice.array_expr, stack_level, module_value, list_weak, result);

    /* range */
    expr_range_dim_list_emit(value->slice.range_dims, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_MK_RANGE;
    bc.mk_range.dims = value->slice.range_dims->count;
    bytecode_add(module_value->code, &bc);

    if (value->slice.array_expr->comb.comb == COMB_TYPE_ARRAY)
    {
        bc.type = BYTECODE_SLICE_ARRAY;
        bc.mk_slice.dims = value->slice.range_dims->count;
        bytecode_add(module_value->code, &bc);
    }
    else if (value->slice.array_expr->comb.comb == COMB_TYPE_RANGE)
    {
        bc.type = BYTECODE_SLICE_RANGE;
        bc.mk_slice.dims = value->slice.range_dims->count;
        bytecode_add(module_value->code, &bc);
    }
    else if (value->slice.array_expr->comb.comb == COMB_TYPE_SLICE)
    {
        bc.type = BYTECODE_SLICE_SLICE;
        bc.mk_slice.dims = value->slice.range_dims->count;
        bytecode_add(module_value->code, &bc);
    }
    else if (value->slice.array_expr->comb.comb == COMB_TYPE_STRING)
    {
        bc.type = BYTECODE_SLICE_STRING;
        bc.mk_slice.dims = value->slice.range_dims->count;
        bytecode_add(module_value->code, &bc);
    }
    else
    {
        /* this should have been detected by typechecker */
        assert(0);
    }
    
    return 0;
}                    

int expr_call_emit(expr * value, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *mark, *label;

    bc.type = BYTECODE_LINE;
    bc.line.no = value->line_no;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_MARK;
    mark = bytecode_add(module_value->code, &bc);

    int v = NUM_FRAME_PTRS;
    if (value->call.params != NULL)
    {
        expr_list_emit(value->call.params, stack_level + v, module_value, list_weak, result);
        v += value->call.params->count;
    }
    expr_emit(value->call.func_expr, stack_level + v, module_value, list_weak, result);

    bc.type = BYTECODE_CALL;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    label = bytecode_add(module_value->code, &bc);
    mark->mark.addr = label->addr;

    return 0;
}

int expr_last_call_emit(expr * value, int stack_level, module * module_value,
                        func_list_weak * list_weak, int * result)
{
    int v = 0;
    bytecode bc = { 0 };

    if (value->call.params != NULL)
    {
        expr_list_emit(value->call.params, stack_level + v, module_value, list_weak, result);
        v += value->call.params->count;
    }
    expr_emit(value->call.func_expr, stack_level + v, module_value, list_weak, result);

    bc.type = BYTECODE_SLIDE;
    bc.slide.q = stack_level + v;
    bc.slide.m = v + 1;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_CALL;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_record_emit(expr * value, int stack_level, module * module_value,
                     func_list_weak * list_weak, int * result)
{
    int count = -1;
    bytecode bc = { 0 };
    
    if (value->call.params != NULL)
    {
        count = value->call.params->count;
        expr_list_emit(value->call.params, stack_level, module_value,
                       list_weak, result);
    }

    assert(count >= 0);

    bc.type = BYTECODE_RECORD;
    bc.record.count = count;
    bytecode_add(module_value->code, &bc);
    
    return 0;
}

int expr_enumtype_record_emit(expr * value, int stack_level, module * module_value,
                              func_list_weak * list_weak, int * result)
{
    int count = 0;
    unsigned int index = 0;
    bytecode bc = { 0 };

    assert(value->call.func_expr->type == EXPR_ENUMTYPE);
    assert(value->call.func_expr->enumtype.id_enumtype_value->type == ENUMTYPE_TYPE_RECORD);
    
    if (value->call.params != NULL)
    {
        count = value->call.params->count;
        expr_list_emit(value->call.params, stack_level, module_value,
                       list_weak, result);
    }

    index = value->call.func_expr->enumtype.id_enumerator_value->index;
    bc.type = BYTECODE_INT;
    bc.int_t.value = index;
    bytecode_add(module_value->code, &bc);
        
    bc.type = BYTECODE_RECORD;
    bc.record.count = count + 1;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_emit(expr * value, int stack_level, module * module_value,
              func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    switch (value->type)
    {
    case EXPR_BOOL:
        expr_int_emit(value, stack_level, module_value, result);
        break;
    case EXPR_INT:
        expr_int_emit(value, stack_level, module_value, result);
        break;
    case EXPR_LONG:
        expr_long_emit(value, stack_level, module_value, result);
        break;
    case EXPR_FLOAT:
        expr_float_emit(value, stack_level, module_value, result);
        break;
    case EXPR_DOUBLE:
        expr_double_emit(value, stack_level, module_value, result);
        break;
    case EXPR_CHAR:
        expr_char_emit(value, stack_level, module_value, result);
        break;
    case EXPR_STRING:
        expr_string_emit(value, stack_level, module_value, result);
        break;
    case EXPR_ENUMTYPE:
        expr_enumtype_emit(value, stack_level, module_value, result);
        break;
    case EXPR_NIL:
        expr_nil_emit(value, stack_level, module_value, result);
        break;
    case EXPR_C_NULL:
        expr_c_null_emit(value, stack_level, module_value, result);
        break;
    case EXPR_ID:
        expr_id_emit(value, stack_level, module_value, result);
        break;
    case EXPR_NEG:
        expr_neg_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_ADD:
        expr_add_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_SUB:
        expr_sub_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_MUL:
        expr_mul_emit(value, stack_level, module_value, list_weak, result);    
        break;
    case EXPR_DIV:
        expr_div_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_MOD:
        expr_mod_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_LT:
        expr_lt_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_GT:
        expr_gt_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_LTE:
        expr_lte_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_GTE:
        expr_gte_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_EQ:
        expr_eq_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_NEQ:
        expr_neq_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_AND:
        expr_and_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_OR:
        expr_or_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_NOT:
        expr_not_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_PIPEL:
        expr_pipel_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_BIN_NOT:
        expr_bin_not_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_BIN_AND:
        expr_bin_and_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_BIN_OR:
        expr_bin_or_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_BIN_XOR:
        expr_bin_xor_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_BIN_SHL:
        expr_bin_shl_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_BIN_SHR:
        expr_bin_shr_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_SUP:
        expr_emit(value->left, stack_level, module_value, list_weak, result);
        break;
    case EXPR_COND:
        expr_cond_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            expr_array_emit(value, stack_level, module_value, list_weak, result);
        }
        break;
    case EXPR_ARRAY_DEREF:
        if (value->array_deref.array_expr->comb.comb == COMB_TYPE_ARRAY)
        {
            expr_array_deref_emit(value, stack_level, module_value, list_weak, result);
        }
        else if (value->array_deref.array_expr->comb.comb == COMB_TYPE_RANGE)
        {
            expr_range_deref_emit(value, stack_level, module_value, list_weak, result);
        }
        else if (value->array_deref.array_expr->comb.comb == COMB_TYPE_SLICE)
        {
            expr_slice_deref_emit(value, stack_level, module_value, list_weak, result);
        }
        else if (value->array_deref.array_expr->comb.comb == COMB_TYPE_STRING)
        {
            expr_string_deref_emit(value, stack_level, module_value, list_weak, result);
        }
        else
        {
            *result = EMIT_FAIL;
            print_error_msg(value->line_no,
                            "cannot deref type %s",
                            comb_type_str(value->array_deref.array_expr->comb.comb));
            assert(0);
        }
        break;
    case EXPR_RANGE_DIM:
        expr_range_dim_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_RANGE:
        expr_range_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_SLICE:
        expr_slice_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_CALL:
        if (value->call.func_expr->comb.comb == COMB_TYPE_RECORD_ID)
        {
            expr_record_emit(value, stack_level, module_value, list_weak, result);
        }
        else if (value->call.func_expr->comb.comb == COMB_TYPE_ENUMTYPE)
        {
            expr_enumtype_record_emit(value, stack_level, module_value, list_weak, result);
        }
        else if (value->call.func_expr->comb.comb == COMB_TYPE_FUNC)
        {
            expr_call_emit(value, stack_level, module_value, list_weak, result);
        }
        else
        {
            assert(0);
        }
        break;
    case EXPR_LAST_CALL:
        expr_last_call_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_emit(value->func_value, stack_level, module_value, list_weak, result);
        }
        break;
    case EXPR_SEQ:
        if (value->seq_value != NULL)
        {
            seq_emit(value->seq_value, stack_level, module_value, list_weak, result);
        }
        break;
    case EXPR_ASS:
        expr_ass_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_WHILE:
        expr_while_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_DO_WHILE:
        expr_do_while_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_FOR:
        expr_for_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_FORIN:
        expr_forin_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_IFLET:
        expr_iflet_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_MATCH:
        expr_match_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_BUILD_IN:
        if (value->func_build_in.param != NULL)
        {
            expr_list_emit(value->func_build_in.param, stack_level, module_value,
                           list_weak, result);
        }

        bc.type = BYTECODE_BUILD_IN;
        bc.build_in.id = value->func_build_in.id;
        bytecode_add(module_value->code, &bc);
        break;
    case EXPR_CONV:
        expr_emit(value->conv.expr_value, stack_level, module_value, list_weak, result);
        expr_conv_emit(value, stack_level, module_value, list_weak, result);
        break;
    case EXPR_LISTCOMP:
        if (value->listcomp_value != NULL)
        {
            listcomp_emit(value->listcomp_value, stack_level, module_value,
                          list_weak, result);
        }
        break;
    case EXPR_ATTR:
        if (value->attr.record_value->comb.comb == COMB_TYPE_RECORD ||
            value->attr.record_value->comb.comb == COMB_TYPE_RECORD_ID)
        {
            expr_record_attr_emit(value, stack_level, module_value, list_weak, result);
        }
        else if (value->attr.record_value->comb.comb == COMB_TYPE_MODULE)
        {
            expr_id_emit(value->attr.id, stack_level, module_value, result);
        }
        else
        {
            assert(0);
        }
        break;
#if 0
    case EXPR_BIND:
        if (value->bind.bind_value != NULL &&
            value->bind.bind_value->expr_value != NULL)
        {
            bind * bind_value = value->bind.bind_value;
            value->bind.bind_value->index = stack_level + 1;

            printf("bind %s %d\n", bind_value->id, bind_value->index);

            expr_emit(value->bind.bind_value->expr_value, stack_level, module_value, list_weak, result);
        }
        break;
#endif
    }
    return 0;
}

int expr_list_emit(expr_list * list, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    int e = 0;
    expr_list_node * node = list->head;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_emit(value, stack_level + e++, module_value, list_weak, result);
        }
        node = node->prev;
    }

    return 0;
}

int seq_list_emit(seq_list * list, int * stack_level, module * module_value,
                  func_list_weak * list_weak,  int * result)
{
    int pop_prev = 0;

    seq_list_node * node = list->tail;
    while (node != NULL)
    {
        seq_item * value = node->value;
        if (value == NULL)
        {
            node = node->next;
            continue;
        }
        if (pop_prev)
        {
            bytecode bc = { 0 };

            /* pop previous value of stack */
            bc.type = BYTECODE_SLIDE;
            bc.slide.m = 0;
            bc.slide.q = 1;            
            bytecode_add(module_value->code, &bc);
        }
        switch (value->type)
        {
            case SEQ_TYPE_EXPR:
            {
                expr_emit(value->expr_value, *stack_level, module_value, list_weak, result);

                pop_prev = 1;
                node = node->next;
            }
            break;
            case SEQ_TYPE_BIND:
            {
                bind_emit(value->bind_value, *stack_level, module_value, list_weak, result);
                (*stack_level)++;

                pop_prev = 0;
                node = node->next;
            }
            break;
            case SEQ_TYPE_FUNC:
            {
                bytecode bc = { 0 };
                unsigned int count = 0;
                seq_list_node * func_first_node = node;

                while (node != NULL &&
                       node->value != NULL &&
                       node->value->type == SEQ_TYPE_FUNC)
                {
                    func * func_value = node->value->func_value;
                    if (func_value)
                    {
                        func_value->index = (*stack_level) + count + 1;
                        count++;
                    }
                    node = node->next;
                }

                bc.type = BYTECODE_ALLOC;
                bc.alloc.n = count;
                bytecode_add(module_value->code, &bc);

                node = func_first_node;
                (*stack_level) = (*stack_level) + count;

                while (node != NULL &&
                       node->value != NULL &&
                       node->value->type == SEQ_TYPE_FUNC)
                {
                    func * func_value = node->value->func_value;
                    if (func_value)
                    {
                        func_emit(func_value, (*stack_level), module_value, list_weak, result);

                        bc.type = BYTECODE_REWRITE;
                        bc.rewrite.j = count--;
                        bytecode_add(module_value->code, &bc);
                    }
                    node = node->next;
                }

                pop_prev = 0;
            }
            break;
            default:
                assert(0);
        }
    }

    return 0;
}

int seq_emit(seq * value, int stack_level, module * module_value,
                  func_list_weak * list_weak,  int * result)
{
    if (value->list)
    {
        int stack_before = stack_level;
        seq_list_emit(value->list, &stack_level, module_value, list_weak, result);

        if (stack_level - stack_before > 0)
        {
            bytecode bc = { 0 };

            bc.type = BYTECODE_SLIDE;
            bc.slide.m = 1;
            bc.slide.q = stack_level - stack_before;

            bytecode_add(module_value->code, &bc);
        }
    }

    return 0;
}                

int expr_yeld_emit(listcomp * listcomp_value, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(listcomp_value->expr_value, stack_level, module_value, list_weak, result);

    /* append value to the list */
    bc.type = BYTECODE_ARRAY_APPEND;
    bc.id_local.stack_level = stack_level - listcomp_value->stack_level;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int generator_array_emit(listcomp * listcomp_value, qualifier_list_node * node,
                         int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode * labelA, * labelE;
    bytecode * cond, * condz;
    qualifier * value = node->value;
    
    expr_emit(value->expr_value, stack_level, module_value,
              list_weak, result);

    /* loop counter */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelA = bytecode_add(module_value->code, &bc);

    /* loop all elements */
    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = 0;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);    

    bc.type = BYTECODE_ID_DIM_LOCAL;
    bc.id_dim_local.stack_level = 2; 
    bc.id_dim_local.index = 0;
    bc.id_dim_local.dim_index = 0; 
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_LT_INT;
    bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);

    /* push value */
    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = 1;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_ID_LOCAL;
    bc.id_local.stack_level = 1;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ARRAYREF_DEREF;
    bc.array_deref.dims = 1;
    bytecode_add(module_value->code, &bc);

    /* remember stack level */
    value->stack_level = stack_level + 3;

    qualifier_stack_emit(listcomp_value, node->next, stack_level + 3, module_value,
                         list_weak, result);

    /* pop value */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 1;
    bytecode_add(module_value->code, &bc);

    /* inc loop counter */
    bc.type = BYTECODE_OP_INC_INT;
    bc.id_local.stack_level = 0;
    bc.id_local.index = 0;
    bytecode_add(module_value->code, &bc);

    /* jump to beginning */
    bc.type = BYTECODE_JUMP;
    cond = bytecode_add(module_value->code, &bc);
    cond->jump.offset = labelA->addr - cond->addr;

    /* loop end */
    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelE->addr - condz->addr;

    /* pop list and counter */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 2;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int generator_range_emit(listcomp * listcomp_value, qualifier_list_node * node,
                         int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode * condd;
    bytecode * conda, * condb;
    bytecode * condea, * condeb;
    bytecode * labelA, * labelB;
    bytecode * labelD;
    bytecode * labelE;
    qualifier * value = node->value;

    expr_emit(value->expr_value, stack_level, module_value, list_weak, result);

    /* deref range so it is faster to access */
    bc.type = BYTECODE_VECREF_DEREF;
    bytecode_add(module_value->code, &bc);

    /* loop counter set to from value */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_ASS_INT;
    bytecode_add(module_value->code, &bc);

    /* get loop direction */
    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 2;
    bc.attr.index = 1;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_LT_INT;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMPZ;
    condd = bytecode_add(module_value->code, &bc);

    /* FROM < TO */
        bc.type = BYTECODE_LABEL;
        labelA = bytecode_add(module_value->code, &bc);

        /* for all */
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_VEC_DEREF;
        bc.attr.stack_level = 2;
        bc.attr.index = 1;
        bytecode_add(module_value->code, &bc);
    
        bc.type = BYTECODE_OP_LTE_INT;
        bytecode_add(module_value->code, &bc);

        /* exit loop if all passed */
        bc.type = BYTECODE_JUMPZ;
        condea = bytecode_add(module_value->code, &bc);

        /* push value */
        bc.type = BYTECODE_OP_DUP_INT;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        /* remember stack level to get value later */
        value->stack_level = stack_level + 3;

        qualifier_stack_emit(listcomp_value, node->next, stack_level + 3, module_value, list_weak, result);

        /* pop value */
        bc.type = BYTECODE_SLIDE;
        bc.slide.m = 0;
        bc.slide.q = 1;
        bytecode_add(module_value->code, &bc);

        /* inc loop counter */
        bc.type = BYTECODE_OP_INC_INT;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        /* jump to beginning */
        bc.type = BYTECODE_JUMP;
        conda = bytecode_add(module_value->code, &bc);
        conda->jump.offset = labelA->addr - conda->addr;

    /* FROM > TO */
    bc.type = BYTECODE_LABEL;
    labelD = bytecode_add(module_value->code, &bc);
    condd->jump.offset = labelD->addr - condd->addr;

        bc.type = BYTECODE_LABEL;
        labelB = bytecode_add(module_value->code, &bc);

        /* for all */
        bc.type = BYTECODE_ID_LOCAL;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_VEC_DEREF;
        bc.attr.stack_level = 2;
        bc.attr.index = 1;
        bytecode_add(module_value->code, &bc);
    
        bc.type = BYTECODE_OP_GTE_INT;
        bytecode_add(module_value->code, &bc);

        /* exit loop if all passed */
        bc.type = BYTECODE_JUMPZ;
        condeb = bytecode_add(module_value->code, &bc);

        /* push value */
        bc.type = BYTECODE_OP_DUP_INT;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        /* remember stack level to get value later */
        value->stack_level = stack_level + 3;

        qualifier_stack_emit(listcomp_value, node->next, stack_level + 3, module_value, list_weak, result);

        /* pop value */
        bc.type = BYTECODE_SLIDE;
        bc.slide.m = 0;
        bc.slide.q = 1;
        bytecode_add(module_value->code, &bc);

        /* inc loop counter */
        bc.type = BYTECODE_OP_DEC_INT;
        bc.id_local.stack_level = 0;
        bc.id_local.index = 0;
        bytecode_add(module_value->code, &bc);

        /* jump to beginning */
        bc.type = BYTECODE_JUMP;
        condb = bytecode_add(module_value->code, &bc);
        condb->jump.offset = labelB->addr - condb->addr;

    /* exit */
    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condea->jump.offset = labelE->addr - condea->addr;
    condeb->jump.offset = labelE->addr - condeb->addr;

    /* pop range and counter */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 2;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int generator_slice_emit(listcomp * listcomp_value, qualifier_list_node * node,
                         int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode * condd;
    bytecode * conda, * condb;
    bytecode * condea, * condeb;
    bytecode * labelA, * labelB;
    bytecode * labelD;
    bytecode * labelE;
    qualifier * value = node->value;

    expr_emit(value->expr_value, stack_level, module_value, list_weak, result);

    /* push array */
    bc.type = BYTECODE_VECREF_VEC_DEREF;
    bc.attr.stack_level = 0;
    bc.attr.index = SLICE_ARRAY_INDEX;
    bytecode_add(module_value->code, &bc);

    /* push range */
    bc.type = BYTECODE_VECREF_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = SLICE_RANGE_INDEX;
    bytecode_add(module_value->code, &bc);

    /* loop counter set to from value */
    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_ASS_INT;
    bytecode_add(module_value->code, &bc);

    /* get loop direction */
    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 1;
    bc.attr.index = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_VEC_DEREF;
    bc.attr.stack_level = 2;
    bc.attr.index = 1;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_OP_LT_INT;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_JUMPZ;
    condd = bytecode_add(module_value->code, &bc);

    /* FROM  < TO */
       bc.type = BYTECODE_LABEL;
       labelA = bytecode_add(module_value->code, &bc);

       /* for all */
       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 0;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_VEC_DEREF;
       bc.attr.stack_level = 2;
       bc.attr.index = 1;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_OP_LTE_INT;
       bytecode_add(module_value->code, &bc);

       /* exit loop if all passed */
       bc.type = BYTECODE_JUMPZ;
       condea = bytecode_add(module_value->code, &bc);

       /* push value */
       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 2;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 1;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_ARRAY_DEREF;
       bc.array_deref.dims = 1;
       bytecode_add(module_value->code, &bc);

       /* remember stack level to get value later */
       value->stack_level = stack_level + 5;

       qualifier_stack_emit(listcomp_value, node->next, stack_level + 5, module_value, list_weak, result);

       /* pop value */
       bc.type = BYTECODE_SLIDE;
       bc.slide.m = 0;
       bc.slide.q = 1;
       bytecode_add(module_value->code, &bc);

       /* inc loop counter */
       bc.type = BYTECODE_OP_INC_INT;
       bc.id_local.stack_level = 0;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       /* jump to beginning */
       bc.type = BYTECODE_JUMP;
       conda = bytecode_add(module_value->code, &bc);
       conda->jump.offset = labelA->addr - conda->addr;

    /* FROM > TO */
    bc.type = BYTECODE_LABEL;
    labelD = bytecode_add(module_value->code, &bc);
    condd->jump.offset = labelD->addr - condd->addr;

       bc.type = BYTECODE_LABEL;
       labelB = bytecode_add(module_value->code, &bc);

       /* for all */
       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 0;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_VEC_DEREF;
       bc.attr.stack_level = 2;
       bc.attr.index = 1;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_OP_GTE_INT;
       bytecode_add(module_value->code, &bc);

       /* exit loop if all passed */
       bc.type = BYTECODE_JUMPZ;
       condeb = bytecode_add(module_value->code, &bc);

       /* push value */
       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 2;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_ID_LOCAL;
       bc.id_local.stack_level = 1;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       bc.type = BYTECODE_ARRAY_DEREF;
       bc.array_deref.dims = 1;
       bytecode_add(module_value->code, &bc);

       /* remember stack level to get value later */
       value->stack_level = stack_level + 5;

       qualifier_stack_emit(listcomp_value, node->next, stack_level + 5, module_value, list_weak, result);

       /* pop value */
       bc.type = BYTECODE_SLIDE;
       bc.slide.m = 0;
       bc.slide.q = 1;
       bytecode_add(module_value->code, &bc);

       /* inc loop counter */
       bc.type = BYTECODE_OP_DEC_INT;
       bc.id_local.stack_level = 0;
       bc.id_local.index = 0;
       bytecode_add(module_value->code, &bc);

       /* jump to beginning */
       bc.type = BYTECODE_JUMP;
       condb = bytecode_add(module_value->code, &bc);
       condb->jump.offset = labelB->addr - condb->addr;

    /* exit */
    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condea->jump.offset = labelE->addr - condea->addr;
    condeb->jump.offset = labelE->addr - condeb->addr;

    /* pop slice, array, range, counter */
    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 0;
    bc.slide.q = 4;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int generator_emit(listcomp * listcomp_value, qualifier_list_node * node,
                   int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    if (node->value->expr_value->comb.comb == COMB_TYPE_ARRAY &&
        node->value->expr_value->comb.comb_dims == 1)
    {
        generator_array_emit(listcomp_value, node, stack_level, module_value,
                             list_weak, result);
    }
    else if (node->value->expr_value->comb.comb == COMB_TYPE_RANGE &&
             node->value->expr_value->comb.comb_dims == 1)
    {
        generator_range_emit(listcomp_value, node, stack_level, module_value,
                             list_weak, result);
    }
    else if (node->value->expr_value->comb.comb == COMB_TYPE_SLICE &&
             node->value->expr_value->comb.comb_dims == 1)
    {
        generator_slice_emit(listcomp_value, node, stack_level, module_value,
                             list_weak, result);
    }
    else
    {
        assert(0);
    }
    

    return 0;
}

int filter_emit(listcomp * listcomp_value, qualifier_list_node * node,
                int stack_level, module * module_value,
                func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode * labelE, * condz;
    qualifier * value = node->value;
    value->stack_level = stack_level;

    expr_emit(value->expr_value, stack_level, module_value,
              list_weak, result);

    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);
    
    qualifier_stack_emit(listcomp_value, node->next, stack_level, module_value,
                         list_weak, result);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelE->addr - condz->addr;     
    
    return 0;
}

int qualifier_emit(listcomp * listcomp_value, qualifier_list_node * node,
                   int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    qualifier * value = node->value;

    switch (value->type)
    {
        case QUALIFIER_GENERATOR:
            generator_emit(listcomp_value, node, stack_level, module_value, list_weak, result);
        break;
        case QUALIFIER_FILTER:
            filter_emit(listcomp_value, node, stack_level, module_value, list_weak, result);
        break;
        case QUALIFIER_UNKNOWN:
            assert(0);
        break;
    }

    return 0;
}

int qualifier_stack_emit(listcomp * listcomp_value, qualifier_list_node * node,
                         int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result)
{
    if (node != NULL)
    {
        qualifier * value = node->value;
        if (value != NULL)
        {
            qualifier_emit(listcomp_value, node, stack_level, module_value,
                           list_weak, result);
        }
    }
    else
    {
        if (listcomp_value->expr_value != NULL)
        {
            expr_yeld_emit(listcomp_value, stack_level, module_value, list_weak, result);
        }
    }

    return 0;
}                         

int expr_conv_emit(expr * value, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    int err = 0;
    bytecode bc = { 0 };

    if (value->conv.expr_value->comb.comb == COMB_TYPE_INT)
    {
        if (value->conv.type == CONV_INT_TO_LONG)
        {
            bc.type = BYTECODE_INT_TO_LONG;
            bytecode_add(module_value->code, &bc);
        }
        else if (value->conv.type == CONV_INT_TO_FLOAT)
        {
            bc.type = BYTECODE_INT_TO_FLOAT;
            bytecode_add(module_value->code, &bc);
        }
        else if (value->conv.type == CONV_INT_TO_DOUBLE)
        {
            bc.type = BYTECODE_INT_TO_DOUBLE;
            bytecode_add(module_value->code, &bc);
        }
        else
        {
            err = 1;
        }
    }
    else if (value->conv.expr_value->comb.comb == COMB_TYPE_LONG)
    {
        if (value->conv.type == CONV_LONG_TO_INT)
        {
            bc.type = BYTECODE_LONG_TO_INT;
            bytecode_add(module_value->code, &bc);
        }
        else if (value->conv.type == CONV_LONG_TO_FLOAT)
        {
            bc.type = BYTECODE_LONG_TO_FLOAT;
            bytecode_add(module_value->code, &bc);
        }
        else if (value->conv.type == CONV_LONG_TO_DOUBLE)
        {
            bc.type = BYTECODE_LONG_TO_DOUBLE;
            bytecode_add(module_value->code, &bc);
        }
        else
        {
            err = 1;
        }
    }
    else if (value->conv.expr_value->comb.comb == COMB_TYPE_FLOAT)
    {
        if (value->conv.type == CONV_FLOAT_TO_INT)
        {
            bc.type = BYTECODE_FLOAT_TO_INT;
            bytecode_add(module_value->code, &bc);
        }
        else if (value->conv.type == CONV_FLOAT_TO_LONG)
        {
            bc.type = BYTECODE_FLOAT_TO_LONG;
            bytecode_add(module_value->code, &bc);
        }
        else if (value->conv.type == CONV_FLOAT_TO_DOUBLE)
        {
            bc.type = BYTECODE_FLOAT_TO_DOUBLE;
            bytecode_add(module_value->code, &bc);
        }
        else
        {
            err = 1;
        }
    }
    else if (value->conv.expr_value->comb.comb == COMB_TYPE_DOUBLE)
    {
        if (value->conv.type == CONV_DOUBLE_TO_INT)
        {
            bc.type = BYTECODE_DOUBLE_TO_INT;
            bytecode_add(module_value->code, &bc);
        }
        else if (value->conv.type == CONV_DOUBLE_TO_LONG)
        {
            bc.type = BYTECODE_DOUBLE_TO_LONG;
            bytecode_add(module_value->code, &bc);
        }
        else if (value->conv.type == CONV_DOUBLE_TO_FLOAT)
        {
            bc.type = BYTECODE_DOUBLE_TO_FLOAT;
            bytecode_add(module_value->code, &bc);
        }
        else
        {
            err = 1;
        }
    }
    else if (value->conv.expr_value->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        if (value->conv.type == CONV_ENUMTYPE_RECORD_TO_INT)
        {
            bc.type = BYTECODE_ENUMTYPE_RECORD_TO_INT;
            bytecode_add(module_value->code, &bc);
        }
        else
        {
            err = 1;
        }        
    }
    else
    {
        err = 1;
    }

    if (err)
    {
        *result = EMIT_FAIL;
        print_error_msg(value->line_no, "cannot convert type %s using conversion %s",
                        comb_type_str(value->conv.expr_value->comb.comb),
                        conv_type_str(value->conv.type));
        assert(0);
    }

    return 0;
}                

int listcomp_emit(listcomp * listcomp_value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    listcomp_value->stack_level = stack_level;

    bc.type = BYTECODE_INT;
    bc.int_t.value = 0;
    bytecode_add(module_value->code, &bc);    

    bc.type = BYTECODE_MK_INIT_ARRAY;
    bc.mk_array.dims = 1;
    bytecode_add(module_value->code, &bc);

    qualifier_stack_emit(listcomp_value, listcomp_value->list->tail, stack_level + 1,
                         module_value, list_weak, result);
    return 0;
}                  

int array_dims_emit(array * array_value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    
    expr_list_emit(array_value->dims, stack_level, module_value, list_weak, result);

    if (array_value->ret->type == PARAM_BOOL)
    {
        bc.type = BYTECODE_MK_ARRAY_INT;
    }
    else if (array_value->ret->type == PARAM_INT)
    {
        bc.type = BYTECODE_MK_ARRAY_INT;
    }
    else if (array_value->ret->type == PARAM_LONG)
    {
        bc.type = BYTECODE_MK_ARRAY_LONG;
    }
    else if (array_value->ret->type == PARAM_FLOAT)
    {
        bc.type = BYTECODE_MK_ARRAY_FLOAT;
    }
    else if (array_value->ret->type == PARAM_DOUBLE)
    {
        bc.type = BYTECODE_MK_ARRAY_DOUBLE;
    }
    else if (array_value->ret->type == PARAM_CHAR)
    {
        bc.type = BYTECODE_MK_ARRAY_CHAR;
    }
    else if (array_value->ret->type == PARAM_STRING)
    {
        bc.type = BYTECODE_MK_ARRAY_STRING;
    }
    else if (array_value->ret->type == PARAM_ENUMTYPE)
    {
        switch (array_value->ret->enumtype_value->type)
        {
            case ENUMTYPE_TYPE_ITEM:
                bc.type = BYTECODE_MK_ARRAY_INT;
            break;
            case ENUMTYPE_TYPE_RECORD:
                bc.type = BYTECODE_MK_ARRAY_RECORD;
            break;
        }
    }
    else if (array_value->ret->type == PARAM_ARRAY)
    {
        bc.type = BYTECODE_MK_ARRAY_ARRAY;
    }
    else if (array_value->ret->type == PARAM_RANGE)
    {
        bc.type = BYTECODE_MK_ARRAY_RECORD;
    }
    else if (array_value->ret->type == PARAM_SLICE)
    {
        bc.type = BYTECODE_MK_ARRAY_RECORD;
    }
    else if (array_value->ret->type == PARAM_RECORD)
    {
        bc.type = BYTECODE_MK_ARRAY_RECORD;
    }
    else if (array_value->ret->type == PARAM_FUNC)
    {
        bc.type = BYTECODE_MK_ARRAY_FUNC;
    }
    else
    {
        assert(0);
    }

    bc.mk_array.dims = array_value->dims->count;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int array_init_elements_emit(expr_list_weak * depth_list, int * elements_count,
                             int stack_level, module * module_value,
                             func_list_weak * list_weak, int * result)
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
            expr_emit(value, stack_level + (*elements_count)++, module_value,
                      list_weak, result);
        }
        else
        {
            break;
        }
        node = node->next;
    }

    return 0;
}

int array_init_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    int elements_count = 0;
    array * array_value = value->array.array_value;

    expr_list_weak * depth_list = expr_list_weak_new();
    array_to_depth_list(value, depth_list);

    array_init_elements_emit(depth_list, &elements_count, stack_level, module_value,
                             list_weak, result);
    expr_list_weak_delete(depth_list);

    expr_list_emit(array_value->dims, stack_level + elements_count, module_value,
                   list_weak, result);

    bc.type = BYTECODE_MK_INIT_ARRAY;
    bc.mk_array.dims = array_value->dims->count;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_array_emit(expr * value, int stack_level, module * module_value,
                    func_list_weak * list_weak, int * result)
{
    array * array_value = value->array.array_value;

    if (array_value->type == ARRAY_INIT)
    {
        array_init_emit(value, stack_level, module_value, list_weak, result);
    }
    else if (array_value->type == ARRAY_SUB)
    {
        assert(0);
    }
    else if (array_value->type == ARRAY_DIMS)
    {
        array_dims_emit(array_value, stack_level, module_value, list_weak, result);
    }

    return 0;
}

int expr_array_deref_emit(expr * value, int stack_level, module * module_value,
                          func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->array_deref.array_expr, stack_level, module_value, list_weak, result);
    expr_list_emit(value->array_deref.ref, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_ARRAYREF_DEREF;
    bc.array_deref.dims = value->array_deref.ref->count;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_range_deref_emit(expr * value, int stack_level, module * module_value,
                          func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->array_deref.array_expr, stack_level, module_value, list_weak, result);
    expr_list_emit(value->array_deref.ref, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_RANGE_DEREF;
    bc.array_deref.dims = value->array_deref.ref->count;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_slice_deref_emit(expr * value, int stack_level, module * module_value,
                          func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->array_deref.array_expr, stack_level, module_value, list_weak, result);
    expr_list_emit(value->array_deref.ref, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_SLICE_DEREF;
    bc.array_deref.dims = value->array_deref.ref->count;
    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_string_deref_emit(expr * value, int stack_level, module * module_value,
                           func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };

    expr_emit(value->array_deref.array_expr, stack_level, module_value, list_weak, result);
    expr_list_emit(value->array_deref.ref, stack_level + 1, module_value, list_weak, result);

    bc.type = BYTECODE_STRING_DEREF;

    bytecode_add(module_value->code, &bc);

    return 0;
}

int expr_enumtype_emit(expr * value, int stack_level, module * module_value, int * result)
{
    bytecode bc = { 0 };
    int index = -1;    
    
    if (value->enumtype.id_enumerator_value != NULL)
    {
        index = value->enumtype.id_enumerator_value->index;
    }
    
    bc.type = BYTECODE_INT;
    bc.int_t.value = index;
    bytecode_add(module_value->code, &bc);

    if (value->enumtype.id_enumtype_value->type == ENUMTYPE_TYPE_RECORD)
    {
        bc.type = BYTECODE_RECORD;
        bc.record.count = 1;
        bytecode_add(module_value->code, &bc);
    }

    return 0;
}

int expr_record_attr_emit(expr * value, int stack_level, module * module_value,
                          func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    int slide_q = 1;

    expr_emit(value->attr.record_value, stack_level, module_value, list_weak, result);

    assert(value->attr.id->id.id_type_value == ID_TYPE_LOCAL);
    assert(value->attr.id->id.id_param_value != NULL);

    if (value->attr.id->id.id_param_value->type == PARAM_DIM)
    {
        int dim_index = -1;
        int array_index = -1;

        dim_index = value->attr.id->id.id_param_value->index;
        assert(dim_index != -1);

        array_index = value->attr.id->id.id_param_value->array->index;
        assert(array_index != -1);

        bc.type = BYTECODE_VECREF_VEC_DEREF;
        bc.attr.stack_level = 0;
        bc.attr.index = array_index;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_ID_DIM_LOCAL;
        bc.id_dim_local.stack_level = 0;
        bc.id_dim_local.index = 0;
        bc.id_dim_local.dim_index = dim_index;
        bytecode_add(module_value->code, &bc);

        slide_q = 2;
    }
    else if (value->attr.id->id.id_param_value->type == PARAM_RANGE_DIM)
    {
        int index = -1;
        int range_index = -1;

        index = value->attr.id->id.id_param_value->index;
        assert(index != -1);

        range_index = value->attr.id->id.id_param_value->range->index;
        assert(range_index != -1);

        bc.type = BYTECODE_VECREF_VEC_DEREF;
        bc.attr.stack_level = 0;
        bc.attr.index = range_index;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_VECREF_VEC_DEREF;
        bc.attr.stack_level = 0;
        bc.attr.index = index;
        bytecode_add(module_value->code, &bc);

        slide_q = 2;
    }
    else if (value->attr.id->id.id_param_value->type == PARAM_SLICE_DIM)
    {
        int dim_index = -1;
        int slice_index = -1;

        slice_index = value->attr.id->id.id_param_value->slice->index;
        assert(slice_index != -1);

        dim_index = value->attr.id->id.id_param_value->index;
        assert(dim_index != -1);

        bc.type = BYTECODE_VECREF_VEC_DEREF;
        bc.attr.stack_level = 0;
        bc.attr.index = slice_index;
        bytecode_add(module_value->code, &bc);

        bc.type = BYTECODE_ID_DIM_SLICE;
        bc.id_dim_slice.stack_level = 0;
        bc.id_dim_slice.index = 0;
        bc.id_dim_slice.dim_index = dim_index;
        bytecode_add(module_value->code, &bc);

        slide_q = 2;
    }
    else
    {
        int index = -1;
        index = value->attr.id->id.id_param_value->index;
        assert(index != -1);

        bc.type = BYTECODE_VECREF_VEC_DEREF;
        bc.attr.stack_level = 0;
        bc.attr.index = index;
        bytecode_add(module_value->code, &bc);

        slide_q = 1;
    }

    bc.type = BYTECODE_SLIDE;
    bc.slide.m = 1;
    bc.slide.q = slide_q;
    bytecode_add(module_value->code, &bc);

    return 0;
}                   

int bind_emit(bind * bind_value, int stack_level, module * module_value,
              func_list_weak * list_weak, int * result)
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
                bind_value->index = stack_level + 1;

                /* printf("bind %s %d\n", bind_value->id, bind_value->index); */

                expr_emit(bind_value->expr_value, stack_level, module_value, list_weak, result);
            }
        break;
    }
    return 0;
}

#if 0
int bind_list_emit(bind_list * list, int stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    int b = 0;
    
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * bind_value = node->value;
        if (bind_value != NULL)
        {
            bind_emit(bind_value, stack_level + b++, module_value, list_weak, result);
        }
        node = node->next;
    }
    return 0;
}
#endif

int except_implicit_emit(func * func_value, int stack_level, module * module_value,
                         func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    
    bc.type = BYTECODE_RETHROW;
    bytecode_add(module_value->code, &bc);

    return 0;
}
    
int except_all_emit(except * value, func * func_value, int stack_level,
                    module * module_value, func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *labelA, *labelE = NULL;
    int param_count = 0;

    if (func_value->decl->params != NULL)
    {
        param_count = func_value->decl->params->count;
    }

    bc.type = BYTECODE_CLEAR_STACK;
    bc.clear.count = param_count;
    labelA = bytecode_add(module_value->code, &bc);

    if (value->expr_value != NULL)
    {
        expr_emit(value->expr_value, stack_level, module_value, list_weak, result);
    }
    
    bc.type = BYTECODE_RET;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);

    exception_tab_insert(module_value->exctab_value, labelA->addr, labelE->addr);    
    
    return 0;
}

int except_emit(except * value, func * func_value, int stack_level,
                module * module_value, func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *labelA, *labelE = NULL;
    bytecode *condz = NULL;
    int param_count = 0;

    if (func_value->decl->params != NULL)
    {
        param_count = func_value->decl->params->count;
    }

    bc.type = BYTECODE_CLEAR_STACK;
    bc.clear.count = param_count;
    labelA = bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_INT;
    bc.int_t.value = value->no;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_PUSH_EXCEPT;
    bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_OP_EQ_INT;
    bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_JUMPZ;
    condz = bytecode_add(module_value->code, &bc);

    if (value->expr_value != NULL)
    {
        expr_emit(value->expr_value, stack_level, module_value, list_weak, result);
    }
    
    bc.type = BYTECODE_RET;
    bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    condz->jump.offset = labelE->addr - condz->addr;
    
    exception_tab_insert(module_value->exctab_value, labelA->addr, labelE->addr);    
    
    return 0;
}

int except_list_emit(except_list * list, func * func_value, int stack_level,
                     module * module_value, func_list_weak * list_weak, int * result)
{
    except_list_node * node = list->tail;
    while (node != NULL)
    {
        except * value = node->value;
        if (value != NULL)
        {
            except_emit(value, func_value, stack_level, module_value, list_weak, result);
        }
        node = node->next;
    }

    return 0;
}

int func_except_emit(func_except * value, func * func_value, int stack_level, 
                     module * module_value, func_list_weak * list_weak, int * result)
{
    if (value->list != NULL)
    {
        except_list_emit(value->list, func_value, stack_level, module_value, list_weak,
                         result);
    }
    if (value->all != NULL)
    {
        except_all_emit(value->all, func_value, stack_level, module_value, list_weak, result);
    }

    return 0;
}

unsigned int func_body_emit_ffi_param(param * value, module * module_value, int * result)
{
    bytecode bc = { 0 };
    unsigned int total_count = 1;

    switch (value->type)
    {
        case PARAM_BOOL:
            bc.type = BYTECODE_FUNC_FFI_BOOL;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_INT:
            bc.type = BYTECODE_FUNC_FFI_INT;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_LONG:
            bc.type = BYTECODE_FUNC_FFI_LONG;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_FLOAT:
            bc.type = BYTECODE_FUNC_FFI_FLOAT;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_DOUBLE:
            bc.type = BYTECODE_FUNC_FFI_DOUBLE;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_CHAR:
            bc.type = BYTECODE_FUNC_FFI_CHAR;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_STRING:
            bc.type = BYTECODE_FUNC_FFI_STRING;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_VOID:
            bc.type = BYTECODE_FUNC_FFI_VOID;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_C_PTR:
            bc.type = BYTECODE_FUNC_FFI_C_PTR;
            bytecode_add(module_value->code, &bc);
        break;
        case PARAM_RECORD:
        {
            bytecode * ffi_rec;
            bc.type = BYTECODE_FUNC_FFI_RECORD;
            bc.ffi_record.count = value->record_value->params->count;
            ffi_rec = bytecode_add(module_value->code, &bc);

            if (value->record_value->cycle == 0)
            {
                value->record_value->cycle = 1;
                total_count = 1 + func_body_emit_ffi_param_list(value->record_value->params, module_value, result);
                value->record_value->cycle = 0;
            }
            else
            {
                *result = EMIT_FAIL;
                print_error_msg(value->line_no, "cannot generate ffi code for infinite records %s\n", value->record_value->id);
            }

            ffi_rec->ffi_record.total_count = total_count;
        }
        break;
        case PARAM_DIM:
        case PARAM_ARRAY:
        case PARAM_RANGE_DIM:
        case PARAM_RANGE:
        case PARAM_SLICE:
        case PARAM_SLICE_DIM:
        case PARAM_ENUMTYPE:
        case PARAM_FUNC:
            *result = EMIT_FAIL;
            print_error_msg(value->line_no, "ffi type not supported");
        break;
    }

    return total_count;
}

unsigned int func_body_emit_ffi_param_list(param_list * params, module * module_value, int * result)
{
    unsigned int total_count = 0;
    param_list_node * node = params->tail;
    
    while (node != NULL)
    {
        param * value = node->value;
        if (value != NULL)
        {
            total_count += func_body_emit_ffi_param(value, module_value, result);
        }
        node = node->next;
    }

    return total_count;
}

int func_body_emit_ffi(func * func_value, module * module_value,
                       func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    unsigned int count = 0;
    bytecode * labelA = NULL;
    bytecode * labelE = NULL;
    
    if (func_value->decl != NULL && func_value->decl->params != NULL)
    {
        count = func_value->decl->params->count;
    }

    bc.type = BYTECODE_FUNC_FFI;
    bc.ffi.count = count;
    bc.ffi.fname_index = strtab_add_string(module_value->strtab_value, func_value->decl->id);
    bc.ffi.libname_index = strtab_add_string(module_value->strtab_value, func_value->libname);

    labelA = bytecode_add(module_value->code, &bc);
    func_value->addr = labelA->addr;
    
    if (func_value->decl != NULL && func_value->decl->params != NULL)
    {
        func_body_emit_ffi_param_list(func_value->decl->params, module_value, result);
    }
    if (func_value->decl != NULL && func_value->decl->ret != NULL)
    {
        func_body_emit_ffi_param(func_value->decl->ret, module_value, result);
    }
    
    bc.type = BYTECODE_RET;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);

    exception_tab_insert(module_value->exctab_value, labelA->addr, labelE->addr);

    except_implicit_emit(func_value, 0, module_value, list_weak, result);

    return 0;
}

int func_body_emit_native(func * func_value, module * module_value,
                          func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    bytecode *labelA, *labelE = NULL;
    int stack_level = 0;

    bc.type = BYTECODE_FUNC_DEF;
    labelA = bytecode_add(module_value->code, &bc);
    func_value->addr = labelA->addr;

    if (func_value->body && func_value->body->exprs)
    {
        expr_emit(func_value->body->exprs, stack_level, module_value, list_weak, result);
        /* TODO: remove this stack_level += func_value->body->binds->count; */
    }
    /* TODO: remove
    if (func_value->body && func_value->body->funcs)
    {
        func_list_emit(func_value->body->funcs, &stack_level, module_value,
                       list_weak, result);
    }
    */
    /* TODO: remove
    if (func_value->body && func_value->body->ret)
    {
        expr_emit(func_value->body->ret, stack_level, module_value, list_weak, result);
    }
    */
    if (func_value->body && func_value->body->exprs && func_value->body->exprs->line_no > 0)
    {
        bc.type = BYTECODE_LINE;
        bc.line.no = func_value->body->exprs->line_no;
        bytecode_add(module_value->code, &bc);
    }

    bc.type = BYTECODE_RET;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);

    exception_tab_insert(module_value->exctab_value, labelA->addr, labelE->addr);

    if (func_value->except != NULL)
    {
        func_except_emit(func_value->except, func_value, 0, module_value, list_weak, result);
    }
    except_implicit_emit(func_value, 0, module_value, list_weak, result);

    return 0;
}

int func_body_emit(func * func_value, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    switch (func_value->type)
    {
        case FUNC_TYPE_UNKNOWN:
            assert(0);
        break;
        case FUNC_TYPE_NATIVE:
            func_body_emit_native(func_value, module_value, list_weak, result);
        break;
        case FUNC_TYPE_FFI:
            func_body_emit_ffi(func_value, module_value, list_weak, result);
        break;
    }
    
    return 0;
}

int func_emit_ffi(func * func_value, int stack_level, module * module_value,
                  func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    int freevar_count = 0;

    bc.type = BYTECODE_FUNC_OBJ;
    bytecode_add(module_value->code, &bc);

    if (func_value->line_no > 0)
    {
        bc.type = BYTECODE_LINE;
        bc.line.no = func_value->line_no;
        bytecode_add(module_value->code, &bc);
    }

    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = freevar_count;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(module_value->code, &bc);

    func_list_weak_add(list_weak, func_value);

    return 0;
}

int func_emit_native(func * func_value, int stack_level, module * module_value,
                     func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    int freevar_count = 0;

    bc.type = BYTECODE_FUNC_OBJ;
    bytecode_add(module_value->code, &bc);

    if (func_value->line_no > 0)
    {
        bc.type = BYTECODE_LINE;
        bc.line.no = func_value->line_no;
        bytecode_add(module_value->code, &bc);
    }

    if (func_value->freevars != NULL)
    {
        func_freevar_list_emit(func_value->freevars, stack_level, module_value, result);
        freevar_count = func_value->freevars->count;
    }

    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = freevar_count;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ID_FUNC_FUNC;
    bc.id_func.func_value = func_value;
    bytecode_add(module_value->code, &bc);

    func_list_weak_add(list_weak, func_value);

    return 0;
}

int func_emit(func * func_value, int stack_level, module * module_value,
              func_list_weak * list_weak, int * result)
{
    switch (func_value->type)
    {
        case FUNC_TYPE_UNKNOWN:
            assert(0);
        break;
        case FUNC_TYPE_NATIVE:
            func_emit_native(func_value, stack_level, module_value, list_weak, result);
        break;
        case FUNC_TYPE_FFI:
            func_emit_ffi(func_value, stack_level, module_value, list_weak, result);
        break;
    }

    return 0;
}              

int func_entry_params(func * func_value, module * module_value, int * result)
{
    if (func_value->entry == 1)
    {
        object * params = NULL;
        object * object_param = NULL;
        unsigned int params_count = 0;

        if (func_value->decl->params != NULL)
        {
            params_count = func_value->decl->params->count;
            params = object_param = malloc(sizeof(object) * (params_count));
            memset(params, 0, sizeof(object) * (params_count));

            param_list_node * node = func_value->decl->params->tail;
            while (node != NULL)
            {
                param * value = node->value;
                if (value->type == PARAM_INT)
                {
                    object_param->type = OBJECT_INT;
                }
                else if (value->type == PARAM_FLOAT)
                {
                    object_param->type = OBJECT_FLOAT;
                }
                object_param++;
                node = node->next;
            }
        }

        functab_add_func(module_value->functab_value, func_value, params, params_count);
    }

    return 0;
}

int seq_list_entry_params(seq_list * list, module * module_value, int * result)
{
    seq_list_node * node = list->tail;
    while (node != NULL)
    {
        seq_item * seq_item_value = node->value;
        if (seq_item_value != NULL &&
            seq_item_value->type == SEQ_TYPE_FUNC &&
            seq_item_value->func_value != NULL)
        {
            func_entry_params(seq_item_value->func_value, module_value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_entry_emit(
    never * nev, int stack_level,
    module * module_value, int * result)
{
    bytecode bc = { 0 };
    bytecode *labelA, *labelE, *labelH;
    bytecode *labelEntry;

    bc.type = BYTECODE_LABEL;
    labelEntry = bytecode_add(module_value->code, &bc);
    module_value->code_entry = labelEntry->addr;

    bc.type = BYTECODE_MARK;
    labelA = bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_PUSH_PARAM;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_GLOBAL_VEC;
    bc.global_vec.count = 0;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_ID_FUNC_ENTRY;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_CALL;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelH = bytecode_add(module_value->code, &bc);
    labelA->mark.addr = labelH->addr;

    bc.type = BYTECODE_HALT;
    bytecode_add(module_value->code, &bc);

    bc.type = BYTECODE_LABEL;
    labelE = bytecode_add(module_value->code, &bc);
    
    bc.type = BYTECODE_UNHANDLED_EXCEPTION;
    bytecode_add(module_value->code, &bc);
    
    exception_tab_insert(module_value->exctab_value, labelA->addr, labelE->addr);            

    return 0;
}

#if 0
int func_list_emit(func_list * list, int * stack_level, module * module_value,
                   func_list_weak * list_weak, int * result)
{
    bytecode bc = { 0 };
    unsigned int n = list->count;

    bc.type = BYTECODE_ALLOC;
    bc.alloc.n = n;
    bytecode_add(module_value->code, &bc);

    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * func_value = node->value;
        if (func_value != NULL)
        {
            /* TODO: check stack_level passed to func_emit */
            func_emit(func_value, (*stack_level) + list->count, module_value, list_weak, result);
            func_value->index = list->count - n + 1;

            bc.type = BYTECODE_REWRITE;
            bc.rewrite.j = n--;
            bytecode_add(module_value->code, &bc);
        }
        node = node->next;
    }
    (*stack_level) += list->count;

    return 0;
}
#endif

int never_emit(never * nev, int * stack_level, module * module_value, func_list_weak * list_weak, int * result)
{
    if (nev->exprs)
    {
        seq_list_emit(nev->exprs, stack_level, module_value, list_weak, result);
    }

    return 0;
}

int use_emit(use * use_value, int * stack_level, module * module_value, func_list_weak * list_weak, int * result)
{
    if (use_value->decl != NULL)
    {
        module_decl_emit(use_value->decl, stack_level, module_value, list_weak, result);
    }

    return 0;    
}

int use_list_emit(use_list * list, int * stack_level, module * module_value, func_list_weak * list_weak, int * result)
{
    use_list_node * node = list->tail;

    while (node != NULL)
    {
        if (node->value != NULL)
        {
            use_emit(node->value, stack_level, module_value, list_weak, result);
        }
        node = node->next;
    }

    return 0;
}

int module_decl_emit(module_decl * value, int * stack_level, module * module_value, func_list_weak * list_weak, int * result)
{
    if (value->nev)
    {
        never_emit(value->nev, stack_level, module_value, list_weak, result);
    }

    return 0;
}

int main_emit(module_decl * module_modules, module_decl * module_main, module * module_value)
{
    int gencode_res = 0;
    int stack_level = -1;
    func_list_weak * list_weak = func_list_weak_new();

    use_list_emit(module_modules->nev->uses, &stack_level, module_value, list_weak, &gencode_res);
    module_decl_emit(module_main, &stack_level, module_value, list_weak, &gencode_res);
    func_entry_emit(module_main->nev, stack_level, module_value, &gencode_res);

    while (list_weak->count > 0)
    {
        func * value = func_list_weak_pop(list_weak);
        if (value != NULL)
        {
            func_body_emit(value, module_value, list_weak, &gencode_res);
        }
    }

    func_list_weak_delete(list_weak);

    /* generate module entry function list */
    seq_list_entry_params(module_main->nev->exprs, module_value, &gencode_res);

    return gencode_res;
}
