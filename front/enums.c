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
#include "enums.h"
#include "enumerator.h"
#include "symtab.h"
#include <stdlib.h>
#include <assert.h>

enumtype * enumtype_new(char * id, enumerator_list * enums)
{
    enumtype * value = (enumtype *)malloc(sizeof(enumtype));
    
    value->id = id;
    value->enums = enums;
    value->stab = symtab_new(32, SYMTAB_TYPE_FUNC, NULL);
    
    return value;
}

void enumtype_delete(enumtype * value)
{
    if (value->id != NULL)
    {
        free(value->id);
    }
    if (value->enums != NULL)
    {
        enumerator_list_delete(value->enums);
    }
    if (value->stab != NULL)
    {
        symtab_delete(value->stab);
    }

    free(value);
}

enumerator * enumtype_find_enumerator(enumtype * enumtype_value, char * id)
{
    enumerator * ret = NULL;
    symtab_entry * entry = NULL;

    if (enumtype_value->stab != NULL && id != NULL)
    {    
        entry = symtab_lookup(enumtype_value->stab, id, SYMTAB_LOOKUP_LOCAL);
    }

    if (entry != NULL && entry->type == SYMTAB_ENUMERATOR)
    {
        ret = entry->enumerator_value;
    }
    else if (entry != NULL && entry->type != SYMTAB_ENUMERATOR)
    {
        assert(0);
    }

    return ret;
}


