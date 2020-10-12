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
#include "tailrec.h"
#include "symtab.h"
#include "iflet.h"
#include "match.h"
#include "forin.h"
#include <assert.h>
#include <stdio.h>

int expr_id_tailrec(unsigned int syn_level, symtab * stab,
                    expr * value, tailrec_op op)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(stab, value->id.id, SYMTAB_LOOKUP_LOCAL);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value != NULL)
        {
            if (op == TAILREC_OP_ADD &&
                syn_level - 1 == entry->syn_level)
            {
                return 1;
            }
        }
    }
    return 0;
}

int expr_match_guard_tailrec(unsigned int syn_level, symtab * stab,
                             match_guard * match_value, tailrec_op op)
{
    switch (match_value->type)
    {
        case MATCH_GUARD_ITEM:
            expr_tailrec(syn_level, stab, match_value->guard_item.expr_value, op);
        break;
        case MATCH_GUARD_RECORD:
            expr_tailrec(syn_level, stab, match_value->guard_record.expr_value, op);
        break;
        case MATCH_GUARD_ELSE:
            expr_tailrec(syn_level, stab, match_value->guard_else.expr_value, op);
        break;
    }
    
    return 0;
}

int expr_match_guard_list_tailrec(unsigned int syn_level, symtab * stab,
                                  match_guard_list * list, tailrec_op op)
{
    match_guard_list_node * node = list->tail;

    while (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL)
        {
            expr_match_guard_tailrec(syn_level, stab, match_value, op);
        }
        node = node->next;
    }

    return 0;
}

int expr_match_tailrec(unsigned int syn_level, symtab * stab,
                       expr * value, tailrec_op op)
{
    expr_tailrec(syn_level, stab, value->match.expr_value, op);
    if (value->match.match_guards != NULL)
    {
        expr_match_guard_list_tailrec(syn_level, stab, value->match.match_guards, op);
    }

    return 0;
}        

int expr_tailrec(unsigned int syn_level, symtab * stab,
                 expr * value, tailrec_op op)
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
    case EXPR_ENUMTYPE:
    case EXPR_NIL:
    case EXPR_C_NULL:
        /* no tailrec possible */
        break;
    case EXPR_ID:
        expr_id_tailrec(syn_level, stab, value, op);
        break;
    case EXPR_NEG:
        expr_tailrec(syn_level, stab, value->left, TAILREC_OP_SKIP);
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
    case EXPR_BIN_AND:
    case EXPR_BIN_OR:
    case EXPR_BIN_XOR:
    case EXPR_BIN_SHL:
    case EXPR_BIN_SHR:
        expr_tailrec(syn_level, stab, value->left, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->right, TAILREC_OP_SKIP);
    break;
    case EXPR_PIPEL:
        expr_tailrec(syn_level, stab, value->left, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->right, op);
    break;
    case EXPR_NOT:
    case EXPR_BIN_NOT:
        expr_tailrec(syn_level, stab, value->left, TAILREC_OP_SKIP);
    break;
    case EXPR_SUP:
        expr_tailrec(syn_level, stab, value->left, op);
    break;
    case EXPR_COND:
        expr_tailrec(syn_level, stab, value->left, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->middle, op);
        expr_tailrec(syn_level, stab, value->right, op);
    break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            array_tailrec(syn_level, stab, value->array.array_value, 
                          TAILREC_OP_SKIP);
        }
    break;
    case EXPR_ARRAY_DEREF:
        if (value->array_deref.array_expr != NULL)
        {
            expr_tailrec(syn_level, stab,
                         value->array_deref.array_expr, TAILREC_OP_SKIP);
        }
        if (value->array_deref.ref != NULL)
        {
            expr_list_tailrec(syn_level, stab,
                              value->array_deref.ref, TAILREC_OP_SKIP);
        }
    break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
    {
        expr_tailrec(syn_level, stab, value->call.func_expr, op);

        /* found tail recursion */        
        if (value->call.func_expr->type == EXPR_ID &&
            expr_id_tailrec(syn_level, stab, value->call.func_expr, op))
        {
            value->type = EXPR_LAST_CALL;
            
            /* printf("tailrec %s\n", value->call.func_expr->id.id); */
        }
        
        if (value->call.params != NULL)
        {
            expr_list_tailrec(syn_level, stab, value->call.params, TAILREC_OP_SKIP);
        }
    }
    break;
    case EXPR_FUNC:
        if (value->func_value != NULL)
        {
            func_tailrec(syn_level + 2, value->func_value);
        }
    break;
    case EXPR_RANGE_DIM:
        if (value->range_dim.from != NULL)
        {
            expr_tailrec(syn_level, stab, value->range_dim.from, TAILREC_OP_SKIP);
        }
        if (value->range_dim.to != NULL)
        {
            expr_tailrec(syn_level, stab, value->range_dim.to, TAILREC_OP_SKIP);
        }
    break;
    case EXPR_RANGE:
        if (value->range.range_dims != NULL)
        {
            expr_list_tailrec(syn_level, stab, value->range.range_dims, TAILREC_OP_SKIP);
        }
    break;
    case EXPR_SLICE:
        if (value->slice.array_expr != NULL)
        {
            expr_tailrec(syn_level, stab, value->slice.array_expr, TAILREC_OP_SKIP);
        }
        if (value->slice.range_dims != NULL)
        {
            expr_list_tailrec(syn_level, stab, value->slice.range_dims, TAILREC_OP_SKIP);
        }
    break;
    case EXPR_SEQ:
        if (value->seq.list != NULL)
        {
            expr_seq_tailrec(syn_level, stab, value->seq.list, op);
        }
    break;
    case EXPR_ASS:
        expr_tailrec(syn_level, stab, value->left, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->right, TAILREC_OP_SKIP);
    break;
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
        expr_tailrec(syn_level, stab, value->whileloop.cond, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->whileloop.do_value, TAILREC_OP_SKIP);
    break;
    case EXPR_FOR:
        expr_tailrec(syn_level, stab, value->forloop.init, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->forloop.cond, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->forloop.incr, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->forloop.do_value, TAILREC_OP_SKIP);
    break;
    case EXPR_FORIN:
        expr_tailrec(syn_level, stab, value->forin_value->in_value, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->forin_value->do_value, TAILREC_OP_SKIP);
    break;
    case EXPR_IFLET:
        expr_tailrec(syn_level, stab, value->iflet_value->expr_value, TAILREC_OP_SKIP);
        expr_tailrec(syn_level, stab, value->iflet_value->then_value, op);
        expr_tailrec(syn_level, stab, value->iflet_value->else_value, op);
    break;
    case EXPR_MATCH:
        expr_match_tailrec(syn_level, stab, value, op);
    break;
    case EXPR_BUILD_IN:
        if (value->func_build_in.param != NULL)
        {
            expr_list_tailrec(syn_level, stab,
                              value->func_build_in.param, TAILREC_OP_SKIP);
        }
    break;
    case EXPR_CONV:
        expr_tailrec(syn_level, stab, value->conv.expr_value, TAILREC_OP_SKIP);
    break;
    case EXPR_LISTCOMP:
        if (value->listcomp_value != NULL)
        {
            listcomp_tailrec(syn_level, value->listcomp_value->stab,
                             value->listcomp_value, TAILREC_OP_SKIP);
        }
        break;
    case EXPR_ATTR:
        if (value->attr.record_value != NULL)
        {
            expr_tailrec(syn_level, stab, value->attr.record_value, TAILREC_OP_SKIP);
        }
        break;
    }

    return 0;
}

int expr_list_tailrec(unsigned int syn_level, symtab * stab,
                      expr_list * list, tailrec_op op)
{
    expr_list_node * node = list->tail;

    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            expr_tailrec(syn_level, stab, value, TAILREC_OP_SKIP);
        }
        node = node->next;
    }
    return 0;
}

int expr_seq_tailrec(unsigned int syn_level, symtab * stab,
                     expr_list * list, tailrec_op op)
{
    expr_list_node * last = NULL;
    expr_list_node * node = NULL;
    
    last = list->head;
    if (last != NULL)
    {
        expr * value = last->value;
        if (value != NULL)
        {
            expr_tailrec(syn_level, stab, value, op);
        }
    }
    
    node = list->tail;
    while (node != last)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_tailrec(syn_level, stab, value, TAILREC_OP_SKIP);
        }
        node = node->next;
    }
    return 0;
}

int qualifier_tailrec(unsigned int syn_level, symtab * stab,
                      qualifier * value, tailrec_op op)
{
    switch (value->type)
    {
        case QUALIFIER_UNKNOWN:
            assert(0);
        break;
        case QUALIFIER_GENERATOR:
            if (value->expr_value != NULL)
            {
                expr_tailrec(syn_level, stab, value->expr_value, op);
            }
        break;
        case QUALIFIER_FILTER:
            if (value->expr_value != NULL)
            {
                expr_tailrec(syn_level, stab, value->expr_value, op);
            }
        break;
    }

    return 0;
}

int qualifier_list_tailrec(unsigned int syn_level, symtab * stab,
                           qualifier_list * list, tailrec_op op)
{
    qualifier_list_node * node = list->tail;

    while (node != NULL)
    {
        qualifier * qualifier_value = node->value;
        if (qualifier_value != NULL)
        {
            qualifier_tailrec(syn_level, stab, qualifier_value, op);
        }
        node = node->next;
    }

    return 0;
}

int listcomp_tailrec(unsigned int syn_level, symtab * stab,
                     listcomp * value, tailrec_op op)
{
    if (value->list != NULL)
    {
        qualifier_list_tailrec(syn_level, stab, value->list, TAILREC_OP_SKIP);
    }
    if (value->expr_value != NULL)
    {
        expr_tailrec(syn_level, stab, value->expr_value, TAILREC_OP_SKIP);
    }

    return 0;
}

int array_tailrec(unsigned int syn_level, symtab * stab,
                  array * value, tailrec_op op)
{
    if (value->type == ARRAY_INIT || value->type == ARRAY_SUB)
    {
        if (value->elements != NULL)
        {
            expr_list_tailrec(syn_level, stab,
                              value->elements, TAILREC_OP_SKIP);
        }
    }
    else if (value->type == ARRAY_DIMS)
    {
        if (value->dims != NULL)
        {
            expr_list_tailrec(syn_level, stab,
                              value->dims, TAILREC_OP_SKIP);
        }
    }
    return 0;
}

int bind_tailrec(unsigned int syn_level, symtab * stab,
                          bind * value, tailrec_op op)
{
    if (value->expr_value != NULL)
    {
        expr_tailrec(syn_level, stab, value->expr_value, op);
    }
    return 0;
}

int bind_list_tailrec(unsigned int syn_level, symtab * stab,
                      bind_list * list, tailrec_op op)
{
    bind_list_node * node = list->tail;

    while (node != NULL)
    {
        bind * value = node->value;
        if (value != NULL)
        {
            bind_tailrec(syn_level, stab, value, op);
        }
        node = node->next;
    }
    return 0;
}

int except_tailrec(unsigned int syn_level, symtab * stab,
                   except * value, tailrec_op op)
{
    if (value->expr_value != NULL)
    {
        expr_tailrec(syn_level, stab, value->expr_value, op);
    }
    return 0;
}

int except_list_tailrec(unsigned int syn_level, symtab * stab,
                        except_list * list, tailrec_op op)
{
    except_list_node * node = list->tail;

    while (node != NULL)
    {
        except * value = node->value;
        if (value != NULL)
        {
            except_tailrec(syn_level, stab, value, op);
        }
        node = node->next;
    }
    return 0;
}

int func_tailrec_ffi(unsigned int syn_level, func * value)
{
    return 0;
}

int func_tailrec_native(unsigned int syn_level, func * value)
{
    if (value->body != NULL && value->body->binds != NULL)
    {
        bind_list_tailrec(syn_level, value->stab, value->body->binds, TAILREC_OP_SKIP);
    }
 
    if (value->body != NULL && value->body->funcs != NULL)
    {
        func_list_tailrec(syn_level, value->body->funcs);
    }
    
    if (value->body != NULL && value->body->ret != NULL)
    {
        expr_tailrec(syn_level, value->stab, value->body->ret, TAILREC_OP_ADD);
    }
    if (value->except != NULL && value->except->list != NULL)
    {
        except_list_tailrec(syn_level, value->stab, value->except->list, TAILREC_OP_SKIP);
    }
    if (value->except != NULL && value->except->all != NULL)
    {
        except_tailrec(syn_level, value->stab, value->except->all, TAILREC_OP_SKIP);
    }
    return 0;
}

int func_tailrec(unsigned int syn_level, func * value)
{
    switch (value->type)
    {
        case FUNC_TYPE_UNKNOWN:
            assert(0);
        break;
        case FUNC_TYPE_NATIVE:
            func_tailrec_native(syn_level, value);
        break;
        case FUNC_TYPE_FFI:
            func_tailrec_ffi(syn_level, value);
        break;
    }

    return 0;
}

int func_list_tailrec(unsigned int syn_level, func_list * list)
{
    func_list_node * node = list->tail;

    while (node != NULL)
    {
        func * value = node->value;
        if (value != NULL)
        {
            func_tailrec(syn_level + 1, value);
        }
        node = node->next;
    }
    return 0;
}

int use_tailrec(use * value)
{
    if (value->decl != NULL &&
        value->decl->type == MODULE_DECL_TYPE_MOD)
    {
        module_decl_tailrec(value->decl);
    }

    return 0;
}

int use_list_tailrec(use_list * list)
{
    use_list_node * node = list->tail;

    while (node != NULL)
    {
        use * value = node->value;
        if (value != NULL)
        {
            use_tailrec(value);
        }
        node = node->next;
    }

    return 0;
}

int never_tailrec(never * nev)
{
    unsigned int syn_level = 0;

    if (nev->uses)
    {
        use_list_tailrec(nev->uses);
    }
    if (nev->binds)
    {
        bind_list_tailrec(syn_level, nev->stab, nev->binds, TAILREC_OP_SKIP);
    }
    if (nev->funcs)
    {
        func_list_tailrec(syn_level, nev->funcs);
    }
    return 0;
}

int module_decl_tailrec(module_decl * module_nev)
{
    if (module_nev->nev)
    {
        return never_tailrec(module_nev->nev);
    }

    return 0;
}
