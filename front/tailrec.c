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
#include <assert.h>
#include <stdio.h>

tailrec_type expr_id_tailrec(unsigned int syn_level, func * func_value, expr * value)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(func_value->stab, value->id.id, SYMTAB_NESTED);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value != NULL)
        {
            func * sup_func_value = entry->func_value;
            if (syn_level - 1 == entry->syn_level && func_value == sup_func_value)
            {
                return TAILREC_FOUND;
            }
        }
    }

    return TAILREC_NOT_FOUND;
}

tailrec_type expr_tailrec(unsigned int syn_level, func * func_value,
                          expr * value, expr ** last_call)
{
    tailrec_type rec = TAILREC_NOT_FOUND;

    switch (value->type)
    {
    case EXPR_INT:
        /* no tailrec possible */
        break;
    case EXPR_FLOAT:
        /* no tailrec possible */
        break;
    case EXPR_ID:
        rec = expr_id_tailrec(syn_level, func_value, value);
        break;
    case EXPR_NEG:
    {
        tailrec_type rec_left;

        rec_left = expr_tailrec(syn_level, func_value, value->left, last_call);
        if (rec_left == TAILREC_FOUND)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else if (rec_left == TAILREC_NOT_FOUND)
        {
            rec = TAILREC_NOT_FOUND;
        }
        else if (rec_left == TAILREC_NOT_POSSIBLE)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
    }
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
    {
        tailrec_type rec_left;
        tailrec_type rec_right;

        rec_left = expr_tailrec(syn_level, func_value, value->left, last_call);
        rec_right = expr_tailrec(syn_level, func_value, value->right, last_call);

        if (rec_left == TAILREC_FOUND || rec_right == TAILREC_FOUND)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else if (rec_left == TAILREC_NOT_POSSIBLE ||
                 rec_right == TAILREC_NOT_POSSIBLE)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else
        {
            rec = TAILREC_NOT_FOUND;
        }
    }
    break;
    case EXPR_NOT:
    {
        tailrec_type rec_left;

        rec_left = expr_tailrec(syn_level, func_value, value->left, last_call);
        if (rec_left == TAILREC_FOUND)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else if (rec_left == TAILREC_NOT_FOUND)
        {
            rec = TAILREC_NOT_FOUND;
        }
        else if (rec_left == TAILREC_NOT_POSSIBLE)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
    }
    break;
    case EXPR_SUP:
        rec = expr_tailrec(syn_level, func_value, value->left, last_call);
        break;
    case EXPR_COND:
    {
        tailrec_type rec_left;
        tailrec_type rec_middle;
        tailrec_type rec_right;

        rec_left = expr_tailrec(syn_level, func_value, value->left, last_call);
        rec_middle = expr_tailrec(syn_level, func_value, value->middle, last_call);
        rec_right = expr_tailrec(syn_level, func_value, value->right, last_call);

        if (rec_left == TAILREC_NOT_POSSIBLE ||
            rec_middle == TAILREC_NOT_POSSIBLE ||
            rec_right == TAILREC_NOT_POSSIBLE)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else if (rec_middle == TAILREC_FOUND && rec_right == TAILREC_FOUND)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else if (rec_middle == TAILREC_FOUND && rec_right == TAILREC_NOT_FOUND)
        {
            rec = TAILREC_FOUND;
        }
        else if (rec_middle == TAILREC_NOT_FOUND && rec_right == TAILREC_FOUND)
        {
            rec = TAILREC_FOUND;
        }
        else
        {
            rec = TAILREC_NOT_FOUND;
        }
    }
    break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            rec = array_tailrec(syn_level, func_value, value->array.array_value, 
                                last_call);
        }
        break;
    case EXPR_ARRAY_DEREF:
    {
        tailrec_type rec_array_expr = TAILREC_NOT_FOUND;
        tailrec_type rec_array_ref = TAILREC_NOT_FOUND;

        if (value->array_deref.array_expr != NULL)
        {
            rec_array_expr = expr_tailrec(syn_level, func_value,
                                          value->array_deref.array_expr, last_call);
        }
        if (value->array_deref.ref != NULL)
        {
            rec_array_ref = expr_list_tailrec(syn_level, func_value,
                                              value->array_deref.ref, last_call);
        }

        if (rec_array_expr == TAILREC_NOT_FOUND &&
            rec_array_ref == TAILREC_NOT_FOUND)
        {
            rec = TAILREC_NOT_FOUND;
        }
        else
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
    }
    break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
    {
        tailrec_type rec_expr = TAILREC_NOT_FOUND;
        tailrec_type rec_vars = TAILREC_NOT_FOUND;

        rec_expr =
            expr_tailrec(syn_level, func_value, value->call.func_expr, last_call);
        if (value->call.params != NULL)
        {
            rec_vars =
                expr_list_tailrec(syn_level, func_value, value->call.params,
                                  last_call);
        }

        if (rec_expr == TAILREC_FOUND && rec_vars == TAILREC_NOT_FOUND)
        {
            *last_call = value;
            rec = TAILREC_FOUND;
        }
        else if (rec_expr == TAILREC_NOT_FOUND && rec_vars == TAILREC_NOT_FOUND)
        {
            rec = TAILREC_NOT_FOUND;
        }
        else
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
    }
    break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            expr * last_call = NULL;
            
            rec = func_tailrec(syn_level + 2, value->func_value, &last_call);
            if (rec == TAILREC_FOUND)
            {
                assert(last_call);
                last_call->type = EXPR_LAST_CALL;
            }
        }
        rec = TAILREC_NOT_FOUND;
        break;
    case EXPR_SEQ:
        if (value->seq.list != NULL)
        {
            rec = expr_seq_tailrec(syn_level, func_value, value->seq.list,
                                   last_call);
        }
        break;
    case EXPR_ASS:
    {
        tailrec_type rec_left;
        tailrec_type rec_right;

        rec_left = expr_tailrec(syn_level, func_value, value->left, last_call);
        rec_right = expr_tailrec(syn_level, func_value, value->right, last_call);

        if (rec_left == TAILREC_FOUND || rec_right == TAILREC_FOUND)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else if (rec_left == TAILREC_NOT_POSSIBLE ||
                 rec_right == TAILREC_NOT_POSSIBLE)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else
        {
            rec = TAILREC_NOT_FOUND;
        }
    }
    break;
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
    {
        tailrec_type rec_cond;
        tailrec_type rec_do;

        rec_cond = expr_tailrec(syn_level, func_value, value->whileloop.cond,
                                last_call);
        rec_do = expr_tailrec(syn_level, func_value,
                              value->whileloop.do_value, last_call);

        if (rec_cond == TAILREC_FOUND || rec_do == TAILREC_FOUND)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else if (rec_cond == TAILREC_NOT_POSSIBLE ||
                 rec_do == TAILREC_NOT_POSSIBLE)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
        else
        {
            rec = TAILREC_NOT_FOUND;
        }
    }
    break;
    case EXPR_BUILD_IN:
        rec = expr_list_tailrec(syn_level, func_value,
                                value->func_build_in.param, last_call);
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        rec = expr_tailrec(syn_level, func_value, value->left, last_call);
        break;
    }
    return rec;
}

tailrec_type expr_list_tailrec(unsigned int syn_level, func * func_value,
                               expr_list * list, expr ** last_call)
{
    tailrec_type rec = TAILREC_NOT_FOUND;

    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            tailrec_type rec_expr;

            rec_expr = expr_tailrec(syn_level, func_value, value, last_call);
            if (rec_expr == TAILREC_FOUND || rec_expr == TAILREC_NOT_POSSIBLE)
            {
                rec = TAILREC_NOT_POSSIBLE;
            }
        }
        node = node->next;
    }

    return rec;
}

tailrec_type expr_seq_tailrec(unsigned int syn_level, func * func_value,
                              expr_list * list, expr ** last_call)
{
    expr_list_node * last = NULL;
    expr_list_node * node = NULL;
    tailrec_type rec = TAILREC_NOT_FOUND;
    
    last = list->head;
    if (last != NULL)
    {
        expr * value = last->value;
        if (value != NULL)
        {
            rec = expr_tailrec(syn_level, func_value, value, last_call);
        }
    }
    
    node = list->tail;
    while (node != last)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            tailrec_type rec_expr;
            
            rec_expr = expr_tailrec(syn_level, func_value, value, last_call);
            if (rec_expr == TAILREC_FOUND || rec_expr == TAILREC_NOT_POSSIBLE)
            {
                rec = TAILREC_NOT_POSSIBLE;
            }
        }
        node = node->next;
    }
    
    return rec;
}

tailrec_type array_tailrec(unsigned int syn_level, func * func_value,
                           array * value, expr ** last_call)
{
    tailrec_type rec = TAILREC_NOT_FOUND;
    tailrec_type rec_array_expr = TAILREC_NOT_FOUND;

    if (value->type == ARRAY_INIT || value->type == ARRAY_SUB)
    {
        if (value->elements != NULL)
        {
            rec_array_expr =
                expr_list_tailrec(syn_level, func_value,
                                  value->elements, last_call);
        }
    }
    else if (value->type == ARRAY_DIMS)
    {
        if (value->dims != NULL)
        {
            rec_array_expr =
                expr_list_tailrec(syn_level, func_value,
                                  value->dims, last_call);
        }
    }

    if (rec_array_expr == TAILREC_NOT_FOUND)
    {
        rec = TAILREC_NOT_FOUND;
    }
    else
    {
        rec = TAILREC_NOT_POSSIBLE;
    }

    return rec;
}

tailrec_type bind_tailrec(unsigned int syn_level, func * func_value,
                          bind * value, expr ** last_call)
{
    tailrec_type rec = TAILREC_NOT_FOUND;

    if (value->expr_value != NULL)
    {
        tailrec_type rec_expr = TAILREC_NOT_FOUND;

        rec_expr = expr_tailrec(syn_level, func_value,
                                value->expr_value, last_call);
        if (rec_expr == TAILREC_FOUND || rec_expr == TAILREC_NOT_POSSIBLE)
        {
            rec = TAILREC_NOT_POSSIBLE;
        }
    }
    
    return rec;
}

tailrec_type bind_list_tailrec(unsigned int syn_level, func * func_value,
                               bind_list * list, expr ** last_call)
{
    tailrec_type rec = TAILREC_NOT_FOUND;

    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * value = node->value;
        if (value != NULL)
        {
            tailrec_type rec_expr;

            rec_expr = bind_tailrec(syn_level, func_value, value, last_call);
            if (rec_expr == TAILREC_FOUND || rec_expr == TAILREC_NOT_POSSIBLE)
            {
                rec = TAILREC_NOT_POSSIBLE;
            }
        }
        node = node->next;
    }

    return rec;
}

tailrec_type func_tailrec(unsigned int syn_level, func * value,
                          expr ** last_call)
{
    tailrec_type rec = TAILREC_NOT_FOUND;

    if (value->body != NULL && value->body->binds != NULL)
    {
        rec = bind_list_tailrec(syn_level, value, value->body->binds, last_call);
    }
    if (value->body != NULL && value->body->funcs != NULL)
    {
        rec = func_list_tailrec(syn_level, value->body->funcs);
    }
    if (value->body && value->body->ret)
    {
        rec = expr_tailrec(syn_level, value, value->body->ret, last_call);
    }

    return rec;
}

int func_list_tailrec(unsigned int syn_level, func_list * list)
{
    func_list_node * node = list->tail;
    while (node != NULL)
    {
        func * value = node->value;
        if (value != NULL)
        {
            expr * last_call = NULL;
            tailrec_type rec = TAILREC_NOT_FOUND;

            rec = func_tailrec(syn_level + 1, value, &last_call);
            if (rec == TAILREC_FOUND)
            {
                assert(last_call);
                last_call->type = EXPR_LAST_CALL;
            }
        }
        node = node->next;
    }
    return 0;
}

int never_tailrec(never * nev)
{
    unsigned int syn_level = 0;

    if (nev->funcs)
    {
        func_list_tailrec(syn_level, nev->funcs);
    }
    return 0;
}
