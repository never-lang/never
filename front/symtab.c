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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "symtab.h"

#define HASH_MAGIC 5381

unsigned int hash(const char * str)
{
    char c;
    unsigned int val = HASH_MAGIC;

    while ((c = *str++) != 0)
    {
        val = ((val << 5) + val) + c;
    }

    return val;
}

symtab_entry * symtab_entry_new(unsigned int size)
{
    symtab_entry * entries = (symtab_entry *)malloc(size * sizeof(symtab_entry));
    memset(entries, 0, size * sizeof(symtab_entry));
    
    return entries;
}

void symtab_entry_delete(symtab_entry * entries)
{
    free(entries);
}

void symtab_entry_add_object(symtab_entry * entries, unsigned int size,
                             int type, const char * id, void * object_value)
{
    unsigned int times = 0;
    unsigned int index = hash(id) % size;
    
    while (entries[index].type != 0)
    {
        index = (index + 1) % size;
        if (times++ > size)
        {
            assert(0);
            return;
        }
    }
    entries[index].type = type;
    entries[index].id = id;
    entries[index].var_value = object_value;
}

symtab_entry * symtab_entry_lookup_object(symtab_entry * entries, unsigned int size, const char * id)
{
    unsigned int times = 0;
    unsigned int index = hash(id) % size;

    while (entries[index].type != 0)
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

void symtab_entry_resize(symtab_entry * entries, int size, symtab_entry * entries_new, int size_new)
{
    int i = 0;
    
    for (i = 0; i < size; i++)
    {
        symtab_entry_add_object(entries_new, size_new,
                                entries[i].type, entries[i].id,
                                entries[i].var_value);
    }
}

void symtab_entry_print(symtab_entry * entry)
{
    if (entry->type == SYMTAB_VAR)
    {
        var * var_value = entry->var_value;            
        if (var_value)
        {
            if (var_value->type == VAR_INT)
            {
                printf("[I][%s][%d]\n", var_value->id, entry->syn_level);
            }
            else if (var_value->type == VAR_FLOAT)
            {
                printf("[R][%s][%d]\n", var_value->id, entry->syn_level);
            }
            else if (var_value->type == VAR_FUNC)
            {
                printf("[P][%s][%d]\n", var_value->id, entry->syn_level);
            }
        }
        else
        {
            printf("[U][%s][%d]\n", entry->id, entry->syn_level);
        }
    }
    else if (entry->type == SYMTAB_FUNC)
    {
        printf("[F][%s][%d]\n", entry->id, entry->syn_level);
    }
}

symtab * symtab_new(unsigned int size, symtab * parent)
{
    symtab * tab = (symtab *)malloc(sizeof(symtab));
    
    tab->size = size;
    tab->count = 0;
    tab->parent = parent;
    tab->entries = symtab_entry_new(size);
    
    return tab;
}

void symtab_delete(symtab * tab)
{
    if (tab->entries)
    {
        symtab_entry_delete(tab->entries);
    }
    free(tab);    
}

void symtab_resize(symtab * tab)
{
    if (tab->count > tab->size * 3 / 4)
    {
        unsigned int size_new = 2 * tab->size;
        symtab_entry * entries_new = symtab_entry_new(size_new);
        
        symtab_entry_resize(tab->entries, tab->size, entries_new, size_new);
        
        symtab_entry_delete(tab->entries);
        
        tab->size = size_new;
        tab->entries = entries_new;
    }
}

void symtab_add_var(symtab * tab, var * var_value)
{
    if (var_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_VAR, var_value->id, var_value);
    tab->count++;
    symtab_resize(tab);    
}

void symtab_add_func(symtab * tab, func * func_value)
{
    if (func_value->id == NULL)
    {
        return;
    }

    symtab_entry_add_object(tab->entries, tab->size, SYMTAB_FUNC, func_value->id, func_value);
    tab->count++;
    symtab_resize(tab);
}

symtab_entry * symtab_lookup(symtab * tab, const char * id, char nested)
{
    symtab_entry * entry = NULL;

    entry = symtab_entry_lookup_object(tab->entries, tab->size, id);
    if (nested && entry == NULL && tab->parent != NULL)
    {
        return symtab_lookup(tab->parent, id, nested);
    }
    else
    {
        return entry;
    }
}

void symtab_set_syn_level(symtab * tab, unsigned int syn_level)
{
    unsigned int i;
    
    for (i = 0; i < tab->size; i++)
    {
        if (tab->entries[i].type != 0)
        {
            tab->entries[i].syn_level = syn_level;
        }
    }
}

void symtab_print(symtab * tab)
{
    unsigned int i;
    
    printf("[%p, %p]\n", tab, tab->parent);
        
    for (i = 0; i < tab->size; i++)
    {
        symtab_entry_print(tab->entries + i);
    }
}


