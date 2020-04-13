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
#include "forin.h"
#include "expr.h"
#include "symtab.h"
#include <stdlib.h>
#include <stdio.h>

forin * forin_new(char * id, expr * in_value, expr * do_value)
{
    forin * value = (forin *)malloc(sizeof(forin));

    value->iter = expr_new_id(id);
    value->in_value = in_value;
    value->do_value = do_value;
    value->stack_level = 0;
    value->line_no = 0;
    value->stab = NULL;

    return value;
}

void forin_delete(forin * value)
{
    if (value->iter != NULL)
    {
        expr_delete(value->iter);
    }
    if (value->in_value != NULL)
    {
        expr_delete(value->in_value);
    }
    if (value->do_value != NULL)
    {
        expr_delete(value->do_value);
    }
    if (value->stab != NULL)
    {
        symtab_delete(value->stab);
    }

    free(value);
}

void forin_print(forin * value)
{
    printf("forin %s %s\n", forin_type_str(value->type), value->iter->id.id);
}

char * forin_type_str(forin_type type)
{
    switch (type)
    {
        case FORIN_TYPE_ARRAY: return "forin array";
        case FORIN_TYPE_RANGE: return "forin range";
        case FORIN_TYPE_SLICE: return "forin slice";
        case FORIN_TYPE_STRING: return "forin string";
    }

    return "forin unknown";
}
