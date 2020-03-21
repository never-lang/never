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
#include "iflet.h"
#include "match.h"
#include "matchbind.h"
#include "tcmatch.h"
#include "expr.h"
#include <stdlib.h>

iflet * iflet_item_new(match_guard_item * guard_item,
                       expr * expr_value, expr * then_value, expr * else_value)
{
    iflet * ret = (iflet *)malloc(sizeof(iflet));
    
    ret->type = IFLET_TYPE_ITEM;
    ret->guard_item = guard_item;
    ret->expr_value = expr_value;
    ret->then_value = then_value;
    ret->else_value = else_value;
    ret->line_no = 0;
    
    return ret;
}
                       
iflet * iflet_record_new(match_guard_record * guard_record,
                         expr * expr_value, expr * then_value, expr * else_value)
{
    iflet * ret = (iflet *)malloc(sizeof(iflet));
    
    ret->type = IFLET_TYPE_RECORD;
    ret->guard_record = guard_record;
    ret->expr_value = expr_value;
    ret->then_value = then_value;
    ret->else_value = else_value;
    ret->line_no = 0;
    
    return ret;
}                         

void iflet_delete(iflet * value)
{
    switch (value->type)
    {
        case IFLET_TYPE_ITEM:
        match_guard_item_delete(value->guard_item);
        break;
        case IFLET_TYPE_RECORD:
        match_guard_record_delete(value->guard_record);
        break;
    }

    if (value->expr_value != NULL)
    {
        expr_delete(value->expr_value);
    }
    if (value->then_value != NULL)
    {
        expr_delete(value->then_value);
    }
    if (value->else_value != NULL)
    {
        expr_delete(value->else_value);
    }

    free(value);
}


