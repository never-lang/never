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
#include "tcforin.h"
#include "typecheck.h"
#include "forin.h"
#include "symtab.h"
#include "expr.h"
#include "func.h"
#include "utils.h"
#include <stdlib.h>

int symtab_add_param_from_forin(symtab * tab, forin * value,
                               unsigned int syn_level, int * result)
{
    switch (value->in_value->comb.comb)
    {
        case COMB_TYPE_ARRAY:
            value->type = FORIN_TYPE_ARRAY;
            expr_set_comb_type(value->iter, value->in_value->comb.array.comb_ret);            

            symtab_add_forin(tab, value, syn_level);
        break;
        case COMB_TYPE_RANGE:
            value->type = FORIN_TYPE_RANGE;
            expr_set_comb_type(value->iter, value->in_value->comb.range.comb_ret);

            symtab_add_forin(tab, value, syn_level);
        break;
        case COMB_TYPE_SLICE:
            value->type = FORIN_TYPE_SLICE;
            expr_set_comb_type(value->iter, value->in_value->comb.slice.comb_ret);

            symtab_add_forin(tab, value, syn_level);
        break;
        case COMB_TYPE_STRING:
            value->type = FORIN_TYPE_STRING;
            value->iter->comb.comb = COMB_TYPE_CHAR;

            symtab_add_forin(tab, value, syn_level);
        break;
        case COMB_TYPE_UNKNOWN:
        case COMB_TYPE_ERR:
        case COMB_TYPE_NIL:
        case COMB_TYPE_BOOL:
        case COMB_TYPE_INT:
        case COMB_TYPE_LONG:
        case COMB_TYPE_FLOAT:
        case COMB_TYPE_DOUBLE:
        case COMB_TYPE_CHAR:
        case COMB_TYPE_VOID:
        case COMB_TYPE_C_PTR:
        case COMB_TYPE_FUNC:
        case COMB_TYPE_ENUMTYPE:
        case COMB_TYPE_ENUMTYPE_ID:
        case COMB_TYPE_RECORD:
        case COMB_TYPE_RECORD_ID:
        case COMB_TYPE_MODULE:
            print_error_msg(value->in_value->line_no,
                            "for in loop expression incorrect type %s",
                            comb_type_str(value->in_value->comb.comb));
        break;
    }

    return 0;
}                               

int expr_forin_check_type(symtab * tab, expr * value, func * func_value,
                          unsigned int syn_level, int * result)
{
    expr_check_type(tab, value->forin_value->in_value, func_value, syn_level, result);

    if ((value->forin_value->in_value->comb.comb == COMB_TYPE_ARRAY &&
            value->forin_value->in_value->comb.array.comb_dims == 1) ||
        (value->forin_value->in_value->comb.comb == COMB_TYPE_RANGE &&
            value->forin_value->in_value->comb.range.comb_dims == 1) ||
        (value->forin_value->in_value->comb.comb == COMB_TYPE_SLICE &&
            value->forin_value->in_value->comb.slice.comb_dims == 1))
    {
        value->comb.comb = COMB_TYPE_INT;
    }
    else
    {
        *result = TYPECHECK_FAIL;
        value->comb.comb = COMB_TYPE_ERR;
        print_error_msg(value->line_no,
                        "for in loop expression is not of one dimensional array, slice or range is %s",
                        comb_type_str(value->forin_value->in_value->comb.comb));
    }

    if (value->forin_value->stab == NULL)
    {
        value->forin_value->stab = symtab_new(8, SYMTAB_TYPE_BLOCK, tab);
    }

    symtab_add_param_from_forin(value->forin_value->stab, value->forin_value, syn_level, result);

    expr_check_type(value->forin_value->stab, value->forin_value->do_value, func_value, syn_level, result);

    return 0;
}                          



