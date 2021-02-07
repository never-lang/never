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
#include "record.h"
#include "param.h"
#include "symtab.h"
#include <stdlib.h>
#include <assert.h>

record * record_new(char * id, param_list * params)
{
    record * value = (record *)malloc(sizeof(record));
    
    value->cycle = 0;
    value->id = id;
    value->params = params;
    value->stab = symtab_new(32, SYMTAB_TYPE_FUNC, NULL);
    value->line_no = 0;

    return value;
}

void record_delete(record * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    if (value->params != NULL)
    {
        param_list_delete(value->params);
    }
    if (value->stab != NULL)
    {
        symtab_delete(value->stab);
    }

    free(value);
}

param * record_find_param(record * record_value, char * id)
{
    param * ret = NULL;
    symtab_entry * entry = NULL;

    if (record_value->stab != NULL && id != NULL)
    {    
        entry = symtab_lookup(record_value->stab, id, SYMTAB_LOOKUP_FUNC);
    }

    if (entry != NULL && entry->type == SYMTAB_PARAM)
    {
        ret = entry->param_value;
    }
    else if (entry != NULL && entry->type != SYMTAB_PARAM)
    {
        assert(0);
    }

    return ret;
}
