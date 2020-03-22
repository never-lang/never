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
#include "tciflet.h"
#include "symtab.h"
#include "expr.h"
#include "typecheck.h"
#include "match.h"
#include "tcmatch.h"
#include "utils.h"
#include <stdlib.h>

int iflet_guard_expr_cmp(iflet * iflet_value, int * result)
{
    expr * value = iflet_value->expr_value;

    switch (iflet_value->type)
    {
    case IFLET_TYPE_ITEM:
        if (value->comb.comb_enumtype != iflet_value->guard_item->enumtype_value)
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(iflet_value->line_no,
                            "enums are different %s and %s\n",
                            value->comb.comb_enumtype->id,
                            iflet_value->guard_item->enum_id);
            return 0;
        }
    break;
    case IFLET_TYPE_RECORD:
        if (value->comb.comb_enumtype != iflet_value->guard_record->enumtype_value)
        {
            *result = TYPECHECK_FAIL;
            print_error_msg(iflet_value->line_no,
                            "enums are different %s and %s\n",
                            value->comb.comb_enumtype->id,
                            iflet_value->guard_record->enum_id);
            return 0;
        }
    break;
    }
    
    return 0;
}

int iflet_check_type(symtab * tab, iflet * iflet_value, func * func_value,
                     unsigned int syn_level, int * result)
{
    expr_check_type(tab, iflet_value->expr_value, func_value, syn_level, result);
    expr_comb_is_enum(iflet_value->expr_value, result);

    switch (iflet_value->type)
    {
        case IFLET_TYPE_ITEM:
            expr_match_guard_item_check_type(tab, iflet_value->guard_item, result);

            expr_check_type(tab, iflet_value->then_value, func_value, syn_level, result);
            expr_check_type(tab, iflet_value->else_value, func_value, syn_level, result);
        break;
        case IFLET_TYPE_RECORD:
            expr_match_guard_record_check_type(tab, iflet_value->guard_record, result);
            
            if (iflet_value->guard_record->matchbinds != NULL)
            {
                enum_matchbind_list(iflet_value->guard_record->matchbinds);
                symtab_add_matchbind_from_matchbind_list(tab, iflet_value->guard_record, syn_level, result);
                expr_check_type(iflet_value->guard_record->stab, iflet_value->then_value, func_value, syn_level, result);
            }
            else
            {
                expr_check_type(tab, iflet_value->then_value, func_value, syn_level, result);
            }
            expr_check_type(tab, iflet_value->else_value, func_value, syn_level, result);
        break;
    }

    if (*result == TYPECHECK_SUCC)
    {
        iflet_guard_expr_cmp(iflet_value, result);
    }

    return 0;
}                          

int expr_iflet_check_type(symtab * tab, expr * value, func * func_value,
                          unsigned int syn_level, int * result)
{
    if (value->iflet_value != NULL)
    {
        iflet_check_type(tab, value->iflet_value, func_value, syn_level, result);

        expr_comb_cmp_and_set(value->iflet_value->then_value, value->iflet_value->else_value,
                              value, result);    

        if (*result == TYPECHECK_FAIL)
        {
            value->comb.comb = COMB_TYPE_ERR;
        }
    }

    return 0;
}

