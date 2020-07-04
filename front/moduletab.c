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
#include "moduletab.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

moduletab_entry * moduletab_entry_new(unsigned int size)
{
    moduletab_entry * entries = (moduletab_entry *)malloc(size * sizeof(moduletab_entry));
    memset(entries, 0, size * sizeof(moduletab_entry));

    return entries;
}

void moduletab_entry_delete(moduletab_entry * entries, unsigned int size)
{
    unsigned int i = 0;

    for (i = 0; i < size; i++)
    {
        if (entries[i].id != NULL)
        {
            free(entries[i].id);
        }
    }

    free(entries);
}

void moduletab_entry_add_module(moduletab_entry * entries, unsigned int size, char * id)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(id) % size;
    while (entries[index].id != 0)
    {
        if (strcmp(entries[index].id, id) == 0)
        {
            return;
        }

        index = (index + 1) %size;
        if (times++ > size)
        {
            assert(0);
        }
    }

    entries[index].id = id;
}

moduletab_entry * moduletab_entry_lookup_module(moduletab_entry * entries, unsigned int size,
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

void moduletab_entry_resize(moduletab_entry * entries, unsigned int size,
                            moduletab_entry * entries_new, unsigned int size_new)
{
    unsigned int i = 0;

    for (i = 0; i < size; i++)
    {
        if (entries[i].id != NULL)
        {
            moduletab_entry_add_module(entries_new, size_new, entries[i].id);
        }
    }
}                            

moduletab * moduletab_new(unsigned int size)
{
    moduletab * tab = (moduletab *)malloc(sizeof(moduletab));

    tab->size = size;
    tab->count = 0;
    tab->entries = moduletab_entry_new(size);

    return tab;
}

void moduletab_delete(moduletab * tab)
{
    if (tab->entries)
    {
        moduletab_entry_delete(tab->entries, tab->size);
    }
    free(tab);
}

void moduletab_resize(moduletab * tab)
{
    if (tab->count > tab->size * 3 / 4)
    {
        unsigned int size_new = 2 * tab->size;
        moduletab_entry * entries_new = moduletab_entry_new(size_new);

        moduletab_entry_resize(tab->entries, tab->size, entries_new, size_new);
        free(tab->entries);

        tab->size = size_new;
        tab->entries = entries_new;
    }
}

void moduletab_add_module(moduletab * tab, char * id)
{
    if (id == NULL)
    {
        return;
    }

    moduletab_entry_add_module(tab->entries, tab->size, id);

    tab->count++;
    moduletab_resize(tab);
}

moduletab_entry * moduletab_lookup_module(moduletab * tab, const char * id)
{
    return moduletab_entry_lookup_module(tab->entries, tab->size, id);
}

