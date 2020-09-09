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
#include "tcmatch.h"
#include "match.h"
#include "matchbind.h"
#include "typecheck.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int symtab_add_matchbind_from_matchbind(symtab * tab, matchbind * matchbind_value,
                                        param * param_value, enumerator * enumerator_value,
                                        enumtype * enumtype_value, unsigned int syn_level, int * result)
{
    symtab_entry * entry = symtab_lookup(tab, matchbind_value->id, SYMTAB_LOOKUP_LOCAL);
    if (entry == NULL)
    {
        matchbind_value->param_value = param_value;
        matchbind_value->enumerator_value = enumerator_value;
        matchbind_value->enumtype_value = enumtype_value;
        symtab_add_matchbind(tab, matchbind_value, syn_level);
    }
    else
    {
        *result = TYPECHECK_FAIL;
        symtab_entry_exists(entry, matchbind_value->line_no);
    }

    return 0;
}                                        

int symtab_add_matchbind_from_matchbind_list(symtab * tab, match_guard_record * guard_record,
                                             unsigned int syn_level, int * result)
{
    matchbind_list * matchbinds = guard_record->matchbinds;
    enumtype * enumtype_value = guard_record->enumtype_value;
    enumerator * enumerator_value = guard_record->enumerator_value;

    if (guard_record->stab == NULL)
    {
        guard_record->stab = symtab_new(8, SYMTAB_TYPE_FUNC, tab);
    }

    if (matchbinds == NULL || enumtype_value == NULL || enumerator_value == NULL)
    {
        return 0;
    }

    if ((enumerator_value->record_value == NULL ||
         enumerator_value->record_value->params == NULL) && matchbinds->count == 0)
    {
        return 0;
    }
    else if ((enumerator_value->record_value == NULL ||
              enumerator_value->record_value->params == NULL) && matchbinds->count > 0)
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(guard_record->line_no, "enum record %s.%s takes no params while guard has %d",
                        guard_record->enum_id,
                        guard_record->item_id,
                        matchbinds->count);
        return 0;
    }

    if (enumerator_value->record_value->params->count != matchbinds->count)
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(guard_record->line_no, "enum record %s.%s takes %d params while guard has %d",
                        guard_record->enum_id,
                        guard_record->item_id,
                        enumerator_value->record_value->params->count,
                        matchbinds->count);
        return 0;
    }

    matchbind_list_node * matchbind_node = matchbinds->tail;
    param_list_node * param_node = enumerator_value->record_value->params->tail;
    while (param_node != NULL || matchbind_node != NULL)
    {
        param * param_value = param_node->value;
        matchbind * matchbind_value = matchbind_node->value;
        if (param_value != NULL || matchbind_value != NULL)
        {
            symtab_add_matchbind_from_matchbind(guard_record->stab,
                                                matchbind_value,
                                                param_value,
                                                enumerator_value,
                                                enumtype_value,
                                                syn_level,
                                                result);
        }
        param_node = param_node->next;
        matchbind_node = matchbind_node->next;
    }
    
    return 0;
}

int expr_match_guard_item_check_type(symtab * tab, match_guard_item * guard_item,
                                     int * result)
{
    if (guard_item->module_id != NULL)
    {
        symtab_entry * mentry = NULL;
        mentry = symtab_lookup(tab, guard_item->module_id, SYMTAB_LOOKUP_GLOBAL);
        if (mentry != NULL &&
            mentry->type == SYMTAB_MODULE_DECL &&
            mentry->module_decl_value != NULL &&
            mentry->module_decl_value->nev != NULL)
        {
            tab = mentry->module_decl_value->nev->stab;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(guard_item->line_no, "cannot find module %s", guard_item->module_id);
        }
    }

    symtab_entry * entry = NULL;
    entry = symtab_lookup(tab, guard_item->enum_id, SYMTAB_LOOKUP_GLOBAL);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_ENUMTYPE && entry->enumtype_value != NULL)
        {
            enumerator * enumerator_value = enumtype_find_enumerator(entry->enumtype_value, guard_item->item_id);
            if (enumerator_value != NULL)
            {
                guard_item->enumtype_value = entry->enumtype_value;
                guard_item->enumerator_value = enumerator_value;
            }
            else
            {
                *result = TYPECHECK_FAIL;
                print_error_msg(guard_item->line_no, "cannot find enum item %s.%s",
                                guard_item->enum_id, guard_item->item_id);
            }
        }
        else
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(guard_item->line_no, "found %s instead of enum %s",
                            symtab_entry_type_str(entry->type),
                            guard_item->enum_id);
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(guard_item->line_no, "cannot find enum %s",
                        guard_item->enum_id);
    }

    return 0;
}                                     

int expr_match_guard_record_check_type(symtab * tab, match_guard_record * guard_record,
                                       int * result)
{
    if (guard_record->module_id != NULL)
    {
        symtab_entry * mentry = NULL;
        mentry = symtab_lookup(tab, guard_record->module_id, SYMTAB_LOOKUP_GLOBAL);
        if (mentry != NULL &&
            mentry->type == SYMTAB_MODULE_DECL &&
            mentry->module_decl_value != NULL &&
            mentry->module_decl_value->nev != NULL)
        {
            tab = mentry->module_decl_value->nev->stab;
        }
        else
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(guard_record->line_no, "cannot find module %s", guard_record->module_id);
        }
    }

    symtab_entry * entry = NULL;
    entry = symtab_lookup(tab, guard_record->enum_id, SYMTAB_LOOKUP_GLOBAL);
    if (entry != NULL)
    {
        if (entry->type == SYMTAB_ENUMTYPE && entry->enumtype_value != NULL)
        {
            enumerator * enumerator_value = enumtype_find_enumerator(entry->enumtype_value, guard_record->item_id);
            if (enumerator_value != NULL)
            {
                guard_record->enumtype_value = entry->enumtype_value;
                guard_record->enumerator_value = enumerator_value;
            }
            else
            {
                *result = TYPECHECK_FAIL;
                print_error_msg(guard_record->line_no, "cannot find enum item %s.%s",
                                guard_record->enum_id, guard_record->item_id);
            }
        }
        else
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(guard_record->line_no, "found %s instead of enum %s",
                            symtab_entry_type_str(entry->type),
                            guard_record->enum_id);
        }
    }
    else
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(guard_record->line_no, "cannot find enum %s",
                        guard_record->enum_id);
    }

    return 0;
}                                     

int expr_match_guard_check_type(symtab * tab, match_guard * match_value,
                                func * func_value, unsigned int syn_level,
                                int * result)
{
    switch (match_value->type)
    {
    case MATCH_GUARD_ITEM:
        expr_match_guard_item_check_type(tab, match_value->guard_item.guard, result);
        expr_check_type(tab, match_value->guard_item.expr_value, func_value, syn_level, result);
    break;
    case MATCH_GUARD_RECORD:
        expr_match_guard_record_check_type(tab, match_value->guard_record.guard, result);
        if (match_value->guard_record.guard->matchbinds != NULL)
        {
            enum_matchbind_list(match_value->guard_record.guard->matchbinds);
            symtab_add_matchbind_from_matchbind_list(tab, match_value->guard_record.guard, syn_level, result);
            expr_check_type(match_value->guard_record.guard->stab, match_value->guard_record.expr_value, func_value, syn_level, result);
        }
        else
        {
            expr_check_type(tab, match_value->guard_record.expr_value, func_value, syn_level, result);
        }
    break;
    case MATCH_GUARD_ELSE:
        expr_check_type(tab, match_value->guard_else.expr_value, func_value, syn_level, result);
    break;
    }

    return 0;
}

int expr_match_guard_list_check_type(symtab * tab, match_guard_list * list,
                                     func * func_value,
                                     unsigned int syn_level, int * result)
{
    match_guard_list_node * node = list->tail;

    while (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL)
        {
            expr_match_guard_check_type(tab, match_value, func_value, syn_level, result);
        }
        node = node->next;
    }

    return 0;
}                                     

int expr_match_guard_left_cmp(expr * value, match_guard * match_value, int * result)
{
    switch (match_value->type)
    {
    case MATCH_GUARD_ITEM:
        if (value->comb.comb_enumtype != match_value->guard_item.guard->enumtype_value)
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(match_value->line_no,
                            "enums are different %s and %s",
                            value->comb.comb_enumtype->id,
                            match_value->guard_item.guard->enum_id);
            return 0;
        }
    break;
    case MATCH_GUARD_RECORD:
        if (value->comb.comb_enumtype != match_value->guard_record.guard->enumtype_value)
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(match_value->line_no,
                            "enums are different %s and %s",
                            value->comb.comb_enumtype->id,
                            match_value->guard_record.guard->enum_id);
            return 0;
        }
    break;
    case MATCH_GUARD_ELSE:
    break;
    }
    
    return 0;
}

int expr_match_guard_list_left_cmp(expr * value, match_guard_list * list, int * result)
{
    if (value->comb.comb != COMB_TYPE_ENUMTYPE)
    {
        *result = TYPECHECK_FAIL;
        print_error_msg(value->line_no,
                        "match expression is %s not enum", comb_type_str(value->comb.comb));
        return 0;
    }

    match_guard_list_node * node = list->tail;
    while (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL)
        {
            expr_match_guard_left_cmp(value, match_value, result);
        }
        node = node->next;
    }

    return 0;
}

unsigned int expr_match_gaurd_list_last_cnt(match_guard_list * list)
{
    unsigned int cnt = 0;
    match_guard_list_node * node = list->tail;
    
    while (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL && match_value->type == MATCH_GUARD_ELSE)
        {
            cnt++; 
        }
        node = node->next;
    }
    
    return cnt;
}

int expr_match_guard_list_last_else(match_guard_list * list)
{
    match_guard_list_node * node = list->head;

    if (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL && match_value->type == MATCH_GUARD_ELSE)
        {
            return TYPECHECK_SUCC;
        }
    }

    return TYPECHECK_FAIL;
}

int expr_match_guard_mark_item(match_guard * match_value)
{
    switch (match_value->type)
    {
        case MATCH_GUARD_ITEM:
        {
            enumerator * enumerator_value = match_value->guard_item.guard->enumerator_value;
            if (enumerator_value != NULL)
            {
                /* not repeated values (warning) */
                if (enumerator_value->mark == 1)
                {
                    print_warning_msg(match_value->line_no,
                                      "repeated enum name %s.%s in match expression",
                                      match_value->guard_item.guard->enum_id,
                                      match_value->guard_item.guard->item_id);
                }
                enumerator_value->mark = 1;
            }
        }
        break;
        case MATCH_GUARD_RECORD:
        {
            enumerator * enumerator_value = match_value->guard_record.guard->enumerator_value;
            if (enumerator_value != NULL)
            {
                /* not repeated values (warning) */
                if (enumerator_value->mark == 1)
                {
                    print_warning_msg(match_value->line_no,
                                      "repeated enum name %s.%s in match expression",
                                      match_value->guard_record.guard->enum_id,
                                      match_value->guard_record.guard->item_id);
                }
                enumerator_value->mark = 1;
            }
        }
        break;
        case MATCH_GUARD_ELSE:
        break;
    }

    return 0;
}

int expr_match_guard_list_mark_items(expr * value, match_guard_list * list)
{
    match_guard_list_node * node = list->tail;

    while (node != NULL)
    {
        match_guard * match_value = node->value;
        if (match_value != NULL)
        {
            expr_match_guard_mark_item(match_value);
        }
        node = node->next;
    }

    return 0;
}

int expr_match_guard_unmark_items(expr * value)
{
    expr * match_expr = value->match.expr_value;

    if (match_expr != NULL && match_expr->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        enumtype * enum_value = match_expr->comb.comb_enumtype;
        if (enum_value != NULL && enum_value->enums != NULL)
        {
            enumerator_list_unmark(enum_value->enums);
        }
    }

    return 0;
}

int expr_match_guard_are_all_mark_items(expr * value, int * result)
{
    expr * match_expr = value->match.expr_value;

    if (match_expr != NULL && match_expr->comb.comb == COMB_TYPE_ENUMTYPE)
    {
        enumtype * enum_value = match_expr->comb.comb_enumtype;
        if (enum_value != NULL && enum_value->enums != NULL)
        {
            enumerator_list * list = enum_value->enums;
            if (list == NULL)
            {
                *result = TYPECHECK_SUCC;
                return 0;
            }

            enumerator_list_node * node = list->tail;
            while (node != NULL)
            {
                enumerator * enumerator_value = node->value;
                if (enumerator_value != NULL && enumerator_value->mark == 0)
                {
                    print_error_msg(value->line_no,
                                    "match expression does not cover %s.%s enum",
                                    enum_value->id, enumerator_value->id);
                    *result = TYPECHECK_FAIL;
                }
                node = node->next;
            }
        }
        else
        {
            *result = TYPECHECK_SUCC;
        }
    }
    else if (match_expr != NULL)
    {
        print_error_msg(value->line_no,
                        "match expression wrong type %s",
                        comb_type_str(match_expr->comb.comb));
        *result = TYPECHECK_FAIL;
    }

    return 0;
}

int expr_match_guard_list_exhaustive(expr * value, match_guard_list * list, int * result)
{
    unsigned int match_else_cnt = 0;
    
    match_else_cnt = expr_match_gaurd_list_last_cnt(list);
    if (match_else_cnt > 0)
    {
        /* not double else (warning) */
        if (match_else_cnt > 1)
        {
            print_warning_msg(value->line_no,
                              "match expression contains more than one else guard");
        }
        
        /* else as last guard present (warning) */
        if (expr_match_guard_list_last_else(list) == TYPECHECK_FAIL)
        {
            print_warning_msg(value->line_no,
                              "match expression else guard is not last");
        }
    }
    else
    {
        expr_match_guard_unmark_items(value);
    
        expr_match_guard_list_mark_items(value, list);
        
        expr_match_guard_are_all_mark_items(value, result);

        expr_match_guard_unmark_items(value);
    }

    return 0;
}

int expr_match_guard_list_right_cmp(expr * value, match_guard_list * list, int * result)
{
    match_guard_list_node * node = list->tail;

    if (node != NULL)
    {
        match_guard * first_guard = node->value;
        expr * first_expr = NULL;

        if (first_guard != NULL)
        {
            first_expr = match_guard_get_expr(first_guard);
            value->comb = first_expr->comb;
        }
        
        node = node->next;

        while (node != NULL && first_expr != NULL)
        {
            match_guard * second_guard = node->value;
            expr * second_expr = NULL;
            
            if (second_guard != NULL)
            {
                second_expr = match_guard_get_expr(second_guard);
            }
        
            if (second_expr != NULL)
            {
                expr_comb_cmp_and_set(first_expr, second_expr, value, result);
            }
        
            node = node->next;
        }
    }

    return 0;
}                                    

int expr_match_check_type(symtab * tab, expr * value, func * func_value,
                          unsigned int syn_level, int * result)
{
    expr_check_type(tab, value->match.expr_value, func_value, syn_level, result);

    /* expr_check is enum */
    expr_comb_is_enum(value->match.expr_value, result);

    /* initially expression type is of check expression */
    value->comb = value->match.expr_value->comb;
    
    if (value->match.match_guards != NULL)
    {
        expr_match_guard_list_check_type(tab, value->match.match_guards, func_value, syn_level, result);

        /* expr_check matches expr_match_guard_list type */
        expr_match_guard_list_left_cmp(value->match.expr_value, value->match.match_guards, result);

        /* exhaustive guards */
        expr_match_guard_list_exhaustive(value, value->match.match_guards, result);

        /* check if guard right sides are the same and set match type */
        expr_match_guard_list_right_cmp(value, value->match.match_guards, result);
    }

    if (*result == TYPECHECK_FAIL)
    {
        value->comb.comb = COMB_TYPE_ERR;
    }

    return 0;
}                              


