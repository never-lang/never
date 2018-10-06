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
#include "strtab.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

strtab_entry * strtab_entry_new(unsigned int size)
{
    strtab_entry * entries = (strtab_entry *)malloc(size * sizeof(strtab_entry));
    memset(entries, 0, size * sizeof(strtab_entry));
    
    return entries;
}

void strtab_entry_delete(strtab_entry * entries, unsigned int size)
{
    unsigned int i = 0;
    
    for (i = 0; i < size; i++)
    {
        if (entries[i].string != NULL)
        {
            free(entries[i].string);
        }
    }

    free(entries);
}

unsigned int strtab_entry_add_string(strtab_entry * entries, unsigned int size,
                                     char * string, unsigned int order)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(string) % size;
    while (entries[index].string != 0)
    {
        if (strcmp(entries[index].string, string) == 0)
        {
            return entries[index].order;
        }
    
        index = (index + 1) % size;
        if (times++ > size)
        {
            assert(0);
        }
    }
    
    entries[index].string = strdup(string);
    entries[index].order = order;
    
    return order;
}

unsigned int strtab_entry_lookup_string(strtab_entry * entries, unsigned int size,
                                        char * string)
{
    unsigned int times = 0;
    unsigned int index = 0;

    index = hash_string(string) % size;
    while (entries[index].string != 0)
    {
        if (strcmp(entries[index].string, string) == 0)
        {
            return entries[index].order;
        }

        index = (index + 1) % size;
        if (times++ > size)
        {
            return 0;
        }
    }
    
    return 0;
}

void strtab_entry_resize(strtab_entry * entries, unsigned int size,
                         strtab_entry * entries_new, unsigned int size_new)
{
    unsigned int i = 0;
    
    for (i = 0; i < size; i++)
    {
        if (entries[i].string != NULL)
        {
            strtab_entry_add_string(entries_new, size_new, entries[i].string,
                                    entries[i].order);
        }
    }
}

strtab * strtab_new(unsigned int size)
{
    strtab * tab = (strtab *)malloc(sizeof(strtab));
    
    tab->size = size;
    tab->count = 1;
    tab->entries = strtab_entry_new(size);
    
    return tab;
}

void strtab_delete(strtab * tab)
{
    if (tab->entries)
    {
        strtab_entry_delete(tab->entries, tab->size);
    }
    free(tab);
}

void strtab_resize(strtab * tab)
{
    if (tab->count > tab->size * 3 / 4)
    {
        unsigned int size_new = 2 * tab->size;
        strtab_entry * entries_new = strtab_entry_new(size_new);

        strtab_entry_resize(tab->entries, tab->size, entries_new, size_new);

        strtab_entry_delete(tab->entries, tab->size);

        tab->size = size_new;
        tab->entries = entries_new;
    }
}

unsigned int strtab_add_string(strtab * tab, char * string)
{
    unsigned int order = 0;
    
    order = strtab_entry_add_string(tab->entries, tab->size, string, tab->count);

    if (order == tab->count)
    {
        tab->count++;
        strtab_resize(tab);
    }

    return order;
}

unsigned int strtab_lookup_string(strtab * tab, char * string)
{
    return strtab_entry_lookup_string(tab->entries, tab->size, string);
}

void strtab_to_array(strtab * tab, char *** strings, unsigned int * size)
{
    unsigned int i = 0;
    
    *size = tab->count;
    *strings = (char **)malloc(*size * sizeof(char *));

    *strings[0] = NULL;
    for (i = 0; i < tab->size; i++)
    {
        if (tab->entries[i].string != NULL)
        {
            (*strings)[tab->entries[i].order] = tab->entries[i].string;
            tab->entries[i].string = NULL;
        }
    }
}

void strtab_array_delete(char ** strings, unsigned int size)
{
    unsigned int i = 0;
    
    for (i = 0; i < size; i++)
    {
        if (strings[i] != NULL)
        {
            free(strings[i]);
        }
    }

    free(strings);
}

void strtab_array_print(char ** strings, unsigned int size)
{
    unsigned int i = 0;

    printf("---- strtab_array beg ---\n");
    
    for (i = 0; i < size; i++)
    {
        if (strings[i] != NULL)
        {
            printf("string %s\n", strings[i]);
        }
    }

    printf("---- strtab_array end ---\n");
}


