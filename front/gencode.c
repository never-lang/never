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
#include "iflet.h"
#include "match.h"
#include <assert.h>
#include <stdio.h>

int expr_enumtype_check_call(func * func_value, symtab * stab, expr * value, int * result)
{
    if (value->enumtype.id_enumerator_value != NULL &&
        value->enumtype.id_enumerator_value->type == ENUMERATOR_TYPE_RECORD &&
        value->enumtype.called == 0)
    {
        *result = GENCODE_FAIL;
        print_error_msg(value->line_no, "enum type record %s::%s not constructed\n",
                        value->enumtype.enum_id, value->enumtype.item_id);
    }
    
    return 0;
}        

/**
 * free variables
 */
int expr_id_gencode(unsigned int syn_level, func * func_value, symtab * stab,
                    expr * value, int * result)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(stab, value->id.id, SYMTAB_LOOKUP_GLOBAL);
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

                    freevar_value->orig.type = FREEVAR_FUNC;
                    freevar_value->orig.func_value = sup_func_value;

                    value->id.id_type_value = ID_TYPE_GLOBAL;
                    value->id.id_freevar_value = freevar_value;
                }
            }
        }
        else if (entry->type == SYMTAB_PARAM && entry->param_value != NULL)
        {
            param * param_value = entry->param_value;
            if (param_value->type == PARAM_INT ||
                param_value->type == PARAM_FLOAT ||
                param_value->type == PARAM_CHAR ||
                param_value->type == PARAM_STRING ||
                param_value->type == PARAM_ENUMTYPE ||
                param_value->type == PARAM_DIM ||
                param_value->type == PARAM_ARRAY ||
                param_value->type == PARAM_RECORD ||
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

                    freevar_value->orig.type = FREEVAR_PARAM;
                    freevar_value->orig.param_value = param_value;

                    value->id.id_type_value = ID_TYPE_GLOBAL;
                    value->id.id_freevar_value = freevar_value;
                }
            }
            else
            {
                printf("unknown param type %s\n", param_type_str(param_value->type));
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

                    freevar_value->orig.type = FREEVAR_BIND;
                    freevar_value->orig.bind_value = bind_value;
                    
                    value->id.id_type_value = ID_TYPE_GLOBAL;
                    value->id.id_freevar_value = freevar_value;
                }
            }
            else
            {
                fprintf(stderr, "unknown bind type %d\n", bind_value->type);
                assert(0);
            }            
        }
        else if (entry->type == SYMTAB_MATCHBIND && entry->matchbind_value != NULL)
        {
            matchbind * matchbind_value = entry->matchbind_value;
            if (syn_level == entry->syn_level)
            {
                value->id.id_type_value = ID_TYPE_MATCHBIND;
                value->id.id_matchbind_value = matchbind_value;
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

                freevar_value->orig.type = FREEVAR_MATCHBIND;
                freevar_value->orig.matchbind_value = matchbind_value;
                
                value->id.id_type_value = ID_TYPE_GLOBAL;
                value->id.id_freevar_value = freevar_value;
            }
        }
        else if (entry->type == SYMTAB_QUALIFIER && entry->qualifier_value != NULL)
        {
            qualifier * qualifier_value = entry->qualifier_value;
            if (syn_level == entry->syn_level)
            {
                value->id.id_type_value = ID_TYPE_QUALIFIER;
                value->id.id_qualifier_value = qualifier_value;
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

                freevar_value->orig.type = FREEVAR_QUALIFIER;
                freevar_value->orig.qualifier_value = qualifier_value;
                
                value->id.id_type_value = ID_TYPE_GLOBAL;
                value->id.id_freevar_value = freevar_value;
            }
        }
        else if (entry->type == SYMTAB_ENUMTYPE && entry->enumtype_value != NULL)
        {
            *result = GENCODE_FAIL;
            print_error_msg(value->line_no, "found enum %s, at this stage it is very bad\n",
                            value->id.id);
            assert(0);
        }
        else if (entry->type == SYMTAB_RECORD && entry->record_value != NULL)
        {
            record * record_value = entry->record_value;

            value->id.id_type_value = ID_TYPE_RECORD;
            value->id.id_record_value = record_value;
        }
        else
        {
            fprintf(stderr, "unknown record type\n");
            assert(0);
        }
    }
    else
    {
        *result = GENCODE_FAIL;
        print_error_msg(
            value->line_no,
            "cannot find variable %s, at this stage it is very bad\n",
            value->id.id);
        assert(0);
    }

    return 0;
}

/**
 * free variables
 */
int func_gencode_freevars_add_global_freevar(func * func_value, freevar * freevar_value)
{
    freevar * freevar_sup_value = NULL;

    if (func_value->freevars == NULL)
    {
         func_value->freevars = freevar_list_new();
    }

    freevar_sup_value =
            freevar_list_add(func_value->freevars, freevar_value->id);

    freevar_sup_value->orig = freevar_value->orig;

    freevar_value->src.type = FREEVAR_FREEVAR;
    freevar_value->src.freevar_value = freevar_sup_value;

    return 0;
} 
 
int func_gencode_freevars_freevar(func * func_value, symtab * stab, freevar * freevar_value,
                                  int * result)
{
    /** search in symtab */
    /** if found then mark as local */
    /** otherwise mark as global and set index */
    symtab_entry * entry = NULL;

    entry = symtab_lookup(stab, freevar_value->id, SYMTAB_LOOKUP_LOCAL);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_PARAM && freevar_value->orig.type == FREEVAR_PARAM &&
                 entry->param_value == freevar_value->orig.param_value)
        {
            freevar_value->src.type = FREEVAR_PARAM;
            freevar_value->src.param_value = entry->param_value;
        }
        else if (entry->type == SYMTAB_BIND && freevar_value->orig.type == FREEVAR_BIND &&
                 entry->bind_value == freevar_value->orig.bind_value)
        {
            freevar_value->src.type = FREEVAR_BIND;
            freevar_value->src.bind_value = entry->bind_value;
        }
        else if (entry->type == SYMTAB_MATCHBIND && freevar_value->orig.type == FREEVAR_MATCHBIND &&
                 entry->matchbind_value == freevar_value->orig.matchbind_value)
        {
            freevar_value->src.type = FREEVAR_MATCHBIND;
            freevar_value->src.matchbind_value = entry->matchbind_value;
        }
        else if (entry->type == SYMTAB_QUALIFIER && freevar_value->orig.type == FREEVAR_QUALIFIER &&
                 entry->qualifier_value == freevar_value->orig.qualifier_value)
        {
            freevar_value->src.type = FREEVAR_QUALIFIER;
            freevar_value->src.qualifier_value = entry->qualifier_value;
        }
        else if (entry->type == SYMTAB_FUNC && freevar_value->orig.type == FREEVAR_FUNC &&
            entry->func_value == freevar_value->orig.func_value)
        {
            freevar_value->src.type = FREEVAR_FUNC;
            freevar_value->src.func_value = entry->func_value;
        }
        else
        {
            func_gencode_freevars_add_global_freevar(func_value, freevar_value);
        }
    }
    else
    {
        func_gencode_freevars_add_global_freevar(func_value, freevar_value);
    }

    return 0;
}

int func_gencode_freevars_iflet_expr(func * func_value, symtab * stab,
                                     expr * value, int * result)
{
    func_gencode_freevars_expr(func_value, stab, value->iflet_value->expr_value, result);
    
    switch (value->iflet_value->type)
    {
        case IFLET_TYPE_ITEM:
            func_gencode_freevars_expr(func_value, stab, value->iflet_value->then_value, result);
            func_gencode_freevars_expr(func_value, stab, value->iflet_value->else_value, result);
        break;
        case IFLET_TYPE_RECORD:
            func_gencode_freevars_expr(func_value, value->iflet_value->guard_record->stab, value->iflet_value->then_value, result);
            func_gencode_freevars_expr(func_value, stab, value->iflet_value->else_value, result);
        break;
    }

    return 0;
}        

int func_gencode_freevars_match_guard(func * func_value, symtab * stab,
                                      match_guard * match_value, int * result)
{
    switch (match_value->type)
    {
        case MATCH_GUARD_ITEM:
            func_gencode_freevars_expr(func_value, stab, match_value->guard_item.expr_value, result);
        break;
        case MATCH_GUARD_RECORD:
            func_gencode_freevars_expr(func_value, match_value->guard_record.guard->stab, match_value->guard_record.expr_value, result);
        break;
        case MATCH_GUARD_ELSE:
            func_gencode_freevars_expr(func_value, stab, match_value->guard_else.expr_value, result);
        break;
    }
    
    return 0;
}

int func_gencode_freevars_match_guard_list(func * func_value, symtab * stab,
                                           match_guard_list * list, int * result)
{
    match_guard_list_node * node = list->tail;

    while (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL)
        {
            func_gencode_freevars_match_guard(func_value, stab, match_value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars_match_expr(func * func_value, symtab * stab,
                                     expr * value, int * result)
{
    func_gencode_freevars_expr(func_value, stab, value->match.expr_value, result);
    if (value->match.match_guards != NULL)
    {
        func_gencode_freevars_match_guard_list(func_value, stab, value->match.match_guards, result);
    }

    return 0;
}

int func_gencode_freevars_qualifier(func * func_value, symtab * stab,
                                    qualifier * value, int * result)
{
    switch (value->type)
    {
        case QUALIFIER_UNKNOWN:
            assert(0);
        break;
        case QUALIFIER_GENERATOR:
            if (value->expr_value != NULL)
            {
                func_gencode_freevars_expr(func_value, stab, value->expr_value, result);
            }
        break;
        case QUALIFIER_FILTER:
            if (value->expr_value != NULL)
            {
                func_gencode_freevars_expr(func_value, stab, value->expr_value, result);
            }
        break;
    }

    return 0;
}                                    

int func_gencode_freevars_qualifier_list(func * func_value, symtab * stab,
                                         qualifier_list * list, int * result)
{
    qualifier_list_node * node = list->tail;

    while (node != NULL)
    {
        qualifier * qualifier_value = node->value;
        if (qualifier_value != NULL)
        {
            func_gencode_freevars_qualifier(func_value, stab, qualifier_value, result);
        }
        node = node->next;
    }


    return 0;
}

int func_gencode_freevars_listcomp(func * func_value, symtab * stab,
                                   listcomp * value, int * result)
{
    if (value->list != NULL)
    {
        func_gencode_freevars_qualifier_list(func_value, stab, value->list, result);
    }
    if (value->expr_value != NULL)
    {
        func_gencode_freevars_expr(func_value, stab, value->expr_value, result);
    }

    return 0;
}                                   

int func_gencode_freevars_array(func * func_value, symtab * stab, array * array_value,
                                int * result)
{
    if (array_value->dims != NULL)
    {
        func_gencode_freevars_expr_list(
            func_value, stab, array_value->dims, result);
    }
    if (array_value->elements != NULL)
    {
        func_gencode_freevars_expr_list(
            func_value, stab, array_value->elements, result);
    }

    return 0;
}                                

int func_gencode_freevars_bind(func * func_value, symtab * stab, bind * bind_value,
                               int * result)
{
    if (bind_value->expr_value)
    {
        func_gencode_freevars_expr(func_value, stab, bind_value->expr_value, result);
    }
    return 0;
}

int func_gencode_freevars_bind_list(func * func_value, symtab * stab, bind_list * list,
                                    int * result)
{
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * bind_value = node->value;
        if (bind_value != NULL)
        {
            func_gencode_freevars_bind(func_value, stab, bind_value, result);
        }
        node = node->next;
    }
    return 0;
}

int func_gencode_freevars_except(func * func_value, symtab * stab, except * except_value,
                                 int * result)
{
    if (except_value->expr_value != NULL)
    {
        func_gencode_freevars_expr(func_value, stab, except_value->expr_value, result);
    }

    return 0;
}

int func_gencode_freevars_except_list(func * func_value, symtab * stab, except_list * list,
                                      int * result)
{
    except_list_node * node = list->tail;
    while (node != NULL)
    {
        except * value = node->value;
        if (value != NULL)
        {
            func_gencode_freevars_except(func_value, stab, value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars_func_except(func * func_value, symtab * stab, func_except * value,
                                      int * result)
{
    if (value->list != NULL)
    {
        func_gencode_freevars_except_list(func_value, stab, value->list, result);
    }
    if (value->all != NULL)
    {
        func_gencode_freevars_except(func_value, stab, value->all, result);
    }

    return 0;
}

int func_gencode_freevars_func(func * func_value, symtab * stab, func * subfunc_value,
                               int * result)
{
    if (subfunc_value->type == FUNC_TYPE_NATIVE && subfunc_value->freevars)
    {
        freevar_list_node * node = subfunc_value->freevars->tail;
        while (node != NULL)
        {
            freevar * freevar_value = node->value;
            if (freevar_value != NULL)
            {
                func_gencode_freevars_freevar(func_value, stab, freevar_value,
                                              result);
            }
            node = node->next;
        }
    }

    return 0;
}

int func_gencode_freevars_func_list(func * func_value, symtab * stab, func_list * list,
                                    int * result)
{
    func_list_node * node = list->tail;

    while (node != NULL)
    {
        func * subfunc_value = node->value;
        if (subfunc_value != NULL)
        {
            func_gencode_freevars_func(func_value, stab, subfunc_value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars(func * func_value, symtab * stab, int * result)
{
    if (func_value->body && func_value->body->binds)
    {
        func_gencode_freevars_bind_list(func_value, stab, func_value->body->binds,
                                        result);
    }
    if (func_value->body && func_value->body->funcs)
    {
        func_gencode_freevars_func_list(func_value, stab, func_value->body->funcs,
                                        result);
    }
    if (func_value->body && func_value->body->ret)
    {
        func_gencode_freevars_expr(func_value, stab, func_value->body->ret, result);
    }
    if (func_value->except)
    {
        func_gencode_freevars_func_except(func_value, stab, func_value->except, result);
    }

    return 0;
}

int func_gencode_freevars_expr_list(func * func_value, symtab * stab, expr_list * list,
                                    int * result)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            func_gencode_freevars_expr(func_value, stab, value, result);
        }
        node = node->next;
    }

    return 0;
}

int func_gencode_freevars_expr(func * func_value, symtab * stab, expr * value, int * result)
{
    switch (value->type)
    {
    case EXPR_INT:
    case EXPR_FLOAT:
    case EXPR_CHAR:
    case EXPR_STRING:
    case EXPR_ID:
    case EXPR_NIL:
        /* not possible */
        break;
    case EXPR_ENUMTYPE:
        expr_enumtype_check_call(func_value, stab, value, result);
        break;
    case EXPR_NEG:
        func_gencode_freevars_expr(func_value, stab, value->left, result);
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
        func_gencode_freevars_expr(func_value, stab, value->left, result);
        func_gencode_freevars_expr(func_value, stab, value->right, result);
        break;
    case EXPR_AND:
    case EXPR_OR:
        func_gencode_freevars_expr(func_value, stab, value->left, result);
        func_gencode_freevars_expr(func_value, stab, value->right, result);
        break;
    case EXPR_NOT:
        func_gencode_freevars_expr(func_value, stab, value->left, result);
        break;
    case EXPR_SUP:
        func_gencode_freevars_expr(func_value, stab, value->left, result);
        break;
    case EXPR_COND:
        func_gencode_freevars_expr(func_value, stab, value->left, result);
        func_gencode_freevars_expr(func_value, stab, value->middle, result);
        func_gencode_freevars_expr(func_value, stab, value->right, result);
        break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            func_gencode_freevars_array(func_value, stab, value->array.array_value, result);
        }
        break;
    case EXPR_ARRAY_DEREF:
        func_gencode_freevars_expr(func_value, stab, value->array_deref.array_expr,
                                   result);
        if (value->array_deref.ref != NULL)
        {
            func_gencode_freevars_expr_list(func_value, stab, value->array_deref.ref,
                                            result);
        }
        break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
        func_gencode_freevars_expr(func_value, stab, value->call.func_expr, result);
        if (value->call.params)
        {
            func_gencode_freevars_expr_list(func_value, stab, value->call.params,
                                            result);
        }
        break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_gencode_freevars_func(func_value, stab, value->func_value, result);
        }
        break;
    case EXPR_SEQ:
        if (value->seq.list != NULL)
        {
            func_gencode_freevars_expr_list(func_value, stab, value->seq.list,
                                            result);
        }
        break;
    case EXPR_ASS:
        func_gencode_freevars_expr(func_value, stab, value->left, result);
        func_gencode_freevars_expr(func_value, stab, value->right, result);
        break;
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
        func_gencode_freevars_expr(func_value, stab, value->whileloop.cond, result);
        func_gencode_freevars_expr(func_value, stab, value->whileloop.do_value, result);
        break;
    case EXPR_FOR:
        func_gencode_freevars_expr(func_value, stab, value->forloop.init, result);
        func_gencode_freevars_expr(func_value, stab, value->forloop.cond, result);
        func_gencode_freevars_expr(func_value, stab, value->forloop.incr, result);
        func_gencode_freevars_expr(func_value, stab, value->forloop.do_value, result);
        break;
    case EXPR_IFLET:
        func_gencode_freevars_iflet_expr(func_value, stab, value, result);
        break;
    case EXPR_MATCH:
        func_gencode_freevars_match_expr(func_value, stab, value, result);
        break;
    case EXPR_BUILD_IN:
        if (value->func_build_in.param != NULL)
        {
            func_gencode_freevars_expr_list(func_value, stab, value->func_build_in.param,
                                            result);
        }
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        func_gencode_freevars_expr(func_value, stab, value->left, result);
        break;
    case EXPR_LISTCOMP:
        if (value->listcomp_value != NULL)
        {
            func_gencode_freevars_listcomp(func_value, value->listcomp_value->stab,
                                           value->listcomp_value, result);
        }
        break;
    case EXPR_ATTR:
        if (value->attr.record_value != NULL)
        {
            func_gencode_freevars_expr(func_value, stab, value->attr.record_value, result);
        }
        break;
    }

    return 0;
}

