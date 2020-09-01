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
#include "inttab.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

inttab_entry * inttab_entry_new(unsigned int size)
{
    inttab_entry * entries = 
        (inttab_entry *)malloc(size * sizeof(inttab_entry));
    memset(entries, 0, size * sizeof(inttab_entry));

    return entries;
}

void inttab_entry_delete(inttab_entry * entries)
{
    free(entries);
}

void inttab_entry_add_enumerator(inttab_entry * entries, unsigned int size, int value, enumerator * enumerator_value)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = value % size;
    while (entries[index].type != INTTAB_ENTRY_EMPTY)
    {
        index = (index + 1) % size;
        if (times++ > size)
        {
            assert(0);
            return;
        }
    }
    entries[index].type = INTTAB_ENTRY_USED;
    entries[index].value = value;
    entries[index].enumerator_value = enumerator_value;
}

inttab_entry * inttab_entry_lookup(inttab_entry * entries, unsigned int size, int value)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = value % size;
    while (entries[index].type != INTTAB_ENTRY_EMPTY)
    {
        if (entries[index].value == value)
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

void inttab_entry_resize(inttab_entry * entries, unsigned int size,
                         inttab_entry * entries_new, unsigned int size_new)
{
    unsigned int i = 0;

    for (i = 0; i < size; i++)
    {
        if (entries[i].type != INTTAB_ENTRY_EMPTY)
        {
            inttab_entry_add_enumerator(entries_new, size_new,
                                        entries[i].value,
                                        entries[i].enumerator_value);
        }
    }
}                         

inttab * inttab_new(unsigned int size)
{
    inttab * tab = (inttab *)malloc(sizeof(inttab));

    tab->size = size;
    tab->count = 0;
    tab->entries = inttab_entry_new(size);

    return tab;
}

void inttab_delete(inttab * tab)
{
    if (tab->entries)
    {
        inttab_entry_delete(tab->entries);
    }
    free(tab);
}

void inttab_resize(inttab * tab)
{
    if (tab->count > tab->size * 3 / 4)
    {
        unsigned int size_new = 2 * tab->size;
        inttab_entry * entries_new = inttab_entry_new(size_new);

        inttab_entry_resize(tab->entries, tab->size, entries_new, size_new);

        inttab_entry_delete(tab->entries);

        tab->size = size_new;
        tab->entries = entries_new;
    }
}

void inttab_add_enumerator(inttab * tab, int value, enumerator * enumerator_value)
{
    inttab_entry_add_enumerator(tab->entries, tab->size, value, enumerator_value);

    tab->count++;
    inttab_resize(tab);
}

inttab_entry * inttab_lookup(inttab * tab, int value)
{
    return inttab_entry_lookup(tab->entries, tab->size, value);
}

