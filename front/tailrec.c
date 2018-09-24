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

int expr_id_tailrec(unsigned int syn_level, func * func_value,
                    expr * value, tailrec_op op, expr_list_weak * list_weak)
{
    symtab_entry * entry = NULL;

    entry = symtab_lookup(func_value->stab, value->id.id, SYMTAB_NESTED);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_FUNC && entry->func_value != NULL)
        {
            func * sup_func_value = entry->func_value;
            if (op == TAILREC_OP_ADD && syn_level - 1 == entry->syn_level && func_value == sup_func_value)
            {
                expr_list_weak_add(list_weak, value, 0);
                return 0;
            }
        }
    }
    return 0;
}

int expr_tailrec(unsigned int syn_level, func * func_value,
                 expr * value, tailrec_op op, expr_list_weak * list_weak)
{
    switch (value->type)
    {
    case EXPR_INT:
        /* no tailrec possible */
        break;
    case EXPR_FLOAT:
        /* no tailrec possible */
        break;
    case EXPR_ID:
        expr_id_tailrec(syn_level, func_value, value, op, list_weak);
        break;
    case EXPR_NEG:
    {
        expr_tailrec(syn_level, func_value, value->left, TAILREC_OP_SKIP, list_weak);
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
        expr_tailrec(syn_level, func_value, value->left, TAILREC_OP_SKIP, list_weak);
        expr_tailrec(syn_level, func_value, value->right, TAILREC_OP_SKIP, list_weak);
    }
    break;
    case EXPR_NOT:
    {
        expr_tailrec(syn_level, func_value, value->left, TAILREC_OP_SKIP, list_weak);
    }
    break;
    case EXPR_SUP:
        expr_tailrec(syn_level, func_value, value->left, op, list_weak);
    break;
    case EXPR_COND:
    {
        expr_tailrec(syn_level, func_value, value->left, TAILREC_OP_SKIP, list_weak);
        expr_tailrec(syn_level, func_value, value->middle, op, list_weak);
        expr_tailrec(syn_level, func_value, value->right, op, list_weak);
    }
    break;
    case EXPR_ARRAY:
        if (value->array.array_value != NULL)
        {
            array_tailrec(syn_level, func_value, value->array.array_value, 
                          TAILREC_OP_SKIP, list_weak);
        }
        break;
    case EXPR_ARRAY_DEREF:
    {
        if (value->array_deref.array_expr != NULL)
        {
            expr_tailrec(syn_level, func_value,
                         value->array_deref.array_expr, TAILREC_OP_SKIP, list_weak);
        }
        if (value->array_deref.ref != NULL)
        {
            expr_list_tailrec(syn_level, func_value,
                              value->array_deref.ref, TAILREC_OP_SKIP, list_weak);
        }
    }
    break;
    case EXPR_CALL:
    case EXPR_LAST_CALL:
    {
        expr_tailrec(syn_level, func_value, value->call.func_expr, op, list_weak);
        if (value->call.params != NULL)
        {
            expr_list_tailrec(syn_level, func_value, value->call.params, TAILREC_OP_SKIP, list_weak);
        }
    }
    break;
    case EXPR_FUNC:
        if (value->func_value)
        {
            func_tailrec(syn_level + 2, value->func_value);
        }
    break;
    case EXPR_SEQ:
        if (value->seq.list != NULL)
        {
            expr_seq_tailrec(syn_level, func_value, value->seq.list,
                             op, list_weak);
        }
        break;
    case EXPR_ASS:
    {
        expr_tailrec(syn_level, func_value, value->left, TAILREC_OP_SKIP, list_weak);
        expr_tailrec(syn_level, func_value, value->right, TAILREC_OP_SKIP, list_weak);
    }
    break;
    case EXPR_WHILE:
    case EXPR_DO_WHILE:
    {
        expr_tailrec(syn_level, func_value, value->whileloop.cond,
                     TAILREC_OP_SKIP, list_weak);
        expr_tailrec(syn_level, func_value,
                     value->whileloop.do_value, TAILREC_OP_SKIP, list_weak);
    }
    break;
    case EXPR_FOR:
    {
        expr_tailrec(syn_level, func_value, value->forloop.init,
                     TAILREC_OP_SKIP, list_weak);
        expr_tailrec(syn_level, func_value, value->forloop.cond,
                     TAILREC_OP_SKIP, list_weak);
        expr_tailrec(syn_level, func_value, value->forloop.incr,
                     TAILREC_OP_SKIP, list_weak);
        expr_tailrec(syn_level, func_value, value->forloop.do_value,
                     TAILREC_OP_SKIP, list_weak);
    }
    break;
    case EXPR_BUILD_IN:
        expr_list_tailrec(syn_level, func_value,
                          value->func_build_in.param, TAILREC_OP_SKIP, list_weak);
        break;
    case EXPR_INT_TO_FLOAT:
    case EXPR_FLOAT_TO_INT:
        expr_tailrec(syn_level, func_value, value->left, TAILREC_OP_SKIP, list_weak);
        break;
    }

    return 0;
}

int expr_list_tailrec(unsigned int syn_level, func * func_value,
                      expr_list * list, tailrec_op op, expr_list_weak * list_weak)
{
    expr_list_node * node = list->tail;
    while (node != NULL)
    {
        expr * value = node->value;
        if (value)
        {
            expr_tailrec(syn_level, func_value, value, TAILREC_OP_SKIP, list_weak);
        }
        node = node->next;
    }

    return 0;
}

int expr_seq_tailrec(unsigned int syn_level, func * func_value,
                     expr_list * list, tailrec_op op, expr_list_weak * list_weak)
{
    expr_list_node * last = NULL;
    expr_list_node * node = NULL;
    
    last = list->head;
    if (last != NULL)
    {
        expr * value = last->value;
        if (value != NULL)
        {
            expr_tailrec(syn_level, func_value, value, op, list_weak);
        }
    }
    
    node = list->tail;
    while (node != last)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            expr_tailrec(syn_level, func_value, value, TAILREC_OP_SKIP, list_weak);
        }
        node = node->next;
    }
    
    return 0;
}

int array_tailrec(unsigned int syn_level, func * func_value,
                  array * value, tailrec_op op, expr_list_weak * list_weak)
{
    if (value->type == ARRAY_INIT || value->type == ARRAY_SUB)
    {
        if (value->elements != NULL)
        {
            expr_list_tailrec(syn_level, func_value,
                              value->elements, TAILREC_OP_SKIP, list_weak);
        }
    }
    else if (value->type == ARRAY_DIMS)
    {
        if (value->dims != NULL)
        {
            expr_list_tailrec(syn_level, func_value,
                              value->dims, TAILREC_OP_SKIP, list_weak);
        }
    }

    return 0;
}

int bind_tailrec(unsigned int syn_level, func * func_value,
                          bind * value, tailrec_op op, expr_list_weak * list_weak)
{
    if (value->expr_value != NULL)
    {
        expr_tailrec(syn_level, func_value,
                     value->expr_value, op, list_weak);
    }
    
    return 0;
}

int bind_list_tailrec(unsigned int syn_level, func * func_value,
                      bind_list * list, tailrec_op op, expr_list_weak * list_weak)
{
    bind_list_node * node = list->tail;
    while (node != NULL)
    {
        bind * value = node->value;
        if (value != NULL)
        {
            bind_tailrec(syn_level, func_value, value, op, list_weak);
        }
        node = node->next;
    }

    return 0;
}

int except_tailrec(unsigned int syn_level, func * func_value,
                   except * value, tailrec_op op, expr_list_weak * list_weak)
{
    if (value->expr_value != NULL)
    {
        expr_tailrec(syn_level, func_value,
                     value->expr_value, op, list_weak);
    }

    return 0;
}

int except_list_tailrec(unsigned int syn_level, func * func_value,
                        except_list * list, tailrec_op op, expr_list_weak * list_weak)
{
    except_list_node * node = list->tail;
    while (node != NULL)
    {
        except * value = node->value;
        if (value != NULL)
        {
            except_tailrec(syn_level, func_value, value, op, list_weak);
        }
        node = node->next;
    }

    return 0;
}

int last_call_list_tailrec(expr_list_weak * list_weak)
{
    expr_list_weak_node * node = list_weak->tail;

    while (node != NULL)
    {
        expr * value = node->value;
        if (value != NULL)
        {
            assert(value->type == EXPR_CALL);
            value->type = EXPR_LAST_CALL;
        }
        node = node->next;
    }

    return 0;
}

int func_tailrec(unsigned int syn_level, func * value)
{
    expr_list_weak * list_weak = expr_list_weak_new();

    if (value->body != NULL && value->body->binds != NULL)
    {
        bind_list_tailrec(syn_level, value, value->body->binds, TAILREC_OP_SKIP, list_weak);
    }
    
    if (value->body != NULL && value->body->funcs != NULL)
    {
        func_list_tailrec(syn_level, value->body->funcs);
    }
    
    if (value->body != NULL && value->body->ret != NULL)
    {
        expr_tailrec(syn_level, value, value->body->ret, TAILREC_OP_ADD, list_weak);
    }
    if (value->except != NULL && value->except->list != NULL)
    {
        except_list_tailrec(syn_level, value, value->except->list, TAILREC_OP_SKIP, list_weak);
    }
    if (value->except != NULL && value->except->all != NULL)
    {
        except_tailrec(syn_level, value, value->except->all, TAILREC_OP_SKIP, list_weak);
    }
    
    last_call_list_tailrec(list_weak);
    expr_list_weak_delete(list_weak);

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

int never_tailrec(never * nev)
{
    unsigned int syn_level = 0;

    if (nev->funcs)
    {
        func_list_tailrec(syn_level, nev->funcs);
    }
    return 0;
}
