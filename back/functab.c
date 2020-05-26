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
#include "functab.h"
#include "object.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

functab_entry * functab_entry_new(unsigned int size)
{
    functab_entry * entries = (functab_entry *)malloc(size * sizeof(functab_entry));
    memset(entries, 0, size * sizeof(functab_entry));

    return entries;
}

void functab_entry_delete(functab_entry * entries)
{
    free(entries);
}

void functab_entry_add_func(functab_entry * entries, unsigned int size,
                            const char * id, unsigned int func_addr,
                            object * params, unsigned int params_count)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(id) % size;
    while (entries[index].id != 0)
    {
        index = (index + 1) % size;
        if (times++ > size)
        {
            assert(0);
            return;
        }
    }

    entries[index].id = id;
    entries[index].func_addr = func_addr;
    entries[index].params = params;
    entries[index].params_count = params_count;
}

functab_entry * functab_entry_lookup(functab_entry * entries, unsigned int size,
                                     const char * id)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(id) % size;
    while (entries[index].id != 0)
    {
        if (strcmp(entries[index].id, id) == 0)
        {
            return &entries[index];
        }

        index = (index + 1) % size;
        if (times++ > size)
        {
            return NULL;
        }
    }

    return NULL;
}

void functab_entry_resize(functab_entry * entries, unsigned int size,
                          functab_entry * entries_new, unsigned int size_new)
{
    unsigned int i = 0;

    for (i = 0; i < size; i++)
    {
        if (entries[i].id != NULL)
        {
            functab_entry_add_func(entries_new, size_new,
                                   entries[i].id, entries[i].func_addr,
                                   entries[i].params, entries[i].params_count);
        }
    }
}

functab * funtab_new(unsigned int size)
{
    functab * tab = (functab *)malloc(sizeof(functab));

    tab->size = size;
    tab->count = 0;
    tab->entries = functab_entry_new(size);

    return tab;
}

void functab_delete(functab * tab)
{
    if (tab->entries)
    {
        functab_entry_delete(tab->entries);
    }
    free(tab);
}

void functab_resize(functab * tab)
{
    if (tab->count > tab->size * 3 / 4)
    {
        unsigned int size_new = 2 * tab->size;
        functab_entry * entries_new = functab_entry_new(size_new);

        functab_entry_resize(tab->entries, tab->size, entries_new, size_new);
        functab_entry_delete(tab->entries);
        tab->size = size_new;
        tab->entries = entries_new;
    }
}

void functab_add_func(functab * tab, const char * id, unsigned int func_addr,
                      object * params, unsigned int params_count)
{
    functab_entry_add_func(tab->entries, tab->size, id, func_addr, params, params_count);

    tab->count++;
    functab_resize(tab);
}

functab_entry * functab_lookup(functab * tab, const char * id)
{
    return functab_entry_lookup(tab->entries, tab->size, id);
}
