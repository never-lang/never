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
#include "dltab.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <dlfcn.h>

dltab_entry * dltab_entry_new(unsigned int size)
{
    dltab_entry * entries = (dltab_entry *)malloc(size * sizeof(dltab_entry));
    memset(entries, 0, size * sizeof(dltab_entry));
    
    return entries;
}

void dltab_entry_delete(dltab_entry * entries, unsigned int size)
{
    unsigned int i = 0;
    
    for (i = 0; i < size; i++)
    {
        if (entries[i].name != NULL)
        {
            free(entries[i].name);
        }
    }

    free(entries);
}

void dltab_entry_add_lib(dltab_entry * entries, unsigned int size,
                         char * name, void * lib)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(name) % size;
    while (entries[index].name != 0)
    {
        index = (index + 1) % size;
        if (times++ > size)
        {
            assert(0);
        }
    }
    
    entries[index].name = strdup(name);
    entries[index].lib = lib;
}

void * dltab_entry_lookup_lib(dltab_entry * entries, unsigned int size,
                              char * name)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(name) % size;
    while (entries[index].name != 0)
    {
        if (strcmp(entries[index].name, name) == 0)
        {
            return entries[index].lib;
        }

        index = (index + 1) % size;
        if (times++ > size)
        {
            return NULL;
        }
    }
    
    return NULL;
}

void dltab_entry_resize(dltab_entry * entries, unsigned int size,
                        dltab_entry * entries_new, unsigned int size_new)
{
    unsigned int i = 0;
    
    for (i = 0; i < size; i++)
    {
        if (entries[i].name != NULL)
        {
            dltab_entry_add_lib(entries_new, size_new, entries[i].name,
                                entries[i].lib);
        }
    }
}

dltab * dltab_new(unsigned int size)
{
    dltab * tab = (dltab *)malloc(sizeof(dltab));
    
    tab->size = size;
    tab->count = 1;
    tab->entries = dltab_entry_new(size);
    
    return tab;
}

void dltab_delete(dltab * tab)
{
    if (tab->entries)
    {
        dltab_entry_delete(tab->entries, tab->size);
    }
    free(tab);
}

void dltab_resize(dltab * tab)
{
    if (tab->count > tab->size * 3 / 4)
    {
        unsigned int size_new = 2 * tab->size;
        dltab_entry * entries_new = dltab_entry_new(size_new);

        dltab_entry_resize(tab->entries, tab->size, entries_new, size_new);
        dltab_entry_delete(tab->entries, tab->size);

        tab->size = size_new;
        tab->entries = entries_new;
    }
}

void dltab_add_lib(dltab * tab, char * name, void * lib)
{
    dltab_entry_add_lib(tab->entries, tab->size, name, lib);

    tab->count++;
    dltab_resize(tab);
}

void * dltab_lookup_lib(dltab * tab, char * name)
{
    return dltab_entry_lookup_lib(tab->entries, tab->size, name);
}

